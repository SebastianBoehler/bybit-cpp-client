#include <cstdlib>
#include <iostream>
#include <optional>
#include <string>

#include "bybit/rest_client.hpp"

// Fetch recent klines (1m) for BTCUSDT in linear category.
int main() {
  const char* key = std::getenv("BYBIT_API_KEY");
  const char* secret = std::getenv("BYBIT_API_SECRET");
  if (!key || !secret) {
    std::cerr << "Please set BYBIT_API_KEY and BYBIT_API_SECRET\n";
    return 1;
  }

  bybit::RestClient client{key, secret, "linear"};
  try {
    std::cout << "Latest 5 BTCUSDT 1m klines:\n";
    std::cout << client.get_kline("BTCUSDT", "1", std::optional<std::string>{}, std::optional<std::string>{}, 5)
              << "\n";
  } catch (const std::exception& ex) {
    std::cerr << "Error: " << ex.what() << "\n";
    return 1;
  }

  return 0;
}
