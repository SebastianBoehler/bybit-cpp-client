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

  bybit::RestClient client{key, secret, "linear"};
  try {
    auto positions = client.get_position_info();
    auto instruments = client.get_instruments_info(50);
    std::cout << "positions length: " << positions.size() << "\n";
    std::cout << "instruments length: " << instruments.size() << "\n";
    // Example market order (disabled by default):
    // client.submit_order("BTCUSDT", "Buy", "Market", "0.001", "demo-order", 1);
  } catch (const std::exception& ex) {
    std::cerr << "Error: " << ex.what() << "\n";
    return 1;
  }
  return 0;
}
