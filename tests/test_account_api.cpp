#include "bybit/rest_client.hpp"

int main() {
  using RestClient = bybit::RestClient;

  auto upgrade_account = &RestClient::upgrade_to_unified_account;
  auto demo_funds = &RestClient::request_demo_trading_funds;
  auto demo_account = &RestClient::create_demo_account;

  (void)upgrade_account;
  (void)demo_funds;
  (void)demo_account;

  return 0;
}
