#include "bybit/http_client.hpp"

#include <curl/curl.h>

#include <mutex>
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

size_t header_callback(char* ptr, size_t size, size_t nmemb, void* userdata) {
  auto real_size = size * nmemb;
  std::string line(ptr, real_size);
  while (!line.empty() && (line.back() == '\r' || line.back() == '\n'))
    line.pop_back();
  auto colon = line.find(':');
  if (colon == std::string::npos)
    return real_size;
  size_t value_start = colon + 1;
  while (value_start < line.size() && line[value_start] == ' ')
    ++value_start;
  auto* headers = static_cast<HttpHeaders*>(userdata);
  headers->emplace_back(line.substr(0, colon), line.substr(value_start));
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

std::mutex& curl_share_mutex() {
  static std::mutex mutex;
  return mutex;
}

void curl_share_lock(CURL*, curl_lock_data, curl_lock_access, void*) {
  curl_share_mutex().lock();
}

void curl_share_unlock(CURL*, curl_lock_data, void*) {
  curl_share_mutex().unlock();
}

CURLSH* shared_curl_cache() {
  static CURLSH* share = [&]() {
    CURLSH* handle = curl_share_init();
    if (!handle)
      throw std::runtime_error("Failed to init curl share handle");
    if (curl_share_setopt(handle, CURLSHOPT_LOCKFUNC, curl_share_lock) != CURLSHE_OK ||
        curl_share_setopt(handle, CURLSHOPT_UNLOCKFUNC, curl_share_unlock) != CURLSHE_OK ||
        curl_share_setopt(handle, CURLSHOPT_SHARE, CURL_LOCK_DATA_DNS) != CURLSHE_OK ||
        curl_share_setopt(handle, CURLSHOPT_SHARE, CURL_LOCK_DATA_SSL_SESSION) != CURLSHE_OK) {
      curl_share_cleanup(handle);
      throw std::runtime_error("Failed to configure curl share handle");
    }
    return handle;
  }();
  return share;
}

void apply_options(CURL* curl, const HttpOptions& options) {
  curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT_MS, options.connect_timeout_ms);
  curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, options.request_timeout_ms);
  curl_easy_setopt(curl, CURLOPT_DNS_CACHE_TIMEOUT, options.dns_cache_timeout_seconds);
  curl_easy_setopt(curl, CURLOPT_MAXCONNECTS, options.max_connections);
  if (options.share_dns_and_ssl_session_cache) {
    curl_easy_setopt(curl, CURLOPT_SHARE, shared_curl_cache());
  }
  curl_easy_setopt(curl, CURLOPT_TCP_KEEPALIVE, options.tcp_keepalive ? 1L : 0L);
  curl_easy_setopt(curl, CURLOPT_TCP_NODELAY, options.tcp_nodelay ? 1L : 0L);
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

std::string HttpClient::warm_up() const {
  return get("/v5/market/time", {}, false);
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
  HttpHeaders response_headers;
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
  curl_easy_setopt(curl_, CURLOPT_HEADERFUNCTION, header_callback);
  curl_easy_setopt(curl_, CURLOPT_HEADERDATA, &response_headers);
  if (headers)
    curl_easy_setopt(curl_, CURLOPT_HTTPHEADER, headers);

  auto res = curl_easy_perform(curl_);
  long status = 0;
  curl_easy_getinfo(curl_, CURLINFO_RESPONSE_CODE, &status);

  if (headers)
    curl_slist_free_all(headers);

  if (res != CURLE_OK) {
    throw std::runtime_error(std::string("curl perform failed: ") + curl_easy_strerror(res));
  }
  if (status >= 400) {
    throw HttpError(status, response, std::move(response_headers));
  }
  return response;
}

std::string HttpClient::post(const std::string& path, const std::string& body, bool is_private) const {
  std::string url = join_url(base_url_, path);
  std::string payload = body;
  std::string response;
  HttpHeaders response_headers;
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
  curl_easy_setopt(curl_, CURLOPT_HEADERFUNCTION, header_callback);
  curl_easy_setopt(curl_, CURLOPT_HEADERDATA, &response_headers);
  curl_easy_setopt(curl_, CURLOPT_HTTPHEADER, headers);

  auto res = curl_easy_perform(curl_);
  long status = 0;
  curl_easy_getinfo(curl_, CURLINFO_RESPONSE_CODE, &status);

  curl_slist_free_all(headers);

  if (res != CURLE_OK) {
    throw std::runtime_error(std::string("curl perform failed: ") + curl_easy_strerror(res));
  }
  if (status >= 400) {
    throw HttpError(status, response, std::move(response_headers));
  }
  return response;
}

std::string HttpClient::post_query(const std::string& path,
                                   const std::vector<std::pair<std::string, std::string>>& params,
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

  std::string response;
  HttpHeaders response_headers;
  std::lock_guard<std::mutex> lock(curl_mutex_);

  struct curl_slist* headers = nullptr;
  headers = curl_slist_append(headers, "Content-Type: application/json");
  if (is_private) {
    auto signed_req = Signer::sign(api_key_, api_secret_, query, recv_window_);
    headers = curl_slist_append(headers, ("X-BAPI-API-KEY: " + api_key_).c_str());
    headers = curl_slist_append(headers, ("X-BAPI-TIMESTAMP: " + signed_req.timestamp).c_str());
    headers = curl_slist_append(headers, ("X-BAPI-RECV-WINDOW: " + signed_req.recv_window).c_str());
    headers = curl_slist_append(headers, ("X-BAPI-SIGN: " + signed_req.signature).c_str());
  }

  curl_easy_reset(curl_);
  apply_options(curl_, options_);
  curl_easy_setopt(curl_, CURLOPT_URL, url.c_str());
  curl_easy_setopt(curl_, CURLOPT_POST, 1L);
  curl_easy_setopt(curl_, CURLOPT_POSTFIELDS, "");
  curl_easy_setopt(curl_, CURLOPT_WRITEFUNCTION, write_callback);
  curl_easy_setopt(curl_, CURLOPT_WRITEDATA, &response);
  curl_easy_setopt(curl_, CURLOPT_HEADERFUNCTION, header_callback);
  curl_easy_setopt(curl_, CURLOPT_HEADERDATA, &response_headers);
  curl_easy_setopt(curl_, CURLOPT_HTTPHEADER, headers);

  auto res = curl_easy_perform(curl_);
  long status = 0;
  curl_easy_getinfo(curl_, CURLINFO_RESPONSE_CODE, &status);

  curl_slist_free_all(headers);

  if (res != CURLE_OK) {
    throw std::runtime_error(std::string("curl perform failed: ") + curl_easy_strerror(res));
  }
  if (status >= 400) {
    throw HttpError(status, response, std::move(response_headers));
  }
  return response;
}

}  // namespace bybit
