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

std::vector<std::string> split_csv(const std::string& csv) {
  std::vector<std::string> out;
  std::stringstream ss(csv);
  std::string item;
  while (std::getline(ss, item, ',')) {
    if (!item.empty()) out.push_back(item);
  }
  return out;
}

// Private positions stream demo. Requires BYBIT_API_KEY / BYBIT_API_SECRET.
// Endpoint defaults to mainnet private: wss://stream.bybit.com/v5/private
int main() {
  const std::string endpoint = getenv_or("BYBIT_WS_ENDPOINT", "wss://stream.bybit.com/v5/private");
  const std::string api_key = getenv_or("BYBIT_API_KEY", "");
  const std::string api_secret = getenv_or("BYBIT_API_SECRET", "");
  const std::string topics_csv = getenv_or("BYBIT_WS_TOPICS", "position");
  const auto topics = split_csv(topics_csv);
  const int runtime_sec = 120;

  if (api_key.empty() || api_secret.empty()) {
    std::cerr << "[demo] BYBIT_API_KEY and BYBIT_API_SECRET are required for private topics\n";
    return 1;
  }

  std::cerr << "[demo] endpoint=" << endpoint << " topics=" << topics_csv << " runtime=" << runtime_sec
            << "s (private)\n";

  bybit::WebSocketClient ws{endpoint, api_key, api_secret};
  ws.enable_auto_reconnect(true, 8);

  ws.set_message_handler([](const std::string& msg) {
    // Print raw JSON messages from private stream (positions, executions, orders, etc.).
    std::cout << msg << std::endl;
  });

  ws.connect();
  ws.subscribe_topics(topics, "private");

  std::this_thread::sleep_for(std::chrono::seconds(runtime_sec));
  ws.close();
  return 0;
}
