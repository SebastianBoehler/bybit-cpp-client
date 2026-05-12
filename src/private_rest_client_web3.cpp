#include "bybit/http_client.hpp"
#include "bybit/rest_client.hpp"

namespace bybit {

std::string PrivateRestClient::get_web3_trade_quote(const std::string& json_body) {
  return http_.post("/v5/alpha/trade/quote", json_body, true);
}

std::string PrivateRestClient::execute_web3_purchase(const std::string& json_body) {
  return http_.post("/v5/alpha/trade/purchase", json_body, true);
}

std::string PrivateRestClient::execute_web3_redeem(const std::string& json_body) {
  return http_.post("/v5/alpha/trade/redeem", json_body, true);
}

std::string PrivateRestClient::get_web3_payment_token_list(const std::string& json_body) {
  return http_.post("/v5/alpha/trade/pay-token-list", json_body, true);
}

std::string PrivateRestClient::get_web3_order_list(const std::string& json_body) {
  return http_.post("/v5/alpha/trade/order-list", json_body, true);
}

std::string PrivateRestClient::get_web3_business_token_list(const std::string& json_body) {
  return http_.post("/v5/alpha/trade/biz-token-list", json_body, true);
}

std::string PrivateRestClient::get_web3_token_price_list(const std::string& json_body) {
  return http_.post("/v5/alpha/trade/biz-token-price-list", json_body, true);
}

std::string PrivateRestClient::get_web3_token_details(const std::string& json_body) {
  return http_.post("/v5/alpha/trade/biz-token-details", json_body, true);
}

std::string PrivateRestClient::get_web3_asset_list(const std::string& json_body) {
  return http_.post("/v5/alpha/trade/asset-list", json_body, true);
}

std::string PrivateRestClient::get_web3_asset_detail(const std::string& json_body) {
  return http_.post("/v5/alpha/trade/asset-detail", json_body, true);
}

}  // namespace bybit
