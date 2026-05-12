#pragma once

#include <optional>
#include <string>
#include <vector>

#include "bybit/http_client.hpp"

namespace bybit {

using QueryParams = std::vector<std::pair<std::string, std::string>>;

struct MovePositionLeg {
  std::string category;
  std::string symbol;
  std::string price;
  std::string side;
  std::string qty;
};

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
                           const std::string& price = "", const std::string& time_in_force = "GTC",
                           const std::optional<bool>& reduce_only = std::nullopt,
                           const std::optional<std::string>& bbo_side_type = std::nullopt,
                           const std::optional<std::string>& bbo_level = std::nullopt);
  std::string batch_submit_orders(const std::vector<std::vector<std::pair<std::string, std::string>>>& order_requests);
  std::string batch_cancel_orders(const std::vector<std::vector<std::pair<std::string, std::string>>>& cancel_requests);
  std::string batch_amend_orders(const std::vector<std::vector<std::pair<std::string, std::string>>>& amend_requests);
  std::string set_leverage(const std::string& symbol, const std::string& buy_leverage,
                           const std::string& sell_leverage);
  std::string get_order_history(const QueryParams& filters = {});
  std::string get_historic_orders(const std::string& order_id);
  std::string get_trade_history(const QueryParams& filters = {});
  std::string get_closed_pnl(const QueryParams& filters = {});
  std::string get_fee_rate();
  std::string get_borrow_quota(const std::string& symbol, const std::string& side);
  // category is provided per-call to avoid forcing a single accountType for all operations.
  std::string get_wallet_balance(const std::string& category, const std::optional<std::string>& coin = std::nullopt);
  std::string get_open_orders(const std::optional<std::string>& symbol = std::nullopt, int limit = 50);
  std::string get_realtime_orders(const QueryParams& filters = {});
  std::string cancel_order(const std::string& symbol, const std::string& order_id);
  std::string amend_order(const std::string& symbol, const std::string& order_id,
                          const std::optional<std::string>& qty = std::nullopt,
                          const std::optional<std::string>& price = std::nullopt);
  std::string get_transaction_log(int limit = 50, const std::optional<std::string>& cursor = std::nullopt);
  std::string move_position(const std::string& from_uid, const std::string& to_uid, const std::string& symbol,
                            const std::string& qty, int position_idx = 1);
  std::string move_positions(const std::string& from_uid, const std::string& to_uid,
                             const std::vector<MovePositionLeg>& legs);
  std::string get_move_position_history(const QueryParams& filters = {});
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
  RestClient(std::string api_key, std::string api_secret, std::string category, HttpOptions options);
  RestClient(std::string api_key, std::string api_secret, std::string category, std::string base_url,
             std::string recv_window, HttpOptions options);

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
  std::string get_server_time();
  std::string get_risk_limit(const std::optional<std::string>& symbol = std::nullopt,
                             const std::optional<std::string>& cursor = std::nullopt);
  std::string submit_order(const std::string& symbol, const std::string& side, const std::string& order_type,
                           const std::string& qty, const std::string& order_link_id, int position_idx,
                           const std::string& price = "", const std::string& time_in_force = "GTC",
                           const std::optional<bool>& reduce_only = std::nullopt,
                           const std::optional<std::string>& bbo_side_type = std::nullopt,
                           const std::optional<std::string>& bbo_level = std::nullopt);
  std::string batch_submit_orders(const std::vector<std::vector<std::pair<std::string, std::string>>>& order_requests);
  std::string batch_cancel_orders(const std::vector<std::vector<std::pair<std::string, std::string>>>& cancel_requests);
  std::string batch_amend_orders(const std::vector<std::vector<std::pair<std::string, std::string>>>& amend_requests);
  std::string set_leverage(const std::string& symbol, const std::string& buy_leverage,
                           const std::string& sell_leverage);
  std::string get_order_history(const QueryParams& filters = {});
  std::string get_historic_orders(const std::string& order_id);
  std::string get_trade_history(const QueryParams& filters = {});
  std::string get_closed_pnl(const QueryParams& filters = {});
  std::string get_fee_rate();
  std::string get_borrow_quota(const std::string& symbol, const std::string& side);
  // category is provided per-call to avoid forcing a single accountType for all operations.
  std::string get_wallet_balance(const std::string& category, const std::optional<std::string>& coin = std::nullopt);
  std::string get_open_orders(const std::optional<std::string>& symbol = std::nullopt, int limit = 50);
  std::string get_realtime_orders(const QueryParams& filters = {});
  std::string cancel_order(const std::string& symbol, const std::string& order_id);
  std::string amend_order(const std::string& symbol, const std::string& order_id,
                          const std::optional<std::string>& qty = std::nullopt,
                          const std::optional<std::string>& price = std::nullopt);
  std::string get_transaction_log(int limit = 50, const std::optional<std::string>& cursor = std::nullopt);
  std::string move_position(const std::string& from_uid, const std::string& to_uid, const std::string& symbol,
                            const std::string& qty, int position_idx = 1);
  std::string move_positions(const std::string& from_uid, const std::string& to_uid,
                             const std::vector<MovePositionLeg>& legs);
  std::string get_move_position_history(const QueryParams& filters = {});
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
