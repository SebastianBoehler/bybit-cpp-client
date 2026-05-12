#include "bybit/rest_client.hpp"

int main() {
  using RestClient = bybit::RestClient;

  auto card_records = &RestClient::get_card_transaction_records;
  (void)card_records;

  return 0;
}
