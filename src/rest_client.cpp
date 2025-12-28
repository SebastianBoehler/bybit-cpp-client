#include "bybit/rest_client.hpp"

#include <optional>
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
std::string RestClient::get_kline(const std::string& symbol, const std::string& interval,
                                  const std::optional<std::string>& start, const std::optional<std::string>& end,
                                  int limit) {
  return public_.get_kline(symbol, interval, start, end, limit);
}
std::string RestClient::get_mark_price_kline(const std::string& symbol, const std::string& interval,
                                             const std::optional<std::string>& start,
                                             const std::optional<std::string>& end, int limit) {
  return public_.get_mark_price_kline(symbol, interval, start, end, limit);
}
std::string RestClient::get_index_price_kline(const std::string& symbol, const std::string& interval,
                                              const std::optional<std::string>& start,
                                              const std::optional<std::string>& end, int limit) {
  return public_.get_index_price_kline(symbol, interval, start, end, limit);
}
std::string RestClient::get_premium_index_price_kline(const std::string& symbol, const std::string& interval,
                                                      const std::optional<std::string>& start,
                                                      const std::optional<std::string>& end, int limit) {
  return public_.get_premium_index_price_kline(symbol, interval, start, end, limit);
}
std::string RestClient::get_recent_trades(const std::string& symbol, int limit) {
  return public_.get_recent_trades(symbol, limit);
}
std::string RestClient::get_funding_history(const std::string& symbol, int limit) {
  return public_.get_funding_history(symbol, limit);
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
std::string RestClient::get_transaction_log(int limit, const std::optional<std::string>& cursor) {
  return private_.get_transaction_log(limit, cursor);
}
std::string RestClient::move_position(const std::string& from_uid, const std::string& to_uid, const std::string& symbol,
                                      const std::string& qty, int position_idx) {
  return private_.move_position(from_uid, to_uid, symbol, qty, position_idx);
}
std::string RestClient::set_trading_stop(const std::string& symbol, int position_idx,
                                         const std::optional<std::string>& take_profit,
                                         const std::optional<std::string>& stop_loss,
                                         const std::optional<std::string>& trailing_stop) {
  return private_.set_trading_stop(symbol, position_idx, take_profit, stop_loss, trailing_stop);
}
std::string RestClient::set_risk_limit(const std::string& symbol, const std::string& risk_id, int position_idx) {
  return private_.set_risk_limit(symbol, risk_id, position_idx);
}
std::string RestClient::add_margin(const std::string& symbol, const std::string& margin, int position_idx) {
  return private_.add_margin(symbol, margin, position_idx);
}
std::string RestClient::switch_position_mode(const std::string& mode, int position_idx) {
  return private_.switch_position_mode(mode, position_idx);
}
std::string RestClient::switch_margin_mode(const std::string& symbol, const std::string& mode, int leverage) {
  return private_.switch_margin_mode(symbol, mode, leverage);
}
std::string RestClient::cancel_all(const std::string& symbol) {
  return private_.cancel_all(symbol);
}

}  // namespace bybit
