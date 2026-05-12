#include "bybit/http_client.hpp"
#include "bybit/rest_client.hpp"

namespace bybit {

std::string PrivateRestClient::create_spread_order(const std::string& json_body) {
  return http_.post("/v5/spread/order/create", json_body, true);
}

std::string PrivateRestClient::amend_spread_order(const std::string& json_body) {
  return http_.post("/v5/spread/order/amend", json_body, true);
}

std::string PrivateRestClient::cancel_spread_order(const std::string& json_body) {
  return http_.post("/v5/spread/order/cancel", json_body, true);
}

std::string PrivateRestClient::cancel_all_spread_orders(const std::string& json_body) {
  return http_.post("/v5/spread/order/cancel-all", json_body, true);
}

std::string PrivateRestClient::get_spread_open_orders(const QueryParams& filters) {
  return http_.get("/v5/spread/order/realtime", filters, true);
}

std::string PrivateRestClient::get_spread_order_history(const QueryParams& filters) {
  return http_.get("/v5/spread/order/history", filters, true);
}

std::string PrivateRestClient::get_spread_trade_history(const QueryParams& filters) {
  return http_.get("/v5/spread/execution/list", filters, true);
}

std::string PrivateRestClient::get_spread_max_order_qty(const QueryParams& filters) {
  return http_.get("/v5/spread/max-qty", filters, true);
}

}  // namespace bybit
