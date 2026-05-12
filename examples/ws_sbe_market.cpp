#include <chrono>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <thread>
#include <vector>

#include "bybit/websocket_client.hpp"

std::vector<std::string> split_csv(const std::string& csv) {
  std::vector<std::string> values;
  std::stringstream stream(csv);
  std::string value;
  while (std::getline(stream, value, ',')) {
    if (!value.empty()) values.push_back(value);
  }
  return values;
}

int main() {
  const char* endpoint_env = std::getenv("BYBIT_SBE_WS_ENDPOINT");
  if (!endpoint_env) {
    std::cerr << "Set BYBIT_SBE_WS_ENDPOINT to your Bybit MMWS public-sbe endpoint.\n";
    return 1;
  }
  const std::string endpoint{endpoint_env};
  const char* symbols_env = std::getenv("BYBIT_WS_SYMBOLS");
  const auto symbols = split_csv(symbols_env ? std::string(symbols_env) : "BTCUSDT,ETHUSDT");

  bybit::WebSocketClient ws{endpoint};
  ws.enable_auto_reconnect(true, 8);

  ws.set_message_handler([](const std::string& message) {
    std::cout << message << std::endl;
  });
  ws.set_binary_message_handler([](const std::string& payload) {
    std::cout << "binary SBE payload bytes=" << payload.size() << std::endl;
  });

  ws.connect();
  ws.subscribe_sbe_bbo(symbols, "sbe-bbo");
  ws.subscribe_sbe_orderbook_50(symbols, "sbe-ob-50");
  ws.subscribe_sbe_public_trades(symbols, "sbe-trades");

  std::this_thread::sleep_for(std::chrono::seconds(30));
  ws.close();
  return 0;
}
