#include "bybit/rest_client.hpp"

int main() {
  using RestClient = bybit::RestClient;

  auto create_rfq = &RestClient::create_rfq;
  auto rfq_config = &RestClient::get_rfq_config;
  auto cancel_rfq = &RestClient::cancel_rfq;
  auto cancel_all_rfqs = &RestClient::cancel_all_rfqs;
  auto accept_quote = &RestClient::accept_non_lp_quote;
  auto create_quote = &RestClient::create_rfq_quote;
  auto execute_quote = &RestClient::execute_rfq_quote;
  auto cancel_quote = &RestClient::cancel_rfq_quote;
  auto cancel_all_quotes = &RestClient::cancel_all_rfq_quotes;
  auto rfqs_realtime = &RestClient::get_rfqs_realtime;
  auto rfqs = &RestClient::get_rfqs;
  auto quotes_realtime = &RestClient::get_rfq_quotes_realtime;
  auto quotes = &RestClient::get_rfq_quotes;
  auto trades = &RestClient::get_rfq_trade_history;
  auto public_trades = &RestClient::get_rfq_public_trades;

  (void)create_rfq;
  (void)rfq_config;
  (void)cancel_rfq;
  (void)cancel_all_rfqs;
  (void)accept_quote;
  (void)create_quote;
  (void)execute_quote;
  (void)cancel_quote;
  (void)cancel_all_quotes;
  (void)rfqs_realtime;
  (void)rfqs;
  (void)quotes_realtime;
  (void)quotes;
  (void)trades;
  (void)public_trades;

  return 0;
}
