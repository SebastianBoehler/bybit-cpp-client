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

  return 0;
}
