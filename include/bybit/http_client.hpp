#pragma once

#include <curl/curl.h>

#include <mutex>
#include <optional>
#include <string>
#include <vector>

namespace bybit {

// Lightweight HTTP helper shared by public/private clients.
class HttpClient {
 public:
  HttpClient(std::string api_key, std::string api_secret, std::string base_url, std::string recv_window);
  ~HttpClient();

  HttpClient(const HttpClient&) = delete;
  HttpClient& operator=(const HttpClient&) = delete;
  HttpClient(HttpClient&&) = delete;
  HttpClient& operator=(HttpClient&&) = delete;

  // GET with optional signing.
  std::string get(const std::string& path, const std::vector<std::pair<std::string, std::string>>& params,
                  bool is_private) const;

  // POST with optional signing; body must be JSON.
  std::string post(const std::string& path, const std::string& body, bool is_private) const;

  const std::string& base_url() const { return base_url_; }
  const std::string& recv_window() const { return recv_window_; }
  const std::string& api_key() const { return api_key_; }
  const std::string& api_secret() const { return api_secret_; }

 private:
  std::string api_key_;
  std::string api_secret_;
  std::string base_url_;
  std::string recv_window_;
  mutable std::mutex curl_mutex_;
  CURL* curl_;
};

std::string to_json_object(const std::vector<std::pair<std::string, std::string>>& kvs);
std::string canonical_query(const std::vector<std::pair<std::string, std::string>>& params);

}  // namespace bybit
