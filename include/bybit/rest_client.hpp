#pragma once

#include <optional>
#include <string>
#include <vector>

#include "bybit/http_client.hpp"

namespace bybit {

class PublicRestClient {
 public:
  PublicRestClient(HttpClient& http, std::string category);
  std::string get_instruments_info(int limit = 1000);
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

 private:
  HttpClient& http_;
  std::string category_;
};

class PrivateRestClient {
 public:
  PrivateRestClient(HttpClient& http, std::string category);

  std::string get_query_api_key();
  std::string get_account_info();
  std::string get_position_info(const std::optional<std::string>& settle_coin = std::nullopt,
                                const std::optional<std::string>& symbol = std::nullopt, int limit = 200);
  std::string submit_order(const std::string& symbol, const std::string& side, const std::string& order_type,
                           const std::string& qty, const std::string& order_link_id, int position_idx,
                           const std::string& price = "", const std::string& time_in_force = "GTC");
  std::string set_leverage(const std::string& symbol, const std::string& buy_leverage,
                           const std::string& sell_leverage);
  std::string get_historic_orders(const std::string& order_id);
  std::string get_fee_rate();
  std::string get_wallet_balance(const std::optional<std::string>& coin = std::nullopt);
  std::string get_open_orders(const std::optional<std::string>& symbol = std::nullopt, int limit = 50);
  std::string cancel_order(const std::string& symbol, const std::string& order_id);
  std::string amend_order(const std::string& symbol, const std::string& order_id,
                          const std::optional<std::string>& qty = std::nullopt,
                          const std::optional<std::string>& price = std::nullopt);
  std::string get_transaction_log(int limit = 50, const std::optional<std::string>& cursor = std::nullopt);
  std::string move_position(const std::string& from_uid, const std::string& to_uid, const std::string& symbol,
                            const std::string& qty, int position_idx = 1);
  std::string set_trading_stop(const std::string& symbol, int position_idx,
                               const std::optional<std::string>& take_profit = std::nullopt,
                               const std::optional<std::string>& stop_loss = std::nullopt,
                               const std::optional<std::string>& trailing_stop = std::nullopt);
  std::string set_risk_limit(const std::string& symbol, const std::string& risk_id, int position_idx = 1);
  std::string add_margin(const std::string& symbol, const std::string& margin, int position_idx = 1);
  std::string switch_position_mode(const std::string& mode, int position_idx = 1);
  std::string switch_margin_mode(const std::string& symbol, const std::string& mode, int leverage);
  std::string cancel_all(const std::string& symbol);

 private:
  HttpClient& http_;
  std::string category_;
};

// Facade preserving previous interface while delegating to public/private clients.
class RestClient {
 public:
  explicit RestClient(std::string api_key, std::string api_secret, std::string category,
                      std::string base_url = "https://api.bybit.com", std::string recv_window = "5000");

  std::string get_query_api_key();
  std::string get_account_info();
  std::string get_position_info(const std::optional<std::string>& settle_coin = std::nullopt,
                                const std::optional<std::string>& symbol = std::nullopt, int limit = 200);
  std::string get_instruments_info(int limit = 1000);
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
  std::string submit_order(const std::string& symbol, const std::string& side, const std::string& order_type,
                           const std::string& qty, const std::string& order_link_id, int position_idx,
                           const std::string& price = "", const std::string& time_in_force = "GTC");
  std::string set_leverage(const std::string& symbol, const std::string& buy_leverage,
                           const std::string& sell_leverage);
  std::string get_historic_orders(const std::string& order_id);
  std::string get_fee_rate();
  std::string get_wallet_balance(const std::optional<std::string>& coin = std::nullopt);
  std::string get_open_orders(const std::optional<std::string>& symbol = std::nullopt, int limit = 50);
  std::string cancel_order(const std::string& symbol, const std::string& order_id);
  std::string amend_order(const std::string& symbol, const std::string& order_id,
                          const std::optional<std::string>& qty = std::nullopt,
                          const std::optional<std::string>& price = std::nullopt);
  std::string get_transaction_log(int limit = 50, const std::optional<std::string>& cursor = std::nullopt);
  std::string move_position(const std::string& from_uid, const std::string& to_uid, const std::string& symbol,
                            const std::string& qty, int position_idx = 1);
  std::string set_trading_stop(const std::string& symbol, int position_idx,
                               const std::optional<std::string>& take_profit = std::nullopt,
                               const std::optional<std::string>& stop_loss = std::nullopt,
                               const std::optional<std::string>& trailing_stop = std::nullopt);
  std::string set_risk_limit(const std::string& symbol, const std::string& risk_id, int position_idx = 1);
  std::string add_margin(const std::string& symbol, const std::string& margin, int position_idx = 1);
  std::string switch_position_mode(const std::string& mode, int position_idx = 1);
  std::string switch_margin_mode(const std::string& symbol, const std::string& mode, int leverage);
  std::string cancel_all(const std::string& symbol);

 private:
  HttpClient http_;
  PublicRestClient public_;
  PrivateRestClient private_;
};

}  // namespace bybit
