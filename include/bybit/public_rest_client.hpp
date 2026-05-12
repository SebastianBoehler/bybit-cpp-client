#pragma once

#include <optional>
#include <string>

#include "bybit/http_client.hpp"
#include "bybit/rest_client_types.hpp"

namespace bybit {

class PublicRestClient {
 public:
  PublicRestClient(HttpClient& http, std::string category);
  std::string get_server_time();
  std::string get_instruments_info(int limit = 1000);
  std::string get_instruments_info(const std::optional<std::string>& symbol, int limit = 1000,
                                   const std::optional<std::string>& cursor = std::nullopt);
  std::string get_tickers(const std::string& symbol = "");
  std::string get_orderbook(const std::string& symbol, int limit = 50);
  std::string get_kline(const std::string& symbol, const std::string& interval,
                        const std::optional<std::string>& start = std::nullopt,
                        const std::optional<std::string>& end = std::nullopt, int limit = 200);
  std::string get_mark_price_kline(const std::string& symbol, const std::string& interval,
                                   const std::optional<std::string>& start = std::nullopt,
                                   const std::optional<std::string>& end = std::nullopt, int limit = 200);
  std::string get_index_price_kline(const std::string& symbol, const std::string& interval,
                                    const std::optional<std::string>& start = std::nullopt,
                                    const std::optional<std::string>& end = std::nullopt, int limit = 200);
  std::string get_premium_index_price_kline(const std::string& symbol, const std::string& interval,
                                            const std::optional<std::string>& start = std::nullopt,
                                            const std::optional<std::string>& end = std::nullopt, int limit = 200);
  std::string get_recent_trades(const std::string& symbol, int limit = 50);
  std::string get_funding_history(const std::string& symbol, int limit = 50);
  std::string get_open_interest(const std::string& symbol, const std::string& interval, int limit = 50);
  std::string get_long_short_ratio(const std::string& symbol, const std::string& period, int limit = 50);
  std::string get_risk_limit(const std::optional<std::string>& symbol = std::nullopt,
                             const std::optional<std::string>& cursor = std::nullopt);
  std::string get_earn_product_info(const QueryParams& filters);
  std::string get_advanced_earn_product_info(const QueryParams& filters);
  std::string get_advanced_earn_product_quote(const QueryParams& filters);
  std::string get_advanced_earn_liquidity_mining_product_info(const QueryParams& filters);
  std::string get_spread_instruments(const QueryParams& filters = {});
  std::string get_spread_orderbook(const std::string& symbol, int limit = 1);
  std::string get_spread_tickers(const std::string& symbol);
  std::string get_spread_recent_trades(const std::string& symbol, int limit = 500);

 private:
  HttpClient& http_;
  std::string category_;
};

}  // namespace bybit
