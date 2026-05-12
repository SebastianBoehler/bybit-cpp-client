#include "bybit/rest_client.hpp"

#include <optional>
#include <string>

namespace bybit {

std::string RestClient::get_asset_info(const QueryParams& filters) {
  return private_.get_asset_info(filters);
}

std::string RestClient::get_asset_overview(const QueryParams& filters) {
  return private_.get_asset_overview(filters);
}

std::string RestClient::get_portfolio_margin_info(const QueryParams& filters) {
  return private_.get_portfolio_margin_info(filters);
}

std::string RestClient::get_total_members_assets(const QueryParams& filters) {
  return private_.get_total_members_assets(filters);
}

std::string RestClient::get_funding_account_history(const QueryParams& filters) {
  return private_.get_funding_account_history(filters);
}

std::string RestClient::get_delivery_records(const QueryParams& filters) {
  return private_.get_delivery_records(filters);
}

std::string RestClient::get_settlement_records(const QueryParams& filters) {
  return private_.get_settlement_records(filters);
}

std::string RestClient::get_coin_exchange_records(const QueryParams& filters) {
  return private_.get_coin_exchange_records(filters);
}

std::string RestClient::get_convert_coin_list(const QueryParams& filters) {
  return private_.get_convert_coin_list(filters);
}

std::string RestClient::request_convert_quote(const QueryParams& params) {
  return private_.request_convert_quote(params);
}

std::string RestClient::confirm_convert_quote(const std::string& quote_tx_id) {
  return private_.confirm_convert_quote(quote_tx_id);
}

std::string RestClient::get_convert_status(const std::string& quote_tx_id, const std::string& account_type) {
  return private_.get_convert_status(quote_tx_id, account_type);
}

std::string RestClient::get_convert_history(const QueryParams& filters) {
  return private_.get_convert_history(filters);
}

std::string RestClient::get_small_balance_coins(const QueryParams& filters) {
  return private_.get_small_balance_coins(filters);
}

std::string RestClient::request_small_balance_quote(const std::string& json_body) {
  return private_.request_small_balance_quote(json_body);
}

std::string RestClient::confirm_small_balance_quote(const std::string& quote_id) {
  return private_.confirm_small_balance_quote(quote_id);
}

std::string RestClient::get_small_balance_history(const QueryParams& filters) {
  return private_.get_small_balance_history(filters);
}

std::string RestClient::get_fiat_convert_coin_list(const QueryParams& filters) {
  return private_.get_fiat_convert_coin_list(filters);
}

std::string RestClient::get_fiat_reference_price(const std::string& symbol) {
  return private_.get_fiat_reference_price(symbol);
}

std::string RestClient::request_fiat_quote(const QueryParams& params) {
  return private_.request_fiat_quote(params);
}

std::string RestClient::confirm_fiat_quote(const QueryParams& params) {
  return private_.confirm_fiat_quote(params);
}

std::string RestClient::get_fiat_convert_status(const QueryParams& filters) {
  return private_.get_fiat_convert_status(filters);
}

std::string RestClient::get_fiat_convert_history(const QueryParams& filters) {
  return private_.get_fiat_convert_history(filters);
}

std::string RestClient::get_fiat_balance(const QueryParams& filters) {
  return private_.get_fiat_balance(filters);
}

std::string RestClient::get_asset_sub_uids() {
  return private_.get_asset_sub_uids();
}

std::string RestClient::get_all_coin_balances(const QueryParams& filters) {
  return private_.get_all_coin_balances(filters);
}

std::string RestClient::get_single_coin_balance(const QueryParams& filters) {
  return private_.get_single_coin_balance(filters);
}

std::string RestClient::get_transferable_coins(const QueryParams& filters) {
  return private_.get_transferable_coins(filters);
}

std::string RestClient::create_internal_transfer(const QueryParams& params) {
  return private_.create_internal_transfer(params);
}

std::string RestClient::get_internal_transfer_records(const QueryParams& filters) {
  return private_.get_internal_transfer_records(filters);
}

std::string RestClient::create_universal_transfer(const QueryParams& params) {
  return private_.create_universal_transfer(params);
}

std::string RestClient::get_universal_transfer_records(const QueryParams& filters) {
  return private_.get_universal_transfer_records(filters);
}

std::string RestClient::get_deposit_records(const QueryParams& filters) {
  return private_.get_deposit_records(filters);
}

std::string RestClient::get_sub_deposit_records(const QueryParams& filters) {
  return private_.get_sub_deposit_records(filters);
}

std::string RestClient::get_internal_deposit_records(const QueryParams& filters) {
  return private_.get_internal_deposit_records(filters);
}

std::string RestClient::get_allowed_deposit_coin_info(const QueryParams& filters) {
  return private_.get_allowed_deposit_coin_info(filters);
}

std::string RestClient::set_deposit_account(const std::string& account_type) {
  return private_.set_deposit_account(account_type);
}

std::string RestClient::get_master_deposit_address(const QueryParams& filters) {
  return private_.get_master_deposit_address(filters);
}

std::string RestClient::get_sub_deposit_address(const QueryParams& filters) {
  return private_.get_sub_deposit_address(filters);
}

std::string RestClient::get_coin_info(const std::optional<std::string>& coin) {
  return private_.get_coin_info(coin);
}

std::string RestClient::get_withdrawable_amount(const QueryParams& filters) {
  return private_.get_withdrawable_amount(filters);
}

std::string RestClient::get_withdrawal_address_list(const QueryParams& filters) {
  return private_.get_withdrawal_address_list(filters);
}

std::string RestClient::get_available_vasps() {
  return private_.get_available_vasps();
}

std::string RestClient::get_withdrawal_records(const QueryParams& filters) {
  return private_.get_withdrawal_records(filters);
}

std::string RestClient::create_withdrawal(const std::string& json_body) {
  return private_.create_withdrawal(json_body);
}

std::string RestClient::cancel_withdrawal(const std::string& withdraw_id) {
  return private_.cancel_withdrawal(withdraw_id);
}

}  // namespace bybit
