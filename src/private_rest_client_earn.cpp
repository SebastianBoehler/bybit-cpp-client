#include "bybit/http_client.hpp"
#include "bybit/rest_client.hpp"

namespace bybit {

std::string PrivateRestClient::place_earn_order(const std::string& json_body) {
  return http_.post("/v5/earn/place-order", json_body, true);
}

std::string PrivateRestClient::get_earn_order_history(const QueryParams& filters) {
  return http_.get("/v5/earn/order", filters, true);
}

std::string PrivateRestClient::get_earn_position(const QueryParams& filters) {
  return http_.get("/v5/earn/position", filters, true);
}

std::string PrivateRestClient::get_earn_yield_history(const QueryParams& filters) {
  return http_.get("/v5/earn/yield", filters, true);
}

std::string PrivateRestClient::get_earn_hourly_yield_history(const QueryParams& filters) {
  return http_.get("/v5/earn/hourly-yield", filters, true);
}

}  // namespace bybit
