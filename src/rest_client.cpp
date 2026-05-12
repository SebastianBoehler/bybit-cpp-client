#include "bybit/rest_client.hpp"

#include <optional>
#include <utility>

namespace bybit {

RestClient::RestClient(std::string api_key, std::string api_secret, std::string category, std::string base_url,
                       std::string recv_window)
    : http_(std::move(api_key), std::move(api_secret), std::move(base_url), std::move(recv_window)),
      public_(http_, category),
      private_(http_, category) {}

RestClient::RestClient(std::string api_key, std::string api_secret, std::string category, HttpOptions options)
    : RestClient(std::move(api_key), std::move(api_secret), std::move(category), "https://api.bybit.com", "5000",
                 std::move(options)) {}

RestClient::RestClient(std::string api_key, std::string api_secret, std::string category, std::string base_url,
                       std::string recv_window, HttpOptions options)
    : http_(std::move(api_key), std::move(api_secret), std::move(base_url), std::move(recv_window),
            std::move(options)),
      public_(http_, category),
      private_(http_, category) {}

std::string RestClient::get_query_api_key() {
  return private_.get_query_api_key();
}
std::string RestClient::get_account_info() {
  return private_.get_account_info();
}
std::string RestClient::upgrade_to_unified_account() {
  return private_.upgrade_to_unified_account();
}
std::string RestClient::request_demo_trading_funds(const std::string& json_body) {
  return private_.request_demo_trading_funds(json_body);
}
std::string RestClient::get_account_instruments_info(const QueryParams& filters) {
  return private_.get_account_instruments_info(filters);
}
std::string RestClient::get_transferable_amount(const std::string& coin_name) {
  return private_.get_transferable_amount(coin_name);
}
std::string RestClient::get_collateral_info(const std::optional<std::string>& currency) {
  return private_.get_collateral_info(currency);
}
std::string RestClient::set_collateral_coin(const std::string& coin, const std::string& collateral_switch) {
  return private_.set_collateral_coin(coin, collateral_switch);
}
std::string RestClient::batch_set_collateral_coins(const std::vector<CollateralCoinSwitch>& request) {
  return private_.batch_set_collateral_coins(request);
}
std::string RestClient::get_borrow_history(const QueryParams& filters) {
  return private_.get_borrow_history(filters);
}
std::string RestClient::get_coin_greeks(const std::optional<std::string>& base_coin) {
  return private_.get_coin_greeks(base_coin);
}
std::string RestClient::manual_borrow(const std::string& coin, const std::string& amount) {
  return private_.manual_borrow(coin, amount);
}
std::string RestClient::manual_repay(const QueryParams& params) {
  return private_.manual_repay(params);
}
std::string RestClient::no_convert_repay(const std::string& coin, const std::optional<std::string>& amount,
                                         const std::optional<std::string>& repayment_type) {
  return private_.no_convert_repay(coin, amount, repayment_type);
}
std::string RestClient::quick_repayment(const std::optional<std::string>& coin) {
  return private_.quick_repayment(coin);
}
std::string RestClient::set_spot_hedging(const std::string& set_hedging_mode) {
  return private_.set_spot_hedging(set_hedging_mode);
}
std::string RestClient::get_mmp_state(const std::string& base_coin) {
  return private_.get_mmp_state(base_coin);
}
std::string RestClient::reset_mmp(const std::string& base_coin) {
  return private_.reset_mmp(base_coin);
}
std::string RestClient::set_mmp(const QueryParams& params) {
  return private_.set_mmp(params);
}
std::string RestClient::get_smp_group() {
  return private_.get_smp_group();
}
std::string RestClient::get_trade_behaviour_config() {
  return private_.get_trade_behaviour_config();
}
std::string RestClient::set_delta_neutral_mode(const std::string& delta_enable) {
  return private_.set_delta_neutral_mode(delta_enable);
}
std::string RestClient::set_price_limit_behaviour(const std::string& category, bool modify_enable) {
  return private_.set_price_limit_behaviour(category, modify_enable);
}
std::string RestClient::get_option_asset_info() {
  return private_.get_option_asset_info();
}
std::string RestClient::get_pay_info(const std::optional<std::string>& coin) {
  return private_.get_pay_info(coin);
}
std::string RestClient::get_trade_info_for_analysis(const QueryParams& filters) {
  return private_.get_trade_info_for_analysis(filters);
}
std::string RestClient::get_position_info(const std::optional<std::string>& settle_coin,
                                          const std::optional<std::string>& symbol, int limit) {
  return private_.get_position_info(settle_coin, symbol, limit);
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
std::string RestClient::get_open_interest(const std::string& symbol, const std::string& interval, int limit) {
  return public_.get_open_interest(symbol, interval, limit);
}
std::string RestClient::get_long_short_ratio(const std::string& symbol, const std::string& period, int limit) {
  return public_.get_long_short_ratio(symbol, period, limit);
}
std::string RestClient::get_server_time() {
  return public_.get_server_time();
}
std::string RestClient::get_risk_limit(const std::optional<std::string>& symbol,
                                       const std::optional<std::string>& cursor) {
  return public_.get_risk_limit(symbol, cursor);
}
std::string RestClient::submit_order(const std::string& symbol, const std::string& side, const std::string& order_type,
                                     const std::string& qty, const std::string& order_link_id, int position_idx,
                                     const std::string& price, const std::string& time_in_force,
                                     const std::optional<bool>& reduce_only,
                                     const std::optional<std::string>& bbo_side_type,
                                     const std::optional<std::string>& bbo_level) {
  return private_.submit_order(symbol, side, order_type, qty, order_link_id, position_idx, price, time_in_force,
                               reduce_only, bbo_side_type, bbo_level);
}
std::string RestClient::batch_submit_orders(
    const std::vector<std::vector<std::pair<std::string, std::string>>>& order_requests) {
  return private_.batch_submit_orders(order_requests);
}
std::string RestClient::batch_cancel_orders(
    const std::vector<std::vector<std::pair<std::string, std::string>>>& cancel_requests) {
  return private_.batch_cancel_orders(cancel_requests);
}
std::string RestClient::batch_amend_orders(
    const std::vector<std::vector<std::pair<std::string, std::string>>>& amend_requests) {
  return private_.batch_amend_orders(amend_requests);
}
std::string RestClient::set_leverage(const std::string& symbol, const std::string& buy_leverage,
                                     const std::string& sell_leverage) {
  return private_.set_leverage(symbol, buy_leverage, sell_leverage);
}
std::string RestClient::pre_check_order(const QueryParams& order_params) {
  return private_.pre_check_order(order_params);
}
std::string RestClient::get_order_history(const QueryParams& filters) {
  return private_.get_order_history(filters);
}
std::string RestClient::get_historic_orders(const std::string& order_id) {
  return private_.get_historic_orders(order_id);
}
std::string RestClient::get_trade_history(const QueryParams& filters) {
  return private_.get_trade_history(filters);
}
std::string RestClient::get_closed_pnl(const QueryParams& filters) {
  return private_.get_closed_pnl(filters);
}
std::string RestClient::get_closed_options_positions(const QueryParams& filters) {
  return private_.get_closed_options_positions(filters);
}
std::string RestClient::get_fee_rate() {
  return private_.get_fee_rate();
}
std::string RestClient::get_borrow_quota(const std::string& symbol, const std::string& side) {
  return private_.get_borrow_quota(symbol, side);
}
std::string RestClient::get_wallet_balance(const std::string& category, const std::optional<std::string>& coin) {
  return private_.get_wallet_balance(category, coin);
}
std::string RestClient::get_open_orders(const std::optional<std::string>& symbol, int limit) {
  return private_.get_open_orders(symbol, limit);
}
std::string RestClient::get_realtime_orders(const QueryParams& filters) {
  return private_.get_realtime_orders(filters);
}
std::string RestClient::cancel_order(const std::string& symbol, const std::string& order_id) {
  return private_.cancel_order(symbol, order_id);
}
std::string RestClient::cancel_all_orders(const QueryParams& filters) {
  return private_.cancel_all_orders(filters);
}
std::string RestClient::set_disconnect_cancel_all(int time_window, const std::optional<std::string>& product) {
  return private_.set_disconnect_cancel_all(time_window, product);
}
std::string RestClient::get_dcp_info() {
  return private_.get_dcp_info();
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
std::string RestClient::move_positions(const std::string& from_uid, const std::string& to_uid,
                                       const std::vector<MovePositionLeg>& legs) {
  return private_.move_positions(from_uid, to_uid, legs);
}
std::string RestClient::get_move_position_history(const QueryParams& filters) {
  return private_.get_move_position_history(filters);
}
std::string RestClient::set_auto_add_margin(const std::string& symbol, int auto_add_margin,
                                            const std::optional<int>& position_idx) {
  return private_.set_auto_add_margin(symbol, auto_add_margin, position_idx);
}
std::string RestClient::confirm_pending_mmr(const std::string& symbol) {
  return private_.confirm_pending_mmr(symbol);
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
