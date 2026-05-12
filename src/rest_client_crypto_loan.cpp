#include "bybit/rest_client.hpp"

namespace bybit {

std::string RestClient::get_crypto_loan_borrowable_coins(const QueryParams& filters) {
  return private_.get_crypto_loan_borrowable_coins(filters);
}

std::string RestClient::get_crypto_loan_collateral_coins(const QueryParams& filters) {
  return private_.get_crypto_loan_collateral_coins(filters);
}

std::string RestClient::get_crypto_loan_max_collateral_reduction(const QueryParams& filters) {
  return private_.get_crypto_loan_max_collateral_reduction(filters);
}

std::string RestClient::adjust_crypto_loan_collateral(const std::string& json_body) {
  return private_.adjust_crypto_loan_collateral(json_body);
}

std::string RestClient::get_crypto_loan_collateral_adjustment_history(const QueryParams& filters) {
  return private_.get_crypto_loan_collateral_adjustment_history(filters);
}

std::string RestClient::get_crypto_loan_position() {
  return private_.get_crypto_loan_position();
}

std::string RestClient::get_crypto_loan_max_loan_amount(const std::string& json_body) {
  return private_.get_crypto_loan_max_loan_amount(json_body);
}

std::string RestClient::borrow_flexible_crypto_loan(const std::string& json_body) {
  return private_.borrow_flexible_crypto_loan(json_body);
}

std::string RestClient::repay_flexible_crypto_loan(const std::string& json_body) {
  return private_.repay_flexible_crypto_loan(json_body);
}

std::string RestClient::repay_flexible_crypto_loan_with_collateral(const std::string& json_body) {
  return private_.repay_flexible_crypto_loan_with_collateral(json_body);
}

std::string RestClient::get_flexible_crypto_loans(const QueryParams& filters) {
  return private_.get_flexible_crypto_loans(filters);
}

std::string RestClient::get_flexible_crypto_loan_borrow_history(const QueryParams& filters) {
  return private_.get_flexible_crypto_loan_borrow_history(filters);
}

std::string RestClient::get_flexible_crypto_loan_repayment_history(const QueryParams& filters) {
  return private_.get_flexible_crypto_loan_repayment_history(filters);
}

std::string RestClient::get_fixed_crypto_loan_lending_market(const QueryParams& filters) {
  return private_.get_fixed_crypto_loan_lending_market(filters);
}

std::string RestClient::get_fixed_crypto_loan_borrowing_market(const QueryParams& filters) {
  return private_.get_fixed_crypto_loan_borrowing_market(filters);
}

std::string RestClient::borrow_fixed_crypto_loan(const std::string& json_body) {
  return private_.borrow_fixed_crypto_loan(json_body);
}

std::string RestClient::renew_fixed_crypto_loan(const std::string& json_body) {
  return private_.renew_fixed_crypto_loan(json_body);
}

std::string RestClient::supply_fixed_crypto_loan(const std::string& json_body) {
  return private_.supply_fixed_crypto_loan(json_body);
}

std::string RestClient::cancel_fixed_crypto_loan_borrow_order(const std::string& json_body) {
  return private_.cancel_fixed_crypto_loan_borrow_order(json_body);
}

std::string RestClient::cancel_fixed_crypto_loan_supply_order(const std::string& json_body) {
  return private_.cancel_fixed_crypto_loan_supply_order(json_body);
}

std::string RestClient::get_fixed_crypto_loan_borrow_contract_info(const QueryParams& filters) {
  return private_.get_fixed_crypto_loan_borrow_contract_info(filters);
}

std::string RestClient::get_fixed_crypto_loan_supply_contract_info(const QueryParams& filters) {
  return private_.get_fixed_crypto_loan_supply_contract_info(filters);
}

std::string RestClient::get_fixed_crypto_loan_borrow_order_info(const QueryParams& filters) {
  return private_.get_fixed_crypto_loan_borrow_order_info(filters);
}

std::string RestClient::get_fixed_crypto_loan_renew_order_info(const QueryParams& filters) {
  return private_.get_fixed_crypto_loan_renew_order_info(filters);
}

std::string RestClient::get_fixed_crypto_loan_supply_order_info(const QueryParams& filters) {
  return private_.get_fixed_crypto_loan_supply_order_info(filters);
}

std::string RestClient::repay_fixed_crypto_loan(const std::string& json_body) {
  return private_.repay_fixed_crypto_loan(json_body);
}

std::string RestClient::repay_fixed_crypto_loan_with_collateral(const std::string& json_body) {
  return private_.repay_fixed_crypto_loan_with_collateral(json_body);
}

std::string RestClient::get_fixed_crypto_loan_repayment_history(const QueryParams& filters) {
  return private_.get_fixed_crypto_loan_repayment_history(filters);
}

}  // namespace bybit
