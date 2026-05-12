#include "bybit/http_client.hpp"
#include "bybit/rest_client.hpp"

namespace bybit {

std::string PublicRestClient::get_advanced_earn_product_info(const QueryParams& filters) {
  return http_.get("/v5/earn/advance/product", filters, false);
}

std::string PublicRestClient::get_advanced_earn_product_quote(const QueryParams& filters) {
  return http_.get("/v5/earn/advance/product-extra-info", filters, false);
}

std::string PublicRestClient::get_advanced_earn_liquidity_mining_product_info(const QueryParams& filters) {
  return http_.get("/v5/earn/liquidity-mining/product", filters, false);
}

}  // namespace bybit
