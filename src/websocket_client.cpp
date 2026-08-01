#include "bybit/websocket_client.hpp"

#include <algorithm>
#include <chrono>

#include "bybit/detail/transport_reaper.hpp"
#include "bybit/detail/websocket_messages.hpp"
#include "bybit/detail/websocket_transport.hpp"
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
  if (api_key_.empty() != api_secret_.empty())
    throw std::invalid_argument("WebSocket API key and secret must be provided together");
#ifndef BYBIT_ENABLE_WEBSOCKET
  if (!url_.empty()) {
    throw std::runtime_error("WebSocket support disabled at build time. Rebuild with BYBIT_ENABLE_WEBSOCKET=ON.");
  }
#else
  detail::ensure_transport_reaper<ix::WebSocket>();
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
  {
    std::scoped_lock lock(state_mutex_);
    if (!lifecycle_.can_send())
      return;
  }
  send_raw(detail::make_subscription_message("subscribe", topics, req_id));
}

void WebSocketClient::unsubscribe(const std::vector<std::string>& topics, const std::string& req_id) {
  forget_topics(topics);
  {
    std::scoped_lock lock(state_mutex_);
    if (!lifecycle_.can_send())
      return;
  }
  send_raw(detail::make_subscription_message("unsubscribe", topics, req_id));
}

void WebSocketClient::ping(const std::string& req_id) {
  send_raw(detail::make_ping_message(req_id));
}

void WebSocketClient::send_binary(const std::string& payload) {
#ifndef BYBIT_ENABLE_WEBSOCKET
  (void)payload;
  throw std::runtime_error("WebSocket support disabled at build time. Rebuild with BYBIT_ENABLE_WEBSOCKET=ON.");
#else
  std::scoped_lock lock(transport_mutex_);
  if (!ws_)
    throw std::runtime_error("WebSocket transport is closed");
  ws_->sendBinary(payload);
#endif
}

void WebSocketClient::send_raw(const std::string& payload) {
#ifndef BYBIT_ENABLE_WEBSOCKET
  (void)payload;
  throw std::runtime_error("WebSocket support disabled at build time. Rebuild with BYBIT_ENABLE_WEBSOCKET=ON.");
#else
  std::scoped_lock lock(transport_mutex_);
  if (!ws_)
    throw std::runtime_error("WebSocket transport is closed");
  ws_->sendText(payload);
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
  send_raw(detail::make_auth_message(api_key_, expires_ms, signature));
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

}  // namespace bybit
