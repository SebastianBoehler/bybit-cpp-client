#include <algorithm>
#include <cctype>
#include <iomanip>
#include <sstream>
#include <type_traits>

#include "bybit/http_client.hpp"

namespace bybit {
namespace {

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
      case '\b':
        oss << "\\b";
        break;
      case '\f':
        oss << "\\f";
        break;
      case '\n':
        oss << "\\n";
        break;
      case '\r':
        oss << "\\r";
        break;
      case '\t':
        oss << "\\t";
        break;
      default:
        if (static_cast<unsigned char>(c) < 0x20) {
          oss << "\\u00" << std::hex << std::setw(2) << std::setfill('0')
              << static_cast<int>(static_cast<unsigned char>(c)) << std::dec;
        } else {
          oss << c;
        }
        break;
    }
  }
  return oss.str();
}

bool is_unreserved(unsigned char c) {
  return std::isalnum(c) || c == '-' || c == '.' || c == '_' || c == '~';
}

std::string percent_encode(const std::string& input) {
  std::ostringstream oss;
  oss << std::uppercase << std::hex;
  for (unsigned char c : input) {
    if (is_unreserved(c)) {
      oss << c;
    } else {
      oss << '%' << std::setw(2) << std::setfill('0') << static_cast<int>(c);
    }
  }
  return oss.str();
}

}  // namespace

std::string to_json_object(const std::vector<std::pair<std::string, std::string>>& kvs) {
  std::ostringstream oss;
  oss << "{";
  for (size_t i = 0; i < kvs.size(); ++i) {
    oss << serialize_json_string(kvs[i].first) << ":" << serialize_json_string(kvs[i].second);
    if (i + 1 < kvs.size())
      oss << ",";
  }
  oss << "}";
  return oss.str();
}

std::string serialize_json_string(const std::string& value) {
  return "\"" + json_escape(value) + "\"";
}

std::string serialize_json_string_array(const std::vector<std::string>& values) {
  std::ostringstream oss;
  oss << "[";
  for (std::size_t i = 0; i < values.size(); ++i) {
    oss << serialize_json_string(values[i]);
    if (i + 1 < values.size())
      oss << ",";
  }
  oss << "]";
  return oss.str();
}

std::string serialize_json_object(const JsonObject& values) {
  std::ostringstream oss;
  oss << "{";
  for (size_t i = 0; i < values.size(); ++i) {
    oss << serialize_json_string(values[i].first) << ":";
    std::visit(
        [&oss](const auto& value) {
          using T = std::decay_t<decltype(value)>;
          if constexpr (std::is_same_v<T, std::string>) {
            oss << serialize_json_string(value);
          } else if constexpr (std::is_same_v<T, bool>) {
            oss << (value ? "true" : "false");
          } else {
            oss << value;
          }
        },
        values[i].second.storage());
    if (i + 1 < values.size())
      oss << ",";
  }
  oss << "}";
  return oss.str();
}

std::string canonical_query(const std::vector<std::pair<std::string, std::string>>& params) {
  std::vector<std::pair<std::string, std::string>> sorted = params;
  std::sort(sorted.begin(), sorted.end(), [](const auto& a, const auto& b) { return a.first < b.first; });
  std::ostringstream oss;
  for (size_t i = 0; i < sorted.size(); ++i) {
    oss << percent_encode(sorted[i].first) << "=" << percent_encode(sorted[i].second);
    if (i + 1 < sorted.size())
      oss << "&";
  }
  return oss.str();
}

}  // namespace bybit
