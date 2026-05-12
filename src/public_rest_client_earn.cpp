#include "bybit/http_client.hpp"
#include "bybit/rest_client.hpp"

namespace bybit {

std::string PublicRestClient::get_earn_product_info(const QueryParams& filters) {
  return http_.get("/v5/earn/product", filters, false);
}

}  // namespace bybit
