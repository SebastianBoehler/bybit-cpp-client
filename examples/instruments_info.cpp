#include <cstdlib>
#include <iostream>
#include <string>

#include "bybit/rest_client.hpp"

// Fetches instrument info for a given category and symbol, prints raw JSON.
int main() {
  const char* key = std::getenv("BYBIT_API_KEY");
  const char* secret = std::getenv("BYBIT_API_SECRET");
  if (!key || !secret) {
    std::cerr << "Please set BYBIT_API_KEY and BYBIT_API_SECRET\n";
    return 1;
  }

  const std::string symbol = "SUIUSDT";
  bybit::RestClient client{key, secret, "linear"};

  try {
    std::cout << "Instrument info for " << symbol << ":\n";
    std::cout << client.get_instruments_info(50) << "\n";
  } catch (const std::exception& ex) {
    std::cerr << "Error: " << ex.what() << "\n";
    return 1;
  }

  return 0;
}
