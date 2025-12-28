#include <optional>
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

std::string PublicRestClient::get_kline(const std::string& symbol, const std::string& interval,
                                        const std::optional<std::string>& start, const std::optional<std::string>& end,
                                        int limit) {
  std::vector<std::pair<std::string, std::string>> params{
      {"category", category_}, {"symbol", symbol}, {"interval", interval}, {"limit", std::to_string(limit)}};
  if (start) params.emplace_back("start", *start);
  if (end) params.emplace_back("end", *end);
  return http_.get("/v5/market/kline", params, false);
}

std::string PublicRestClient::get_mark_price_kline(const std::string& symbol, const std::string& interval,
                                                   const std::optional<std::string>& start,
                                                   const std::optional<std::string>& end, int limit) {
  std::vector<std::pair<std::string, std::string>> params{
      {"category", category_}, {"symbol", symbol}, {"interval", interval}, {"limit", std::to_string(limit)}};
  if (start) params.emplace_back("start", *start);
  if (end) params.emplace_back("end", *end);
  return http_.get("/v5/market/mark-price-kline", params, false);
}

std::string PublicRestClient::get_index_price_kline(const std::string& symbol, const std::string& interval,
                                                    const std::optional<std::string>& start,
                                                    const std::optional<std::string>& end, int limit) {
  std::vector<std::pair<std::string, std::string>> params{
      {"category", category_}, {"symbol", symbol}, {"interval", interval}, {"limit", std::to_string(limit)}};
  if (start) params.emplace_back("start", *start);
  if (end) params.emplace_back("end", *end);
  return http_.get("/v5/market/index-price-kline", params, false);
}

std::string PublicRestClient::get_premium_index_price_kline(const std::string& symbol, const std::string& interval,
                                                            const std::optional<std::string>& start,
                                                            const std::optional<std::string>& end, int limit) {
  std::vector<std::pair<std::string, std::string>> params{
      {"category", category_}, {"symbol", symbol}, {"interval", interval}, {"limit", std::to_string(limit)}};
  if (start) params.emplace_back("start", *start);
  if (end) params.emplace_back("end", *end);
  return http_.get("/v5/market/premium-index-price-kline", params, false);
}

std::string PublicRestClient::get_recent_trades(const std::string& symbol, int limit) {
  std::vector<std::pair<std::string, std::string>> params{
      {"category", category_}, {"symbol", symbol}, {"limit", std::to_string(limit)}};
  return http_.get("/v5/market/recent-trade", params, false);
}

std::string PublicRestClient::get_funding_history(const std::string& symbol, int limit) {
  std::vector<std::pair<std::string, std::string>> params{
      {"category", category_}, {"symbol", symbol}, {"limit", std::to_string(limit)}};
  return http_.get("/v5/market/funding/history", params, false);
}

std::string PublicRestClient::get_open_interest(const std::string& symbol, const std::string& interval, int limit) {
  std::vector<std::pair<std::string, std::string>> params{
      {"category", category_}, {"symbol", symbol}, {"interval", interval}, {"limit", std::to_string(limit)}};
  return http_.get("/v5/market/open-interest", params, false);
}

std::string PublicRestClient::get_long_short_ratio(const std::string& symbol, const std::string& period, int limit) {
  std::vector<std::pair<std::string, std::string>> params{
      {"category", category_}, {"symbol", symbol}, {"period", period}, {"limit", std::to_string(limit)}};
  return http_.get("/v5/market/account-ratio", params, false);
}

}  // namespace bybit
