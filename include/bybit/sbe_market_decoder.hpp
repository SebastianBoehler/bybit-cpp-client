#pragma once

#include <cstdint>
#include <string>
#include <string_view>
#include <variant>
#include <vector>

namespace bybit::sbe {

struct MessageHeader {
  std::uint16_t block_length{};
  std::uint16_t template_id{};
  std::uint16_t schema_id{};
  std::uint16_t version{};
};

struct BookLevel {
  std::int64_t price{};
  std::int64_t size{};
};

struct BestObRpiEvent {
  MessageHeader header;
  std::int64_t ts{};
  std::int64_t seq{};
  std::int64_t cts{};
  std::int64_t update_id{};
  std::int64_t ask_normal_price{};
  std::int64_t ask_normal_size{};
  std::int64_t ask_rpi_price{};
  std::int64_t ask_rpi_size{};
  std::int64_t bid_normal_price{};
  std::int64_t bid_normal_size{};
  std::int64_t bid_rpi_price{};
  std::int64_t bid_rpi_size{};
  std::int8_t price_exponent{};
  std::int8_t size_exponent{};
  std::string symbol;
};

struct OrderbookL50Event {
  MessageHeader header;
  std::int64_t ts{};
  std::int64_t seq{};
  std::int64_t cts{};
  std::int64_t update_id{};
  std::int8_t price_exponent{};
  std::int8_t size_exponent{};
  std::uint8_t package_type{};
  std::vector<BookLevel> asks;
  std::vector<BookLevel> bids;
  std::string symbol;
};

struct TradeItem {
  std::int64_t fill_time{};
  std::int64_t price{};
  std::int64_t size{};
  std::int64_t seq{};
  std::uint8_t side{};
  bool is_block_trade{};
  bool is_rpi{};
  std::string exec_id;
};

struct PublicTradeEvent {
  MessageHeader header;
  std::int64_t ts{};
  std::int8_t price_exponent{};
  std::int8_t size_exponent{};
  std::vector<TradeItem> trade_items;
  std::string symbol;
};

using MarketMessage = std::variant<BestObRpiEvent, OrderbookL50Event, PublicTradeEvent>;

MessageHeader decode_message_header(std::string_view payload);
MarketMessage decode_market_message(std::string_view payload);

}  // namespace bybit::sbe
