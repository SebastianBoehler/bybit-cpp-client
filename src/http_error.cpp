#include "bybit/http_client.hpp"

#include <cctype>
#include <optional>
#include <sstream>
#include <string>
#include <utility>

namespace bybit {
namespace {

std::string http_error_message(long status, const std::string& body) {
  std::ostringstream oss;
  oss << "HTTP status " << status << " body: " << body;
  return oss.str();
}

std::optional<size_t> value_start(const std::string& body, const std::string& key) {
  const std::string quoted_key = "\"" + key + "\"";
  size_t pos = body.find(quoted_key);
  if (pos == std::string::npos) return std::nullopt;
  pos = body.find(':', pos + quoted_key.size());
  if (pos == std::string::npos) return std::nullopt;
  ++pos;
  while (pos < body.size() && std::isspace(static_cast<unsigned char>(body[pos]))) ++pos;
  return pos;
}

std::optional<long> json_long(const std::string& body, const std::string& key) {
  auto start = value_start(body, key);
  if (!start || *start >= body.size()) return std::nullopt;
  size_t end = *start;
  if (body[end] == '-') ++end;
  while (end < body.size() && std::isdigit(static_cast<unsigned char>(body[end]))) ++end;
  if (end == *start || (body[*start] == '-' && end == *start + 1)) return std::nullopt;
  try {
    return std::stol(body.substr(*start, end - *start));
  } catch (...) {
    return std::nullopt;
  }
}

std::optional<std::string> json_string(const std::string& body, const std::string& key) {
  auto start = value_start(body, key);
  if (!start || *start >= body.size() || body[*start] != '"') return std::nullopt;
  std::string value;
  for (size_t i = *start + 1; i < body.size(); ++i) {
    if (body[i] == '"') return value;
    if (body[i] == '\\' && i + 1 < body.size()) {
      ++i;
      if (body[i] == 'n') value.push_back('\n');
      else value.push_back(body[i]);
    } else {
      value.push_back(body[i]);
    }
  }
  return std::nullopt;
}

}  // namespace

HttpError::HttpError(long status_code, std::string body)
    : std::runtime_error(http_error_message(status_code, body)),
      status_code_(status_code),
      body_(std::move(body)),
      ret_code_(json_long(body_, "retCode")),
      ret_msg_(json_string(body_, "retMsg")) {}

}  // namespace bybit
