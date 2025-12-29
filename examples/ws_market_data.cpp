#include <chrono>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <thread>
#include <vector>

#include "bybit/websocket_client.hpp"

std::string getenv_or(const char* key, const std::string& fallback) {
  if (const char* v = std::getenv(key)) return std::string(v);
  return fallback;
}

std::vector<std::string> split_symbols(const std::string& csv) {
  std::vector<std::string> out;
  std::stringstream ss(csv);
  std::string item;
  while (std::getline(ss, item, ',')) {
    if (!item.empty()) out.push_back(item);
  }
  return out;
}

// Market-data demo: tickers, orderbook depth-1, klines, public trades.
int main() {
  // Default to mainnet linear; override with BYBIT_WS_ENDPOINT to use spot/inverse/testnet/etc.
  const std::string endpoint = getenv_or("BYBIT_WS_ENDPOINT", "wss://stream.bybit.com/v5/public/linear");
  const std::string symbols_csv = getenv_or("BYBIT_WS_SYMBOLS", "BTCUSDT,ETHUSDT");
  const auto symbols = split_symbols(symbols_csv);
  const int runtime_sec = 120;

  std::cerr << "[demo] endpoint=" << endpoint << " symbols=" << symbols_csv << " runtime=" << runtime_sec
            << "s (tickers/orderbook/kline/trades)\n";

  bybit::WebSocketClient ws{endpoint};

  ws.enable_auto_reconnect(true, 8);

  ws.set_message_handler([](const std::string& msg) {
    // Print raw JSON messages (including subscribes and data deltas).
    std::cout << msg << std::endl;
  });

  ws.connect();

  // Subscribe multiple topics/symbols (Bybit format).
  std::vector<std::string> orderbook_topics;
  for (const auto& s : symbols) {
    orderbook_topics.push_back("orderbook.1." + s);
  }
  ws.subscribe_tickers(symbols, "tickers");
  ws.subscribe(orderbook_topics, "orderbook");
  ws.subscribe_kline(symbols, "1", "kline-1");  // 1-minute bars
  ws.subscribe_public_trades(symbols, "trades-1");

  std::this_thread::sleep_for(std::chrono::seconds(runtime_sec));

  // Optional unsubscribe; keep connection up to observe data until process exit.
  // ws.unsubscribe(orderbook_topics, "orderbook");
  ws.close();
  return 0;
}
