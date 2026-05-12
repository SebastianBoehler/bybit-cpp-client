#include <string>

#include "bybit/rest_client.hpp"

int main() {
  using RestClient = bybit::RestClient;

  auto info = &RestClient::get_leverage_token_info;
  auto market = &RestClient::get_leverage_token_market;
  auto purchase = &RestClient::purchase_leverage_token;
  auto redeem = &RestClient::redeem_leverage_token;
  auto records = &RestClient::get_leverage_token_order_records;

  (void)info;
  (void)market;
  (void)purchase;
  (void)redeem;
  (void)records;

  return 0;
}
