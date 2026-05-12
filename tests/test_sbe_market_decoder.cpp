#include <cassert>
#include <cstdint>
#include <string>
#include <type_traits>
#include <variant>

#include "bybit/sbe_market_decoder.hpp"

namespace {

template <typename T>
void put(std::string& out, T value) {
  using U = std::make_unsigned_t<T>;
  auto raw = static_cast<U>(value);
  for (std::size_t i = 0; i < sizeof(T); ++i) {
    out.push_back(static_cast<char>((raw >> (i * 8)) & 0xff));
  }
}

void put_string(std::string& out, const std::string& value) {
  put<std::uint8_t>(out, static_cast<std::uint8_t>(value.size()));
  out.append(value);
}

std::string header(std::uint16_t block_length, std::uint16_t template_id) {
  std::string out;
  put<std::uint16_t>(out, block_length);
  put<std::uint16_t>(out, template_id);
  put<std::uint16_t>(out, 1);
  put<std::uint16_t>(out, 0);
  return out;
}

}  // namespace

int main() {
  {
    auto payload = header(98, 20000);
    for (std::int64_t value = 1; value <= 12; ++value)
      put<std::int64_t>(payload, value);
    put<std::int8_t>(payload, -2);
    put<std::int8_t>(payload, -3);
    put_string(payload, "BTCUSDT");
    auto decoded = std::get<bybit::sbe::BestObRpiEvent>(bybit::sbe::decode_market_message(payload));
    assert(decoded.header.template_id == 20000);
    assert(decoded.ask_normal_price == 5);
    assert(decoded.bid_rpi_size == 12);
    assert(decoded.price_exponent == -2);
    assert(decoded.symbol == "BTCUSDT");
  }

  {
    auto payload = header(35, 20001);
    put<std::int64_t>(payload, 10);
    put<std::int64_t>(payload, 11);
    put<std::int64_t>(payload, 12);
    put<std::int64_t>(payload, 13);
    put<std::int8_t>(payload, -1);
    put<std::int8_t>(payload, -4);
    put<std::uint8_t>(payload, 1);
    put<std::uint16_t>(payload, 16);
    put<std::uint16_t>(payload, 1);
    put<std::int64_t>(payload, 100);
    put<std::int64_t>(payload, 2);
    put<std::uint16_t>(payload, 16);
    put<std::uint16_t>(payload, 1);
    put<std::int64_t>(payload, 99);
    put<std::int64_t>(payload, 3);
    put_string(payload, "ETHUSDT");
    auto decoded = std::get<bybit::sbe::OrderbookL50Event>(bybit::sbe::decode_market_message(payload));
    assert(decoded.package_type == 1);
    assert(decoded.asks.size() == 1);
    assert(decoded.bids[0].price == 99);
    assert(decoded.symbol == "ETHUSDT");
  }

  {
    auto payload = header(10, 20002);
    put<std::int64_t>(payload, 50);
    put<std::int8_t>(payload, -2);
    put<std::int8_t>(payload, -3);
    put<std::uint16_t>(payload, 35);
    put<std::uint16_t>(payload, 1);
    put<std::int64_t>(payload, 51);
    put<std::int64_t>(payload, 1000);
    put<std::int64_t>(payload, 4);
    put<std::int64_t>(payload, 52);
    put<std::uint8_t>(payload, 1);
    put<std::uint8_t>(payload, 0);
    put<std::uint8_t>(payload, 1);
    put_string(payload, "exec-1");
    put_string(payload, "SOLUSDT");
    auto decoded = std::get<bybit::sbe::PublicTradeEvent>(bybit::sbe::decode_market_message(payload));
    assert(decoded.trade_items.size() == 1);
    assert(decoded.trade_items[0].is_rpi);
    assert(decoded.trade_items[0].exec_id == "exec-1");
    assert(decoded.symbol == "SOLUSDT");
  }
}
