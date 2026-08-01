#include "bybit/detail/websocket_messages.hpp"

#include <sstream>

#include "bybit/json.hpp"

namespace bybit::detail {

std::string make_subscription_message(const std::string& operation, const std::vector<std::string>& topics,
                                      const std::string& request_id) {
  std::ostringstream oss;
  oss << "{\"op\":" << serialize_json_string(operation) << ",\"args\":" << serialize_json_string_array(topics);
  if (!request_id.empty())
    oss << ",\"req_id\":" << serialize_json_string(request_id);
  oss << "}";
  return oss.str();
}

std::string make_ping_message(const std::string& request_id) {
  std::ostringstream oss;
  oss << "{\"op\":\"ping\"";
  if (!request_id.empty())
    oss << ",\"req_id\":" << serialize_json_string(request_id);
  oss << "}";
  return oss.str();
}

std::string make_auth_message(const std::string& api_key, const std::string& expires_ms, const std::string& signature) {
  return "{\"op\":\"auth\",\"args\":[" + serialize_json_string(api_key) + "," + expires_ms + "," +
         serialize_json_string(signature) + "]}";
}

}  // namespace bybit::detail
