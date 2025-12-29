#pragma once

#include <openssl/evp.h>

#include <string>
#include <vector>

namespace bybit {

struct SignedRequest {
  std::string timestamp;
  std::string recv_window;
  std::string signature;
};

class Signer {
 public:
  // Computes signature for Bybit v5: sign(timestamp + apiKey + recvWindow + payload)
  static SignedRequest sign(const std::string& api_key, const std::string& api_secret, const std::string& payload,
                            const std::string& recv_window = "5000");

  // Deterministic variant for testing or websocket auth that allows supplying a timestamp explicitly.
  static SignedRequest sign_with_timestamp(const std::string& api_key, const std::string& api_secret,
                                           const std::string& payload, const std::string& timestamp,
                                           const std::string& recv_window = "5000");
};

}  // namespace bybit
