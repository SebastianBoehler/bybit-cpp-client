#include <optional>
#include <string>

#include "bybit/rest_client.hpp"

namespace bybit {

std::string PrivateRestClient::purchase_leverage_token(const std::string& lt_coin, const std::string& amount,
                                                       const std::optional<std::string>& serial_no) {
  QueryParams params{{"ltCoin", lt_coin}, {"amount", amount}};
  if (serial_no)
    params.emplace_back("serialNo", *serial_no);
  return http_.post("/v5/spot-lever-token/purchase", to_json_object(params), true);
}

std::string PrivateRestClient::redeem_leverage_token(const std::string& lt_coin, const std::string& quantity,
                                                     const std::optional<std::string>& serial_no) {
  QueryParams params{{"ltCoin", lt_coin}, {"quantity", quantity}};
  if (serial_no)
    params.emplace_back("serialNo", *serial_no);
  return http_.post("/v5/spot-lever-token/redeem", to_json_object(params), true);
}

std::string PrivateRestClient::get_leverage_token_order_records(const QueryParams& filters) {
  return http_.get("/v5/spot-lever-token/order-record", filters, true);
}

}  // namespace bybit
