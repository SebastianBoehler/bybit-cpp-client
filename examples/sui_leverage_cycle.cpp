#include <chrono>
#include <cstdlib>
#include <iostream>
#include <string>

#include "bybit/rest_client.hpp"

// Demonstrates:
// 1) Set leverage to 2x for SUIUSDT (linear/perp)
// 2) Open a small long position
// 3) Bump leverage to 4x and fetch positions to observe margin change
// 4) Close the position
//
// NOTE: This places real orders if your API key has trading permissions.
// Use testnet keys or very small size. Ensure BYBIT_API_KEY/SECRET are set in your environment.
int main() {
  const char* key = std::getenv("BYBIT_API_KEY");
  const char* secret = std::getenv("BYBIT_API_SECRET");
  if (!key || !secret) {
    std::cerr << "Please set BYBIT_API_KEY and BYBIT_API_SECRET\n";
    return 1;
  }

  const std::string symbol = "SUIUSDT";
  // Adjust the qty to your risk tolerance and account settings.
  // SUIUSDT min notional typically requires >1 contract; using 10 here.
  const std::string qty = "10";  // contracts

  bybit::RestClient client{key, secret, "linear"};

  try {
    // Unique order link IDs
    auto now_ms =
        std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch())
            .count();
    std::string open_link_id = "sui-open-" + std::to_string(now_ms);
    std::string close_link_id = "sui-close-" + std::to_string(now_ms);

    std::cout << "Setting leverage to 2x...\n";
    std::cout << client.set_leverage(symbol, "2", "2") << "\n\n";

    std::cout << "Opening long position (market buy)...\n";
    std::cout << client.submit_order(symbol, "Buy", "Market", qty, open_link_id, 1) << "\n\n";

    std::cout << "Positions after open:\n";
    std::cout << client.get_position_info("USDT") << "\n\n";

    std::cout << "Bumping leverage to 4x...\n";
    std::cout << client.set_leverage(symbol, "4", "4") << "\n\n";

    std::cout << "Positions after leverage change:\n";
    std::cout << client.get_position_info("USDT") << "\n\n";

    std::cout << "Closing position (market sell)...\n";
    std::cout << client.submit_order(symbol, "Sell", "Market", qty, close_link_id, 1) << "\n\n";

    std::cout << "Positions after close:\n";
    std::cout << client.get_position_info("USDT") << "\n";
  } catch (const std::exception& ex) {
    std::cerr << "Error: " << ex.what() << "\n";
    return 1;
  }

  return 0;
}
