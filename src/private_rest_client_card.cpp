#include "bybit/http_client.hpp"
#include "bybit/rest_client.hpp"

namespace bybit {

std::string PrivateRestClient::get_card_transaction_records(const QueryParams& filters) {
  return http_.post_query("/v5/card/transaction/query-asset-records", filters, true);
}

std::string PrivateRestClient::get_card_point_balance() {
  return http_.post_query("/v5/card/reward/points/balance", {}, true);
}

std::string PrivateRestClient::get_card_point_records(const QueryParams& filters) {
  return http_.post_query("/v5/card/reward/points/records", filters, true);
}

std::string PrivateRestClient::get_card_tier_info() {
  return http_.post_query("/v5/card/reward/points/tier", {}, true);
}

std::string PrivateRestClient::get_card_mall_items(const QueryParams& filters) {
  return http_.post_query("/v5/card/reward/mall/item/list", filters, true);
}

std::string PrivateRestClient::get_card_cashback_detail(const std::string& biz_txn_id) {
  return http_.post_query("/v5/card/reward/point/cashback/detail", {{"bizTxnId", biz_txn_id}}, true);
}

}  // namespace bybit
