#include "bybit/rest_client.hpp"

namespace bybit {

std::string RestClient::get_institutional_loan_product_info(const QueryParams& filters, bool authenticated) {
  return private_.get_institutional_loan_product_info(filters, authenticated);
}

std::string RestClient::get_institutional_loan_margin_coin_info(const QueryParams& filters, bool authenticated) {
  return private_.get_institutional_loan_margin_coin_info(filters, authenticated);
}

std::string RestClient::get_institutional_loan_orders(const QueryParams& filters) {
  return private_.get_institutional_loan_orders(filters);
}

std::string RestClient::get_institutional_loan_repayment_orders(const QueryParams& filters) {
  return private_.get_institutional_loan_repayment_orders(filters);
}

std::string RestClient::get_institutional_loan_ltv() {
  return private_.get_institutional_loan_ltv();
}

std::string RestClient::bind_or_unbind_institutional_loan_uid(const std::string& json_body) {
  return private_.bind_or_unbind_institutional_loan_uid(json_body);
}

std::string RestClient::repay_institutional_loan(const std::string& json_body) {
  return private_.repay_institutional_loan(json_body);
}

}  // namespace bybit
