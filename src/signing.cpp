#include "bybit/signing.hpp"

#include <openssl/hmac.h>

#include <chrono>
#include <iomanip>
#include <sstream>
#include <stdexcept>

namespace bybit {

namespace {
std::string hmac_sha256_hex(const std::string& key, const std::string& data) {
  unsigned int len = EVP_MAX_MD_SIZE;
  unsigned char hash[EVP_MAX_MD_SIZE];

  HMAC_CTX* ctx = HMAC_CTX_new();
  if (!ctx) throw std::runtime_error("Failed to allocate HMAC_CTX");

  if (HMAC_Init_ex(ctx, key.data(), static_cast<int>(key.size()), EVP_sha256(), nullptr) != 1) {
    HMAC_CTX_free(ctx);
    throw std::runtime_error("HMAC_Init_ex failed");
  }
  if (HMAC_Update(ctx, reinterpret_cast<const unsigned char*>(data.data()), data.size()) != 1) {
    HMAC_CTX_free(ctx);
    throw std::runtime_error("HMAC_Update failed");
  }
  if (HMAC_Final(ctx, hash, &len) != 1) {
    HMAC_CTX_free(ctx);
    throw std::runtime_error("HMAC_Final failed");
  }
  HMAC_CTX_free(ctx);

  std::ostringstream oss;
  for (unsigned int i = 0; i < len; ++i) {
    oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(hash[i]);
  }
  return oss.str();
}
}  // namespace

SignedRequest Signer::sign(const std::string& api_key, const std::string& api_secret, const std::string& payload,
                           const std::string& recv_window) {
  auto now = std::chrono::system_clock::now();
  auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
  std::string timestamp = std::to_string(ms);

  std::string to_sign = timestamp + api_key + recv_window + payload;
  std::string signature = hmac_sha256_hex(api_secret, to_sign);
  return {timestamp, recv_window, signature};
}

}  // namespace bybit
