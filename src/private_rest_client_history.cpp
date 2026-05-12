#include <algorithm>
#include <string>
#include <utility>
#include <vector>

#include "bybit/http_client.hpp"
#include "bybit/rest_client.hpp"

namespace bybit {
namespace {

bool has_param(const QueryParams& params, const std::string& key) {
  return std::any_of(params.begin(), params.end(), [&key](const auto& param) { return param.first == key; });
}

QueryParams with_category(const std::string& category, const QueryParams& filters) {
  QueryParams params;
  if (!has_param(filters, "category")) {
    params.emplace_back("category", category);
  }
  params.insert(params.end(), filters.begin(), filters.end());
  return params;
}

}  // namespace

std::string PrivateRestClient::get_order_history(const QueryParams& filters) {
  return http_.get("/v5/order/history", with_category(category_, filters), true);
}

std::string PrivateRestClient::get_realtime_orders(const QueryParams& filters) {
  return http_.get("/v5/order/realtime", with_category(category_, filters), true);
}

std::string PrivateRestClient::get_trade_history(const QueryParams& filters) {
  return http_.get("/v5/execution/list", with_category(category_, filters), true);
}

std::string PrivateRestClient::get_closed_pnl(const QueryParams& filters) {
  return http_.get("/v5/position/closed-pnl", with_category(category_, filters), true);
}

std::string PrivateRestClient::get_closed_options_positions(const QueryParams& filters) {
  return http_.get("/v5/position/get-closed-positions", with_category("option", filters), true);
}

std::string PrivateRestClient::get_pre_upgrade_order_history(const QueryParams& filters) {
  return http_.get("/v5/pre-upgrade/order/history", with_category(category_, filters), true);
}

std::string PrivateRestClient::get_pre_upgrade_trade_history(const QueryParams& filters) {
  return http_.get("/v5/pre-upgrade/execution/list", with_category(category_, filters), true);
}

std::string PrivateRestClient::get_pre_upgrade_closed_pnl(const QueryParams& filters) {
  return http_.get("/v5/pre-upgrade/position/closed-pnl", with_category(category_, filters), true);
}

std::string PrivateRestClient::get_pre_upgrade_transaction_log(const QueryParams& filters) {
  return http_.get("/v5/pre-upgrade/account/transaction-log", with_category(category_, filters), true);
}

std::string PrivateRestClient::get_pre_upgrade_delivery_records(const QueryParams& filters) {
  return http_.get("/v5/pre-upgrade/asset/delivery-record", with_category("option", filters), true);
}

std::string PrivateRestClient::get_pre_upgrade_settlement_records(const QueryParams& filters) {
  return http_.get("/v5/pre-upgrade/asset/settlement-record", with_category("linear", filters), true);
}

std::string PrivateRestClient::get_move_position_history(const QueryParams& filters) {
  return http_.get("/v5/position/move-history", with_category(category_, filters), true);
}

std::string PrivateRestClient::get_borrow_quota(const std::string& symbol, const std::string& side) {
  QueryParams params{{"category", "spot"}, {"symbol", symbol}, {"side", side}};
  return http_.get("/v5/order/spot-borrow-check", params, true);
}

}  // namespace bybit
