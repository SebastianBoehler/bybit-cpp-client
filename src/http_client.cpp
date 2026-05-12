#include "bybit/http_client.hpp"

#include <curl/curl.h>

#include <algorithm>
#include <mutex>
#include <sstream>
#include <stdexcept>

#include "bybit/signing.hpp"

namespace bybit {
namespace {
size_t write_callback(char* ptr, size_t size, size_t nmemb, void* userdata) {
  auto real_size = size * nmemb;
  auto* buffer = static_cast<std::string*>(userdata);
  buffer->append(ptr, real_size);
  return real_size;
}

std::string join_url(const std::string& base, const std::string& path) {
  if (!base.empty() && base.back() == '/' && !path.empty() && path.front() == '/') {
    return base + path.substr(1);
  }
  if (!base.empty() && base.back() != '/' && !path.empty() && path.front() != '/') {
    return base + "/" + path;
  }
  return base + path;
}

std::string json_escape(const std::string& input) {
  std::ostringstream oss;
  for (char c : input) {
    switch (c) {
      case '"':
        oss << "\\\"";
        break;
      case '\\':
        oss << "\\\\";
        break;
      default:
        oss << c;
        break;
    }
  }
  return oss.str();
}

void ensure_curl_global() {
  static bool initialized = false;
  static std::mutex init_mutex;
  std::lock_guard<std::mutex> lock(init_mutex);
  if (!initialized) {
    if (curl_global_init(CURL_GLOBAL_DEFAULT) != 0) {
      throw std::runtime_error("Failed to init curl");
    }
    initialized = true;
  }
}

void apply_options(CURL* curl, const HttpOptions& options) {
  curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT_MS, options.connect_timeout_ms);
  curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, options.request_timeout_ms);
  curl_easy_setopt(curl, CURLOPT_DNS_CACHE_TIMEOUT, options.dns_cache_timeout_seconds);
  curl_easy_setopt(curl, CURLOPT_TCP_KEEPALIVE, options.tcp_keepalive ? 1L : 0L);
  if (options.tcp_keepalive) {
    curl_easy_setopt(curl, CURLOPT_TCP_KEEPIDLE, options.tcp_keepidle_seconds);
    curl_easy_setopt(curl, CURLOPT_TCP_KEEPINTVL, options.tcp_keepintvl_seconds);
  }
  if (!options.proxy.empty()) {
    curl_easy_setopt(curl, CURLOPT_PROXY, options.proxy.c_str());
  }
  if (!options.user_agent.empty()) {
    curl_easy_setopt(curl, CURLOPT_USERAGENT, options.user_agent.c_str());
  }
}

}  // namespace

std::string to_json_object(const std::vector<std::pair<std::string, std::string>>& kvs) {
  std::ostringstream oss;
  oss << "{";
  for (size_t i = 0; i < kvs.size(); ++i) {
    oss << "\"" << json_escape(kvs[i].first) << "\":\"" << json_escape(kvs[i].second) << "\"";
    if (i + 1 < kvs.size()) oss << ",";
  }
  oss << "}";
  return oss.str();
}

std::string canonical_query(const std::vector<std::pair<std::string, std::string>>& params) {
  std::vector<std::pair<std::string, std::string>> sorted = params;
  std::sort(sorted.begin(), sorted.end(), [](const auto& a, const auto& b) { return a.first < b.first; });
  std::ostringstream oss;
  for (size_t i = 0; i < sorted.size(); ++i) {
    oss << sorted[i].first << "=" << sorted[i].second;
    if (i + 1 < sorted.size()) oss << "&";
  }
  return oss.str();
}

HttpClient::HttpClient(std::string api_key, std::string api_secret, std::string base_url, std::string recv_window,
                       HttpOptions options)
    : api_key_(std::move(api_key)),
      api_secret_(std::move(api_secret)),
      base_url_(std::move(base_url)),
      recv_window_(std::move(recv_window)),
      options_(std::move(options)),
      curl_(nullptr) {
  ensure_curl_global();
  curl_ = curl_easy_init();
  if (!curl_) {
    throw std::runtime_error("Failed to init curl easy handle");
  }
  apply_options(curl_, options_);
}

HttpClient::~HttpClient() {
  if (curl_) {
    curl_easy_cleanup(curl_);
  }
}

