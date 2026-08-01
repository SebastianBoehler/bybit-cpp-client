#include <cstdint>
#include <cstring>
#include <stdexcept>
#include <string>

#include "bybit/sbe_order_entry_encoder.hpp"

namespace {

void check(bool condition, const char* expression, int line) {
  if (!condition)
    throw std::runtime_error("check failed at line " + std::to_string(line) + ": " + expression);
}

#define CHECK(expression) check(static_cast<bool>(expression), #expression, __LINE__)

template <typename Invoke>
void expect_runtime_error(Invoke invoke) {
  bool threw = false;
  try {
    invoke();
  } catch (const std::runtime_error&) {
    threw = true;
  }
  CHECK(threw);
}

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

void put_var16(std::string& out, const std::string& value) {
  put_le<std::uint16_t>(out, static_cast<std::uint16_t>(value.size()));
  out.append(value);
}

void put_sbe_header(std::string& out, std::uint16_t block_length, std::uint16_t template_id) {
  put_le<std::uint16_t>(out, block_length);
  put_le<std::uint16_t>(out, template_id);
  put_le<std::uint16_t>(out, 2);
  put_le<std::uint16_t>(out, 2);
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
    CHECK(encoded.size() == 208);
    CHECK(read_le<std::uint16_t>(encoded, 0) == 200);
    CHECK(read_le<std::uint16_t>(encoded, 2) == 1);
    CHECK(read_le<std::uint16_t>(encoded, 4) == 2);
    CHECK(read_le<std::uint16_t>(encoded, 6) == 2);
    CHECK(fixed_string(encoded, 8, 6) == "auth-1");
    CHECK(fixed_string(encoded, 72, 3) == "key");
    CHECK(read_le<std::uint64_t>(encoded, 136) == 42);
    CHECK(fixed_string(encoded, 144, 3) == "sig");
  }

  {
    const auto encoded = bybit::sbe::encode_ping_request(1234);
    CHECK(encoded.size() == 16);
    CHECK(read_le<std::uint16_t>(encoded, 0) == 8);
    CHECK(read_le<std::uint16_t>(encoded, 2) == 3);
    CHECK(read_le<std::uint64_t>(encoded, 8) == 1234);
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
    request.rpi_taker_access = true;

    const auto encoded = bybit::sbe::encode_create_order_request(request);
    CHECK(encoded.size() == 250);
    CHECK(read_le<std::uint16_t>(encoded, 0) == 242);
    CHECK(read_le<std::uint16_t>(encoded, 2) == 5);
    CHECK(read_le<std::uint16_t>(encoded, 6) == 2);
    CHECK(fixed_string(encoded, 8, 8) == "create-1");
    CHECK(read_le<std::uint64_t>(encoded, 72) == 1000);
    CHECK(read_le<std::uint32_t>(encoded, 80) == 5000);
    CHECK(fixed_string(encoded, 84, 4) == "desk");
    CHECK(static_cast<unsigned char>(encoded[148]) == 2);
    CHECK(read_le<std::int64_t>(encoded, 149) == 11);
    CHECK(static_cast<unsigned char>(encoded[157]) == 1);
    CHECK(static_cast<unsigned char>(encoded[159]) == 252);
    CHECK(read_le<std::int64_t>(encoded, 160) == 125000);
    CHECK(fixed_string(encoded, 177, 6) == "link-1");
    CHECK(static_cast<unsigned char>(encoded[241]) == 1);
    CHECK(static_cast<unsigned char>(encoded[248]) == 1);
    CHECK(static_cast<unsigned char>(encoded[249]) == 1);
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
    CHECK(encoded.size() == 303);
    CHECK(read_le<std::uint16_t>(encoded, 0) == 295);
    CHECK(read_le<std::uint16_t>(encoded, 2) == 7);
    CHECK(static_cast<unsigned char>(encoded[148]) == 1);
    CHECK(read_le<std::int64_t>(encoded, 149) == 22);
    CHECK(fixed_string(encoded, 157, 7) == "order-1");
    CHECK(fixed_string(encoded, 221, 6) == "link-2");
    CHECK(static_cast<unsigned char>(encoded[285]) == 253);
    CHECK(read_le<std::int64_t>(encoded, 286) == 5000);
  }

