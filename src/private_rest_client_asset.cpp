#include <optional>
#include <string>

#include "bybit/http_client.hpp"
#include "bybit/rest_client.hpp"

namespace bybit {

std::string PrivateRestClient::get_asset_info(const QueryParams& filters) {
  return http_.get("/v5/asset/transfer/query-asset-info", filters, true);
}

std::string PrivateRestClient::get_all_coin_balances(const QueryParams& filters) {
  return http_.get("/v5/asset/transfer/query-account-coins-balance", filters, true);
}

std::string PrivateRestClient::get_single_coin_balance(const QueryParams& filters) {
  return http_.get("/v5/asset/transfer/query-account-coin-balance", filters, true);
}

std::string PrivateRestClient::get_transferable_coins(const QueryParams& filters) {
  return http_.get("/v5/asset/transfer/query-transfer-coin-list", filters, true);
}

std::string PrivateRestClient::create_internal_transfer(const QueryParams& params) {
  return http_.post("/v5/asset/transfer/inter-transfer", to_json_object(params), true);
}

std::string PrivateRestClient::get_internal_transfer_records(const QueryParams& filters) {
  return http_.get("/v5/asset/transfer/query-inter-transfer-list", filters, true);
}

std::string PrivateRestClient::create_universal_transfer(const QueryParams& params) {
  return http_.post("/v5/asset/transfer/universal-transfer", to_json_object(params), true);
}

std::string PrivateRestClient::get_universal_transfer_records(const QueryParams& filters) {
  return http_.get("/v5/asset/transfer/query-universal-transfer-list", filters, true);
}

std::string PrivateRestClient::get_deposit_records(const QueryParams& filters) {
  return http_.get("/v5/asset/deposit/query-record", filters, true);
}

std::string PrivateRestClient::get_internal_deposit_records(const QueryParams& filters) {
  return http_.get("/v5/asset/deposit/query-internal-record", filters, true);
}

std::string PrivateRestClient::get_master_deposit_address(const QueryParams& filters) {
  return http_.get("/v5/asset/deposit/query-address", filters, true);
}

std::string PrivateRestClient::get_sub_deposit_address(const QueryParams& filters) {
  return http_.get("/v5/asset/deposit/query-sub-member-address", filters, true);
}

std::string PrivateRestClient::get_coin_info(const std::optional<std::string>& coin) {
  QueryParams params;
  if (coin) params.emplace_back("coin", *coin);
  return http_.get("/v5/asset/coin/query-info", params, true);
}

std::string PrivateRestClient::get_withdrawable_amount(const QueryParams& filters) {
  return http_.get("/v5/asset/withdraw/withdrawable-amount", filters, true);
}

std::string PrivateRestClient::get_withdrawal_records(const QueryParams& filters) {
  return http_.get("/v5/asset/withdraw/query-record", filters, true);
}

std::string PrivateRestClient::create_withdrawal(const std::string& json_body) {
  return http_.post("/v5/asset/withdraw/create", json_body, true);
}

}  // namespace bybit
