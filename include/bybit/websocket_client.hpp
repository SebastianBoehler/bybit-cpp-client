#pragma once

#include <chrono>
#include <condition_variable>
#include <cstdint>
#include <functional>
#include <memory>
#include <mutex>
#include <optional>
#include <stdexcept>
#include <string>
#include <thread>
#include <vector>

#include "bybit/detail/websocket_lifecycle.hpp"
#include "bybit/detail/websocket_transport.hpp"

#ifdef BYBIT_ENABLE_WEBSOCKET
#include <IXWebSocket.h>
#else
namespace ix {
class WebSocket;
}
#endif

namespace bybit {

// Minimal dedicated websocket client for Bybit v5 streams (public or private).
class WebSocketClient {
 public:
  using MessageHandler = std::function<void(const std::string&)>;
  using BinaryMessageHandler = std::function<void(const std::string&)>;

  WebSocketClient(std::string url, std::string api_key = "", std::string api_secret = "",
                  std::string recv_window = "5000");
  ~WebSocketClient() noexcept;

  // Register a callback invoked on text messages.
  void set_message_handler(MessageHandler handler);
  void set_binary_message_handler(BinaryMessageHandler handler);

  // Start the connection (non-blocking). On open, auth is sent automatically when api_key/api_secret are set.
  void connect();
  void close();
  bool is_open() const;

  // Generic topic subscribe/unsubscribe (raw topic strings).
  void subscribe_topic(const std::string& topic, const std::string& req_id = "");
  void subscribe_topics(const std::vector<std::string>& topics, const std::string& req_id = "");
  void unsubscribe_topic(const std::string& topic, const std::string& req_id = "");
  void unsubscribe_topics(const std::vector<std::string>& topics, const std::string& req_id = "");

  void subscribe(const std::vector<std::string>& topics, const std::string& req_id = "");
  void unsubscribe(const std::vector<std::string>& topics, const std::string& req_id = "");
  void ping(const std::string& req_id = "");
  void send_binary(const std::string& payload);

  // Convenience helpers for common public topics.
  void subscribe_tickers(const std::vector<std::string>& symbols, const std::string& req_id = "");
  void subscribe_orderbook(const std::vector<std::string>& symbols, int depth = 1, const std::string& req_id = "");
  void subscribe_kline(const std::vector<std::string>& symbols, const std::string& interval,
                       const std::string& req_id = "");
  void subscribe_public_trades(const std::vector<std::string>& symbols, const std::string& req_id = "");
  void subscribe_all_liquidation(const std::vector<std::string>& symbols, const std::string& req_id = "");
  void subscribe_sbe_bbo(const std::vector<std::string>& symbols, const std::string& req_id = "");
  void subscribe_sbe_orderbook_50(const std::vector<std::string>& symbols, const std::string& req_id = "");
  void subscribe_sbe_public_trades(const std::vector<std::string>& symbols, const std::string& req_id = "");

  // Convenience helpers for common private topics.
  void subscribe_private_order(const std::string& category = "", const std::string& req_id = "");
  void subscribe_private_execution(const std::string& category = "", const std::string& req_id = "");
  void subscribe_private_position(const std::string& category = "", const std::string& req_id = "");
  void subscribe_private_wallet(const std::string& req_id = "");
  void subscribe_private_greeks(const std::string& req_id = "");

  void unsubscribe_tickers(const std::vector<std::string>& symbols, const std::string& req_id = "");
  void unsubscribe_orderbook(const std::vector<std::string>& symbols, int depth = 1, const std::string& req_id = "");
  void unsubscribe_kline(const std::vector<std::string>& symbols, const std::string& interval,
                         const std::string& req_id = "");
  void unsubscribe_public_trades(const std::vector<std::string>& symbols, const std::string& req_id = "");
  void unsubscribe_all_liquidation(const std::vector<std::string>& symbols, const std::string& req_id = "");
  void unsubscribe_sbe_bbo(const std::vector<std::string>& symbols, const std::string& req_id = "");
  void unsubscribe_sbe_orderbook_50(const std::vector<std::string>& symbols, const std::string& req_id = "");
  void unsubscribe_sbe_public_trades(const std::vector<std::string>& symbols, const std::string& req_id = "");

  void unsubscribe_private_order(const std::string& category = "", const std::string& req_id = "");
  void unsubscribe_private_execution(const std::string& category = "", const std::string& req_id = "");
  void unsubscribe_private_position(const std::string& category = "", const std::string& req_id = "");
  void unsubscribe_private_wallet(const std::string& req_id = "");
  void unsubscribe_private_greeks(const std::string& req_id = "");

  // Auto reconnect/resubscribe support.
  void enable_auto_reconnect(bool enabled = true, int max_retries = 5);

 private:
  void send_raw(const std::string& payload);
  void authenticate();
  static std::string now_ms();
  static std::vector<std::string> make_topics(const std::string& prefix, const std::vector<std::string>& symbols,
                                              const std::string& suffix = "");
  static std::string private_topic(const std::string& topic, const std::string& category);
  void remember_topics(const std::vector<std::string>& topics);
  void forget_topics(const std::vector<std::string>& topics);
  void resubscribe_all();
  void schedule_reconnect();
  void start_reconnect_worker();
  void start_ping_timer();
  bool stop_workers(std::optional<std::uint64_t> expected_generation = std::nullopt);

#ifdef BYBIT_ENABLE_WEBSOCKET
  std::unique_ptr<ix::WebSocket> ws_;
#endif
  std::string url_;
  std::string api_key_;
  std::string api_secret_;
  std::string recv_window_;
  MessageHandler handler_;
  BinaryMessageHandler binary_handler_;
  mutable std::mutex handler_mutex_;
  mutable std::mutex state_mutex_;
  // Guards the transport pointer only; never hold it while waiting for IXWebSocket threads to stop.
  mutable std::mutex transport_mutex_;
  // Serializes transport start/stop. WebSocket callbacks and sends never acquire it.
  std::mutex transport_control_mutex_;
  // Serializes worker start/stop and guarantees each join is owned by one caller.
  std::mutex worker_control_mutex_;
  std::condition_variable state_cv_;
  detail::WebSocketLifecycle lifecycle_;
  std::optional<std::chrono::steady_clock::time_point> reconnect_due_;
  std::vector<std::string> subscribed_topics_;
  std::thread reconnect_worker_;
  std::thread ping_worker_;
  bool workers_stopping_{false};
  std::uint64_t connection_generation_{0};
  detail::WebSocketCallbackOwner<WebSocketClient> callback_owner_{this};
};

}  // namespace bybit
