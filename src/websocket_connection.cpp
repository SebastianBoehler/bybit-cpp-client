#include <chrono>

#include "bybit/detail/transport_reaper.hpp"
#include "bybit/detail/websocket_auth.hpp"
#include "bybit/detail/websocket_transport.hpp"
#include "bybit/websocket_client.hpp"

namespace bybit {

WebSocketClient::~WebSocketClient() noexcept {
#ifdef BYBIT_ENABLE_WEBSOCKET
  callback_owner_.invalidate();
  try {
    close();
  } catch (...) {
  }
#endif
}

void WebSocketClient::connect() {
#ifndef BYBIT_ENABLE_WEBSOCKET
  throw std::runtime_error("WebSocket support disabled at build time. Rebuild with BYBIT_ENABLE_WEBSOCKET=ON.");
#else
  if (detail::in_websocket_callback(this))
    return;
  std::uint64_t generation = 0;
  {
    std::scoped_lock lock(state_mutex_);
    if (!lifecycle_.begin_connect())
      return;
    generation = ++connection_generation_;
  }
  if (!stop_workers(generation))
    return;

  std::unique_lock transport_control_lock(transport_control_mutex_);
  {
    std::scoped_lock lock(state_mutex_);
    if (generation != connection_generation_)
      return;
  }
  callback_owner_.invalidate();
  std::unique_ptr<ix::WebSocket> previous_transport;
  {
    std::scoped_lock transport_lock(transport_mutex_);
    previous_transport = std::move(ws_);
  }
  if (previous_transport)
    previous_transport->stop();
  {
    std::scoped_lock lock(state_mutex_);
    if (generation != connection_generation_)
      return;
  }

  auto transport = std::make_unique<ix::WebSocket>();
  transport->setUrl(url_);
  detail::configure_managed_reconnect(*transport);
  const auto callback_owner = callback_owner_.renew(this);
  transport->setOnMessageCallback([callback_owner](const ix::WebSocketMessagePtr& msg) {
    if (!msg)
      return;
    auto* client = callback_owner->load(std::memory_order_acquire);
    if (!client)
      return;
    detail::WebSocketCallbackScope callback_scope{client};
    switch (msg->type) {
      case ix::WebSocketMessageType::Open: {
        detail::OpenAction action = detail::OpenAction::None;
        {
          std::scoped_lock lock(client->state_mutex_);
          action = client->lifecycle_.on_open(!client->api_key_.empty() && !client->api_secret_.empty());
          client->reconnect_due_.reset();
        }
        try {
          if (action == detail::OpenAction::Authenticate)
            client->authenticate();
          else if (action == detail::OpenAction::Ready)
            client->resubscribe_all();
        } catch (const std::runtime_error&) {
          // A concurrent close may remove the transport after the lifecycle action was selected.
        }
        break;
      }
      case ix::WebSocketMessageType::Close:
      case ix::WebSocketMessageType::Error:
        client->schedule_reconnect();
        break;
      case ix::WebSocketMessageType::Message: {
        if (!msg->binary) {
          const auto auth_result = detail::parse_websocket_auth_result(msg->str);
          detail::AuthAction auth_action = detail::AuthAction::None;
          if (auth_result) {
            std::scoped_lock lock(client->state_mutex_);
            auth_action = client->lifecycle_.on_auth_result(*auth_result);
          }
          if (auth_action == detail::AuthAction::Ready) {
            try {
              client->resubscribe_all();
            } catch (const std::runtime_error&) {
              // A concurrent close may remove the transport after authentication succeeds.
            }
          } else if (auth_action == detail::AuthAction::Close) {
            std::scoped_lock transport_lock(client->transport_mutex_);
            if (client->ws_)
              client->ws_->close(4003, "Bybit authentication failed");
          }
        }

        MessageHandler text_handler;
        BinaryMessageHandler binary_handler;
        {
          std::scoped_lock lock(client->handler_mutex_);
          text_handler = client->handler_;
          binary_handler = client->binary_handler_;
        }
        if (msg->binary && binary_handler)
          binary_handler(msg->str);
        else if (text_handler)
          text_handler(msg->str);
        break;
      }
      default:
        break;
    }
  });
  std::scoped_lock worker_control_lock(worker_control_mutex_);
  {
    std::scoped_lock lock(state_mutex_);
    if (generation != connection_generation_) {
      callback_owner_.invalidate();
      return;
    }
    workers_stopping_ = false;
  }
  ix::WebSocket* active_transport = transport.get();
  {
    std::scoped_lock transport_lock(transport_mutex_);
    ws_ = std::move(transport);
  }
  start_reconnect_worker();
  start_ping_timer();
  active_transport->start();
#endif
}

void WebSocketClient::close() {
#ifndef BYBIT_ENABLE_WEBSOCKET
  throw std::runtime_error("WebSocket support disabled at build time. Rebuild with BYBIT_ENABLE_WEBSOCKET=ON.");
#else
  const bool callback_close = detail::in_websocket_callback(this);
  {
    std::scoped_lock lock(state_mutex_);
    ++connection_generation_;
    lifecycle_.close_intentionally();
    reconnect_due_.reset();
  }
  stop_workers();
  std::unique_lock transport_control_lock(transport_control_mutex_, std::defer_lock);
  if (!callback_close)
    transport_control_lock.lock();
  callback_owner_.invalidate();
  std::unique_ptr<ix::WebSocket> transport;
  {
    std::scoped_lock transport_lock(transport_mutex_);
    transport = std::move(ws_);
  }
  if (callback_close)
    detail::defer_transport_cleanup(std::move(transport));
  else if (transport)
    transport->stop();
#endif
}

bool WebSocketClient::is_open() const {
#ifdef BYBIT_ENABLE_WEBSOCKET
  std::scoped_lock transport_lock(transport_mutex_);
  return ws_ && ws_->getReadyState() == ix::ReadyState::Open;
#else
  return false;
#endif
}

void WebSocketClient::enable_auto_reconnect(bool enabled, int max_retries) {
  std::scoped_lock lock(state_mutex_);
  lifecycle_.configure_reconnect(enabled, max_retries);
  if (!enabled)
    reconnect_due_.reset();
  state_cv_.notify_all();
}

void WebSocketClient::schedule_reconnect() {
#ifdef BYBIT_ENABLE_WEBSOCKET
  std::scoped_lock lock(state_mutex_);
  const auto delay = lifecycle_.on_disconnect();
  if (!delay)
    return;
  reconnect_due_ = std::chrono::steady_clock::now() + *delay;
  state_cv_.notify_all();
#endif
}

void WebSocketClient::start_reconnect_worker() {
#ifdef BYBIT_ENABLE_WEBSOCKET
  if (reconnect_worker_.joinable())
    return;
  reconnect_worker_ = std::thread([this] {
    std::unique_lock lock(state_mutex_);
    while (!workers_stopping_) {
      state_cv_.wait(lock, [this] { return workers_stopping_ || reconnect_due_.has_value(); });
      if (workers_stopping_)
        break;

      const auto due = *reconnect_due_;
      if (state_cv_.wait_until(lock, due,
                               [this, due] { return workers_stopping_ || !reconnect_due_ || *reconnect_due_ != due; }))
        continue;
      reconnect_due_.reset();

      lock.unlock();
      {
        std::scoped_lock transport_control_lock(transport_control_mutex_);
        ix::WebSocket* transport = nullptr;
        {
          std::scoped_lock transport_lock(transport_mutex_);
          transport = ws_.get();
        }
        if (transport)
          transport->stop();
        bool should_start = false;
        {
          std::scoped_lock state_lock(state_mutex_);
          lifecycle_.reconnect_finished();
          should_start = !workers_stopping_ && transport;
        }
        if (should_start)
          transport->start();
      }
      lock.lock();
    }
  });
#endif
}

void WebSocketClient::start_ping_timer() {
#ifdef BYBIT_ENABLE_WEBSOCKET
  if (ping_worker_.joinable())
    return;
  ping_worker_ = std::thread([this] {
    std::unique_lock lock(state_mutex_);
    while (!workers_stopping_) {
      if (state_cv_.wait_for(lock, std::chrono::seconds(20), [this] { return workers_stopping_; }))
        break;
      lock.unlock();
      if (is_open()) {
        try {
          send_raw(R"({"op":"ping"})");
        } catch (...) {
        }
      }
      lock.lock();
    }
  });
#endif
}

bool WebSocketClient::stop_workers(std::optional<std::uint64_t> expected_generation) {
#ifdef BYBIT_ENABLE_WEBSOCKET
  std::scoped_lock worker_control_lock(worker_control_mutex_);
  {
    std::scoped_lock lock(state_mutex_);
    if (expected_generation && *expected_generation != connection_generation_)
      return false;
    workers_stopping_ = true;
  }
  state_cv_.notify_all();
  if (reconnect_worker_.joinable())
    reconnect_worker_.join();
  if (ping_worker_.joinable())
    ping_worker_.join();
  return true;
#else
  (void)expected_generation;
  return true;
#endif
}

}  // namespace bybit
