#include "bybit/rest_client.hpp"

#include <optional>
#include <string>

namespace bybit {

std::string RestClient::get_asset_info(const QueryParams& filters) {
  return private_.get_asset_info(filters);
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

std::string RestClient::get_internal_deposit_records(const QueryParams& filters) {
  return private_.get_internal_deposit_records(filters);
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

std::string RestClient::get_withdrawal_records(const QueryParams& filters) {
  return private_.get_withdrawal_records(filters);
}

std::string RestClient::create_withdrawal(const std::string& json_body) {
  return private_.create_withdrawal(json_body);
}

}  // namespace bybit
