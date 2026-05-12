#include <optional>
#include <string>

#include "bybit/rest_client.hpp"

int main() {
  using RestClient = bybit::RestClient;
  using QueryParams = bybit::QueryParams;

  auto instruments_filters = static_cast<std::string (RestClient::*)(QueryParams)>(&RestClient::get_instruments_info);
  auto instruments_cursor = static_cast<std::string (RestClient::*)(
      const std::optional<std::string>&, int, const std::optional<std::string>&)>(&RestClient::get_instruments_info);
  auto rpi_orderbook = &RestClient::get_rpi_orderbook;
  auto historical_volatility = &RestClient::get_historical_volatility;
  auto insurance = &RestClient::get_insurance;
  auto delivery_price = &RestClient::get_delivery_price;
  auto new_delivery_price = &RestClient::get_new_delivery_price;
  auto index_components = &RestClient::get_index_price_components;
  auto price_limit = &RestClient::get_order_price_limit;
  auto adl_alert = &RestClient::get_adl_alert;
  auto fee_group = &RestClient::get_fee_group_info;
  auto announcements = &RestClient::get_announcements;
  auto system_status = &RestClient::get_system_status;

  (void)instruments_filters;
  (void)instruments_cursor;
  (void)rpi_orderbook;
  (void)historical_volatility;
  (void)insurance;
  (void)delivery_price;
  (void)new_delivery_price;
  (void)index_components;
  (void)price_limit;
  (void)adl_alert;
  (void)fee_group;
  (void)announcements;
  (void)system_status;

  return 0;
}
