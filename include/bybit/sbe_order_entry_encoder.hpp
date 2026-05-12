#pragma once

#include <cstdint>
#include <string>

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

std::string encode_auth_request(const AuthRequest& request);
std::string encode_ping_request(std::uint64_t timestamp);
std::string encode_create_order_request(const CreateOrderRequest& request);
std::string encode_replace_order_request(const ReplaceOrderRequest& request);
std::string encode_cancel_order_request(const CancelOrderRequest& request);

}  // namespace bybit::sbe
