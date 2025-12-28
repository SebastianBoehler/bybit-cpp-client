#include <cstdlib>
#include <iostream>

#include "bybit/rest_client.hpp"

int main() {
  const char* key = std::getenv("BYBIT_API_KEY");
  const char* secret = std::getenv("BYBIT_API_SECRET");
  const char* category = std::getenv("BYBIT_CATEGORY");
  if (!key || !secret || !category) {
    std::cout << "BYBIT_API_KEY/BYBIT_API_SECRET/BYBIT_CATEGORY not set; skipping readonly test.\n";
    return 0;  // Skip gracefully
  }

  try {
    bybit::RestClient client{key, secret, category};
    auto api_key_info = client.get_query_api_key();
    auto account_info = client.get_account_info();
    auto instruments = client.get_instruments_info(50);
    // Print minimal lengths to confirm responses without dumping secrets
    std::cout << "api_key_info length: " << api_key_info.size() << "\n";
    std::cout << "account_info length: " << account_info.size() << "\n";
    std::cout << "instruments length: " << instruments.size() << "\n";
    return 0;
  } catch (const std::exception& ex) {
    std::cerr << "Readonly test failed: " << ex.what() << "\n";
    return 1;
  }
}
