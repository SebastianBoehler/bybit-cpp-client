#include "bybit/rest_client.hpp"

#include <optional>
#include <string>

namespace bybit {

std::string RestClient::get_spot_margin_vip_data(const QueryParams& filters) {
  return private_.get_spot_margin_vip_data(filters);
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

std::string RestClient::get_spot_margin_repayment_available_amount(const std::string& currency) {
  return private_.get_spot_margin_repayment_available_amount(currency);
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

}  // namespace bybit
