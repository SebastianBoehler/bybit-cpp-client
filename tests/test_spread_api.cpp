#include "bybit/rest_client.hpp"

int main() {
  using RestClient = bybit::RestClient;

  auto instruments = &RestClient::get_spread_instruments;
  auto orderbook = &RestClient::get_spread_orderbook;
  auto tickers = &RestClient::get_spread_tickers;
  auto recent_trades = &RestClient::get_spread_recent_trades;
  auto create_order = &RestClient::create_spread_order;
  auto amend_order = &RestClient::amend_spread_order;
  auto cancel_order = &RestClient::cancel_spread_order;
  auto cancel_all = &RestClient::cancel_all_spread_orders;
  auto open_orders = &RestClient::get_spread_open_orders;
  auto order_history = &RestClient::get_spread_order_history;
  auto trade_history = &RestClient::get_spread_trade_history;
  auto max_qty = &RestClient::get_spread_max_order_qty;

  (void)instruments;
  (void)orderbook;
  (void)tickers;
  (void)recent_trades;
  (void)create_order;
  (void)amend_order;
  (void)cancel_order;
  (void)cancel_all;
  (void)open_orders;
  (void)order_history;
  (void)trade_history;
  (void)max_qty;

  return 0;
}
