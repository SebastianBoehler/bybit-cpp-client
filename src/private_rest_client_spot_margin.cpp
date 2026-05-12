#include <optional>
#include <string>

#include "bybit/http_client.hpp"
#include "bybit/rest_client.hpp"

namespace bybit {

std::string PrivateRestClient::get_spot_margin_vip_data(const QueryParams& filters) {
  return http_.get("/v5/spot-margin-trade/data", filters, false);
}

std::string PrivateRestClient::get_spot_margin_currency_data(const std::optional<std::string>& currency) {
  QueryParams params;
  if (currency)
    params.emplace_back("currency", *currency);
  return http_.get("/v5/spot-margin-trade/currency-data", params, true);
}

std::string PrivateRestClient::get_spot_margin_state() {
  return http_.get("/v5/spot-margin-trade/state", {}, true);
}

std::string PrivateRestClient::switch_spot_margin_mode(const std::string& spot_margin_mode) {
  return http_.post("/v5/spot-margin-trade/switch-mode", to_json_object({{"spotMarginMode", spot_margin_mode}}), true);
}

std::string PrivateRestClient::set_spot_margin_leverage(const std::string& leverage,
                                                        const std::optional<std::string>& currency) {
  QueryParams params{{"leverage", leverage}};
  if (currency)
    params.emplace_back("currency", *currency);
  return http_.post("/v5/spot-margin-trade/set-leverage", to_json_object(params), true);
}

std::string PrivateRestClient::get_spot_margin_max_borrowable(const std::string& currency) {
  return http_.get("/v5/spot-margin-trade/max-borrowable", {{"currency", currency}}, true);
}

std::string PrivateRestClient::get_spot_margin_position_tiers(const std::optional<std::string>& currency) {
  QueryParams params;
  if (currency)
    params.emplace_back("currency", *currency);
  return http_.get("/v5/spot-margin-trade/position-tiers", params, true);
}

std::string PrivateRestClient::get_spot_margin_coin_state(const std::optional<std::string>& currency) {
  QueryParams params;
  if (currency)
    params.emplace_back("currency", *currency);
  return http_.get("/v5/spot-margin-trade/coinstate", params, true);
}

std::string PrivateRestClient::get_spot_margin_repayment_available_amount(const std::string& currency) {
  return http_.get("/v5/spot-margin-trade/repayment-available-amount", {{"currency", currency}}, true);
}

std::string PrivateRestClient::set_spot_margin_auto_repay_mode(const std::string& auto_repay_mode,
                                                               const std::optional<std::string>& currency) {
  QueryParams params{{"autoRepayMode", auto_repay_mode}};
  if (currency)
    params.emplace_back("currency", *currency);
  return http_.post("/v5/spot-margin-trade/set-auto-repay-mode", to_json_object(params), true);
}

std::string PrivateRestClient::get_spot_margin_auto_repay_mode(const std::optional<std::string>& currency) {
  QueryParams params;
  if (currency)
    params.emplace_back("currency", *currency);
  return http_.get("/v5/spot-margin-trade/get-auto-repay-mode", params, true);
}

std::string PrivateRestClient::get_spot_margin_interest_rate_history(const QueryParams& filters) {
  return http_.get("/v5/spot-margin-trade/interest-rate-history", filters, true);
}

std::string PrivateRestClient::get_spot_margin_tiered_collateral_ratio(const QueryParams& filters) {
  return http_.get("/v5/spot-margin-trade/collateral", filters, false);
}

std::string PrivateRestClient::get_spot_margin_fixed_borrow_order_quote(const QueryParams& filters) {
  return http_.get("/v5/spot-margin-trade/fixedborrow-order-quote", filters, true);
}

std::string PrivateRestClient::create_spot_margin_fixed_borrow(const QueryParams& params) {
  return http_.post("/v5/spot-margin-trade/fixedborrow", to_json_object(params), true);
}

std::string PrivateRestClient::renew_spot_margin_fixed_borrow(const std::string& loan_id,
                                                              const std::optional<std::string>& qty) {
  QueryParams params{{"loanId", loan_id}};
  if (qty)
    params.emplace_back("qty", *qty);
  return http_.post("/v5/spot-margin-trade/fixedborrow-renew", to_json_object(params), true);
}

std::string PrivateRestClient::get_spot_margin_fixed_borrow_order_info(const QueryParams& filters) {
  return http_.get("/v5/spot-margin-trade/fixedborrow-order-info", filters, true);
}

std::string PrivateRestClient::get_spot_margin_fixed_borrow_contract_info(const QueryParams& filters) {
  return http_.get("/v5/spot-margin-trade/fixedborrow-contract-info", filters, true);
}

std::string PrivateRestClient::get_spot_margin_liability(const std::string& currency) {
  return http_.get("/v5/spot-margin-trade/liability", {{"currency", currency}}, true);
}

std::string PrivateRestClient::get_spot_cross_margin_vip_data(const QueryParams& filters) {
  return http_.get("/v5/spot-cross-margin-trade/data", filters, false);
}

std::string PrivateRestClient::get_spot_cross_margin_pledge_token(const QueryParams& filters) {
  return http_.get("/v5/spot-cross-margin-trade/pledge-token", filters, false);
}

std::string PrivateRestClient::get_spot_cross_margin_borrow_token(const QueryParams& filters) {
  return http_.get("/v5/spot-cross-margin-trade/borrow-token", filters, false);
}

std::string PrivateRestClient::get_spot_cross_margin_loan_info(const std::string& coin) {
  return http_.get("/v5/spot-cross-margin-trade/loan-info", {{"coin", coin}}, true);
}

std::string PrivateRestClient::get_spot_cross_margin_account(const QueryParams& filters) {
  return http_.get("/v5/spot-cross-margin-trade/account", filters, true);
}

std::string PrivateRestClient::borrow_spot_cross_margin(const std::string& coin, const std::string& qty) {
  return http_.post("/v5/spot-cross-margin-trade/loan", to_json_object({{"coin", coin}, {"qty", qty}}), true);
}

std::string PrivateRestClient::repay_spot_cross_margin(const std::string& coin, const std::optional<std::string>& qty) {
  QueryParams params{{"coin", coin}};
  if (qty)
    params.emplace_back("qty", *qty);
  return http_.post("/v5/spot-cross-margin-trade/repay", to_json_object(params), true);
}

std::string PrivateRestClient::get_spot_cross_margin_borrow_orders(const QueryParams& filters) {
  return http_.get("/v5/spot-cross-margin-trade/orders", filters, true);
}

std::string PrivateRestClient::get_spot_cross_margin_repayment_history(const QueryParams& filters) {
  return http_.get("/v5/spot-cross-margin-trade/repay-history", filters, true);
}

std::string PrivateRestClient::switch_spot_cross_margin(const std::string& switch_flag) {
  return http_.post("/v5/spot-cross-margin-trade/switch", to_json_object({{"switch", switch_flag}}), true);
}

}  // namespace bybit
