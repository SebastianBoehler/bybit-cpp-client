#include "bybit/http_client.hpp"
#include "bybit/rest_client.hpp"

namespace bybit {

std::string PrivateRestClient::get_card_transaction_records(const QueryParams& filters) {
  return http_.post_query("/v5/card/transaction/query-asset-records", filters, true);
}

}  // namespace bybit