std::string HttpClient::get(const std::string& path, const std::vector<std::pair<std::string, std::string>>& params,
                            bool is_private) const {
  std::vector<std::pair<std::string, std::string>> final_params = params;
  if (is_private) {
    final_params.emplace_back("recvWindow", recv_window_);
  }

  std::string query = canonical_query(final_params);
  std::string url = join_url(base_url_, path);
  if (!query.empty()) {
    url += "?" + query;
  }

  std::string payload = query;
  std::string response;
  std::lock_guard<std::mutex> lock(curl_mutex_);

  struct curl_slist* headers = nullptr;
  if (is_private) {
    auto signed_req = Signer::sign(api_key_, api_secret_, payload, recv_window_);
    headers = curl_slist_append(headers, ("X-BAPI-API-KEY: " + api_key_).c_str());
    headers = curl_slist_append(headers, ("X-BAPI-TIMESTAMP: " + signed_req.timestamp).c_str());
    headers = curl_slist_append(headers, ("X-BAPI-RECV-WINDOW: " + signed_req.recv_window).c_str());
    headers = curl_slist_append(headers, ("X-BAPI-SIGN: " + signed_req.signature).c_str());
  }

  curl_easy_reset(curl_);
  apply_options(curl_, options_);
  curl_easy_setopt(curl_, CURLOPT_URL, url.c_str());
  curl_easy_setopt(curl_, CURLOPT_HTTPGET, 1L);
  curl_easy_setopt(curl_, CURLOPT_WRITEFUNCTION, write_callback);
  curl_easy_setopt(curl_, CURLOPT_WRITEDATA, &response);
  if (headers) curl_easy_setopt(curl_, CURLOPT_HTTPHEADER, headers);

  auto res = curl_easy_perform(curl_);
  long status = 0;
  curl_easy_getinfo(curl_, CURLINFO_RESPONSE_CODE, &status);

  if (headers) curl_slist_free_all(headers);

  if (res != CURLE_OK) {
    throw std::runtime_error(std::string("curl perform failed: ") + curl_easy_strerror(res));
  }
  if (status >= 400) {
    std::ostringstream oss;
    oss << "HTTP status " << status << " body: " << response;
    throw std::runtime_error(oss.str());
  }
  return response;
}

std::string HttpClient::post(const std::string& path, const std::string& body, bool is_private) const {
  std::string url = join_url(base_url_, path);
  std::string payload = body;
  std::string response;
  std::lock_guard<std::mutex> lock(curl_mutex_);

  struct curl_slist* headers = nullptr;
  headers = curl_slist_append(headers, "Content-Type: application/json");
  if (is_private) {
    auto signed_req = Signer::sign(api_key_, api_secret_, payload, recv_window_);
    headers = curl_slist_append(headers, ("X-BAPI-API-KEY: " + api_key_).c_str());
    headers = curl_slist_append(headers, ("X-BAPI-TIMESTAMP: " + signed_req.timestamp).c_str());
    headers = curl_slist_append(headers, ("X-BAPI-RECV-WINDOW: " + signed_req.recv_window).c_str());
    headers = curl_slist_append(headers, ("X-BAPI-SIGN: " + signed_req.signature).c_str());
  }

  curl_easy_reset(curl_);
  apply_options(curl_, options_);
  curl_easy_setopt(curl_, CURLOPT_URL, url.c_str());
  curl_easy_setopt(curl_, CURLOPT_POST, 1L);
  curl_easy_setopt(curl_, CURLOPT_POSTFIELDS, body.c_str());
  curl_easy_setopt(curl_, CURLOPT_WRITEFUNCTION, write_callback);
  curl_easy_setopt(curl_, CURLOPT_WRITEDATA, &response);
  curl_easy_setopt(curl_, CURLOPT_HTTPHEADER, headers);

  auto res = curl_easy_perform(curl_);
  long status = 0;
  curl_easy_getinfo(curl_, CURLINFO_RESPONSE_CODE, &status);

  curl_slist_free_all(headers);

  if (res != CURLE_OK) {
    throw std::runtime_error(std::string("curl perform failed: ") + curl_easy_strerror(res));
  }
  if (status >= 400) {
    std::ostringstream oss;
    oss << "HTTP status " << status << " body: " << response;
    throw std::runtime_error(oss.str());
  }
  return response;
}

}  // namespace bybit
