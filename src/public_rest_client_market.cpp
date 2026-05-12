#include <utility>
#include <vector>

#include "bybit/public_rest_client.hpp"

namespace bybit {

std::string PublicRestClient::get_rpi_orderbook(const std::string& symbol, int limit) {
  return http_.get("/v5/market/rpi_orderbook",
                   {{"category", category_}, {"symbol", symbol}, {"limit", std::to_string(limit)}}, false);
}

std::string PublicRestClient::get_historical_volatility(const QueryParams& filters) {
  return http_.get("/v5/market/historical-volatility", filters, false);
}

std::string PublicRestClient::get_insurance(const QueryParams& filters) {
  return http_.get("/v5/market/insurance", filters, false);
}

std::string PublicRestClient::get_delivery_price(const QueryParams& filters) {
  return http_.get("/v5/market/delivery-price", filters, false);
}

std::string PublicRestClient::get_new_delivery_price(const QueryParams& filters) {
  return http_.get("/v5/market/new-delivery-price", filters, false);
}

std::string PublicRestClient::get_index_price_components(const std::string& index_name) {
  return http_.get("/v5/market/index-price-components", {{"indexName", index_name}}, false);
}

std::string PublicRestClient::get_order_price_limit(const std::string& symbol,
                                                    const std::optional<std::string>& category) {
  QueryParams params{{"category", category.value_or(category_)}, {"symbol", symbol}};
  return http_.get("/v5/market/price-limit", params, false);
}

std::string PublicRestClient::get_adl_alert(const std::optional<std::string>& symbol) {
  QueryParams params;
  if (symbol)
    params.emplace_back("symbol", *symbol);
  return http_.get("/v5/market/adlAlert", params, false);
}

std::string PublicRestClient::get_fee_group_info(const QueryParams& filters) {
  return http_.get("/v5/market/fee-group-info", filters, false);
}

std::string PublicRestClient::get_announcements(const std::string& locale, QueryParams filters) {
  filters.emplace_back("locale", locale);
  return http_.get("/v5/announcements/index", filters, false);
}

std::string PublicRestClient::get_system_status(const QueryParams& filters) {
  return http_.get("/v5/system/status", filters, false);
}

}  // namespace bybit
