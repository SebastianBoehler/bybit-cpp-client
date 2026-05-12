#include "bybit/sbe_order_entry_encoder.hpp"

#include <stdexcept>
#include <type_traits>

namespace bybit::sbe {
namespace {

constexpr std::uint16_t kOrderEntrySchema = 2;
constexpr std::uint16_t kOrderEntryVersion = 1;
constexpr std::uint16_t kAuthTemplate = 1;
constexpr std::uint16_t kPingTemplate = 3;
constexpr std::uint16_t kCreateOrderTemplate = 5;
constexpr std::uint16_t kReplaceOrderTemplate = 7;
constexpr std::uint16_t kCancelOrderTemplate = 9;
constexpr std::size_t kString64 = 64;
constexpr std::uint16_t kAuthBlock = 200;
constexpr std::uint16_t kPingBlock = 8;
constexpr std::uint16_t kCreateOrderBlock = 241;
constexpr std::uint16_t kReplaceOrderBlock = 295;
constexpr std::uint16_t kCancelOrderBlock = 277;

template <typename T>
void put_le(std::string& out, T value) {
  static_assert(std::is_integral_v<T>);
  using U = std::make_unsigned_t<T>;
  auto raw = static_cast<U>(value);
  for (std::size_t i = 0; i < sizeof(T); ++i) {
    out.push_back(static_cast<char>((raw >> (i * 8)) & 0xff));
  }
}

template <typename T>
void put_enum(std::string& out, T value) {
  put_le<std::uint8_t>(out, static_cast<std::uint8_t>(value));
}

void put_fixed_string(std::string& out, const std::string& value, std::size_t length) {
  if (value.size() > length) throw std::invalid_argument("SBE fixed string is too long");
  out.append(value);
  out.append(length - value.size(), '\0');
}

void put_header(std::string& out, std::uint16_t block_length, std::uint16_t template_id) {
  put_le<std::uint16_t>(out, block_length);
  put_le<std::uint16_t>(out, template_id);
  put_le<std::uint16_t>(out, kOrderEntrySchema);
  put_le<std::uint16_t>(out, kOrderEntryVersion);
}

void put_order_header(std::string& out, const OrderRequestHeader& header) {
  put_fixed_string(out, header.req_id, kString64);
  put_le<std::uint64_t>(out, header.timestamp);
  put_le<std::uint32_t>(out, header.recv_window);
  put_fixed_string(out, header.referer, kString64);
}

void put_decimal(std::string& out, const Decimal64& value) {
  put_le<std::int8_t>(out, value.exponent);
  put_le<std::int64_t>(out, value.mantissa);
}

}  // namespace

std::string encode_auth_request(const AuthRequest& request) {
  std::string out;
  out.reserve(8 + kAuthBlock);
  put_header(out, kAuthBlock, kAuthTemplate);
  put_fixed_string(out, request.req_id, kString64);
  put_fixed_string(out, request.api_key, kString64);
  put_le<std::uint64_t>(out, request.expires);
  put_fixed_string(out, request.signature, kString64);
  return out;
}

std::string encode_ping_request(std::uint64_t timestamp) {
  std::string out;
  out.reserve(8 + kPingBlock);
  put_header(out, kPingBlock, kPingTemplate);
  put_le<std::uint64_t>(out, timestamp);
  return out;
}

std::string encode_create_order_request(const CreateOrderRequest& request) {
  std::string out;
  out.reserve(8 + kCreateOrderBlock);
  put_header(out, kCreateOrderBlock, kCreateOrderTemplate);
  put_order_header(out, request.header);
  put_enum(out, request.category);
  put_le<std::int64_t>(out, request.symbol_id);
  put_enum(out, request.side);
  put_enum(out, request.order_type);
  put_decimal(out, request.qty);
  put_decimal(out, request.price);
  put_fixed_string(out, request.order_link_id, kString64);
  put_enum(out, request.time_in_force);
  put_enum(out, request.position_idx);
  put_enum(out, request.market_unit);
  put_le<std::uint8_t>(out, request.is_leverage ? 1 : 0);
  put_le<std::uint8_t>(out, request.reduce_only ? 1 : 0);
  put_le<std::uint8_t>(out, request.close_on_trigger ? 1 : 0);
  put_le<std::uint8_t>(out, request.mmp ? 1 : 0);
  put_enum(out, request.smp_type);
  return out;
}

std::string encode_replace_order_request(const ReplaceOrderRequest& request) {
  std::string out;
  out.reserve(8 + kReplaceOrderBlock);
  put_header(out, kReplaceOrderBlock, kReplaceOrderTemplate);
  put_order_header(out, request.header);
  put_enum(out, request.category);
  put_le<std::int64_t>(out, request.symbol_id);
  put_fixed_string(out, request.order_id, kString64);
  put_fixed_string(out, request.order_link_id, kString64);
  put_decimal(out, request.qty);
  put_decimal(out, request.price);
  return out;
}

std::string encode_cancel_order_request(const CancelOrderRequest& request) {
  std::string out;
  out.reserve(8 + kCancelOrderBlock);
  put_header(out, kCancelOrderBlock, kCancelOrderTemplate);
  put_order_header(out, request.header);
  put_enum(out, request.category);
  put_le<std::int64_t>(out, request.symbol_id);
  put_fixed_string(out, request.order_id, kString64);
  put_fixed_string(out, request.order_link_id, kString64);
  return out;
}

}  // namespace bybit::sbe
