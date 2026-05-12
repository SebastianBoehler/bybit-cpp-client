#include <string>

#include "bybit/websocket_client.hpp"

int main() {
  using WebSocketClient = bybit::WebSocketClient;

  auto text_handler = &WebSocketClient::set_message_handler;
  auto binary_handler = &WebSocketClient::set_binary_message_handler;
  auto send_binary = &WebSocketClient::send_binary;
  auto sbe_bbo = &WebSocketClient::subscribe_sbe_bbo;
  auto sbe_book = &WebSocketClient::subscribe_sbe_orderbook_50;
  auto sbe_trades = &WebSocketClient::subscribe_sbe_public_trades;
  auto all_liquidation = &WebSocketClient::subscribe_all_liquidation;
  auto order = &WebSocketClient::subscribe_private_order;
  auto execution = &WebSocketClient::subscribe_private_execution;
  auto position = &WebSocketClient::subscribe_private_position;
  auto wallet = &WebSocketClient::subscribe_private_wallet;
  auto greeks = &WebSocketClient::subscribe_private_greeks;
  auto unsbe_bbo = &WebSocketClient::unsubscribe_sbe_bbo;
  auto unsbe_book = &WebSocketClient::unsubscribe_sbe_orderbook_50;
  auto unsbe_trades = &WebSocketClient::unsubscribe_sbe_public_trades;
  auto unall_liquidation = &WebSocketClient::unsubscribe_all_liquidation;
  auto unorder = &WebSocketClient::unsubscribe_private_order;
  auto unexecution = &WebSocketClient::unsubscribe_private_execution;
  auto unposition = &WebSocketClient::unsubscribe_private_position;
  auto unwallet = &WebSocketClient::unsubscribe_private_wallet;
  auto ungreeks = &WebSocketClient::unsubscribe_private_greeks;

  (void)text_handler;
  (void)binary_handler;
  (void)send_binary;
  (void)sbe_bbo;
  (void)sbe_book;
  (void)sbe_trades;
  (void)all_liquidation;
  (void)order;
  (void)execution;
  (void)position;
  (void)wallet;
  (void)greeks;
  (void)unsbe_bbo;
  (void)unsbe_book;
  (void)unsbe_trades;
  (void)unall_liquidation;
  (void)unorder;
  (void)unexecution;
  (void)unposition;
  (void)unwallet;
  (void)ungreeks;

  return 0;
}
