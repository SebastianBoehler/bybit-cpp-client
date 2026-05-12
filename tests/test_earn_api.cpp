#include "bybit/rest_client.hpp"

int main() {
  using RestClient = bybit::RestClient;

  auto product_info = &RestClient::get_earn_product_info;
  auto place_order = &RestClient::place_earn_order;
  auto order_history = &RestClient::get_earn_order_history;
  auto position = &RestClient::get_earn_position;
  auto yield_history = &RestClient::get_earn_yield_history;
  auto hourly_yield = &RestClient::get_earn_hourly_yield_history;
  (void)product_info;
  (void)place_order;
  (void)order_history;
  (void)position;
  (void)yield_history;
  (void)hourly_yield;

  return 0;
}
