#pragma once

#include <optional>
#include <string>
#include <vector>

#include "bybit/http_client.hpp"

namespace bybit {

using QueryParams = std::vector<std::pair<std::string, std::string>>;

struct MovePositionLeg {
  std::string category, symbol, price, side, qty;
};

struct CollateralCoinSwitch {
  std::string coin, collateral_switch;
};

class PublicRestClient {
 public:
  PublicRestClient(HttpClient& http, std::string category);
  std::string get_server_time();
  std::string get_instruments_info(int limit = 1000);
  std::string get_instruments_info(const std::optional<std::string>& symbol, int limit = 1000,
                                   const std::optional<std::string>& cursor = std::nullopt);
  std::string get_tickers(const std::string& symbol = "");
  std::string get_orderbook(const std::string& symbol, int limit = 50);
  std::string get_kline(const std::string& symbol, const std::string& interval,
                        const std::optional<std::string>& start = std::nullopt, const std::optional<std::string>& end = std::nullopt, int limit = 200);
  std::string get_mark_price_kline(const std::string& symbol, const std::string& interval,
                                   const std::optional<std::string>& start = std::nullopt, const std::optional<std::string>& end = std::nullopt, int limit = 200);
  std::string get_index_price_kline(const std::string& symbol, const std::string& interval,
                                    const std::optional<std::string>& start = std::nullopt, const std::optional<std::string>& end = std::nullopt, int limit = 200);
  std::string get_premium_index_price_kline(const std::string& symbol, const std::string& interval,
                                            const std::optional<std::string>& start = std::nullopt, const std::optional<std::string>& end = std::nullopt, int limit = 200);
  std::string get_recent_trades(const std::string& symbol, int limit = 50);
  std::string get_funding_history(const std::string& symbol, int limit = 50);
  std::string get_open_interest(const std::string& symbol, const std::string& interval, int limit = 50);
  std::string get_long_short_ratio(const std::string& symbol, const std::string& period, int limit = 50);
  std::string get_risk_limit(const std::optional<std::string>& symbol = std::nullopt,
                             const std::optional<std::string>& cursor = std::nullopt);
  std::string get_spread_instruments(const QueryParams& filters = {});
  std::string get_spread_orderbook(const std::string& symbol, int limit = 1);
  std::string get_spread_tickers(const std::string& symbol);
  std::string get_spread_recent_trades(const std::string& symbol, int limit = 500);

 private:
  HttpClient& http_;
  std::string category_;
};

class PrivateRestClient {
 public:
  PrivateRestClient(HttpClient& http, std::string category);

