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

    auto server_time = client.get_server_time();
    if (server_time.find("timeSecond") == std::string::npos) {
      std::cerr << "Server time response did not include timeSecond\n";
      return 1;
    }
    return 0;
  } catch (const bybit::HttpError& ex) {
    if (ex.status_code() == 403 || ex.body().find("CloudFront") != std::string::npos) {
      std::cerr << "Public test warning: access blocked from runner region (403). Marking as skip.\n";
      return 0;
    }
    std::cerr << "Public test failed: " << ex.what() << "\n";
    return 1;
  } catch (const std::exception& ex) {
    const std::string msg = ex.what();
    if (msg.find("HTTP status 403") != std::string::npos || msg.find("CloudFront") != std::string::npos) {
      std::cerr << "Public test warning: access blocked from runner region (403). Marking as skip.\n";
      return 0;
    }
    std::cerr << "Public test failed: " << ex.what() << "\n";
    return 1;
  }
}
