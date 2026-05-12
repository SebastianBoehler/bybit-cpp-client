#include "bybit/rest_client.hpp"

int main() {
  using RestClient = bybit::RestClient;

  auto earnings = &RestClient::get_broker_earnings;
  auto account = &RestClient::get_broker_account_info;
  auto deposits = &RestClient::get_broker_sub_deposit_records;
  auto set_rate = &RestClient::set_broker_rate_limit;
  auto rate_cap = &RestClient::get_broker_rate_limit_cap;
  auto rate_limits = &RestClient::get_broker_rate_limits;
  auto voucher_spec = &RestClient::get_broker_voucher_spec;
  auto issue_voucher = &RestClient::issue_broker_voucher;
  auto issued_voucher = &RestClient::get_broker_issued_voucher;

  (void)earnings;
  (void)account;
  (void)deposits;
  (void)set_rate;
  (void)rate_cap;
  (void)rate_limits;
  (void)voucher_spec;
  (void)issue_voucher;
  (void)issued_voucher;

  return 0;
}
