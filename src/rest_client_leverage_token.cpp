#include <optional>
#include <string>

#include "bybit/rest_client.hpp"

namespace bybit {

std::string RestClient::get_leverage_token_info(const QueryParams& filters) {
  return public_.get_leverage_token_info(filters);
}

std::string RestClient::get_leverage_token_market(const std::string& lt_coin) {
  return public_.get_leverage_token_market(lt_coin);
}

std::string RestClient::purchase_leverage_token(const std::string& lt_coin, const std::string& amount,
                                                const std::optional<std::string>& serial_no) {
  return private_.purchase_leverage_token(lt_coin, amount, serial_no);
}

std::string RestClient::redeem_leverage_token(const std::string& lt_coin, const std::string& quantity,
                                              const std::optional<std::string>& serial_no) {
  return private_.redeem_leverage_token(lt_coin, quantity, serial_no);
}

std::string RestClient::get_leverage_token_order_records(const QueryParams& filters) {
  return private_.get_leverage_token_order_records(filters);
}

}  // namespace bybit
