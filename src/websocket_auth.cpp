#include "bybit/detail/websocket_auth.hpp"

#include <cctype>

namespace bybit::detail {
namespace {

std::size_t skip_space(std::string_view text, std::size_t pos) {
  while (pos < text.size() && std::isspace(static_cast<unsigned char>(text[pos])))
    ++pos;
  return pos;
}

bool token_ended(std::string_view text, std::size_t pos) {
  pos = skip_space(text, pos);
  return pos == text.size() || text[pos] == ',' || text[pos] == '}' || text[pos] == ']';
}

std::optional<std::size_t> top_level_value(std::string_view json, std::string_view key) {
  int depth = 0;
  for (std::size_t i = 0; i < json.size(); ++i) {
    if (json[i] == '{' || json[i] == '[') {
      ++depth;
      continue;
    }
    if (json[i] == '}' || json[i] == ']') {
      --depth;
      continue;
    }
    if (json[i] != '"')
      continue;

    const auto start = i + 1;
    bool escaped = false;
    for (++i; i < json.size(); ++i) {
      if (escaped) {
        escaped = false;
      } else if (json[i] == '\\') {
        escaped = true;
      } else if (json[i] == '"') {
        break;
      }
    }
    if (i >= json.size() || depth != 1 || escaped)
      continue;

    const auto colon = skip_space(json, i + 1);
    if (colon < json.size() && json[colon] == ':' && json.substr(start, i - start) == key)
      return skip_space(json, colon + 1);
  }
  return std::nullopt;
}

bool string_field_is(std::string_view json, std::string_view key, std::string_view expected) {
  const auto value = top_level_value(json, key);
  if (!value || *value >= json.size() || json[*value] != '"')
    return false;
  const auto end = json.find('"', *value + 1);
  return end != std::string_view::npos && json.substr(*value + 1, end - *value - 1) == expected &&
         token_ended(json, end + 1);
}

std::optional<bool> boolean_field(std::string_view json, std::string_view key) {
  const auto value = top_level_value(json, key);
  if (!value)
    return std::nullopt;
  if (json.substr(*value, 4) == "true" && token_ended(json, *value + 4))
    return true;
  if (json.substr(*value, 5) == "false" && token_ended(json, *value + 5))
    return false;
  return std::nullopt;
}

std::optional<bool> zero_integer_field(std::string_view json, std::string_view key) {
  const auto value = top_level_value(json, key);
  if (!value)
    return std::nullopt;
  std::size_t end = *value;
  if (end < json.size() && (json[end] == '-' || json[end] == '+'))
    ++end;
  const auto digits = end;
  bool is_zero = true;
  while (end < json.size() && std::isdigit(static_cast<unsigned char>(json[end]))) {
    is_zero = is_zero && json[end] == '0';
    ++end;
  }
  if (end == digits || !token_ended(json, end))
    return std::nullopt;
  return is_zero;
}

}  // namespace

std::optional<bool> parse_websocket_auth_result(std::string_view message) {
  if (!string_field_is(message, "op", "auth"))
    return std::nullopt;
  if (const auto success = boolean_field(message, "success"))
    return success;
  return zero_integer_field(message, "retCode");
}

}  // namespace bybit::detail
