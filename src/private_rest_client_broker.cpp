#include "bybit/http_client.hpp"
#include "bybit/rest_client.hpp"

namespace bybit {

std::string PrivateRestClient::get_broker_earnings(const QueryParams& filters) {
  return http_.get("/v5/broker/earnings-info", filters, true);
}

std::string PrivateRestClient::get_broker_account_info() {
  return http_.get("/v5/broker/account-info", {}, true);
}

std::string PrivateRestClient::get_broker_sub_deposit_records(const QueryParams& filters) {
  return http_.get("/v5/broker/asset/query-sub-member-deposit-record", filters, true);
}

std::string PrivateRestClient::set_broker_rate_limit(const std::string& json_body) {
  return http_.post("/v5/broker/apilimit/set", json_body, true);
}

std::string PrivateRestClient::get_broker_rate_limit_cap() {
  return http_.get("/v5/broker/apilimit/query-cap", {}, true);
}

std::string PrivateRestClient::get_broker_rate_limits(const QueryParams& filters) {
  return http_.get("/v5/broker/apilimit/query-all", filters, true);
}

std::string PrivateRestClient::get_broker_voucher_spec(const std::string& json_body) {
  return http_.post("/v5/broker/award/info", json_body, true);
}

std::string PrivateRestClient::issue_broker_voucher(const std::string& json_body) {
  return http_.post("/v5/broker/award/distribute-award", json_body, true);
}

std::string PrivateRestClient::get_broker_issued_voucher(const std::string& json_body) {
  return http_.post("/v5/broker/award/distribution-record", json_body, true);
}

}  // namespace bybit
