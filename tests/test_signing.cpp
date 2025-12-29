#include <iostream>

#include "bybit/signing.hpp"

int main() {
  const std::string api_key = "key";
  const std::string api_secret = "secret";
  const std::string timestamp = "1700000000000";
  const std::string recv_window = "5000";
  const std::string payload = "";

  auto signed_req = bybit::Signer::sign_with_timestamp(api_key, api_secret, payload, timestamp, recv_window);

  const std::string expected =
      "409b3edd049b76dda5f93952b22658a4ccbd498ce7934755f12cb9249b80777a";  // computed via python/hmac

  if (signed_req.signature != expected) {
    std::cerr << "Unexpected signature: " << signed_req.signature << "\n";
    return 1;
  }
  if (signed_req.timestamp != timestamp || signed_req.recv_window != recv_window) {
    std::cerr << "Unexpected timestamp/recv_window\n";
    return 1;
  }
  return 0;
}
