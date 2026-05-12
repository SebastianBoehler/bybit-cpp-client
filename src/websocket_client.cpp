#include "bybit/websocket_client.hpp"

#include <algorithm>
#include <chrono>
#include <sstream>

#include "bybit/signing.hpp"

namespace bybit {

std::string WebSocketClient::now_ms() {
  auto now = std::chrono::system_clock::now();
  auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
  return std::to_string(ms);
}

WebSocketClient::WebSocketClient(std::string url, std::string api_key, std::string api_secret, std::string recv_window)
    : url_(std::move(url)),
      api_key_(std::move(api_key)),
      api_secret_(std::move(api_secret)),
      recv_window_(std::move(recv_window)) {
#ifndef BYBIT_ENABLE_WEBSOCKET
  if (!url_.empty()) {
    throw std::runtime_error("WebSocket support disabled at build time. Rebuild with BYBIT_ENABLE_WEBSOCKET=ON.");
  }
#else
  ws_.setUrl(url_);
#endif
}

void WebSocketClient::set_message_handler(MessageHandler handler) {
  std::scoped_lock lock(handler_mutex_);
  handler_ = std::move(handler);
}

void WebSocketClient::set_binary_message_handler(BinaryMessageHandler handler) {
  std::scoped_lock lock(handler_mutex_);
  binary_handler_ = std::move(handler);
}

void WebSocketClient::connect() {
#ifndef BYBIT_ENABLE_WEBSOCKET
  throw std::runtime_error("WebSocket support disabled at build time. Rebuild with BYBIT_ENABLE_WEBSOCKET=ON.");
#else
  ws_.setOnMessageCallback([this](const ix::WebSocketMessagePtr& msg) {
    if (!msg) return;
    switch (msg->type) {
      case ix::WebSocketMessageType::Open: {
        std::scoped_lock lock(state_mutex_);
        reconnect_attempts_ = 0;
      }
        // Authenticate first if keys are provided, then resubscribe remembered topics.
        if (!api_key_.empty() && !api_secret_.empty()) {
          authenticate();
        }
        resubscribe_all();
        break;
      case ix::WebSocketMessageType::Close:
        schedule_reconnect();
        break;
      case ix::WebSocketMessageType::Error:
        schedule_reconnect();
        break;
      case ix::WebSocketMessageType::Message: {
        std::scoped_lock lock(handler_mutex_);
        if (msg->binary && binary_handler_) {
          binary_handler_(msg->str);
        } else if (handler_) {
          handler_(msg->str);
        }
        break;
      }
      default:
        break;
    }
  });
  ws_.start();
  start_ping_timer();
#endif
}

void WebSocketClient::close() {
#ifndef BYBIT_ENABLE_WEBSOCKET
  throw std::runtime_error("WebSocket support disabled at build time. Rebuild with BYBIT_ENABLE_WEBSOCKET=ON.");
#else
  stop_ping_timer();
  ws_.stop();
#endif
}

bool WebSocketClient::is_open() const {
#ifdef BYBIT_ENABLE_WEBSOCKET
  return ws_.getReadyState() == ix::ReadyState::Open;
#else
  return false;
#endif
}

void WebSocketClient::subscribe_topic(const std::string& topic, const std::string& req_id) {
  subscribe(std::vector<std::string>{topic}, req_id);
}

void WebSocketClient::subscribe_topics(const std::vector<std::string>& topics, const std::string& req_id) {
  subscribe(topics, req_id);
}

void WebSocketClient::unsubscribe_topic(const std::string& topic, const std::string& req_id) {
  unsubscribe(std::vector<std::string>{topic}, req_id);
}

void WebSocketClient::unsubscribe_topics(const std::vector<std::string>& topics, const std::string& req_id) {
  unsubscribe(topics, req_id);
}

void WebSocketClient::subscribe(const std::vector<std::string>& topics, const std::string& req_id) {
  remember_topics(topics);
  std::ostringstream oss;
  oss << "{\"op\":\"subscribe\",\"args\":" << serialize_args(topics);
  if (!req_id.empty()) oss << ",\"req_id\":\"" << req_id << "\"";
  oss << "}";
  send_raw(oss.str());
}

void WebSocketClient::unsubscribe(const std::vector<std::string>& topics, const std::string& req_id) {
  forget_topics(topics);
  std::ostringstream oss;
  oss << "{\"op\":\"unsubscribe\",\"args\":" << serialize_args(topics);
  if (!req_id.empty()) oss << ",\"req_id\":\"" << req_id << "\"";
  oss << "}";
  send_raw(oss.str());
}

void WebSocketClient::ping(const std::string& req_id) {
  std::ostringstream oss;
  oss << "{\"op\":\"ping\"";
  if (!req_id.empty()) oss << ",\"req_id\":\"" << req_id << "\"";
  oss << "}";
  send_raw(oss.str());
}

void WebSocketClient::enable_auto_reconnect(bool enabled, int max_retries) {
  std::scoped_lock lock(state_mutex_);
  auto_reconnect_ = enabled;
  max_retries_ = max_retries;
  reconnect_attempts_ = 0;
}

void WebSocketClient::send_raw(const std::string& payload) {
#ifndef BYBIT_ENABLE_WEBSOCKET
  (void)payload;
  throw std::runtime_error("WebSocket support disabled at build time. Rebuild with BYBIT_ENABLE_WEBSOCKET=ON.");
#else
  ws_.sendText(payload);
#endif
}

void WebSocketClient::authenticate() {
#ifndef BYBIT_ENABLE_WEBSOCKET
  throw std::runtime_error("WebSocket support disabled at build time. Rebuild with BYBIT_ENABLE_WEBSOCKET=ON.");
#else
  // Bybit v5 private WS auth: expires_ms must be in the future. Signature = HMAC_SHA256(secret, "GET/realtime" +
  // expires_ms)
  auto expires_ms = std::to_string(std::stoll(now_ms()) + 10000);  // +10s buffer
  const std::string to_sign = "GET/realtime" + expires_ms;
  const std::string signature = Signer::hmac_sha256_hex(api_secret_, to_sign);
  std::ostringstream oss;
  // Bybit expects [api_key, expires_ms, signature].
  // Use numeric expires in JSON (no quotes).
  oss << "{\"op\":\"auth\",\"args\":[\"" << api_key_ << "\"," << expires_ms << ",\"" << signature << "\"]}";
  send_raw(oss.str());
#endif
}

void WebSocketClient::remember_topics(const std::vector<std::string>& topics) {
  std::scoped_lock lock(state_mutex_);
  for (const auto& t : topics) {
    if (std::find(subscribed_topics_.begin(), subscribed_topics_.end(), t) == subscribed_topics_.end()) {
      subscribed_topics_.push_back(t);
    }
  }
}

void WebSocketClient::forget_topics(const std::vector<std::string>& topics) {
  std::scoped_lock lock(state_mutex_);
  for (const auto& t : topics) {
    subscribed_topics_.erase(std::remove(subscribed_topics_.begin(), subscribed_topics_.end(), t),
                             subscribed_topics_.end());
  }
}

void WebSocketClient::resubscribe_all() {
  std::vector<std::string> copy;
  {
    std::scoped_lock lock(state_mutex_);
    copy = subscribed_topics_;
  }
  if (!copy.empty()) {
    subscribe(copy, "resub");
  }
}

void WebSocketClient::schedule_reconnect() {
#ifndef BYBIT_ENABLE_WEBSOCKET
  return;
#else
  stop_ping_timer();
  std::scoped_lock lock(state_mutex_);
  if (!auto_reconnect_) return;
  if (reconnect_attempts_ >= max_retries_) {
    return;
  }
  int attempt = ++reconnect_attempts_;
  int backoff_ms = std::min(1000 * (1 << std::min(attempt, 6)), 10000);  // cap at 10s
  std::thread([this, backoff_ms]() {
    std::this_thread::sleep_for(std::chrono::milliseconds(backoff_ms));
    ws_.stop();
    ws_.start();
    if (!api_key_.empty()) {
      authenticate();
    }
    resubscribe_all();
    start_ping_timer();
  }).detach();
#endif
}

void WebSocketClient::start_ping_timer() {
#ifndef BYBIT_ENABLE_WEBSOCKET
  return;
#else
  if (ping_running_) return;
  stop_ping_ = false;
  ping_running_ = true;
  std::thread([this]() {
    while (!stop_ping_) {
      std::this_thread::sleep_for(std::chrono::seconds(20));
      if (stop_ping_) break;
      try {
        send_raw(R"({"op":"ping"})");
      } catch (...) {
        // swallow to avoid breaking the loop
      }
    }
    ping_running_ = false;
  }).detach();
#endif
}

void WebSocketClient::stop_ping_timer() {
  stop_ping_ = true;
}

}  // namespace bybit
