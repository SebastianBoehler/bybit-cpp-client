#include "bybit/rest_client.hpp"

namespace bybit {

std::string RestClient::get_rpi_orderbook(const std::string& symbol, int limit) {
  return public_.get_rpi_orderbook(symbol, limit);
}

std::string RestClient::get_historical_volatility(const QueryParams& filters) {
  return public_.get_historical_volatility(filters);
}

std::string RestClient::get_insurance(const QueryParams& filters) {
  return public_.get_insurance(filters);
}

std::string RestClient::get_delivery_price(const QueryParams& filters) {
  return public_.get_delivery_price(filters);
}

std::string RestClient::get_new_delivery_price(const QueryParams& filters) {
  return public_.get_new_delivery_price(filters);
}

std::string RestClient::get_index_price_components(const std::string& index_name) {
  return public_.get_index_price_components(index_name);
}

std::string RestClient::get_order_price_limit(const std::string& symbol,
                                              const std::optional<std::string>& category) {
  return public_.get_order_price_limit(symbol, category);
}

std::string RestClient::get_adl_alert(const std::optional<std::string>& symbol) {
  return public_.get_adl_alert(symbol);
}

std::string RestClient::get_fee_group_info(const QueryParams& filters) {
  return public_.get_fee_group_info(filters);
}

}  // namespace bybit
