#include "bybit/rest_client.hpp"

namespace bybit {

std::string RestClient::create_strategy_order(const std::string& json_body) {
  return private_.create_strategy_order(json_body);
}

std::string RestClient::get_strategy_list(const QueryParams& filters) {
  return private_.get_strategy_list(filters);
}

std::string RestClient::get_strategy_order_list(const QueryParams& filters) {
  return private_.get_strategy_order_list(filters);
}

std::string RestClient::stop_strategy(const std::string& strategy_id) {
  return private_.stop_strategy(strategy_id);
}

}  // namespace bybit
