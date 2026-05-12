#include "bybit/rest_client.hpp"

#include <optional>
#include <string>

namespace bybit {

std::string RestClient::get_spot_margin_vip_data(const QueryParams& filters) {
  return private_.get_spot_margin_vip_data(filters);
}

std::string RestClient::get_spot_margin_currency_data(const std::optional<std::string>& currency) {
  return private_.get_spot_margin_currency_data(currency);
}

std::string RestClient::get_spot_margin_state() {
  return private_.get_spot_margin_state();
}

std::string RestClient::switch_spot_margin_mode(const std::string& spot_margin_mode) {
  return private_.switch_spot_margin_mode(spot_margin_mode);
}

std::string RestClient::set_spot_margin_leverage(const std::string& leverage,
                                                 const std::optional<std::string>& currency) {
  return private_.set_spot_margin_leverage(leverage, currency);
}

std::string RestClient::get_spot_margin_max_borrowable(const std::string& currency) {
  return private_.get_spot_margin_max_borrowable(currency);
}

std::string RestClient::get_spot_margin_position_tiers(const std::optional<std::string>& currency) {
  return private_.get_spot_margin_position_tiers(currency);
}

std::string RestClient::get_spot_margin_coin_state(const std::optional<std::string>& currency) {
  return private_.get_spot_margin_coin_state(currency);
}

std::string RestClient::get_spot_margin_repayment_available_amount(const std::string& currency) {
  return private_.get_spot_margin_repayment_available_amount(currency);
}

std::string RestClient::set_spot_margin_auto_repay_mode(const std::string& auto_repay_mode,
                                                        const std::optional<std::string>& currency) {
  return private_.set_spot_margin_auto_repay_mode(auto_repay_mode, currency);
}

std::string RestClient::get_spot_margin_auto_repay_mode(const std::optional<std::string>& currency) {
  return private_.get_spot_margin_auto_repay_mode(currency);
}

std::string RestClient::get_spot_margin_interest_rate_history(const QueryParams& filters) {
  return private_.get_spot_margin_interest_rate_history(filters);
}

std::string RestClient::get_spot_margin_tiered_collateral_ratio(const QueryParams& filters) {
  return private_.get_spot_margin_tiered_collateral_ratio(filters);
}

std::string RestClient::get_spot_margin_fixed_borrow_order_quote(const QueryParams& filters) {
  return private_.get_spot_margin_fixed_borrow_order_quote(filters);
}

std::string RestClient::create_spot_margin_fixed_borrow(const QueryParams& params) {
  return private_.create_spot_margin_fixed_borrow(params);
}

std::string RestClient::renew_spot_margin_fixed_borrow(const std::string& loan_id,
                                                       const std::optional<std::string>& qty) {
  return private_.renew_spot_margin_fixed_borrow(loan_id, qty);
}

std::string RestClient::get_spot_margin_fixed_borrow_order_info(const QueryParams& filters) {
  return private_.get_spot_margin_fixed_borrow_order_info(filters);
}

std::string RestClient::get_spot_margin_fixed_borrow_contract_info(const QueryParams& filters) {
  return private_.get_spot_margin_fixed_borrow_contract_info(filters);
}

std::string RestClient::get_spot_margin_liability(const std::string& currency) {
  return private_.get_spot_margin_liability(currency);
}

}  // namespace bybit
