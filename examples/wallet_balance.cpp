#include <cstdlib>
#include <iostream>
#include <optional>
#include <string>

#include "bybit/rest_client.hpp"

int main(int argc, char** argv) {
  const char* key = std::getenv("BYBIT_API_KEY");
  const char* secret = std::getenv("BYBIT_API_SECRET");
  const char* coins = std::getenv("BYBIT_COINS");  // optional comma-separated uppercase list

  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << " <category>\n";
    std::cerr << "  category examples: UNIFIED, linear, inverse, spot\n";
    return 1;
  }

  const std::string category = argv[1];

  if (!key || !secret) {
    std::cerr << "Please set BYBIT_API_KEY and BYBIT_API_SECRET\n";
    return 1;
  }

  try {
    bybit::RestClient client{key, secret, category};
    std::optional<std::string> coins_opt = (coins && *coins) ? std::optional<std::string>{coins} : std::nullopt;
    auto balances = client.get_wallet_balance(category, coins_opt);
    std::cout << "Wallet balance response:\n" << balances << "\n";
  } catch (const std::exception& ex) {
    std::cerr << "Error: " << ex.what() << "\n";
    return 1;
  }
  return 0;
}
