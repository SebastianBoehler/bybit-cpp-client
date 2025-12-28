#include <optional>
#include <string>
#include <utility>
#include <vector>

#include "bybit/http_client.hpp"
#include "bybit/rest_client.hpp"

namespace bybit {

PrivateRestClient::PrivateRestClient(HttpClient& http, std::string category)
    : http_(http), category_(std::move(category)) {}

std::string PrivateRestClient::get_query_api_key() {
  return http_.get("/v5/user/query-api", {}, true);
}

std::string PrivateRestClient::get_account_info() {
  return http_.get("/v5/account/info", {}, true);
}

std::string PrivateRestClient::get_position_info(const std::optional<std::string>& settle_coin, int limit) {
  std::vector<std::pair<std::string, std::string>> params{{"category", category_}, {"limit", std::to_string(limit)}};
  if (settle_coin) params.emplace_back("settleCoin", *settle_coin);
  return http_.get("/v5/position/list", params, true);
}

std::string PrivateRestClient::submit_order(const std::string& symbol, const std::string& side,
                                            const std::string& order_type, const std::string& qty,
                                            const std::string& order_link_id, int position_idx) {
  std::vector<std::pair<std::string, std::string>> body_kv{{"category", category_},
                                                           {"symbol", symbol},
                                                           {"side", side},
                                                           {"orderType", order_type},
                                                           {"qty", qty},
                                                           {"orderLinkId", order_link_id},
                                                           {"positionIdx", std::to_string(position_idx)},
                                                           {"recvWindow", http_.recv_window()}};
  return http_.post("/v5/order/create", to_json_object(body_kv), true);
}

std::string PrivateRestClient::set_leverage(const std::string& symbol, const std::string& buy_leverage,
                                            const std::string& sell_leverage) {
  std::vector<std::pair<std::string, std::string>> body_kv{{"category", category_},
                                                           {"symbol", symbol},
                                                           {"buyLeverage", buy_leverage},
                                                           {"sellLeverage", sell_leverage},
                                                           {"recvWindow", http_.recv_window()}};
  return http_.post("/v5/position/set-leverage", to_json_object(body_kv), true);
}

std::string PrivateRestClient::get_historic_orders(const std::string& order_id) {
  std::vector<std::pair<std::string, std::string>> params{{"category", category_}, {"orderId", order_id}};
  return http_.get("/v5/order/history", params, true);
}

std::string PrivateRestClient::get_fee_rate() {
  std::vector<std::pair<std::string, std::string>> params{{"category", category_}};
  return http_.get("/v5/account/fee-rate", params, true);
}

std::string PrivateRestClient::get_wallet_balance(const std::optional<std::string>& coin) {
  std::vector<std::pair<std::string, std::string>> params{{"accountType", category_}};
  if (coin) params.emplace_back("coin", *coin);
  return http_.get("/v5/account/wallet-balance", params, true);
}

std::string PrivateRestClient::get_open_orders(const std::optional<std::string>& symbol, int limit) {
  std::vector<std::pair<std::string, std::string>> params{{"category", category_}, {"limit", std::to_string(limit)}};
  if (symbol) params.emplace_back("symbol", *symbol);
  return http_.get("/v5/order/realtime", params, true);
}

std::string PrivateRestClient::cancel_order(const std::string& symbol, const std::string& order_id) {
  std::vector<std::pair<std::string, std::string>> body_kv{
      {"category", category_}, {"symbol", symbol}, {"orderId", order_id}, {"recvWindow", http_.recv_window()}};
  return http_.post("/v5/order/cancel", to_json_object(body_kv), true);
}

std::string PrivateRestClient::amend_order(const std::string& symbol, const std::string& order_id,
                                           const std::optional<std::string>& qty,
                                           const std::optional<std::string>& price) {
  std::vector<std::pair<std::string, std::string>> body_kv{
      {"category", category_}, {"symbol", symbol}, {"orderId", order_id}, {"recvWindow", http_.recv_window()}};
  if (qty) body_kv.emplace_back("qty", *qty);
  if (price) body_kv.emplace_back("price", *price);
  return http_.post("/v5/order/amend", to_json_object(body_kv), true);
}

}  // namespace bybit
