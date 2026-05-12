#include "bybit/sbe_order_entry_encoder.hpp"

#include <cassert>
#include <cstdint>
#include <cstring>
#include <string>

namespace {

template <typename T>
T read_le(const std::string& data, std::size_t pos) {
  T value{};
  std::memcpy(&value, data.data() + pos, sizeof(T));
  return value;
}

template <typename T>
void put_le(std::string& out, T value) {
  for (std::size_t i = 0; i < sizeof(T); ++i) {
    out.push_back(static_cast<char>((static_cast<std::uint64_t>(value) >> (i * 8)) & 0xff));
  }
}

void put_fixed(std::string& out, const std::string& value, std::size_t length) {
  out.append(value);
  out.append(length - value.size(), '\0');
}

void put_var(std::string& out, const std::string& value) {
  put_le<std::uint8_t>(out, static_cast<std::uint8_t>(value.size()));
  out.append(value);
}

void put_sbe_header(std::string& out, std::uint16_t block_length, std::uint16_t template_id) {
  put_le<std::uint16_t>(out, block_length);
  put_le<std::uint16_t>(out, template_id);
  put_le<std::uint16_t>(out, 2);
  put_le<std::uint16_t>(out, 1);
}

void put_response_header(std::string& out) {
  put_fixed(out, "req-1", 64);
  put_fixed(out, "conn-1", 64);
  put_fixed(out, "trace-1", 64);
  put_le<std::int64_t>(out, 1);
  put_le<std::int64_t>(out, 2);
  put_le<std::int64_t>(out, 50);
  put_le<std::int64_t>(out, 49);
  put_le<std::int64_t>(out, 3);
}

void put_batch_response_item(std::string& out) {
  put_le<std::int32_t>(out, 0);
  put_le<std::uint8_t>(out, 2);
  put_le<std::int64_t>(out, 11);
  put_fixed(out, "order-1", 64);
  put_fixed(out, "link-1", 64);
}

}  // namespace

int main() {
  {
    bybit::sbe::BatchCreateOrderRequest request;
    request.header = {"batch-create", 1000, 5000, "desk"};
    request.category = bybit::sbe::Category::Linear;
    request.orders.push_back({11,
                              bybit::sbe::Side::Buy,
                              bybit::sbe::OrderType::Limit,
                              {-4, 125000},
                              {-2, 4312500},
                              "link-1",
                              bybit::sbe::TimeInForce::GoodTillCancel,
                              bybit::sbe::PositionIdx::HedgeBuy,
                              bybit::sbe::MarketUnit::BaseCoin,
                              true,
                              false,
                              false,
                              false,
                              bybit::sbe::SmpType::CancelTaker});

    const auto encoded = bybit::sbe::encode_batch_create_order_request(request);
    assert(encoded.size() == 253);
    assert(read_le<std::uint16_t>(encoded, 0) == 141);
    assert(read_le<std::uint16_t>(encoded, 2) == 11);
    assert(static_cast<unsigned char>(encoded[148]) == 2);
    assert(read_le<std::uint16_t>(encoded, 149) == 100);
    assert(read_le<std::uint16_t>(encoded, 151) == 1);
    assert(read_le<std::int64_t>(encoded, 153) == 11);
    assert(static_cast<unsigned char>(encoded[161]) == 1);
    assert(static_cast<unsigned char>(encoded[163]) == 252);
    assert(read_le<std::int64_t>(encoded, 164) == 125000);
  }

  {
    bybit::sbe::BatchCancelOrderRequest request;
    request.header = {"batch-cancel", 2000, 5000, ""};
    request.category = bybit::sbe::Category::Option;
    request.orders.push_back({22, "order-2", "link-2"});

    const auto encoded = bybit::sbe::encode_batch_cancel_order_request(request);
    assert(encoded.size() == 289);
    assert(read_le<std::uint16_t>(encoded, 2) == 15);
    assert(static_cast<unsigned char>(encoded[148]) == 4);
    assert(read_le<std::uint16_t>(encoded, 149) == 136);
    assert(read_le<std::int64_t>(encoded, 153) == 22);
  }

  {
    bybit::sbe::BatchReplaceOrderRequest request;
    request.header = {"batch-replace", 3000, 5000, ""};
    request.category = bybit::sbe::Category::Spot;
    request.orders.push_back({33, "order-3", "link-3", {-3, 5000}, {-1, 123}});

    const auto encoded = bybit::sbe::encode_batch_replace_order_request(request);
    assert(encoded.size() == 307);
    assert(read_le<std::uint16_t>(encoded, 2) == 13);
    assert(static_cast<unsigned char>(encoded[148]) == 1);
    assert(read_le<std::uint16_t>(encoded, 149) == 154);
    assert(read_le<std::int64_t>(encoded, 153) == 33);
  }

  {
    std::string payload;
    put_sbe_header(payload, 236, 12);
    put_response_header(payload);
    put_le<std::int32_t>(payload, 0);
    put_le<std::uint16_t>(payload, 141);
    put_le<std::uint16_t>(payload, 1);
    put_batch_response_item(payload);
    put_var(payload, "OK");
    put_var(payload, "2026-05-12T00:00:00Z");
    put_var(payload, "accepted");

    const auto decoded = bybit::sbe::decode_batch_order_response(payload);
    assert(decoded.header.template_id == 12);
    assert(decoded.response_header.req_id == "req-1");
    assert(decoded.ret_code == 0);
    assert(decoded.items.size() == 1);
    assert(decoded.items[0].category == bybit::sbe::Category::Linear);
    assert(decoded.items[0].symbol_id == 11);
    assert(decoded.items[0].order_id == "order-1");
    assert(decoded.items[0].msg == "OK");
    assert(decoded.items[0].created_at == "2026-05-12T00:00:00Z");
    assert(decoded.ret_msg == "accepted");
  }

  {
    std::string payload;
    put_sbe_header(payload, 236, 16);
    put_response_header(payload);
    put_le<std::int32_t>(payload, 0);
    put_le<std::uint16_t>(payload, 141);
    put_le<std::uint16_t>(payload, 1);
    put_batch_response_item(payload);
    put_var(payload, "cancelled");
    put_var(payload, "OK");

    const auto decoded = bybit::sbe::decode_batch_order_response(payload);
    assert(decoded.header.template_id == 16);
    assert(decoded.items.size() == 1);
    assert(decoded.items[0].msg == "cancelled");
    assert(decoded.items[0].created_at.empty());
    assert(decoded.ret_msg == "OK");
  }
}
