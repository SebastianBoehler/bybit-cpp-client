#include "bybit/signing.hpp"

#include <openssl/core_names.h>
#include <openssl/evp.h>
#include <openssl/params.h>

#include <chrono>
#include <iomanip>
#include <memory>
#include <sstream>
#include <stdexcept>

namespace bybit {

namespace {
struct EvpMacDeleter {
  void operator()(EVP_MAC* mac) const {
    EVP_MAC_free(mac);
  }
};

struct EvpMacCtxDeleter {
  void operator()(EVP_MAC_CTX* ctx) const {
    EVP_MAC_CTX_free(ctx);
  }
};
}  // namespace

std::string Signer::hmac_sha256_hex(const std::string& key, const std::string& data) {
  std::unique_ptr<EVP_MAC, EvpMacDeleter> mac(EVP_MAC_fetch(nullptr, "HMAC", nullptr));
  if (!mac) {
    throw std::runtime_error("EVP_MAC_fetch(HMAC) failed");
  }

  std::unique_ptr<EVP_MAC_CTX, EvpMacCtxDeleter> ctx(EVP_MAC_CTX_new(mac.get()));
  if (!ctx) {
    throw std::runtime_error("EVP_MAC_CTX_new failed");
  }

  char digest[] = "SHA256";
  OSSL_PARAM params[] = {OSSL_PARAM_construct_utf8_string(OSSL_MAC_PARAM_DIGEST, digest, 0),
                         OSSL_PARAM_construct_end()};

  const auto* key_bytes = reinterpret_cast<const unsigned char*>(key.data());
  if (EVP_MAC_init(ctx.get(), key_bytes, key.size(), params) != 1) {
    throw std::runtime_error("EVP_MAC_init failed");
  }

  const auto* data_bytes = reinterpret_cast<const unsigned char*>(data.data());
  if (EVP_MAC_update(ctx.get(), data_bytes, data.size()) != 1) {
    throw std::runtime_error("EVP_MAC_update failed");
  }

  unsigned char hash[EVP_MAX_MD_SIZE];
  size_t len = 0;
  if (EVP_MAC_final(ctx.get(), hash, &len, sizeof(hash)) != 1) {
    throw std::runtime_error("EVP_MAC_final failed");
  }

  std::ostringstream oss;
  for (size_t i = 0; i < len; ++i) {
    oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(hash[i]);
  }
  return oss.str();
}

SignedRequest Signer::sign(const std::string& api_key, const std::string& api_secret, const std::string& payload,
                           const std::string& recv_window) {
  auto now = std::chrono::system_clock::now();
  auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
  std::string timestamp = std::to_string(ms);

  std::string to_sign = timestamp + api_key + recv_window + payload;
  std::string signature = Signer::hmac_sha256_hex(api_secret, to_sign);
  return {timestamp, recv_window, signature};
}

SignedRequest Signer::sign_with_timestamp(const std::string& api_key, const std::string& api_secret,
                                          const std::string& payload, const std::string& timestamp,
                                          const std::string& recv_window) {
  std::string to_sign = timestamp + api_key + recv_window + payload;
  std::string signature = Signer::hmac_sha256_hex(api_secret, to_sign);
  return {timestamp, recv_window, signature};
}

}  // namespace bybit
