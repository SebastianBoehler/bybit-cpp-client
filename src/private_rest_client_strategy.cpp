#include "bybit/http_client.hpp"
#include "bybit/rest_client.hpp"

namespace bybit {

std::string PrivateRestClient::create_strategy_order(const std::string& json_body) {
  return http_.post("/v5/strategy/create", json_body, true);
}

std::string PrivateRestClient::get_strategy_list(const QueryParams& filters) {
  return http_.get("/v5/strategy/list", filters, true);
}

std::string PrivateRestClient::get_strategy_order_list(const QueryParams& filters) {
  return http_.get("/v5/strategy/order-list", filters, true);
}

std::string PrivateRestClient::stop_strategy(const std::string& strategy_id) {
  return http_.post("/v5/strategy/stop", to_json_object({{"strategyId", strategy_id}}), true);
}

}  // namespace bybit
