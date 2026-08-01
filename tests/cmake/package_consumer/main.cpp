#include "bybit/rest_client.hpp"
#include "bybit/websocket_client.hpp"

int main() {
  bybit::RestClient client{"", "", "linear"};
  bybit::WebSocketClient websocket_client{""};
  return client.get_server_time().empty() ? 1 : 0;
}
