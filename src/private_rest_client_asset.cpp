#include <optional>
#include <string>

#include "bybit/http_client.hpp"
#include "bybit/rest_client.hpp"

namespace bybit {

std::string PrivateRestClient::get_asset_info(const QueryParams& filters) {
  return http_.get("/v5/asset/transfer/query-asset-info", filters, true);
}

std::string PrivateRestClient::get_asset_overview(const QueryParams& filters) {
  return http_.get("/v5/asset/asset-overview", filters, true);
}

std::string PrivateRestClient::get_portfolio_margin_info(const QueryParams& filters) {
  return http_.get("/v5/asset/portfolio-margin", filters, true);
}

std::string PrivateRestClient::get_total_members_assets(const QueryParams& filters) {
  return http_.get("/v5/asset/total-members-assets", filters, true);
}

std::string PrivateRestClient::get_funding_account_history(const QueryParams& filters) {
  return http_.get("/v5/asset/fundinghistory", filters, true);
}

std::string PrivateRestClient::get_delivery_records(const QueryParams& filters) {
  return http_.get("/v5/asset/delivery-record", filters, true);
}

std::string PrivateRestClient::get_settlement_records(const QueryParams& filters) {
  return http_.get("/v5/asset/settlement-record", filters, true);
}

std::string PrivateRestClient::get_coin_exchange_records(const QueryParams& filters) {
  return http_.get("/v5/asset/exchange/order-record", filters, true);
}

std::string PrivateRestClient::get_convert_coin_list(const QueryParams& filters) {
  return http_.get("/v5/asset/exchange/query-coin-list", filters, true);
}

std::string PrivateRestClient::request_convert_quote(const QueryParams& params) {
  return http_.post("/v5/asset/exchange/quote-apply", to_json_object(params), true);
}

std::string PrivateRestClient::confirm_convert_quote(const std::string& quote_tx_id) {
  return http_.post("/v5/asset/exchange/convert-execute", to_json_object({{"quoteTxId", quote_tx_id}}), true);
}

std::string PrivateRestClient::get_convert_status(const std::string& quote_tx_id, const std::string& account_type) {
  return http_.get("/v5/asset/exchange/convert-result-query",
                   {{"quoteTxId", quote_tx_id}, {"accountType", account_type}}, true);
}

std::string PrivateRestClient::get_convert_history(const QueryParams& filters) {
  return http_.get("/v5/asset/exchange/query-convert-history", filters, true);
}

std::string PrivateRestClient::get_small_balance_coins(const QueryParams& filters) {
  return http_.get("/v5/asset/covert/small-balance-list", filters, true);
}

std::string PrivateRestClient::request_small_balance_quote(const std::string& json_body) {
  return http_.post("/v5/asset/covert/get-quote", json_body, true);
}

std::string PrivateRestClient::confirm_small_balance_quote(const std::string& quote_id) {
  return http_.post("/v5/asset/covert/small-balance-execute", to_json_object({{"quoteId", quote_id}}), true);
}

std::string PrivateRestClient::get_small_balance_history(const QueryParams& filters) {
  return http_.get("/v5/asset/covert/small-balance-history", filters, true);
}

std::string PrivateRestClient::get_fiat_convert_coin_list(const QueryParams& filters) {
  return http_.get("/v5/fiat/query-coin-list", filters, true);
}

std::string PrivateRestClient::get_fiat_reference_price(const std::string& symbol) {
  return http_.get("/v5/fiat/reference-price", {{"symbol", symbol}}, true);
}

std::string PrivateRestClient::request_fiat_quote(const QueryParams& params) {
  return http_.post("/v5/fiat/quote-apply", to_json_object(params), true);
}

std::string PrivateRestClient::confirm_fiat_quote(const QueryParams& params) {
  return http_.post("/v5/fiat/trade-execute", to_json_object(params), true);
}

std::string PrivateRestClient::get_fiat_convert_status(const QueryParams& filters) {
  return http_.get("/v5/fiat/trade-query", filters, true);
}

std::string PrivateRestClient::get_fiat_convert_history(const QueryParams& filters) {
  return http_.get("/v5/fiat/query-trade-history", filters, true);
}

std::string PrivateRestClient::get_fiat_balance(const QueryParams& filters) {
  return http_.get("/v5/fiat/balance-query", filters, true);
}

std::string PrivateRestClient::get_asset_sub_uids() {
  return http_.get("/v5/asset/transfer/query-sub-member-list", {}, true);
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

std::string PrivateRestClient::get_sub_deposit_records(const QueryParams& filters) {
  return http_.get("/v5/asset/deposit/query-sub-member-record", filters, true);
}

std::string PrivateRestClient::get_internal_deposit_records(const QueryParams& filters) {
  return http_.get("/v5/asset/deposit/query-internal-record", filters, true);
}

std::string PrivateRestClient::get_allowed_deposit_coin_info(const QueryParams& filters) {
  return http_.get("/v5/asset/deposit/query-allowed-list", filters, true);
}

std::string PrivateRestClient::set_deposit_account(const std::string& account_type) {
  return http_.post("/v5/asset/deposit/deposit-to-account", to_json_object({{"accountType", account_type}}), true);
}

std::string PrivateRestClient::get_master_deposit_address(const QueryParams& filters) {
  return http_.get("/v5/asset/deposit/query-address", filters, true);
}

std::string PrivateRestClient::get_sub_deposit_address(const QueryParams& filters) {
  return http_.get("/v5/asset/deposit/query-sub-member-address", filters, true);
}

std::string PrivateRestClient::get_coin_info(const std::optional<std::string>& coin) {
  QueryParams params;
  if (coin)
    params.emplace_back("coin", *coin);
  return http_.get("/v5/asset/coin/query-info", params, true);
}

std::string PrivateRestClient::get_withdrawable_amount(const QueryParams& filters) {
  return http_.get("/v5/asset/withdraw/withdrawable-amount", filters, true);
}

std::string PrivateRestClient::get_withdrawal_address_list(const QueryParams& filters) {
  return http_.get("/v5/asset/withdraw/query-address", filters, true);
}

std::string PrivateRestClient::get_available_vasps() {
  return http_.get("/v5/asset/withdraw/vasp/list", {}, true);
}

std::string PrivateRestClient::get_withdrawal_records(const QueryParams& filters) {
  return http_.get("/v5/asset/withdraw/query-record", filters, true);
}

std::string PrivateRestClient::create_withdrawal(const std::string& json_body) {
  return http_.post("/v5/asset/withdraw/create", json_body, true);
}

std::string PrivateRestClient::cancel_withdrawal(const std::string& withdraw_id) {
  return http_.post("/v5/asset/withdraw/cancel", to_json_object({{"id", withdraw_id}}), true);
}

}  // namespace bybit
