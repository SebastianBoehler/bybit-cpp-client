#include <cstdlib>
#include <iostream>
#include <optional>
#include <string>

#include "bybit/rest_client.hpp"

int main() {
  const char* key = std::getenv("BYBIT_API_KEY");
  const char* secret = std::getenv("BYBIT_API_SECRET");
  const char* category = std::getenv("BYBIT_CATEGORY");  // e.g., "UNIFIED"
  const char* coins = std::getenv("BYBIT_COINS");        // optional comma-separated uppercase list
  if (!key || !secret || !category) {
    std::cerr << "Please set BYBIT_API_KEY, BYBIT_API_SECRET, and BYBIT_CATEGORY\n";
    return 1;
  }

  try {
    bybit::RestClient client{key, secret, category};
    std::optional<std::string> coins_opt = (coins && *coins) ? std::optional<std::string>{coins} : std::nullopt;
    auto balances = client.get_wallet_balance(coins_opt);
    std::cout << "Wallet balance response:\n" << balances << "\n";
  } catch (const std::exception& ex) {
    std::cerr << "Error: " << ex.what() << "\n";
    return 1;
  }
  return 0;
}
