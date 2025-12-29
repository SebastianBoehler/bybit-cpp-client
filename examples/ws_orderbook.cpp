#include <chrono>
#include <iostream>
#include <thread>
#include <vector>

#include "bybit/websocket_client.hpp"

// Order book streaming demo: subscribes to depth-50 book updates for given symbols.
int main() {
  const std::string endpoint = std::getenv("BYBIT_WS_ENDPOINT") ? std::string(std::getenv("BYBIT_WS_ENDPOINT"))
                                                                : "wss://stream.bybit.com/v5/public/linear";
  const std::string symbols_env =
      std::getenv("BYBIT_WS_SYMBOLS") ? std::string(std::getenv("BYBIT_WS_SYMBOLS")) : "BTCUSDT,ETHUSDT";

  std::vector<std::string> symbols;
  {
    std::stringstream ss(symbols_env);
    std::string item;
    while (std::getline(ss, item, ',')) {
      if (!item.empty()) symbols.push_back(item);
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
