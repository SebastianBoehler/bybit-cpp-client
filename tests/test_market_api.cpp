#include "bybit/rest_client.hpp"

int main() {
  using RestClient = bybit::RestClient;

  auto rpi_orderbook = &RestClient::get_rpi_orderbook;
  auto historical_volatility = &RestClient::get_historical_volatility;
  auto insurance = &RestClient::get_insurance;
  auto delivery_price = &RestClient::get_delivery_price;
  auto new_delivery_price = &RestClient::get_new_delivery_price;
  auto index_components = &RestClient::get_index_price_components;
  auto price_limit = &RestClient::get_order_price_limit;
  auto adl_alert = &RestClient::get_adl_alert;
  auto fee_group = &RestClient::get_fee_group_info;

  (void)rpi_orderbook;
  (void)historical_volatility;
  (void)insurance;
  (void)delivery_price;
  (void)new_delivery_price;
  (void)index_components;
  (void)price_limit;
  (void)adl_alert;
  (void)fee_group;

  return 0;
}
