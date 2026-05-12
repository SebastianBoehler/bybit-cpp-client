#include "bybit/rest_client.hpp"

int main() {
  using RestClient = bybit::RestClient;

  auto borrowable = &RestClient::get_crypto_loan_borrowable_coins;
  auto collateral = &RestClient::get_crypto_loan_collateral_coins;
  auto max_reduction = &RestClient::get_crypto_loan_max_collateral_reduction;
  auto adjust = &RestClient::adjust_crypto_loan_collateral;
  auto adjustment_history = &RestClient::get_crypto_loan_collateral_adjustment_history;
  auto position = &RestClient::get_crypto_loan_position;
  auto max_loan = &RestClient::get_crypto_loan_max_loan_amount;
  auto flex_borrow = &RestClient::borrow_flexible_crypto_loan;
  auto flex_repay = &RestClient::repay_flexible_crypto_loan;
  auto flex_collateral_repay = &RestClient::repay_flexible_crypto_loan_with_collateral;
  auto flex_loans = &RestClient::get_flexible_crypto_loans;
  auto flex_borrow_history = &RestClient::get_flexible_crypto_loan_borrow_history;
  auto flex_repay_history = &RestClient::get_flexible_crypto_loan_repayment_history;
  auto fixed_lending_market = &RestClient::get_fixed_crypto_loan_lending_market;
  auto fixed_borrowing_market = &RestClient::get_fixed_crypto_loan_borrowing_market;
  auto fixed_borrow = &RestClient::borrow_fixed_crypto_loan;
  auto fixed_renew = &RestClient::renew_fixed_crypto_loan;
  auto fixed_supply = &RestClient::supply_fixed_crypto_loan;
  auto fixed_cancel_borrow = &RestClient::cancel_fixed_crypto_loan_borrow_order;
  auto fixed_cancel_supply = &RestClient::cancel_fixed_crypto_loan_supply_order;
  auto fixed_borrow_contract = &RestClient::get_fixed_crypto_loan_borrow_contract_info;
  auto fixed_supply_contract = &RestClient::get_fixed_crypto_loan_supply_contract_info;
  auto fixed_borrow_order = &RestClient::get_fixed_crypto_loan_borrow_order_info;
  auto fixed_renew_order = &RestClient::get_fixed_crypto_loan_renew_order_info;
  auto fixed_supply_order = &RestClient::get_fixed_crypto_loan_supply_order_info;
  auto fixed_repay = &RestClient::repay_fixed_crypto_loan;
  auto fixed_collateral_repay = &RestClient::repay_fixed_crypto_loan_with_collateral;
  auto fixed_repay_history = &RestClient::get_fixed_crypto_loan_repayment_history;

  (void)borrowable;
  (void)collateral;
  (void)max_reduction;
  (void)adjust;
  (void)adjustment_history;
  (void)position;
  (void)max_loan;
  (void)flex_borrow;
  (void)flex_repay;
  (void)flex_collateral_repay;
  (void)flex_loans;
  (void)flex_borrow_history;
  (void)flex_repay_history;
  (void)fixed_lending_market;
  (void)fixed_borrowing_market;
  (void)fixed_borrow;
  (void)fixed_renew;
  (void)fixed_supply;
  (void)fixed_cancel_borrow;
  (void)fixed_cancel_supply;
  (void)fixed_borrow_contract;
  (void)fixed_supply_contract;
  (void)fixed_borrow_order;
  (void)fixed_renew_order;
  (void)fixed_supply_order;
  (void)fixed_repay;
  (void)fixed_collateral_repay;
  (void)fixed_repay_history;

  return 0;
}
