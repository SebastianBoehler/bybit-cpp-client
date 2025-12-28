#include <utility>
#include <vector>

#include "bybit/http_client.hpp"
#include "bybit/rest_client.hpp"

namespace bybit {

PublicRestClient::PublicRestClient(HttpClient& http, std::string category)
    : http_(http), category_(std::move(category)) {}

std::string PublicRestClient::get_instruments_info(int limit) {
  std::vector<std::pair<std::string, std::string>> params{{"category", category_}, {"limit", std::to_string(limit)}};
  return http_.get("/v5/market/instruments-info", params, false);
}

}  // namespace bybit
