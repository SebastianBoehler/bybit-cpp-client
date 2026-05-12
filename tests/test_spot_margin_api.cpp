#include <string>

#include "bybit/rest_client.hpp"

int main() {
  using RestClient = bybit::RestClient;

  auto currency_data = &RestClient::get_spot_margin_currency_data;
  auto max_borrowable = &RestClient::get_spot_margin_max_borrowable;
  auto position_tiers = &RestClient::get_spot_margin_position_tiers;
  auto coin_state = &RestClient::get_spot_margin_coin_state;
  auto set_auto_repay = &RestClient::set_spot_margin_auto_repay_mode;
  auto get_auto_repay = &RestClient::get_spot_margin_auto_repay_mode;
  auto fixed_quote = &RestClient::get_spot_margin_fixed_borrow_order_quote;
  auto fixed_borrow = &RestClient::create_spot_margin_fixed_borrow;
  auto fixed_renew = &RestClient::renew_spot_margin_fixed_borrow;
  auto fixed_order_info = &RestClient::get_spot_margin_fixed_borrow_order_info;
  auto fixed_contract_info = &RestClient::get_spot_margin_fixed_borrow_contract_info;
  auto liability = &RestClient::get_spot_margin_liability;
  auto cross_vip_data = &RestClient::get_spot_cross_margin_vip_data;
  auto cross_pledge_token = &RestClient::get_spot_cross_margin_pledge_token;
  auto cross_borrow_token = &RestClient::get_spot_cross_margin_borrow_token;
  auto cross_loan_info = &RestClient::get_spot_cross_margin_loan_info;
  auto cross_account = &RestClient::get_spot_cross_margin_account;
  auto cross_borrow = &RestClient::borrow_spot_cross_margin;
  auto cross_repay = &RestClient::repay_spot_cross_margin;
  auto cross_borrow_orders = &RestClient::get_spot_cross_margin_borrow_orders;
  auto cross_repay_history = &RestClient::get_spot_cross_margin_repayment_history;
  auto cross_switch = &RestClient::switch_spot_cross_margin;

  (void)currency_data;
  (void)max_borrowable;
  (void)position_tiers;
  (void)coin_state;
  (void)set_auto_repay;
  (void)get_auto_repay;
  (void)fixed_quote;
  (void)fixed_borrow;
  (void)fixed_renew;
  (void)fixed_order_info;
  (void)fixed_contract_info;
  (void)liability;
  (void)cross_vip_data;
  (void)cross_pledge_token;
  (void)cross_borrow_token;
  (void)cross_loan_info;
  (void)cross_account;
  (void)cross_borrow;
  (void)cross_repay;
  (void)cross_borrow_orders;
  (void)cross_repay_history;
  (void)cross_switch;

  return 0;
}
