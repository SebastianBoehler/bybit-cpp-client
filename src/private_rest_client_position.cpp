#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "bybit/http_client.hpp"
#include "bybit/rest_client.hpp"

namespace bybit {
namespace {

std::string json_string(const std::string& value) {
  std::ostringstream oss;
  oss << "\"";
  for (char c : value) {
    if (c == '"' || c == '\\') {
      oss << "\\";
    }
    oss << c;
  }
  oss << "\"";
  return oss.str();
}

std::string build_move_positions_body(const std::string& from_uid, const std::string& to_uid,
                                      const std::vector<MovePositionLeg>& legs) {
  if (legs.empty()) {
    throw std::invalid_argument("move_positions requires at least one leg");
  }
  if (legs.size() > 25) {
    throw std::invalid_argument("move_positions supports at most 25 legs per request");
  }

  std::ostringstream oss;
  oss << "{\"fromUid\":" << json_string(from_uid) << ",\"toUid\":" << json_string(to_uid) << ",\"list\":[";
  for (size_t i = 0; i < legs.size(); ++i) {
    const auto& leg = legs[i];
    oss << to_json_object({{"category", leg.category},
                           {"symbol", leg.symbol},
                           {"price", leg.price},
                           {"side", leg.side},
                           {"qty", leg.qty}});
    if (i + 1 < legs.size()) oss << ",";
  }
  oss << "]}";
  return oss.str();
}

}  // namespace

std::string PrivateRestClient::move_positions(const std::string& from_uid, const std::string& to_uid,
                                              const std::vector<MovePositionLeg>& legs) {
  return http_.post("/v5/position/move-positions", build_move_positions_body(from_uid, to_uid, legs), true);
}

}  // namespace bybit
