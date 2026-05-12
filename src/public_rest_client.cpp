#include <optional>
#include <string>
#include <utility>
#include <vector>

#include "bybit/http_client.hpp"
#include "bybit/rest_client.hpp"

namespace bybit {
namespace {

bool has_param(const QueryParams& params, const std::string& key) {
  for (const auto& param : params) {
    if (param.first == key) return true;
  }
  return false;
}

}  // namespace

PublicRestClient::PublicRestClient(HttpClient& http, std::string category)
    : http_(http), category_(std::move(category)) {}

std::string PublicRestClient::get_server_time() {
  return http_.get("/v5/market/time", {}, false);
}

std::string PublicRestClient::get_instruments_info(int limit) {
  return get_instruments_info(QueryParams{{"limit", std::to_string(limit)}});
}

std::string PublicRestClient::get_instruments_info(QueryParams filters) {
  if (!has_param(filters, "category")) filters.emplace_back("category", category_);
  return http_.get("/v5/market/instruments-info", filters, false);
}

std::string PublicRestClient::get_instruments_info(const std::optional<std::string>& symbol, int limit,
                                                   const std::optional<std::string>& cursor) {
  QueryParams params{{"limit", std::to_string(limit)}};
  if (symbol) params.emplace_back("symbol", *symbol);
  if (cursor) params.emplace_back("cursor", *cursor);
  return get_instruments_info(std::move(params));
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

std::string PublicRestClient::get_risk_limit(const std::optional<std::string>& symbol,
                                             const std::optional<std::string>& cursor) {
  std::vector<std::pair<std::string, std::string>> params{{"category", category_}};
  if (symbol) params.emplace_back("symbol", *symbol);
  if (cursor) params.emplace_back("cursor", *cursor);
  return http_.get("/v5/market/risk-limit", params, false);
}

std::string PublicRestClient::get_spread_instruments(const QueryParams& filters) {
  return http_.get("/v5/spread/instrument", filters, false);
}

std::string PublicRestClient::get_spread_orderbook(const std::string& symbol, int limit) {
  return http_.get("/v5/spread/orderbook", {{"symbol", symbol}, {"limit", std::to_string(limit)}}, false);
}

std::string PublicRestClient::get_spread_tickers(const std::string& symbol) {
  return http_.get("/v5/spread/tickers", {{"symbol", symbol}}, false);
}

std::string PublicRestClient::get_spread_recent_trades(const std::string& symbol, int limit) {
  return http_.get("/v5/spread/recent-trade", {{"symbol", symbol}, {"limit", std::to_string(limit)}}, false);
}

}  // namespace bybit
