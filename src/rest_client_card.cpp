#include "bybit/rest_client.hpp"

namespace bybit {

std::string RestClient::get_card_transaction_records(const QueryParams& filters) {
  return private_.get_card_transaction_records(filters);
}

}  // namespace bybit
