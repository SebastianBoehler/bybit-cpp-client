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

std::string PublicRestClient::get_tickers(const std::string& symbol) {
  std::vector<std::pair<std::string, std::string>> params{{"category", category_}};
  if (!symbol.empty()) params.emplace_back("symbol", symbol);
  return http_.get("/v5/market/tickers", params, false);
}

std::string PublicRestClient::get_orderbook(const std::string& symbol, int limit) {
  std::vector<std::pair<std::string, std::string>> params{
      {"category", category_}, {"symbol", symbol}, {"limit", std::to_string(limit)}};
  return http_.get("/v5/market/orderbook", params, false);
}

}  // namespace bybit
