#pragma once

#include <optional>
#include <string>

#include "bybit/http_client.hpp"

namespace bybit {

class PublicRestClient {
 public:
  PublicRestClient(HttpClient& http, std::string category);
  std::string get_instruments_info(int limit = 1000);

 private:
  HttpClient& http_;
  std::string category_;
};

class PrivateRestClient {
 public:
  PrivateRestClient(HttpClient& http, std::string category);

  std::string get_query_api_key();
  std::string get_account_info();
  std::string get_position_info(const std::optional<std::string>& settle_coin = std::nullopt, int limit = 200);
  std::string submit_order(const std::string& symbol, const std::string& side, const std::string& order_type,
                           const std::string& qty, const std::string& order_link_id, int position_idx);
  std::string set_leverage(const std::string& symbol, const std::string& buy_leverage,
                           const std::string& sell_leverage);
  std::string get_historic_orders(const std::string& order_id);
  std::string get_fee_rate();

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
  std::string get_position_info(const std::optional<std::string>& settle_coin = std::nullopt, int limit = 200);
  std::string get_instruments_info(int limit = 1000);
  std::string submit_order(const std::string& symbol, const std::string& side, const std::string& order_type,
                           const std::string& qty, const std::string& order_link_id, int position_idx);
  std::string set_leverage(const std::string& symbol, const std::string& buy_leverage,
                           const std::string& sell_leverage);
  std::string get_historic_orders(const std::string& order_id);
  std::string get_fee_rate();

 private:
  HttpClient http_;
  PublicRestClient public_;
  PrivateRestClient private_;
};

}  // namespace bybit
