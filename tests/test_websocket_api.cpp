#include <string>

#include "bybit/websocket_client.hpp"

int main() {
  using WebSocketClient = bybit::WebSocketClient;

  auto text_handler = &WebSocketClient::set_message_handler;
  auto binary_handler = &WebSocketClient::set_binary_message_handler;
  auto sbe_bbo = &WebSocketClient::subscribe_sbe_bbo;
  auto sbe_book = &WebSocketClient::subscribe_sbe_orderbook_50;
  auto sbe_trades = &WebSocketClient::subscribe_sbe_public_trades;
  auto unsbe_bbo = &WebSocketClient::unsubscribe_sbe_bbo;
  auto unsbe_book = &WebSocketClient::unsubscribe_sbe_orderbook_50;
  auto unsbe_trades = &WebSocketClient::unsubscribe_sbe_public_trades;

  (void)text_handler;
  (void)binary_handler;
  (void)sbe_bbo;
  (void)sbe_book;
  (void)sbe_trades;
  (void)unsbe_bbo;
  (void)unsbe_book;
  (void)unsbe_trades;

  return 0;
}