  {
    bybit::sbe::CancelOrderRequest request;
    request.header = {"cancel-1", 3000, 4000, "ref"};
    request.category = bybit::sbe::Category::Option;
    request.symbol_id = 33;
    request.order_id = "order-2";
    request.order_link_id = "link-3";

    const auto encoded = bybit::sbe::encode_cancel_order_request(request);
    CHECK(encoded.size() == 285);
    CHECK(read_le<std::uint16_t>(encoded, 0) == 277);
    CHECK(read_le<std::uint16_t>(encoded, 2) == 9);
    CHECK(static_cast<unsigned char>(encoded[148]) == 4);
    CHECK(read_le<std::int64_t>(encoded, 149) == 33);
    CHECK(fixed_string(encoded, 157, 7) == "order-2");
    CHECK(fixed_string(encoded, 221, 6) == "link-3");
  }

  {
    bybit::sbe::AuthRequest request;
    request.req_id.assign(65, 'x');
    try {
      (void)bybit::sbe::encode_auth_request(request);
      CHECK(false);
    } catch (const std::invalid_argument&) {
    }
  }

  {
    std::string payload;
    put_sbe_header(payload, 132, 2);
    put_fixed(payload, "auth-1", 64);
    put_le<std::int32_t>(payload, 0);
    put_fixed(payload, "conn-1", 64);
    const std::string ret_msg(300, 'x');
    put_var16(payload, ret_msg);

    const auto decoded = bybit::sbe::decode_auth_response(payload);
    CHECK(decoded.header.template_id == 2);
    CHECK(decoded.req_id == "auth-1");
    CHECK(decoded.ret_code == 0);
    CHECK(decoded.conn_id == "conn-1");
    CHECK(decoded.ret_msg == ret_msg);
  }

  {
    std::string payload;
    put_sbe_header(payload, 16, 4);
    put_le<std::uint64_t>(payload, 10);
    put_le<std::uint64_t>(payload, 20);

    const auto decoded = bybit::sbe::decode_pong_response(payload);
    CHECK(decoded.timestamp == 10);
    CHECK(decoded.pong_time == 20);
  }

  {
    std::string payload;
    put_sbe_header(payload, 364, 6);
    put_response_header(payload);
    put_le<std::int32_t>(payload, 0);
    put_fixed(payload, "order-1", 64);
    put_fixed(payload, "link-1", 64);
    put_var16(payload, "OK");

    const auto decoded = bybit::sbe::decode_order_response(payload);
    CHECK(decoded.header.template_id == 6);
    CHECK(decoded.response_header.req_id == "req-1");
    CHECK(decoded.response_header.bapi_limit == 50);
    CHECK(decoded.ret_code == 0);
    CHECK(decoded.order_id == "order-1");
    CHECK(decoded.order_link_id == "link-1");
    CHECK(decoded.ret_msg == "OK");
  }

  {
    std::string payload;
    put_sbe_header(payload, 236, 17);
    put_response_header(payload);
    put_le<std::int32_t>(payload, 10001);
    put_var16(payload, "bad request");

    const auto decoded = bybit::sbe::decode_common_error_response(payload);
    CHECK(decoded.header.template_id == 17);
    CHECK(decoded.ret_code == 10001);
    CHECK(decoded.ret_msg == "bad request");
  }

  {
    std::string payload;
    put_sbe_header(payload, 132, 2);
    put_fixed(payload, "auth-1", 64);
    put_le<std::int32_t>(payload, 0);
    put_fixed(payload, "conn-1", 64);
    put_le<std::uint16_t>(payload, 4);
    payload += "no";
    expect_runtime_error([&payload] { (void)bybit::sbe::decode_auth_response(payload); });
  }

  {
    std::string payload;
    put_sbe_header(payload, 131, 2);
    expect_runtime_error([&payload] { (void)bybit::sbe::decode_auth_response(payload); });
  }
}
