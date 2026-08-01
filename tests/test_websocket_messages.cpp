#include <iostream>
#include <string>

#include "bybit/detail/websocket_messages.hpp"
#include "bybit/json.hpp"

int main() {
  const std::string control{"line\b\f\n\r\t\x01"};
  if (bybit::serialize_json_string(control) != R"("line\b\f\n\r\t\u0001")") {
    std::cerr << "RFC 8259 control escaping drifted\n";
    return 1;
  }
  if (bybit::serialize_json_string_array({"tickers.BTC\"USDT", control}) !=
      R"(["tickers.BTC\"USDT","line\b\f\n\r\t\u0001"])") {
    std::cerr << "websocket topic array escaping drifted\n";
    return 1;
  }
  if (bybit::detail::make_subscription_message("subscribe", {"topic\"one"}, "req\n1") !=
      R"({"op":"subscribe","args":["topic\"one"],"req_id":"req\n1"})") {
    std::cerr << "subscription envelope escaping drifted\n";
    return 1;
  }
  if (bybit::detail::make_ping_message("ping\"1") != R"({"op":"ping","req_id":"ping\"1"})") {
    std::cerr << "ping envelope escaping drifted\n";
    return 1;
  }
  if (bybit::detail::make_auth_message("key\"one", "123", "abc") != R"({"op":"auth","args":["key\"one",123,"abc"]})") {
    std::cerr << "auth envelope escaping drifted\n";
    return 1;
  }
  return 0;
}
