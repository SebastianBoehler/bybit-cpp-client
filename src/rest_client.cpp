#include "bybit/rest_client.hpp"

#include <optional>
#include <utility>
#include <vector>

#include "bybit/http_client.hpp"

namespace bybit {

PublicRestClient::PublicRestClient(HttpClient& http, std::string category)
    : http_(http), category_(std::move(category)) {}

std::string PublicRestClient::get_instruments_info(int limit) {
  std::vector<std::pair<std::string, std::string>> params{{"category", category_}, {"limit", std::to_string(limit)}};
  return http_.get("/v5/market/instruments-info", params, false);
}

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

RestClient::RestClient(std::string api_key, std::string api_secret, std::string category, std::string base_url,
                       std::string recv_window)
    : http_(std::move(api_key), std::move(api_secret), std::move(base_url), std::move(recv_window)),
      public_(http_, category),
      private_(http_, category) {}

std::string RestClient::get_query_api_key() {
  return private_.get_query_api_key();
}
std::string RestClient::get_account_info() {
  return private_.get_account_info();
}
std::string RestClient::get_position_info(const std::optional<std::string>& settle_coin, int limit) {
  return private_.get_position_info(settle_coin, limit);
}
std::string RestClient::get_instruments_info(int limit) {
  return public_.get_instruments_info(limit);
}
std::string RestClient::submit_order(const std::string& symbol, const std::string& side, const std::string& order_type,
                                     const std::string& qty, const std::string& order_link_id, int position_idx) {
  return private_.submit_order(symbol, side, order_type, qty, order_link_id, position_idx);
}
std::string RestClient::set_leverage(const std::string& symbol, const std::string& buy_leverage,
                                     const std::string& sell_leverage) {
  return private_.set_leverage(symbol, buy_leverage, sell_leverage);
}
std::string RestClient::get_historic_orders(const std::string& order_id) {
  return private_.get_historic_orders(order_id);
}
std::string RestClient::get_fee_rate() {
  return private_.get_fee_rate();
}

}  // namespace bybit
