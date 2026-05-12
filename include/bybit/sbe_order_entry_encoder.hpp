#pragma once

#include <cstdint>
#include <string>
#include <string_view>

namespace bybit::sbe {

enum class Category : std::uint8_t { Unknown = 0, Spot = 1, Linear = 2, Inverse = 3, Option = 4 };
enum class PositionIdx : std::uint8_t { OneWay = 0, HedgeBuy = 1, HedgeSell = 2, Unknown = 253 };
enum class OrderType : std::uint8_t { Unknown = 0, Market = 1, Limit = 2 };
enum class Side : std::uint8_t { Unknown = 0, Buy = 1, Sell = 2 };
enum class TimeInForce : std::uint8_t {
  Unknown = 0,
  GoodTillCancel = 1,
  PostOnly = 2,
  ImmediateOrCancel = 3,
  FillOrKill = 4,
  Rpi = 5
};
enum class SmpType : std::uint8_t { Unknown = 0, CancelTaker = 1, CancelMaker = 2, CancelBoth = 3 };
enum class MarketUnit : std::uint8_t { Unknown = 0, BaseCoin = 1, QuoteCoin = 2 };

struct Decimal64 {
  std::int8_t exponent{};
  std::int64_t mantissa{};
};

struct OrderRequestHeader {
  std::string req_id;
  std::uint64_t timestamp{};
  std::uint32_t recv_window{};
  std::string referer;
};

struct AuthRequest {
  std::string req_id;
  std::string api_key;
  std::uint64_t expires{};
  std::string signature;
};

struct CreateOrderRequest {
  OrderRequestHeader header;
  Category category{Category::Unknown};
  std::int64_t symbol_id{};
  Side side{Side::Unknown};
  OrderType order_type{OrderType::Unknown};
  Decimal64 qty;
  Decimal64 price;
  std::string order_link_id;
  TimeInForce time_in_force{TimeInForce::Unknown};
  PositionIdx position_idx{PositionIdx::OneWay};
  MarketUnit market_unit{MarketUnit::Unknown};
  bool is_leverage{};
  bool reduce_only{};
  bool close_on_trigger{};
  bool mmp{};
  SmpType smp_type{SmpType::Unknown};
};

struct ReplaceOrderRequest {
  OrderRequestHeader header;
  Category category{Category::Unknown};
  std::int64_t symbol_id{};
  std::string order_id;
  std::string order_link_id;
  Decimal64 qty;
  Decimal64 price;
};

struct CancelOrderRequest {
  OrderRequestHeader header;
  Category category{Category::Unknown};
  std::int64_t symbol_id{};
  std::string order_id;
  std::string order_link_id;
};

struct OrderEntryMessageHeader {
  std::uint16_t block_length{};
  std::uint16_t template_id{};
  std::uint16_t schema_id{};
  std::uint16_t version{};
};

struct ApiResponseHeader {
  std::string req_id;
  std::string conn_id;
  std::string trace_id;
  std::int64_t time_now{};
  std::int64_t in_time{};
  std::int64_t bapi_limit{};
  std::int64_t bapi_limit_status{};
  std::int64_t bapi_limit_reset_timestamp{};
};

struct AuthResponse {
  OrderEntryMessageHeader header;
  std::string req_id;
  std::int32_t ret_code{};
  std::string conn_id;
  std::string ret_msg;
};

struct PongResponse {
  OrderEntryMessageHeader header;
  std::uint64_t timestamp{};
  std::uint64_t pong_time{};
};

struct OrderResponse {
  OrderEntryMessageHeader header;
  ApiResponseHeader response_header;
  std::int32_t ret_code{};
  std::string order_id;
  std::string order_link_id;
  std::string ret_msg;
};

struct CommonErrorResponse {
  OrderEntryMessageHeader header;
  ApiResponseHeader response_header;
  std::int32_t ret_code{};
  std::string ret_msg;
};

std::string encode_auth_request(const AuthRequest& request);
std::string encode_ping_request(std::uint64_t timestamp);
std::string encode_create_order_request(const CreateOrderRequest& request);
std::string encode_replace_order_request(const ReplaceOrderRequest& request);
std::string encode_cancel_order_request(const CancelOrderRequest& request);

OrderEntryMessageHeader decode_order_entry_header(std::string_view payload);
AuthResponse decode_auth_response(std::string_view payload);
PongResponse decode_pong_response(std::string_view payload);
OrderResponse decode_order_response(std::string_view payload);
CommonErrorResponse decode_common_error_response(std::string_view payload);

}  // namespace bybit::sbe
