#include "bybit/sbe_order_entry_encoder.hpp"

#include <cassert>
#include <cstdint>
#include <cstring>
#include <stdexcept>
#include <string>

namespace {

template <typename T>
T read_le(const std::string& data, std::size_t pos) {
  T value{};
  std::memcpy(&value, data.data() + pos, sizeof(T));
  return value;
}

std::string fixed_string(const std::string& data, std::size_t pos, std::size_t length) {
  return data.substr(pos, length);
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

}  // namespace

int main() {
  {
    const auto encoded = bybit::sbe::encode_auth_request({"auth-1", "key", 42, "sig"});
    assert(encoded.size() == 208);
    assert(read_le<std::uint16_t>(encoded, 0) == 200);
    assert(read_le<std::uint16_t>(encoded, 2) == 1);
    assert(read_le<std::uint16_t>(encoded, 4) == 2);
    assert(read_le<std::uint16_t>(encoded, 6) == 1);
    assert(fixed_string(encoded, 8, 6) == "auth-1");
    assert(fixed_string(encoded, 72, 3) == "key");
    assert(read_le<std::uint64_t>(encoded, 136) == 42);
    assert(fixed_string(encoded, 144, 3) == "sig");
  }

  {
    const auto encoded = bybit::sbe::encode_ping_request(1234);
    assert(encoded.size() == 16);
    assert(read_le<std::uint16_t>(encoded, 0) == 8);
    assert(read_le<std::uint16_t>(encoded, 2) == 3);
    assert(read_le<std::uint64_t>(encoded, 8) == 1234);
  }

  {
    bybit::sbe::OrderRequestHeader header;
    header.req_id = "create-1";
    header.timestamp = 1000;
    header.recv_window = 5000;
    header.referer = "desk";

    bybit::sbe::CreateOrderRequest request;
    request.header = header;
    request.category = bybit::sbe::Category::Linear;
    request.symbol_id = 11;
    request.side = bybit::sbe::Side::Buy;
    request.order_type = bybit::sbe::OrderType::Limit;
    request.qty = {-4, 125000};
    request.price = {-2, 4312500};
    request.order_link_id = "link-1";
    request.time_in_force = bybit::sbe::TimeInForce::GoodTillCancel;
    request.position_idx = bybit::sbe::PositionIdx::HedgeBuy;
    request.market_unit = bybit::sbe::MarketUnit::BaseCoin;
    request.is_leverage = true;
    request.smp_type = bybit::sbe::SmpType::CancelTaker;

    const auto encoded = bybit::sbe::encode_create_order_request(request);
    assert(encoded.size() == 249);
    assert(read_le<std::uint16_t>(encoded, 0) == 241);
    assert(read_le<std::uint16_t>(encoded, 2) == 5);
    assert(fixed_string(encoded, 8, 8) == "create-1");
    assert(read_le<std::uint64_t>(encoded, 72) == 1000);
    assert(read_le<std::uint32_t>(encoded, 80) == 5000);
    assert(fixed_string(encoded, 84, 4) == "desk");
    assert(static_cast<unsigned char>(encoded[148]) == 2);
    assert(read_le<std::int64_t>(encoded, 149) == 11);
    assert(static_cast<unsigned char>(encoded[157]) == 1);
    assert(static_cast<unsigned char>(encoded[159]) == 252);
    assert(read_le<std::int64_t>(encoded, 160) == 125000);
    assert(fixed_string(encoded, 177, 6) == "link-1");
    assert(static_cast<unsigned char>(encoded[241]) == 1);
    assert(static_cast<unsigned char>(encoded[248]) == 1);
  }

  {
    bybit::sbe::OrderRequestHeader header{"replace-1", 2000, 4000, ""};
    bybit::sbe::ReplaceOrderRequest request;
    request.header = header;
    request.category = bybit::sbe::Category::Spot;
    request.symbol_id = 22;
    request.order_id = "order-1";
    request.order_link_id = "link-2";
    request.qty = {-3, 5000};
    request.price = {-1, 123};

    const auto encoded = bybit::sbe::encode_replace_order_request(request);
    assert(encoded.size() == 303);
    assert(read_le<std::uint16_t>(encoded, 0) == 295);
    assert(read_le<std::uint16_t>(encoded, 2) == 7);
    assert(static_cast<unsigned char>(encoded[148]) == 1);
    assert(read_le<std::int64_t>(encoded, 149) == 22);
    assert(fixed_string(encoded, 157, 7) == "order-1");
    assert(fixed_string(encoded, 221, 6) == "link-2");
    assert(static_cast<unsigned char>(encoded[285]) == 253);
    assert(read_le<std::int64_t>(encoded, 286) == 5000);
  }

  {
    bybit::sbe::CancelOrderRequest request;
    request.header = {"cancel-1", 3000, 4000, "ref"};
    request.category = bybit::sbe::Category::Option;
    request.symbol_id = 33;
    request.order_id = "order-2";
    request.order_link_id = "link-3";

    const auto encoded = bybit::sbe::encode_cancel_order_request(request);
    assert(encoded.size() == 285);
    assert(read_le<std::uint16_t>(encoded, 0) == 277);
    assert(read_le<std::uint16_t>(encoded, 2) == 9);
    assert(static_cast<unsigned char>(encoded[148]) == 4);
    assert(read_le<std::int64_t>(encoded, 149) == 33);
    assert(fixed_string(encoded, 157, 7) == "order-2");
    assert(fixed_string(encoded, 221, 6) == "link-3");
  }

  {
    bybit::sbe::AuthRequest request;
    request.req_id.assign(65, 'x');
    try {
      (void)bybit::sbe::encode_auth_request(request);
      assert(false);
    } catch (const std::invalid_argument&) {
    }
  }

  {
    std::string payload;
    put_sbe_header(payload, 132, 2);
    put_fixed(payload, "auth-1", 64);
    put_le<std::int32_t>(payload, 0);
    put_fixed(payload, "conn-1", 64);
    put_var(payload, "OK");

    const auto decoded = bybit::sbe::decode_auth_response(payload);
    assert(decoded.header.template_id == 2);
    assert(decoded.req_id == "auth-1");
    assert(decoded.ret_code == 0);
    assert(decoded.conn_id == "conn-1");
    assert(decoded.ret_msg == "OK");
  }

  {
    std::string payload;
    put_sbe_header(payload, 16, 4);
    put_le<std::uint64_t>(payload, 10);
    put_le<std::uint64_t>(payload, 20);

    const auto decoded = bybit::sbe::decode_pong_response(payload);
    assert(decoded.timestamp == 10);
    assert(decoded.pong_time == 20);
  }

  {
    std::string payload;
    put_sbe_header(payload, 364, 6);
    put_response_header(payload);
    put_le<std::int32_t>(payload, 0);
    put_fixed(payload, "order-1", 64);
    put_fixed(payload, "link-1", 64);
    put_var(payload, "OK");

    const auto decoded = bybit::sbe::decode_order_response(payload);
    assert(decoded.header.template_id == 6);
    assert(decoded.response_header.req_id == "req-1");
    assert(decoded.response_header.bapi_limit == 50);
    assert(decoded.ret_code == 0);
    assert(decoded.order_id == "order-1");
    assert(decoded.order_link_id == "link-1");
    assert(decoded.ret_msg == "OK");
  }

  {
    std::string payload;
    put_sbe_header(payload, 236, 17);
    put_response_header(payload);
    put_le<std::int32_t>(payload, 10001);
    put_var(payload, "bad request");

    const auto decoded = bybit::sbe::decode_common_error_response(payload);
    assert(decoded.header.template_id == 17);
    assert(decoded.ret_code == 10001);
    assert(decoded.ret_msg == "bad request");
  }
}
