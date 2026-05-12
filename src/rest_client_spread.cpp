#include "bybit/rest_client.hpp"

namespace bybit {

std::string RestClient::get_spread_instruments(const QueryParams& filters) {
  return public_.get_spread_instruments(filters);
}

std::string RestClient::get_spread_orderbook(const std::string& symbol, int limit) {
  return public_.get_spread_orderbook(symbol, limit);
}

std::string RestClient::get_spread_tickers(const std::string& symbol) {
  return public_.get_spread_tickers(symbol);
}

std::string RestClient::get_spread_recent_trades(const std::string& symbol, int limit) {
  return public_.get_spread_recent_trades(symbol, limit);
}

std::string RestClient::create_spread_order(const std::string& json_body) {
  return private_.create_spread_order(json_body);
}

std::string RestClient::amend_spread_order(const std::string& json_body) {
  return private_.amend_spread_order(json_body);
}

std::string RestClient::cancel_spread_order(const std::string& json_body) {
  return private_.cancel_spread_order(json_body);
}

std::string RestClient::cancel_all_spread_orders(const std::string& json_body) {
  return private_.cancel_all_spread_orders(json_body);
}

std::string RestClient::get_spread_open_orders(const QueryParams& filters) {
  return private_.get_spread_open_orders(filters);
}

std::string RestClient::get_spread_order_history(const QueryParams& filters) {
  return private_.get_spread_order_history(filters);
}

std::string RestClient::get_spread_trade_history(const QueryParams& filters) {
  return private_.get_spread_trade_history(filters);
}

std::string RestClient::get_spread_max_order_qty(const QueryParams& filters) {
  return private_.get_spread_max_order_qty(filters);
}

}  // namespace bybit
