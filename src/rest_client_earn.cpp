#include "bybit/rest_client.hpp"

namespace bybit {

std::string RestClient::get_earn_product_info(const QueryParams& filters) {
  return public_.get_earn_product_info(filters);
}

std::string RestClient::place_earn_order(const std::string& json_body) {
  return private_.place_earn_order(json_body);
}

std::string RestClient::get_earn_order_history(const QueryParams& filters) {
  return private_.get_earn_order_history(filters);
}

std::string RestClient::get_earn_position(const QueryParams& filters) {
  return private_.get_earn_position(filters);
}

std::string RestClient::get_earn_yield_history(const QueryParams& filters) {
  return private_.get_earn_yield_history(filters);
}

std::string RestClient::get_earn_hourly_yield_history(const QueryParams& filters) {
  return private_.get_earn_hourly_yield_history(filters);
}

}  // namespace bybit
