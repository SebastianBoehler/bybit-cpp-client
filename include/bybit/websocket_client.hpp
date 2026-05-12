#pragma once

#include <atomic>
#include <functional>
#include <mutex>
#include <stdexcept>
#include <string>
#include <thread>
#include <vector>

#ifdef BYBIT_ENABLE_WEBSOCKET
#include <ixwebsocket/IXWebSocket.h>
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
  void subscribe_sbe_bbo(const std::vector<std::string>& symbols, const std::string& req_id = "");
  void subscribe_sbe_orderbook_50(const std::vector<std::string>& symbols, const std::string& req_id = "");
  void subscribe_sbe_public_trades(const std::vector<std::string>& symbols, const std::string& req_id = "");

  void unsubscribe_tickers(const std::vector<std::string>& symbols, const std::string& req_id = "");
  void unsubscribe_orderbook(const std::vector<std::string>& symbols, int depth = 1, const std::string& req_id = "");
  void unsubscribe_kline(const std::vector<std::string>& symbols, const std::string& interval,
                         const std::string& req_id = "");
  void unsubscribe_public_trades(const std::vector<std::string>& symbols, const std::string& req_id = "");
  void unsubscribe_sbe_bbo(const std::vector<std::string>& symbols, const std::string& req_id = "");
  void unsubscribe_sbe_orderbook_50(const std::vector<std::string>& symbols, const std::string& req_id = "");
  void unsubscribe_sbe_public_trades(const std::vector<std::string>& symbols, const std::string& req_id = "");

  // Auto reconnect/resubscribe support.
  void enable_auto_reconnect(bool enabled = true, int max_retries = 5);

 private:
  void send_raw(const std::string& payload);
  void authenticate();
  std::string serialize_args(const std::vector<std::string>& topics) const;
  static std::string now_ms();
  static std::vector<std::string> make_topics(const std::string& prefix, const std::vector<std::string>& symbols,
                                              const std::string& suffix = "");
  void remember_topics(const std::vector<std::string>& topics);
  void forget_topics(const std::vector<std::string>& topics);
  void resubscribe_all();
  void schedule_reconnect();
  void start_ping_timer();
  void stop_ping_timer();

#ifdef BYBIT_ENABLE_WEBSOCKET
  ix::WebSocket ws_;
#endif
  std::string url_;
  std::string api_key_;
  std::string api_secret_;
  std::string recv_window_;
  MessageHandler handler_;
  BinaryMessageHandler binary_handler_;
  mutable std::mutex handler_mutex_;
  mutable std::mutex state_mutex_;
  bool auto_reconnect_{false};
  int max_retries_{5};
  int reconnect_attempts_{0};
  std::vector<std::string> subscribed_topics_;
  std::atomic<bool> ping_running_{false};
  std::atomic<bool> stop_ping_{false};
};

}  // namespace bybit
