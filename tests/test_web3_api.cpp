#include "bybit/rest_client.hpp"

int main() {
  using RestClient = bybit::RestClient;

  auto trade_quote = &RestClient::get_web3_trade_quote;
  auto purchase = &RestClient::execute_web3_purchase;
  auto redeem = &RestClient::execute_web3_redeem;
  auto payment_tokens = &RestClient::get_web3_payment_token_list;
  auto orders = &RestClient::get_web3_order_list;
  auto tokens = &RestClient::get_web3_business_token_list;
  auto token_prices = &RestClient::get_web3_token_price_list;
  auto token_details = &RestClient::get_web3_token_details;
  auto assets = &RestClient::get_web3_asset_list;
  auto asset_detail = &RestClient::get_web3_asset_detail;
  (void)trade_quote;
  (void)purchase;
  (void)redeem;
  (void)payment_tokens;
  (void)orders;
  (void)tokens;
  (void)token_prices;
  (void)token_details;
  (void)assets;
  (void)asset_detail;

  return 0;
}
