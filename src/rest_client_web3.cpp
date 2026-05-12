#include "bybit/rest_client.hpp"

namespace bybit {

std::string RestClient::get_web3_trade_quote(const std::string& json_body) {
  return private_.get_web3_trade_quote(json_body);
}

std::string RestClient::execute_web3_purchase(const std::string& json_body) {
  return private_.execute_web3_purchase(json_body);
}

std::string RestClient::execute_web3_redeem(const std::string& json_body) {
  return private_.execute_web3_redeem(json_body);
}

std::string RestClient::get_web3_payment_token_list(const std::string& json_body) {
  return private_.get_web3_payment_token_list(json_body);
}

std::string RestClient::get_web3_order_list(const std::string& json_body) {
  return private_.get_web3_order_list(json_body);
}

std::string RestClient::get_web3_business_token_list(const std::string& json_body) {
  return private_.get_web3_business_token_list(json_body);
}

std::string RestClient::get_web3_token_price_list(const std::string& json_body) {
  return private_.get_web3_token_price_list(json_body);
}

std::string RestClient::get_web3_token_details(const std::string& json_body) {
  return private_.get_web3_token_details(json_body);
}

std::string RestClient::get_web3_asset_list(const std::string& json_body) {
  return private_.get_web3_asset_list(json_body);
}

std::string RestClient::get_web3_asset_detail(const std::string& json_body) {
  return private_.get_web3_asset_detail(json_body);
}

}  // namespace bybit
