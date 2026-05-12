#include <chrono>
#include <iostream>
#include <thread>
#include <vector>

#include "bybit/websocket_client.hpp"

// Order book streaming demo: subscribes to depth-50 book updates for given symbols.
int main() {
  const char* endpoint_env = std::getenv("BYBIT_WS_ENDPOINT");
  const char* symbols_value = std::getenv("BYBIT_WS_SYMBOLS");
  const std::string endpoint = endpoint_env ? std::string(endpoint_env) : "wss://stream.bybit.com/v5/public/linear";
  const std::string symbols_env = symbols_value ? std::string(symbols_value) : "BTCUSDT,ETHUSDT";

  std::vector<std::string> symbols;
  {
    std::stringstream ss(symbols_env);
    std::string item;
    while (std::getline(ss, item, ',')) {
      if (!item.empty())
        symbols.push_back(item);
    }
  }

  bybit::WebSocketClient ws{endpoint};

  ws.set_message_handler([](const std::string& msg) {
    // Print raw JSON; in practice you may want to parse and maintain a local book.
    std::cout << msg << std::endl;
  });

  ws.connect();

  constexpr int depth = 50;

  ws.subscribe_orderbook(symbols, depth, "ob-50");

  std::this_thread::sleep_for(std::chrono::seconds(10));

  ws.unsubscribe_orderbook(symbols, depth, "ob-50");
  ws.close();
  return 0;
}
