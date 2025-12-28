#include <iostream>

#include "bybit/rest_client.hpp"

int main() {
  // Public endpoints do not require auth; empty key/secret is acceptable for public calls
  bybit::RestClient client{"", "", "linear"};
  try {
    auto instruments = client.get_instruments_info(10);
    std::cout << "instruments length: " << instruments.size() << "\n";
    if (instruments.empty()) {
      std::cerr << "Public instruments call returned empty list\n";
      return 1;
    }
    return 0;
  } catch (const std::exception& ex) {
    std::cerr << "Public test failed: " << ex.what() << "\n";
    return 1;
  }
}
