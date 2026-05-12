#include "bybit/rest_client.hpp"

int main() {
  using RestClient = bybit::RestClient;

  auto create_strategy = &RestClient::create_strategy_order;
  auto strategies = &RestClient::get_strategy_list;
  auto strategy_orders = &RestClient::get_strategy_order_list;
  auto stop_strategy = &RestClient::stop_strategy;

  (void)create_strategy;
  (void)strategies;
  (void)strategy_orders;
  (void)stop_strategy;

  return 0;
}
