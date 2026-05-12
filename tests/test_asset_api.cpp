#include "bybit/rest_client.hpp"

int main() {
  using RestClient = bybit::RestClient;

  auto request_quote = &RestClient::request_convert_quote;
  auto confirm_quote = &RestClient::confirm_convert_quote;
  auto convert_status = &RestClient::get_convert_status;
  auto convert_history = &RestClient::get_convert_history;
  auto small_balance_coins = &RestClient::get_small_balance_coins;
  auto small_balance_quote = &RestClient::request_small_balance_quote;
  auto small_balance_confirm = &RestClient::confirm_small_balance_quote;
  auto small_balance_history = &RestClient::get_small_balance_history;
  auto fiat_coin_list = &RestClient::get_fiat_convert_coin_list;
  auto fiat_reference_price = &RestClient::get_fiat_reference_price;
  auto fiat_quote = &RestClient::request_fiat_quote;
  auto fiat_confirm = &RestClient::confirm_fiat_quote;
  auto fiat_status = &RestClient::get_fiat_convert_status;
  auto fiat_history = &RestClient::get_fiat_convert_history;
  auto fiat_balance = &RestClient::get_fiat_balance;
  auto available_vasps = &RestClient::get_available_vasps;

  (void)request_quote;
  (void)confirm_quote;
  (void)convert_status;
  (void)convert_history;
  (void)small_balance_coins;
  (void)small_balance_quote;
  (void)small_balance_confirm;
  (void)small_balance_history;
  (void)fiat_coin_list;
  (void)fiat_reference_price;
  (void)fiat_quote;
  (void)fiat_confirm;
  (void)fiat_status;
  (void)fiat_history;
  (void)fiat_balance;
  (void)available_vasps;

  return 0;
}
