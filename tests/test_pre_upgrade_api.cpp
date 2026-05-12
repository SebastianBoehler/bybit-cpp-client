#include "bybit/rest_client.hpp"

int main() {
  using RestClient = bybit::RestClient;

  auto order_history = &RestClient::get_pre_upgrade_order_history;
  auto trade_history = &RestClient::get_pre_upgrade_trade_history;
  auto closed_pnl = &RestClient::get_pre_upgrade_closed_pnl;
  auto transaction_log = &RestClient::get_pre_upgrade_transaction_log;
  auto delivery_records = &RestClient::get_pre_upgrade_delivery_records;
  auto settlement_records = &RestClient::get_pre_upgrade_settlement_records;

  (void)order_history;
  (void)trade_history;
  (void)closed_pnl;
  (void)transaction_log;
  (void)delivery_records;
  (void)settlement_records;

  return 0;
}
