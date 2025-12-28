#include "bybit/rest_client.hpp"

#include <utility>

namespace bybit {

RestClient::RestClient(std::string api_key, std::string api_secret, std::string category, std::string base_url,
                       std::string recv_window)
    : http_(std::move(api_key), std::move(api_secret), std::move(base_url), std::move(recv_window)),
      public_(http_, category),
      private_(http_, category) {}

std::string RestClient::get_query_api_key() {
  return private_.get_query_api_key();
}
std::string RestClient::get_account_info() {
  return private_.get_account_info();
}
std::string RestClient::get_position_info(const std::optional<std::string>& settle_coin, int limit) {
  return private_.get_position_info(settle_coin, limit);
}
std::string RestClient::get_instruments_info(int limit) {
  return public_.get_instruments_info(limit);
}
std::string RestClient::get_tickers(const std::string& symbol) {
  return public_.get_tickers(symbol);
}
std::string RestClient::get_orderbook(const std::string& symbol, int limit) {
  return public_.get_orderbook(symbol, limit);
}
std::string RestClient::submit_order(const std::string& symbol, const std::string& side, const std::string& order_type,
                                     const std::string& qty, const std::string& order_link_id, int position_idx) {
  return private_.submit_order(symbol, side, order_type, qty, order_link_id, position_idx);
}
std::string RestClient::set_leverage(const std::string& symbol, const std::string& buy_leverage,
                                     const std::string& sell_leverage) {
  return private_.set_leverage(symbol, buy_leverage, sell_leverage);
}
std::string RestClient::get_historic_orders(const std::string& order_id) {
  return private_.get_historic_orders(order_id);
}
std::string RestClient::get_fee_rate() {
  return private_.get_fee_rate();
}
std::string RestClient::get_wallet_balance(const std::optional<std::string>& coin) {
  return private_.get_wallet_balance(coin);
}
std::string RestClient::get_open_orders(const std::optional<std::string>& symbol, int limit) {
  return private_.get_open_orders(symbol, limit);
}
std::string RestClient::cancel_order(const std::string& symbol, const std::string& order_id) {
  return private_.cancel_order(symbol, order_id);
}
std::string RestClient::amend_order(const std::string& symbol, const std::string& order_id,
                                    const std::optional<std::string>& qty, const std::optional<std::string>& price) {
  return private_.amend_order(symbol, order_id, qty, price);
}

}  // namespace bybit
