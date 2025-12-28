#include "bybit/http_client.hpp"

#include <curl/curl.h>

#include <algorithm>
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
  if (!initialized) {
    if (curl_global_init(CURL_GLOBAL_DEFAULT) != 0) {
      throw std::runtime_error("Failed to init curl");
    }
    initialized = true;
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

HttpClient::HttpClient(std::string api_key, std::string api_secret, std::string base_url, std::string recv_window)
    : api_key_(std::move(api_key)),
      api_secret_(std::move(api_secret)),
      base_url_(std::move(base_url)),
      recv_window_(std::move(recv_window)) {
  ensure_curl_global();
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
  struct curl_slist* headers = nullptr;
  if (is_private) {
    auto signed_req = Signer::sign(api_key_, api_secret_, payload, recv_window_);
    headers = curl_slist_append(headers, ("X-BAPI-API-KEY: " + api_key_).c_str());
    headers = curl_slist_append(headers, ("X-BAPI-TIMESTAMP: " + signed_req.timestamp).c_str());
    headers = curl_slist_append(headers, ("X-BAPI-RECV-WINDOW: " + signed_req.recv_window).c_str());
    headers = curl_slist_append(headers, ("X-BAPI-SIGN: " + signed_req.signature).c_str());
  }

  CURL* curl = curl_easy_init();
  if (!curl) {
    if (headers) curl_slist_free_all(headers);
    throw std::runtime_error("Failed to init curl easy handle");
  }

  std::string response;
  curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
  curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
  if (headers) curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

  auto res = curl_easy_perform(curl);
  long status = 0;
  curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &status);

  curl_easy_cleanup(curl);
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

  struct curl_slist* headers = nullptr;
  headers = curl_slist_append(headers, "Content-Type: application/json");
  if (is_private) {
    auto signed_req = Signer::sign(api_key_, api_secret_, payload, recv_window_);
    headers = curl_slist_append(headers, ("X-BAPI-API-KEY: " + api_key_).c_str());
    headers = curl_slist_append(headers, ("X-BAPI-TIMESTAMP: " + signed_req.timestamp).c_str());
    headers = curl_slist_append(headers, ("X-BAPI-RECV-WINDOW: " + signed_req.recv_window).c_str());
    headers = curl_slist_append(headers, ("X-BAPI-SIGN: " + signed_req.signature).c_str());
  }

  CURL* curl = curl_easy_init();
  if (!curl) {
    curl_slist_free_all(headers);
    throw std::runtime_error("Failed to init curl easy handle");
  }

  std::string response;
  curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
  curl_easy_setopt(curl, CURLOPT_POST, 1L);
  curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body.c_str());
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
  curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

  auto res = curl_easy_perform(curl);
  long status = 0;
  curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &status);

  curl_easy_cleanup(curl);
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
