#include "bybit/http_client.hpp"
#include "bybit/rest_client.hpp"

namespace bybit {

std::string PrivateRestClient::place_advanced_earn_order(const std::string& json_body) {
  return http_.post("/v5/earn/advance/place-order", json_body, true);
}

std::string PrivateRestClient::get_advanced_earn_position(const QueryParams& filters) {
  return http_.get("/v5/earn/advance/position", filters, true);
}

std::string PrivateRestClient::get_advanced_earn_order_history(const QueryParams& filters) {
  return http_.get("/v5/earn/advance/order", filters, true);
}

std::string PrivateRestClient::add_advanced_earn_liquidity_mining_liquidity(const std::string& json_body) {
  return http_.post("/v5/earn/liquidity-mining/add-liquidity", json_body, true);
}

std::string PrivateRestClient::remove_advanced_earn_liquidity_mining_liquidity(const std::string& json_body) {
  return http_.post("/v5/earn/liquidity-mining/remove-liquidity", json_body, true);
}

std::string PrivateRestClient::reinvest_advanced_earn_liquidity_mining(const std::string& json_body) {
  return http_.post("/v5/earn/liquidity-mining/reinvest", json_body, true);
}

std::string PrivateRestClient::add_advanced_earn_liquidity_mining_margin(const std::string& json_body) {
  return http_.post("/v5/earn/liquidity-mining/add-margin", json_body, true);
}

std::string PrivateRestClient::claim_advanced_earn_liquidity_mining_interest(const std::string& json_body) {
  return http_.post("/v5/earn/liquidity-mining/claim-interest", json_body, true);
}

std::string PrivateRestClient::get_advanced_earn_liquidity_mining_position(const QueryParams& filters) {
  return http_.get("/v5/earn/liquidity-mining/position", filters, true);
}

std::string PrivateRestClient::get_advanced_earn_liquidity_mining_order_history(const QueryParams& filters) {
  return http_.get("/v5/earn/liquidity-mining/order", filters, true);
}

std::string PrivateRestClient::get_advanced_earn_liquidity_mining_yield_records(const QueryParams& filters) {
  return http_.get("/v5/earn/liquidity-mining/yield-records", filters, true);
}

std::string PrivateRestClient::get_advanced_earn_liquidity_mining_liquidation_records(const QueryParams& filters) {
  return http_.get("/v5/earn/liquidity-mining/liquidation-records", filters, true);
}

}  // namespace bybit
