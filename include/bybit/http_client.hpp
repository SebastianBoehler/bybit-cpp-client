#pragma once

#include <curl/curl.h>

#include <mutex>
#include <optional>
#include <stdexcept>
#include <string>
#include <vector>

namespace bybit {

struct HttpOptions {
  long connect_timeout_ms = 10000;
  long request_timeout_ms = 30000;
  long dns_cache_timeout_seconds = 300;
  long max_connections = 8;
  bool share_dns_and_ssl_session_cache = true;
  bool tcp_keepalive = true;
  bool tcp_nodelay = true;
  long tcp_keepidle_seconds = 60;
  long tcp_keepintvl_seconds = 30;
  std::string proxy;
  std::string user_agent = "bybit-cpp-client/0.1.0";
};

class HttpError : public std::runtime_error {
 public:
  HttpError(long status_code, std::string body);
  long status_code() const { return status_code_; }
  const std::string& body() const { return body_; }

 private:
  long status_code_;
  std::string body_;
};

// Lightweight HTTP helper shared by public/private clients.
class HttpClient {
 public:
  HttpClient(std::string api_key, std::string api_secret, std::string base_url, std::string recv_window,
             HttpOptions options = {});
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

  // POST with query parameters and an empty JSON body.
  std::string post_query(const std::string& path, const std::vector<std::pair<std::string, std::string>>& params,
                         bool is_private) const;

  const std::string& base_url() const { return base_url_; }
  const std::string& recv_window() const { return recv_window_; }
  const std::string& api_key() const { return api_key_; }
  const std::string& api_secret() const { return api_secret_; }
  const HttpOptions& options() const { return options_; }

 private:
  std::string api_key_;
  std::string api_secret_;
  std::string base_url_;
  std::string recv_window_;
  HttpOptions options_;
  mutable std::mutex curl_mutex_;
  CURL* curl_;
};

std::string to_json_object(const std::vector<std::pair<std::string, std::string>>& kvs);
std::string canonical_query(const std::vector<std::pair<std::string, std::string>>& params);

}  // namespace bybit
