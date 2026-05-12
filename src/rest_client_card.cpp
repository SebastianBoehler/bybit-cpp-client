#include "bybit/rest_client.hpp"

namespace bybit {

std::string RestClient::get_card_transaction_records(const QueryParams& filters) {
  return private_.get_card_transaction_records(filters);
}

std::string RestClient::get_card_point_balance() {
  return private_.get_card_point_balance();
}

std::string RestClient::get_card_point_records(const QueryParams& filters) {
  return private_.get_card_point_records(filters);
}

std::string RestClient::get_card_tier_info() {
  return private_.get_card_tier_info();
}

std::string RestClient::get_card_mall_items(const QueryParams& filters) {
  return private_.get_card_mall_items(filters);
}

std::string RestClient::get_card_cashback_detail(const std::string& biz_txn_id) {
  return private_.get_card_cashback_detail(biz_txn_id);
}

}  // namespace bybit
