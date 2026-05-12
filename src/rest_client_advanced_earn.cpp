#include "bybit/rest_client.hpp"

namespace bybit {

std::string RestClient::get_advanced_earn_product_info(const QueryParams& filters) {
  return public_.get_advanced_earn_product_info(filters);
}

std::string RestClient::get_advanced_earn_product_quote(const QueryParams& filters) {
  return public_.get_advanced_earn_product_quote(filters);
}

std::string RestClient::place_advanced_earn_order(const std::string& json_body) {
  return private_.place_advanced_earn_order(json_body);
}

std::string RestClient::get_advanced_earn_position(const QueryParams& filters) {
  return private_.get_advanced_earn_position(filters);
}

std::string RestClient::get_advanced_earn_order_history(const QueryParams& filters) {
  return private_.get_advanced_earn_order_history(filters);
}

std::string RestClient::get_advanced_earn_liquidity_mining_product_info(const QueryParams& filters) {
  return public_.get_advanced_earn_liquidity_mining_product_info(filters);
}

std::string RestClient::add_advanced_earn_liquidity_mining_liquidity(const std::string& json_body) {
  return private_.add_advanced_earn_liquidity_mining_liquidity(json_body);
}

std::string RestClient::remove_advanced_earn_liquidity_mining_liquidity(const std::string& json_body) {
  return private_.remove_advanced_earn_liquidity_mining_liquidity(json_body);
}

std::string RestClient::reinvest_advanced_earn_liquidity_mining(const std::string& json_body) {
  return private_.reinvest_advanced_earn_liquidity_mining(json_body);
}

std::string RestClient::add_advanced_earn_liquidity_mining_margin(const std::string& json_body) {
  return private_.add_advanced_earn_liquidity_mining_margin(json_body);
}

std::string RestClient::claim_advanced_earn_liquidity_mining_interest(const std::string& json_body) {
  return private_.claim_advanced_earn_liquidity_mining_interest(json_body);
}

std::string RestClient::get_advanced_earn_liquidity_mining_position(const QueryParams& filters) {
  return private_.get_advanced_earn_liquidity_mining_position(filters);
}

std::string RestClient::get_advanced_earn_liquidity_mining_order_history(const QueryParams& filters) {
  return private_.get_advanced_earn_liquidity_mining_order_history(filters);
}

std::string RestClient::get_advanced_earn_liquidity_mining_yield_records(const QueryParams& filters) {
  return private_.get_advanced_earn_liquidity_mining_yield_records(filters);
}

std::string RestClient::get_advanced_earn_liquidity_mining_liquidation_records(const QueryParams& filters) {
  return private_.get_advanced_earn_liquidity_mining_liquidation_records(filters);
}

}  // namespace bybit
