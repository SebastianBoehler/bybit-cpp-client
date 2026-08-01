#include <optional>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "bybit/http_client.hpp"
#include "bybit/rest_client.hpp"

namespace bybit {
namespace {
std::string build_batch_body(const std::string& category, const std::vector<JsonObject>& requests) {
  std::ostringstream oss;
  oss << "{\"category\":" << serialize_json_string(category) << ",\"request\":[";
  for (size_t i = 0; i < requests.size(); ++i) {
    oss << serialize_json_object(requests[i]);
    if (i + 1 < requests.size())
      oss << ",";
  }
  oss << "]}";
  return oss.str();
}
}  // namespace

PrivateRestClient::PrivateRestClient(HttpClient& http, std::string category)
    : http_(http), category_(std::move(category)) {}

std::string PrivateRestClient::get_query_api_key() {
  return http_.get("/v5/user/query-api", {}, true);
}

std::string PrivateRestClient::get_account_info() {
  return http_.get("/v5/account/info", {}, true);
}

std::string PrivateRestClient::get_position_info(const std::optional<std::string>& settle_coin,
                                                 const std::optional<std::string>& symbol, int limit) {
  std::vector<std::pair<std::string, std::string>> params{{"category", category_}, {"limit", std::to_string(limit)}};
  if (settle_coin)
    params.emplace_back("settleCoin", *settle_coin);
  if (symbol)
    params.emplace_back("symbol", *symbol);
  return http_.get("/v5/position/list", params, true);
}

std::string PrivateRestClient::submit_order(
    const std::string& symbol, const std::string& side, const std::string& order_type, const std::string& qty,
    const std::string& order_link_id, int position_idx, const std::string& price, const std::string& time_in_force,
    const std::optional<bool>& reduce_only, const std::optional<std::string>& bbo_side_type,
    const std::optional<std::string>& bbo_level, const std::optional<bool>& rpi_taker_access) {
  JsonObject body_kv{{"category", category_},      {"symbol", symbol}, {"side", side},
                     {"orderType", order_type},    {"qty", qty},       {"orderLinkId", order_link_id},
                     {"positionIdx", position_idx}};
  if (!price.empty())
    body_kv.emplace_back("price", price);
  if (!time_in_force.empty())
    body_kv.emplace_back("timeInForce", time_in_force);
  if (reduce_only.has_value())
    body_kv.emplace_back("reduceOnly", *reduce_only);
  if (bbo_side_type.has_value())
    body_kv.emplace_back("bboSideType", *bbo_side_type);
  if (bbo_level.has_value())
    body_kv.emplace_back("bboLevel", *bbo_level);
  if (rpi_taker_access.has_value())
    body_kv.emplace_back("rpiTakerAccess", *rpi_taker_access);
  return http_.post("/v5/order/create", serialize_json_object(body_kv), true);
}

std::string PrivateRestClient::batch_submit_orders(const std::vector<JsonObject>& order_requests) {
  return http_.post("/v5/order/create-batch", build_batch_body(category_, order_requests), true);
}

std::string PrivateRestClient::batch_cancel_orders(const std::vector<JsonObject>& cancel_requests) {
  return http_.post("/v5/order/cancel-batch", build_batch_body(category_, cancel_requests), true);
}

std::string PrivateRestClient::batch_amend_orders(const std::vector<JsonObject>& amend_requests) {
  return http_.post("/v5/order/amend-batch", build_batch_body(category_, amend_requests), true);
}

std::string PrivateRestClient::set_leverage(const std::string& symbol, const std::string& buy_leverage,
                                            const std::string& sell_leverage) {
  std::vector<std::pair<std::string, std::string>> body_kv{
      {"category", category_}, {"symbol", symbol}, {"buyLeverage", buy_leverage}, {"sellLeverage", sell_leverage}};
  return http_.post("/v5/position/set-leverage", to_json_object(body_kv), true);
}

std::string PrivateRestClient::get_historic_orders(const std::string& order_id) {
  return get_order_history({{"orderId", order_id}});
}

std::string PrivateRestClient::get_fee_rate(const std::optional<std::string>& symbol) {
  QueryParams params{{"category", category_}};
  if (symbol)
    params.emplace_back("symbol", *symbol);
  return http_.get("/v5/account/fee-rate", params, true);
}

std::string PrivateRestClient::get_wallet_balance(const std::string& category, const std::optional<std::string>& coin) {
  std::vector<std::pair<std::string, std::string>> params{{"accountType", category}};
  if (coin)
    params.emplace_back("coin", *coin);
  return http_.get("/v5/account/wallet-balance", params, true);
}

std::string PrivateRestClient::get_open_orders(const std::optional<std::string>& symbol, int limit) {
  QueryParams params{{"limit", std::to_string(limit)}};
  if (symbol)
    params.emplace_back("symbol", *symbol);
  return get_realtime_orders(params);
}

std::string PrivateRestClient::cancel_order(const std::string& symbol, const std::string& order_id) {
  std::vector<std::pair<std::string, std::string>> body_kv{
      {"category", category_}, {"symbol", symbol}, {"orderId", order_id}};
  return http_.post("/v5/order/cancel", to_json_object(body_kv), true);
}

std::string PrivateRestClient::amend_order(const std::string& symbol, const std::string& order_id,
                                           const std::optional<std::string>& qty,
                                           const std::optional<std::string>& price) {
  std::vector<std::pair<std::string, std::string>> body_kv{
      {"category", category_}, {"symbol", symbol}, {"orderId", order_id}};
  if (qty)
    body_kv.emplace_back("qty", *qty);
  if (price)
    body_kv.emplace_back("price", *price);
  return http_.post("/v5/order/amend", to_json_object(body_kv), true);
}

std::string PrivateRestClient::get_transaction_log(int limit, const std::optional<std::string>& cursor) {
  QueryParams params{{"accountType", "UNIFIED"}, {"category", category_}, {"limit", std::to_string(limit)}};
  if (cursor)
    params.emplace_back("cursor", *cursor);
  return http_.get("/v5/account/transaction-log", params, true);
}

std::string PrivateRestClient::set_trading_stop(const std::string& symbol, int position_idx,
                                                const std::optional<std::string>& take_profit,
                                                const std::optional<std::string>& stop_loss,
                                                const std::optional<std::string>& trailing_stop) {
  JsonObject body_kv{{"category", category_}, {"symbol", symbol}, {"positionIdx", position_idx}};
  if (take_profit)
    body_kv.emplace_back("takeProfit", *take_profit);
  if (stop_loss)
    body_kv.emplace_back("stopLoss", *stop_loss);
  if (trailing_stop)
    body_kv.emplace_back("trailingStop", *trailing_stop);
  return http_.post("/v5/position/trading-stop", serialize_json_object(body_kv), true);
}

std::string PrivateRestClient::set_risk_limit(const std::string& symbol, int risk_id, int position_idx) {
  JsonObject body_kv{{"category", category_}, {"symbol", symbol}, {"riskId", risk_id}, {"positionIdx", position_idx}};
  return http_.post("/v5/position/set-risk-limit", serialize_json_object(body_kv), true);
}

std::string PrivateRestClient::add_margin(const std::string& symbol, const std::string& margin, int position_idx) {
  JsonObject body_kv{{"category", category_}, {"symbol", symbol}, {"margin", margin}, {"positionIdx", position_idx}};
  return http_.post("/v5/position/add-margin", serialize_json_object(body_kv), true);
}

std::string PrivateRestClient::switch_position_mode(int mode, const std::optional<std::string>& symbol,
                                                    const std::optional<std::string>& coin) {
  if (!symbol && !coin)
    throw std::invalid_argument("switch_position_mode requires symbol or coin");
  if (mode != 0 && mode != 3)
    throw std::invalid_argument("switch_position_mode mode must be 0 or 3");

  JsonObject body_kv{{"category", category_}};
  if (symbol)
    body_kv.emplace_back("symbol", *symbol);
  if (coin)
    body_kv.emplace_back("coin", *coin);
  body_kv.emplace_back("mode", mode);
  return http_.post("/v5/position/switch-mode", serialize_json_object(body_kv), true);
}

std::string PrivateRestClient::switch_margin_mode(const std::string& set_margin_mode) {
  return http_.post("/v5/account/set-margin-mode", serialize_json_object({{"setMarginMode", set_margin_mode}}), true);
}

std::string PrivateRestClient::cancel_all(const std::string& symbol) {
  return cancel_all_orders({{"symbol", symbol}});
}

}  // namespace bybit
