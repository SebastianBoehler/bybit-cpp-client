#include "bybit/rest_client.hpp"

namespace bybit {

std::string RestClient::get_broker_earnings(const QueryParams& filters) {
  return private_.get_broker_earnings(filters);
}

std::string RestClient::get_broker_account_info() {
  return private_.get_broker_account_info();
}

std::string RestClient::get_broker_sub_deposit_records(const QueryParams& filters) {
  return private_.get_broker_sub_deposit_records(filters);
}

std::string RestClient::set_broker_rate_limit(const std::string& json_body) {
  return private_.set_broker_rate_limit(json_body);
}

std::string RestClient::get_broker_rate_limit_cap() {
  return private_.get_broker_rate_limit_cap();
}

std::string RestClient::get_broker_rate_limits(const QueryParams& filters) {
  return private_.get_broker_rate_limits(filters);
}

std::string RestClient::get_broker_voucher_spec(const std::string& json_body) {
  return private_.get_broker_voucher_spec(json_body);
}

std::string RestClient::issue_broker_voucher(const std::string& json_body) {
  return private_.issue_broker_voucher(json_body);
}

std::string RestClient::get_broker_issued_voucher(const std::string& json_body) {
  return private_.get_broker_issued_voucher(json_body);
}

}  // namespace bybit
