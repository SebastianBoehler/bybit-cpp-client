#include "bybit/http_client.hpp"
#include "bybit/rest_client.hpp"

namespace bybit {

std::string PrivateRestClient::get_crypto_loan_borrowable_coins(const QueryParams& filters) {
  return http_.get("/v5/crypto-loan-common/loanable-data", filters, false);
}

std::string PrivateRestClient::get_crypto_loan_collateral_coins(const QueryParams& filters) {
  return http_.get("/v5/crypto-loan-common/collateral-data", filters, false);
}

std::string PrivateRestClient::get_crypto_loan_max_collateral_reduction(const QueryParams& filters) {
  return http_.get("/v5/crypto-loan-common/max-collateral-amount", filters, true);
}

std::string PrivateRestClient::adjust_crypto_loan_collateral(const std::string& json_body) {
  return http_.post("/v5/crypto-loan-common/adjust-ltv", json_body, true);
}

std::string PrivateRestClient::get_crypto_loan_collateral_adjustment_history(const QueryParams& filters) {
  return http_.get("/v5/crypto-loan-common/adjustment-history", filters, true);
}

std::string PrivateRestClient::get_crypto_loan_position() {
  return http_.get("/v5/crypto-loan-common/position", {}, true);
}

std::string PrivateRestClient::get_crypto_loan_max_loan_amount(const std::string& json_body) {
  return http_.post("/v5/crypto-loan-common/max-loan", json_body, true);
}

std::string PrivateRestClient::borrow_flexible_crypto_loan(const std::string& json_body) {
  return http_.post("/v5/crypto-loan-flexible/borrow", json_body, true);
}

std::string PrivateRestClient::repay_flexible_crypto_loan(const std::string& json_body) {
  return http_.post("/v5/crypto-loan-flexible/repay", json_body, true);
}

std::string PrivateRestClient::repay_flexible_crypto_loan_with_collateral(const std::string& json_body) {
  return http_.post("/v5/crypto-loan-flexible/repay-collateral", json_body, true);
}

std::string PrivateRestClient::get_flexible_crypto_loans(const QueryParams& filters) {
  return http_.get("/v5/crypto-loan-flexible/ongoing-coin", filters, true);
}

std::string PrivateRestClient::get_flexible_crypto_loan_borrow_history(const QueryParams& filters) {
  return http_.get("/v5/crypto-loan-flexible/borrow-history", filters, true);
}

std::string PrivateRestClient::get_flexible_crypto_loan_repayment_history(const QueryParams& filters) {
  return http_.get("/v5/crypto-loan-flexible/repayment-history", filters, true);
}

std::string PrivateRestClient::get_fixed_crypto_loan_lending_market(const QueryParams& filters) {
  return http_.get("/v5/crypto-loan-fixed/supply-order-quote", filters, false);
}

std::string PrivateRestClient::get_fixed_crypto_loan_borrowing_market(const QueryParams& filters) {
  return http_.get("/v5/crypto-loan-fixed/borrow-order-quote", filters, false);
}

std::string PrivateRestClient::borrow_fixed_crypto_loan(const std::string& json_body) {
  return http_.post("/v5/crypto-loan-fixed/borrow", json_body, true);
}

std::string PrivateRestClient::renew_fixed_crypto_loan(const std::string& json_body) {
  return http_.post("/v5/crypto-loan-fixed/renew", json_body, true);
}

std::string PrivateRestClient::supply_fixed_crypto_loan(const std::string& json_body) {
  return http_.post("/v5/crypto-loan-fixed/supply", json_body, true);
}

std::string PrivateRestClient::cancel_fixed_crypto_loan_borrow_order(const std::string& json_body) {
  return http_.post("/v5/crypto-loan-fixed/borrow-order-cancel", json_body, true);
}

std::string PrivateRestClient::cancel_fixed_crypto_loan_supply_order(const std::string& json_body) {
  return http_.post("/v5/crypto-loan-fixed/supply-order-cancel", json_body, true);
}

std::string PrivateRestClient::get_fixed_crypto_loan_borrow_contract_info(const QueryParams& filters) {
  return http_.get("/v5/crypto-loan-fixed/borrow-contract-info", filters, true);
}

std::string PrivateRestClient::get_fixed_crypto_loan_supply_contract_info(const QueryParams& filters) {
  return http_.get("/v5/crypto-loan-fixed/supply-contract-info", filters, true);
}

std::string PrivateRestClient::get_fixed_crypto_loan_borrow_order_info(const QueryParams& filters) {
  return http_.get("/v5/crypto-loan-fixed/borrow-order-info", filters, true);
}

std::string PrivateRestClient::get_fixed_crypto_loan_renew_order_info(const QueryParams& filters) {
  return http_.get("/v5/crypto-loan-fixed/renew-info", filters, true);
}

std::string PrivateRestClient::get_fixed_crypto_loan_supply_order_info(const QueryParams& filters) {
  return http_.get("/v5/crypto-loan-fixed/supply-order-info", filters, true);
}

std::string PrivateRestClient::repay_fixed_crypto_loan(const std::string& json_body) {
  return http_.post("/v5/crypto-loan-fixed/fully-repay", json_body, true);
}

std::string PrivateRestClient::repay_fixed_crypto_loan_with_collateral(const std::string& json_body) {
  return http_.post("/v5/crypto-loan-fixed/repay-collateral", json_body, true);
}

std::string PrivateRestClient::get_fixed_crypto_loan_repayment_history(const QueryParams& filters) {
  return http_.get("/v5/crypto-loan-fixed/repayment-history", filters, true);
}

std::string PrivateRestClient::get_legacy_crypto_loan_borrowable_coins(const QueryParams& filters) {
  return http_.get("/v5/crypto-loan/loanable-data", filters, false);
}

std::string PrivateRestClient::get_legacy_crypto_loan_collateral_coins(const QueryParams& filters) {
  return http_.get("/v5/crypto-loan/collateral-data", filters, false);
}

std::string PrivateRestClient::get_legacy_crypto_loan_account_limits(const QueryParams& filters) {
  return http_.get("/v5/crypto-loan/borrowable-collateralisable-number", filters, true);
}

std::string PrivateRestClient::borrow_legacy_crypto_loan(const std::string& json_body) {
  return http_.post("/v5/crypto-loan/borrow", json_body, true);
}

std::string PrivateRestClient::repay_legacy_crypto_loan(const std::string& json_body) {
  return http_.post("/v5/crypto-loan/repay", json_body, true);
}

std::string PrivateRestClient::get_legacy_crypto_loan_unpaid_loans(const QueryParams& filters) {
  return http_.get("/v5/crypto-loan/ongoing-orders", filters, true);
}

std::string PrivateRestClient::get_legacy_crypto_loan_repayment_history(const QueryParams& filters) {
  return http_.get("/v5/crypto-loan/repayment-history", filters, true);
}

std::string PrivateRestClient::get_legacy_crypto_loan_completed_history(const QueryParams& filters) {
  return http_.get("/v5/crypto-loan/borrow-history", filters, true);
}

std::string PrivateRestClient::get_legacy_crypto_loan_max_collateral_reduction(const QueryParams& filters) {
  return http_.get("/v5/crypto-loan/max-collateral-amount", filters, true);
}

std::string PrivateRestClient::adjust_legacy_crypto_loan_collateral(const std::string& json_body) {
  return http_.post("/v5/crypto-loan/adjust-ltv", json_body, true);
}

std::string PrivateRestClient::get_legacy_crypto_loan_ltv_adjustment_history(const QueryParams& filters) {
  return http_.get("/v5/crypto-loan/adjustment-history", filters, true);
}

}  // namespace bybit
