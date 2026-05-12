#include <string>
#include <vector>

#include "bybit/websocket_client.hpp"

namespace bybit {

std::vector<std::string> WebSocketClient::make_topics(const std::string& prefix,
                                                      const std::vector<std::string>& symbols,
                                                      const std::string& suffix) {
  std::vector<std::string> topics;
  topics.reserve(symbols.size());
  for (const auto& symbol : symbols) {
    topics.push_back(prefix + symbol + suffix);
  }
  return topics;
}

std::string WebSocketClient::serialize_args(const std::vector<std::string>& topics) const {
  std::string args;
  for (size_t i = 0; i < topics.size(); ++i) {
    if (i > 0)
      args += ",";
    args += "\"" + topics[i] + "\"";
  }
  return "[" + args + "]";
}

std::string WebSocketClient::private_topic(const std::string& topic, const std::string& category) {
  if (category.empty()) {
    return topic;
  }
  return topic + "." + category;
}

void WebSocketClient::subscribe_tickers(const std::vector<std::string>& symbols, const std::string& req_id) {
  subscribe(make_topics("tickers.", symbols), req_id);
}

void WebSocketClient::subscribe_orderbook(const std::vector<std::string>& symbols, int depth,
                                          const std::string& req_id) {
  subscribe(make_topics("orderbook." + std::to_string(depth) + ".", symbols), req_id);
}

void WebSocketClient::subscribe_kline(const std::vector<std::string>& symbols, const std::string& interval,
                                      const std::string& req_id) {
  subscribe(make_topics("kline." + interval + ".", symbols), req_id);
}

void WebSocketClient::subscribe_public_trades(const std::vector<std::string>& symbols, const std::string& req_id) {
  subscribe(make_topics("publicTrade.", symbols), req_id);
}

void WebSocketClient::subscribe_all_liquidation(const std::vector<std::string>& symbols, const std::string& req_id) {
  subscribe(make_topics("allLiquidation.", symbols), req_id);
}

void WebSocketClient::subscribe_sbe_bbo(const std::vector<std::string>& symbols, const std::string& req_id) {
  subscribe(make_topics("ob.rpi.1.sbe.", symbols), req_id);
}

void WebSocketClient::subscribe_sbe_orderbook_50(const std::vector<std::string>& symbols, const std::string& req_id) {
  subscribe(make_topics("ob.50.sbe.", symbols), req_id);
}

void WebSocketClient::subscribe_sbe_public_trades(const std::vector<std::string>& symbols, const std::string& req_id) {
  subscribe(make_topics("publicTrade.sbe.", symbols), req_id);
}

void WebSocketClient::subscribe_private_order(const std::string& category, const std::string& req_id) {
  subscribe_topic(private_topic("order", category), req_id);
}

void WebSocketClient::subscribe_private_execution(const std::string& category, const std::string& req_id) {
  subscribe_topic(private_topic("execution", category), req_id);
}

void WebSocketClient::subscribe_private_position(const std::string& category, const std::string& req_id) {
  subscribe_topic(private_topic("position", category), req_id);
}

void WebSocketClient::subscribe_private_wallet(const std::string& req_id) {
  subscribe_topic("wallet", req_id);
}

void WebSocketClient::subscribe_private_greeks(const std::string& req_id) {
  subscribe_topic("greeks", req_id);
}

void WebSocketClient::unsubscribe_tickers(const std::vector<std::string>& symbols, const std::string& req_id) {
  unsubscribe(make_topics("tickers.", symbols), req_id);
}

void WebSocketClient::unsubscribe_orderbook(const std::vector<std::string>& symbols, int depth,
                                            const std::string& req_id) {
  unsubscribe(make_topics("orderbook." + std::to_string(depth) + ".", symbols), req_id);
}

void WebSocketClient::unsubscribe_kline(const std::vector<std::string>& symbols, const std::string& interval,
                                        const std::string& req_id) {
  unsubscribe(make_topics("kline." + interval + ".", symbols), req_id);
}

void WebSocketClient::unsubscribe_public_trades(const std::vector<std::string>& symbols, const std::string& req_id) {
  unsubscribe(make_topics("publicTrade.", symbols), req_id);
}

void WebSocketClient::unsubscribe_all_liquidation(const std::vector<std::string>& symbols, const std::string& req_id) {
  unsubscribe(make_topics("allLiquidation.", symbols), req_id);
}

void WebSocketClient::unsubscribe_sbe_bbo(const std::vector<std::string>& symbols, const std::string& req_id) {
  unsubscribe(make_topics("ob.rpi.1.sbe.", symbols), req_id);
}

void WebSocketClient::unsubscribe_sbe_orderbook_50(const std::vector<std::string>& symbols, const std::string& req_id) {
  unsubscribe(make_topics("ob.50.sbe.", symbols), req_id);
}

void WebSocketClient::unsubscribe_sbe_public_trades(const std::vector<std::string>& symbols,
                                                    const std::string& req_id) {
  unsubscribe(make_topics("publicTrade.sbe.", symbols), req_id);
}

void WebSocketClient::unsubscribe_private_order(const std::string& category, const std::string& req_id) {
  unsubscribe_topic(private_topic("order", category), req_id);
}

void WebSocketClient::unsubscribe_private_execution(const std::string& category, const std::string& req_id) {
  unsubscribe_topic(private_topic("execution", category), req_id);
}

void WebSocketClient::unsubscribe_private_position(const std::string& category, const std::string& req_id) {
  unsubscribe_topic(private_topic("position", category), req_id);
}

void WebSocketClient::unsubscribe_private_wallet(const std::string& req_id) {
  unsubscribe_topic("wallet", req_id);
}

void WebSocketClient::unsubscribe_private_greeks(const std::string& req_id) {
  unsubscribe_topic("greeks", req_id);
}

}  // namespace bybit
