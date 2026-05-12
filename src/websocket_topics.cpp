#include "bybit/websocket_client.hpp"

#include <string>
#include <vector>

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
    if (i > 0) args += ",";
    args += "\"" + topics[i] + "\"";
  }
  return "[" + args + "]";
}

}  // namespace bybit
