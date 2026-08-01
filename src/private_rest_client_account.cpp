#include <optional>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "bybit/http_client.hpp"
#include "bybit/rest_client.hpp"

namespace bybit {
namespace {

std::string build_batch_collateral_body(const std::vector<CollateralCoinSwitch>& request) {
  if (request.empty()) {
    throw std::invalid_argument("batch_set_collateral_coins requires at least one collateral switch");
  }

  std::ostringstream oss;
  oss << "{\"request\":[";
  for (size_t i = 0; i < request.size(); ++i) {
    oss << to_json_object({{"coin", request[i].coin}, {"collateralSwitch", request[i].collateral_switch}});
    if (i + 1 < request.size())
      oss << ",";
  }
  oss << "]}";
  return oss.str();
}

std::string build_price_limit_body(const std::string& category, bool modify_enable) {
  std::ostringstream oss;
  oss << "{\"category\":" << serialize_json_string(category)
      << ",\"modifyEnable\":" << (modify_enable ? "true" : "false") << "}";
  return oss.str();
}

}  // namespace

std::string PrivateRestClient::upgrade_to_unified_account() {
  return http_.post("/v5/account/upgrade-to-uta", "{}", true);
}

std::string PrivateRestClient::request_demo_trading_funds(const std::string& json_body) {
  return http_.post("/v5/account/demo-apply-money", json_body, true);
}

std::string PrivateRestClient::get_account_instruments_info(const QueryParams& filters) {
  return http_.get("/v5/account/instruments-info", filters, true);
}

std::string PrivateRestClient::get_transferable_amount(const std::string& coin_name) {
  return http_.get("/v5/account/withdrawal", {{"coinName", coin_name}}, true);
}

std::string PrivateRestClient::get_collateral_info(const std::optional<std::string>& currency) {
  QueryParams params;
  if (currency)
    params.emplace_back("currency", *currency);
  return http_.get("/v5/account/collateral-info", params, true);
}

std::string PrivateRestClient::set_collateral_coin(const std::string& coin, const std::string& collateral_switch) {
  return http_.post("/v5/account/set-collateral-switch",
                    to_json_object({{"coin", coin}, {"collateralSwitch", collateral_switch}}), true);
}

std::string PrivateRestClient::batch_set_collateral_coins(const std::vector<CollateralCoinSwitch>& request) {
  return http_.post("/v5/account/set-collateral-switch-batch", build_batch_collateral_body(request), true);
}

std::string PrivateRestClient::get_borrow_history(const QueryParams& filters) {
  return http_.get("/v5/account/borrow-history", filters, true);
}

std::string PrivateRestClient::get_coin_greeks(const std::optional<std::string>& base_coin) {
  QueryParams params;
  if (base_coin)
    params.emplace_back("baseCoin", *base_coin);
  return http_.get("/v5/asset/coin-greeks", params, true);
}

std::string PrivateRestClient::manual_borrow(const std::string& coin, const std::string& amount) {
  return http_.post("/v5/account/borrow", to_json_object({{"coin", coin}, {"amount", amount}}), true);
}

std::string PrivateRestClient::manual_repay(const QueryParams& params) {
  return http_.post("/v5/account/repay", to_json_object(params), true);
}

std::string PrivateRestClient::no_convert_repay(const std::string& coin, const std::optional<std::string>& amount,
                                                const std::optional<std::string>& repayment_type) {
  QueryParams params{{"coin", coin}};
  if (amount)
    params.emplace_back("amount", *amount);
  if (repayment_type)
    params.emplace_back("repaymentType", *repayment_type);
  return http_.post("/v5/account/no-convert-repay", to_json_object(params), true);
}

std::string PrivateRestClient::quick_repayment(const std::optional<std::string>& coin) {
  QueryParams params;
  if (coin)
    params.emplace_back("coin", *coin);
  return http_.post("/v5/account/quick-repayment", to_json_object(params), true);
}

std::string PrivateRestClient::set_spot_hedging(const std::string& set_hedging_mode) {
  return http_.post("/v5/account/set-hedging-mode", to_json_object({{"setHedgingMode", set_hedging_mode}}), true);
}

std::string PrivateRestClient::get_mmp_state(const std::string& base_coin) {
  return http_.get("/v5/account/mmp-state", {{"baseCoin", base_coin}}, true);
}

std::string PrivateRestClient::reset_mmp(const std::string& base_coin) {
  return http_.post("/v5/account/mmp-reset", to_json_object({{"baseCoin", base_coin}}), true);
}

std::string PrivateRestClient::set_mmp(const QueryParams& params) {
  return http_.post("/v5/account/mmp-modify", to_json_object(params), true);
}

std::string PrivateRestClient::get_smp_group() {
  return http_.get("/v5/account/smp-group", {}, true);
}

std::string PrivateRestClient::get_trade_behaviour_config() {
  return http_.get("/v5/account/user-setting-config", {}, true);
}

std::string PrivateRestClient::get_limit_price_behaviour() {
  return get_trade_behaviour_config();
}

std::string PrivateRestClient::set_delta_neutral_mode(const std::string& delta_enable) {
  return http_.post("/v5/account/set-delta-mode", to_json_object({{"deltaEnable", delta_enable}}), true);
}

std::string PrivateRestClient::set_price_limit_behaviour(const std::string& category, bool modify_enable) {
  return http_.post("/v5/account/set-limit-px-action", build_price_limit_body(category, modify_enable), true);
}

std::string PrivateRestClient::get_option_asset_info() {
  return http_.get("/v5/account/option-asset-info", {}, true);
}

std::string PrivateRestClient::get_pay_info(const std::optional<std::string>& coin) {
  QueryParams params;
  if (coin)
    params.emplace_back("coin", *coin);
  return http_.get("/v5/account/pay-info", params, true);
}

std::string PrivateRestClient::get_trade_info_for_analysis(const QueryParams& filters) {
  return http_.get("/v5/account/trade-info-for-analysis", filters, true);
}

}  // namespace bybit
