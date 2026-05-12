#include "bybit/rest_client.hpp"

int main() {
  using RestClient = bybit::RestClient;

  auto card_records = &RestClient::get_card_transaction_records;
  auto point_balance = &RestClient::get_card_point_balance;
  auto point_records = &RestClient::get_card_point_records;
  auto tier_info = &RestClient::get_card_tier_info;
  auto mall_items = &RestClient::get_card_mall_items;
  auto cashback_detail = &RestClient::get_card_cashback_detail;
  (void)card_records;
  (void)point_balance;
  (void)point_records;
  (void)tier_info;
  (void)mall_items;
  (void)cashback_detail;

  return 0;
}
