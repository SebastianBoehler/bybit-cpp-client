#include <cstdlib>
#include <iostream>

#include "bybit/rest_client.hpp"

int main() {
  const char* key = std::getenv("BYBIT_API_KEY");
  const char* secret = std::getenv("BYBIT_API_SECRET");
  if (!key || !secret) {
    std::cerr << "Please set BYBIT_API_KEY and BYBIT_API_SECRET\n";
    return 1;
  }

  // Spot category
  bybit::RestClient client{key, secret, "spot"};
  try {
    auto instruments = client.get_instruments_info(50);
    std::cout << "instruments length: " << instruments.size() << "\n";
    // Example market order (disabled by default):
    // client.submit_order("BTCUSDT", "Buy", "Market", "0.001", "spot-demo", 1);
  } catch (const std::exception& ex) {
    std::cerr << "Error: " << ex.what() << "\n";
    return 1;
  }
  return 0;
}
