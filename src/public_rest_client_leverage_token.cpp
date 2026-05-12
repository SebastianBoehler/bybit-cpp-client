#include "bybit/public_rest_client.hpp"

namespace bybit {

std::string PublicRestClient::get_leverage_token_info(const QueryParams& filters) {
  return http_.get("/v5/spot-lever-token/info", filters, false);
}

std::string PublicRestClient::get_leverage_token_market(const std::string& lt_coin) {
  return http_.get("/v5/spot-lever-token/reference", {{"ltCoin", lt_coin}}, false);
}

}  // namespace bybit
