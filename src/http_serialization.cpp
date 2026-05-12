#include "bybit/http_client.hpp"

#include <algorithm>
#include <sstream>

namespace bybit {
namespace {

std::string json_escape(const std::string& input) {
  std::ostringstream oss;
  for (char c : input) {
    if (c == '"' || c == '\\') oss << '\\';
    oss << c;
  }
  return oss.str();
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

}  // namespace bybit
