#include "bybit/rest_client.hpp"

int main() {
  using RestClient = bybit::RestClient;

  auto upgrade_account = &RestClient::upgrade_to_unified_account;
  auto demo_funds = &RestClient::request_demo_trading_funds;
  auto demo_account = &RestClient::create_demo_account;
  auto sign_agreement = &RestClient::sign_agreement;
  auto sub_uids_unlimited = &RestClient::get_sub_uid_list_unlimited;
  auto fund_custodial = &RestClient::get_fund_custodial_sub_accounts;
  auto limit_price_behaviour = &RestClient::get_limit_price_behaviour;

  (void)upgrade_account;
  (void)demo_funds;
  (void)demo_account;
  (void)sign_agreement;
  (void)sub_uids_unlimited;
  (void)fund_custodial;
  (void)limit_price_behaviour;

  return 0;
}
