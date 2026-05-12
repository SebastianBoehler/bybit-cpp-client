#include "bybit/rest_client.hpp"

int main() {
  using RestClient = bybit::RestClient;

  auto affiliate_list = &RestClient::get_affiliate_user_list;
  auto affiliate_info = &RestClient::get_affiliate_user_info;

  (void)affiliate_list;
  (void)affiliate_info;

  return 0;
}
