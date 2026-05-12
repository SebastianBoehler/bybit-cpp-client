#include "bybit/rest_client.hpp"

int main() {
  using RestClient = bybit::RestClient;

  auto request_quote = &RestClient::request_convert_quote;
  auto confirm_quote = &RestClient::confirm_convert_quote;
  auto convert_status = &RestClient::get_convert_status;
  auto convert_history = &RestClient::get_convert_history;
  auto available_vasps = &RestClient::get_available_vasps;

  (void)request_quote;
  (void)confirm_quote;
  (void)convert_status;
  (void)convert_history;
  (void)available_vasps;

  return 0;
}