  std::string get_query_api_key();
  std::string get_account_info();
  std::string upgrade_to_unified_account();
  std::string request_demo_trading_funds(const std::string& json_body);
  std::string get_account_instruments_info(const QueryParams& filters = {});
  std::string get_transferable_amount(const std::string& coin_name);
  std::string get_collateral_info(const std::optional<std::string>& currency = std::nullopt);
  std::string set_collateral_coin(const std::string& coin, const std::string& collateral_switch);
  std::string batch_set_collateral_coins(const std::vector<CollateralCoinSwitch>& request);
  std::string get_borrow_history(const QueryParams& filters = {});
  std::string get_coin_greeks(const std::optional<std::string>& base_coin = std::nullopt);
  std::string manual_borrow(const std::string& coin, const std::string& amount);
  std::string manual_repay(const QueryParams& params = {});
  std::string no_convert_repay(const std::string& coin, const std::optional<std::string>& amount = std::nullopt,
                               const std::optional<std::string>& repayment_type = std::nullopt);
  std::string quick_repayment(const std::optional<std::string>& coin = std::nullopt);
  std::string set_spot_hedging(const std::string& set_hedging_mode);
  std::string get_mmp_state(const std::string& base_coin);
  std::string reset_mmp(const std::string& base_coin);
  std::string set_mmp(const QueryParams& params);
  std::string get_smp_group();
  std::string get_trade_behaviour_config();
  std::string set_delta_neutral_mode(const std::string& delta_enable);
  std::string set_price_limit_behaviour(const std::string& category, bool modify_enable);
  std::string get_option_asset_info();
  std::string get_pay_info(const std::optional<std::string>& coin = std::nullopt);
  std::string get_trade_info_for_analysis(const QueryParams& filters);
  std::string get_sub_uid_list();
  std::string create_demo_account();
  std::string create_sub_uid(const std::string& json_body);
  std::string delete_sub_uid(const std::string& sub_member_id);
  std::string freeze_sub_uid(int subuid, int frozen);
  std::string create_sub_api_key(const std::string& json_body);
  std::string get_sub_api_keys(const QueryParams& filters);
  std::string update_master_api_key(const std::string& json_body);
  std::string delete_master_api_key();
  std::string update_sub_api_key(const std::string& json_body);
  std::string delete_sub_api_key(const std::optional<std::string>& api_key = std::nullopt);
  std::string get_uid_wallet_type(const std::optional<std::string>& member_ids = std::nullopt);
  std::string get_affiliate_user_list(const QueryParams& filters = {});
  std::string get_affiliate_user_info(const QueryParams& filters = {});
  std::string get_spot_margin_vip_data(const QueryParams& filters = {});
  std::string get_spot_margin_currency_data(const std::optional<std::string>& currency = std::nullopt);
  std::string get_spot_margin_state();
  std::string switch_spot_margin_mode(const std::string& spot_margin_mode);
  std::string set_spot_margin_leverage(const std::string& leverage, const std::optional<std::string>& currency = std::nullopt);
  std::string get_spot_margin_max_borrowable(const std::string& currency);
  std::string get_spot_margin_position_tiers(const std::optional<std::string>& currency = std::nullopt);
  std::string get_spot_margin_coin_state(const std::optional<std::string>& currency = std::nullopt);
  std::string get_spot_margin_repayment_available_amount(const std::string& currency);
  std::string set_spot_margin_auto_repay_mode(const std::string& auto_repay_mode,
                                              const std::optional<std::string>& currency = std::nullopt);
  std::string get_spot_margin_auto_repay_mode(const std::optional<std::string>& currency = std::nullopt);
  std::string get_spot_margin_interest_rate_history(const QueryParams& filters);
  std::string get_spot_margin_tiered_collateral_ratio(const QueryParams& filters = {});
  std::string get_spot_margin_fixed_borrow_order_quote(const QueryParams& filters);
  std::string create_spot_margin_fixed_borrow(const QueryParams& params);
  std::string renew_spot_margin_fixed_borrow(const std::string& loan_id, const std::optional<std::string>& qty = std::nullopt);
  std::string get_spot_margin_fixed_borrow_order_info(const QueryParams& filters = {});
  std::string get_spot_margin_fixed_borrow_contract_info(const QueryParams& filters = {});
  std::string get_spot_margin_liability(const std::string& currency);
  std::string get_asset_info(const QueryParams& filters);
  std::string get_asset_overview(const QueryParams& filters = {});
  std::string get_portfolio_margin_info(const QueryParams& filters = {});
  std::string get_total_members_assets(const QueryParams& filters = {});
  std::string get_funding_account_history(const QueryParams& filters = {});
  std::string get_delivery_records(const QueryParams& filters = {});
  std::string get_settlement_records(const QueryParams& filters = {});
  std::string get_coin_exchange_records(const QueryParams& filters = {});
  std::string get_convert_coin_list(const QueryParams& filters);
  std::string request_convert_quote(const QueryParams& params);
  std::string confirm_convert_quote(const std::string& quote_tx_id);
  std::string get_convert_status(const std::string& quote_tx_id, const std::string& account_type);
  std::string get_convert_history(const QueryParams& filters = {});
  std::string get_small_balance_coins(const QueryParams& filters);
  std::string request_small_balance_quote(const std::string& json_body);
  std::string confirm_small_balance_quote(const std::string& quote_id);
  std::string get_small_balance_history(const QueryParams& filters = {});
  std::string get_fiat_convert_coin_list(const QueryParams& filters = {});
  std::string get_fiat_reference_price(const std::string& symbol);
  std::string request_fiat_quote(const QueryParams& params);
  std::string confirm_fiat_quote(const QueryParams& params);
  std::string get_fiat_convert_status(const QueryParams& filters);
  std::string get_fiat_convert_history(const QueryParams& filters = {});
  std::string get_fiat_balance(const QueryParams& filters = {});
  std::string get_asset_sub_uids();
  std::string get_all_coin_balances(const QueryParams& filters);
  std::string get_single_coin_balance(const QueryParams& filters);
  std::string get_transferable_coins(const QueryParams& filters);
  std::string create_internal_transfer(const QueryParams& params);
  std::string get_internal_transfer_records(const QueryParams& filters = {});
  std::string create_universal_transfer(const QueryParams& params);
  std::string get_universal_transfer_records(const QueryParams& filters = {});
  std::string get_deposit_records(const QueryParams& filters = {});
  std::string get_sub_deposit_records(const QueryParams& filters);
  std::string get_internal_deposit_records(const QueryParams& filters = {});
  std::string get_allowed_deposit_coin_info(const QueryParams& filters = {});
  std::string set_deposit_account(const std::string& account_type);
  std::string get_master_deposit_address(const QueryParams& filters);
  std::string get_sub_deposit_address(const QueryParams& filters);
  std::string get_coin_info(const std::optional<std::string>& coin = std::nullopt);
  std::string get_withdrawable_amount(const QueryParams& filters);
  std::string get_withdrawal_address_list(const QueryParams& filters = {});
  std::string get_available_vasps();
  std::string get_withdrawal_records(const QueryParams& filters = {});
  std::string create_withdrawal(const std::string& json_body);
  std::string cancel_withdrawal(const std::string& withdraw_id);
  std::string get_position_info(const std::optional<std::string>& settle_coin = std::nullopt,
                                const std::optional<std::string>& symbol = std::nullopt, int limit = 200);
  std::string submit_order(const std::string& symbol, const std::string& side, const std::string& order_type,
                           const std::string& qty, const std::string& order_link_id, int position_idx,
                           const std::string& price = "", const std::string& time_in_force = "GTC", const std::optional<bool>& reduce_only = std::nullopt,
                           const std::optional<std::string>& bbo_side_type = std::nullopt, const std::optional<std::string>& bbo_level = std::nullopt);
  std::string batch_submit_orders(const std::vector<std::vector<std::pair<std::string, std::string>>>& order_requests);
  std::string batch_cancel_orders(const std::vector<std::vector<std::pair<std::string, std::string>>>& cancel_requests);
  std::string batch_amend_orders(const std::vector<std::vector<std::pair<std::string, std::string>>>& amend_requests);
  std::string set_leverage(const std::string& symbol, const std::string& buy_leverage, const std::string& sell_leverage);
  std::string pre_check_order(const QueryParams& order_params);
  std::string get_order_history(const QueryParams& filters = {});
  std::string get_historic_orders(const std::string& order_id);
  std::string get_trade_history(const QueryParams& filters = {});
  std::string get_closed_pnl(const QueryParams& filters = {});
  std::string get_closed_options_positions(const QueryParams& filters = {});
  std::string create_spread_order(const std::string& json_body);
  std::string amend_spread_order(const std::string& json_body);
  std::string cancel_spread_order(const std::string& json_body);
  std::string cancel_all_spread_orders(const std::string& json_body);
  std::string get_spread_open_orders(const QueryParams& filters = {});
  std::string get_spread_order_history(const QueryParams& filters = {});
  std::string get_spread_trade_history(const QueryParams& filters = {});
  std::string get_spread_max_order_qty(const QueryParams& filters);
  std::string create_rfq(const std::string& json_body);
  std::string get_rfq_config();
  std::string cancel_rfq(const std::string& json_body);
  std::string cancel_all_rfqs();
  std::string accept_non_lp_quote(const std::string& json_body);
  std::string create_rfq_quote(const std::string& json_body);
  std::string execute_rfq_quote(const std::string& json_body);
  std::string cancel_rfq_quote(const std::string& json_body);
  std::string cancel_all_rfq_quotes();
  std::string get_rfqs_realtime(const QueryParams& filters = {});
  std::string get_rfqs(const QueryParams& filters = {});
  std::string get_rfq_quotes_realtime(const QueryParams& filters = {});
  std::string get_rfq_quotes(const QueryParams& filters = {});
  std::string get_rfq_trade_history(const QueryParams& filters = {});
  std::string get_rfq_public_trades(const QueryParams& filters = {});
  std::string get_crypto_loan_borrowable_coins(const QueryParams& filters = {});
  std::string get_crypto_loan_collateral_coins(const QueryParams& filters = {});
  std::string get_crypto_loan_max_collateral_reduction(const QueryParams& filters);
  std::string adjust_crypto_loan_collateral(const std::string& json_body);
  std::string get_crypto_loan_collateral_adjustment_history(const QueryParams& filters = {});
  std::string get_crypto_loan_position();
  std::string get_crypto_loan_max_loan_amount(const std::string& json_body);
  std::string borrow_flexible_crypto_loan(const std::string& json_body);
  std::string repay_flexible_crypto_loan(const std::string& json_body);
  std::string repay_flexible_crypto_loan_with_collateral(const std::string& json_body);
  std::string get_flexible_crypto_loans(const QueryParams& filters = {});
  std::string get_flexible_crypto_loan_borrow_history(const QueryParams& filters = {});
  std::string get_flexible_crypto_loan_repayment_history(const QueryParams& filters = {});
  std::string get_fixed_crypto_loan_lending_market(const QueryParams& filters);
  std::string get_fixed_crypto_loan_borrowing_market(const QueryParams& filters);
  std::string borrow_fixed_crypto_loan(const std::string& json_body);
  std::string renew_fixed_crypto_loan(const std::string& json_body);
  std::string supply_fixed_crypto_loan(const std::string& json_body);
  std::string cancel_fixed_crypto_loan_borrow_order(const std::string& json_body);
  std::string cancel_fixed_crypto_loan_supply_order(const std::string& json_body);
  std::string get_fixed_crypto_loan_borrow_contract_info(const QueryParams& filters = {});
  std::string get_fixed_crypto_loan_supply_contract_info(const QueryParams& filters = {});
  std::string get_fixed_crypto_loan_borrow_order_info(const QueryParams& filters = {});
  std::string get_fixed_crypto_loan_renew_order_info(const QueryParams& filters = {});
  std::string get_fixed_crypto_loan_supply_order_info(const QueryParams& filters = {});
  std::string repay_fixed_crypto_loan(const std::string& json_body);
  std::string repay_fixed_crypto_loan_with_collateral(const std::string& json_body);
  std::string get_fixed_crypto_loan_repayment_history(const QueryParams& filters = {});
  std::string get_institutional_loan_product_info(const QueryParams& filters = {}, bool authenticated = false);
  std::string get_institutional_loan_margin_coin_info(const QueryParams& filters = {}, bool authenticated = false);
  std::string get_institutional_loan_orders(const QueryParams& filters = {});
  std::string get_institutional_loan_repayment_orders(const QueryParams& filters = {});
  std::string get_institutional_loan_ltv();
  std::string bind_or_unbind_institutional_loan_uid(const std::string& json_body);
  std::string repay_institutional_loan(const std::string& json_body);
  std::string get_pre_upgrade_order_history(const QueryParams& filters = {});
  std::string get_pre_upgrade_trade_history(const QueryParams& filters = {});
  std::string get_pre_upgrade_closed_pnl(const QueryParams& filters = {});
  std::string get_pre_upgrade_transaction_log(const QueryParams& filters = {});
  std::string get_pre_upgrade_delivery_records(const QueryParams& filters = {});
  std::string get_pre_upgrade_settlement_records(const QueryParams& filters = {});
  std::string get_fee_rate();
  std::string get_borrow_quota(const std::string& symbol, const std::string& side);
  std::string get_wallet_balance(const std::string& category, const std::optional<std::string>& coin = std::nullopt);
  std::string get_open_orders(const std::optional<std::string>& symbol = std::nullopt, int limit = 50);
  std::string get_realtime_orders(const QueryParams& filters = {});
  std::string cancel_order(const std::string& symbol, const std::string& order_id);
  std::string cancel_all_orders(const QueryParams& filters = {});
  std::string set_disconnect_cancel_all(int time_window, const std::optional<std::string>& product = std::nullopt);
  std::string get_dcp_info();
  std::string amend_order(const std::string& symbol, const std::string& order_id, const std::optional<std::string>& qty = std::nullopt,
                          const std::optional<std::string>& price = std::nullopt);
  std::string get_transaction_log(int limit = 50, const std::optional<std::string>& cursor = std::nullopt);
  std::string move_position(const std::string& from_uid, const std::string& to_uid, const std::string& symbol, const std::string& qty, int position_idx = 1);
  std::string move_positions(const std::string& from_uid, const std::string& to_uid, const std::vector<MovePositionLeg>& legs);
  std::string get_move_position_history(const QueryParams& filters = {});
  std::string set_auto_add_margin(const std::string& symbol, int auto_add_margin, const std::optional<int>& position_idx = std::nullopt);
  std::string confirm_pending_mmr(const std::string& symbol);
  std::string set_trading_stop(const std::string& symbol, int position_idx, const std::optional<std::string>& take_profit = std::nullopt,
                               const std::optional<std::string>& stop_loss = std::nullopt, const std::optional<std::string>& trailing_stop = std::nullopt);
  std::string set_risk_limit(const std::string& symbol, const std::string& risk_id, int position_idx = 1);
  std::string add_margin(const std::string& symbol, const std::string& margin, int position_idx = 1);
  std::string switch_position_mode(const std::string& mode, int position_idx = 1);
  std::string switch_margin_mode(const std::string& symbol, const std::string& mode, int leverage);
  std::string cancel_all(const std::string& symbol);

 private:
  HttpClient& http_;
  std::string category_;
};

}  // namespace bybit
