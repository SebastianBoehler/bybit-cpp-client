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
    if (i + 1 < legs.size())
      oss << ",";
  }
  oss << "]}";
  return oss.str();
}

}  // namespace

std::string PrivateRestClient::move_positions(const std::string& from_uid, const std::string& to_uid,
                                              const std::vector<MovePositionLeg>& legs) {
  return http_.post("/v5/position/move-positions", build_move_positions_body(from_uid, to_uid, legs), true);
}

std::string PrivateRestClient::set_auto_add_margin(const std::string& symbol, int auto_add_margin,
                                                   const std::optional<int>& position_idx) {
  if (auto_add_margin != 0 && auto_add_margin != 1) {
    throw std::invalid_argument("set_auto_add_margin auto_add_margin must be 0 or 1");
  }

  QueryParams body_kv{{"category", category_}, {"symbol", symbol}, {"autoAddMargin", std::to_string(auto_add_margin)}};
  if (position_idx) {
    body_kv.emplace_back("positionIdx", std::to_string(*position_idx));
  }
  return http_.post("/v5/position/set-auto-add-margin", to_json_object(body_kv), true);
}

std::string PrivateRestClient::confirm_pending_mmr(const std::string& symbol) {
  return http_.post("/v5/position/confirm-pending-mmr", to_json_object({{"category", category_}, {"symbol", symbol}}),
                    true);
}

}  // namespace bybit
