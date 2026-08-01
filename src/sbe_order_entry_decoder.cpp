#include <cstring>
#include <stdexcept>
#include <type_traits>

#include "bybit/sbe_order_entry_encoder.hpp"

namespace bybit::sbe {
namespace {

constexpr std::uint16_t kOrderEntrySchema = 2;
constexpr std::uint16_t kCommonErrorTemplate = 17;
constexpr std::size_t kString64 = 64;
constexpr std::uint16_t kAuthResponseBlock = 132;
constexpr std::uint16_t kPongResponseBlock = 16;
constexpr std::uint16_t kOrderResponseBlock = 364;
constexpr std::uint16_t kBatchResponseBlock = 236;
constexpr std::uint16_t kBatchResponseItemBlock = 141;
constexpr std::uint16_t kCommonErrorBlock = 236;

class Reader {
 public:
  explicit Reader(std::string_view payload) : payload_(payload) {}

  template <typename T>
  T read_le() {
    static_assert(std::is_integral_v<T>);
    require(sizeof(T));
    using U = std::make_unsigned_t<T>;
    U raw{};
    for (std::size_t i = 0; i < sizeof(T); ++i) {
      raw |= static_cast<U>(static_cast<unsigned char>(payload_[pos_ + i])) << (i * 8);
    }
    pos_ += sizeof(T);
    T value{};
    std::memcpy(&value, &raw, sizeof(T));
    return value;
  }

  std::string read_fixed_string(std::size_t length) {
    require(length);
    const auto data = payload_.substr(pos_, length);
    pos_ += length;
    const auto end = data.find('\0');
    return std::string(data.substr(0, end));
  }

  std::string read_var_string16() {
    const auto length = read_le<std::uint16_t>();
    require(length);
    const auto data = payload_.substr(pos_, length);
    pos_ += length;
    return std::string(data);
  }

  void seek(std::size_t pos) {
    if (pos > payload_.size())
      throw std::runtime_error("truncated SBE order entry payload");
    pos_ = pos;
  }

  std::size_t pos() const {
    return pos_;
  }

 private:
  void require(std::size_t length) const {
    if (payload_.size() - pos_ < length)
      throw std::runtime_error("truncated SBE order entry payload");
  }

  std::string_view payload_;
  std::size_t pos_{};
};

OrderEntryMessageHeader read_message_header(Reader& reader) {
  return {reader.read_le<std::uint16_t>(), reader.read_le<std::uint16_t>(), reader.read_le<std::uint16_t>(),
          reader.read_le<std::uint16_t>()};
}

void require_schema(const OrderEntryMessageHeader& header) {
  if (header.schema_id != kOrderEntrySchema)
    throw std::runtime_error("unexpected SBE order entry schema");
}

void require_template(const OrderEntryMessageHeader& header, std::uint16_t template_id) {
  require_schema(header);
  if (header.template_id != template_id)
    throw std::runtime_error("unexpected SBE order entry template");
}

void require_block(const OrderEntryMessageHeader& header, std::uint16_t min_block_length) {
  if (header.block_length < min_block_length)
    throw std::runtime_error("unexpected SBE order entry block length");
}

ApiResponseHeader read_response_header(Reader& reader) {
  ApiResponseHeader header;
  header.req_id = reader.read_fixed_string(kString64);
  header.conn_id = reader.read_fixed_string(kString64);
  header.trace_id = reader.read_fixed_string(kString64);
  header.time_now = reader.read_le<std::int64_t>();
  header.in_time = reader.read_le<std::int64_t>();
  header.bapi_limit = reader.read_le<std::int64_t>();
  header.bapi_limit_status = reader.read_le<std::int64_t>();
  header.bapi_limit_reset_timestamp = reader.read_le<std::int64_t>();
  return header;
}

bool is_batch_response(std::uint16_t template_id) {
  return template_id == 12 || template_id == 14 || template_id == 16;
}

BatchOrderResponseItem read_batch_item(Reader& reader, std::uint16_t block_length) {
  if (block_length < kBatchResponseItemBlock)
    throw std::runtime_error("unexpected SBE batch item block length");
  const auto item_end = reader.pos() + block_length;
  BatchOrderResponseItem item;
  item.code = reader.read_le<std::int32_t>();
  item.category = static_cast<Category>(reader.read_le<std::uint8_t>());
  item.symbol_id = reader.read_le<std::int64_t>();
  item.order_id = reader.read_fixed_string(kString64);
  item.order_link_id = reader.read_fixed_string(kString64);
  reader.seek(item_end);
  item.msg = reader.read_var_string16();
  return item;
}

}  // namespace

OrderEntryMessageHeader decode_order_entry_header(std::string_view payload) {
  Reader reader(payload);
  return read_message_header(reader);
}

AuthResponse decode_auth_response(std::string_view payload) {
  Reader reader(payload);
  auto header = read_message_header(reader);
  require_template(header, 2);
  require_block(header, kAuthResponseBlock);

  AuthResponse response;
  response.header = header;
  response.req_id = reader.read_fixed_string(kString64);
  response.ret_code = reader.read_le<std::int32_t>();
  response.conn_id = reader.read_fixed_string(kString64);
  reader.seek(8 + header.block_length);
  response.ret_msg = reader.read_var_string16();
  return response;
}

PongResponse decode_pong_response(std::string_view payload) {
  Reader reader(payload);
  auto header = read_message_header(reader);
  require_template(header, 4);
  require_block(header, kPongResponseBlock);

  PongResponse response;
  response.header = header;
  response.timestamp = reader.read_le<std::uint64_t>();
  response.pong_time = reader.read_le<std::uint64_t>();
  return response;
}

OrderResponse decode_order_response(std::string_view payload) {
  Reader reader(payload);
  auto header = read_message_header(reader);
  require_schema(header);
  if (header.template_id != 6 && header.template_id != 8 && header.template_id != 10) {
    throw std::runtime_error("unexpected SBE order response template");
  }
  require_block(header, kOrderResponseBlock);

  OrderResponse response;
  response.header = header;
  response.response_header = read_response_header(reader);
  response.ret_code = reader.read_le<std::int32_t>();
  response.order_id = reader.read_fixed_string(kString64);
  response.order_link_id = reader.read_fixed_string(kString64);
  reader.seek(8 + header.block_length);
  response.ret_msg = reader.read_var_string16();
  return response;
}

BatchOrderResponse decode_batch_order_response(std::string_view payload) {
  Reader reader(payload);
  auto header = read_message_header(reader);
  require_schema(header);
  if (!is_batch_response(header.template_id))
    throw std::runtime_error("unexpected SBE batch response template");
  require_block(header, kBatchResponseBlock);

  BatchOrderResponse response;
  response.header = header;
  response.response_header = read_response_header(reader);
  response.ret_code = reader.read_le<std::int32_t>();
  reader.seek(8 + header.block_length);
  const auto item_block_length = reader.read_le<std::uint16_t>();
  const auto item_count = reader.read_le<std::uint16_t>();
  response.items.reserve(item_count);
  for (std::uint16_t i = 0; i < item_count; ++i) {
    response.items.push_back(read_batch_item(reader, item_block_length));
  }
  response.ret_msg = reader.read_var_string16();
  return response;
}

CommonErrorResponse decode_common_error_response(std::string_view payload) {
  Reader reader(payload);
  auto header = read_message_header(reader);
  require_template(header, kCommonErrorTemplate);
  require_block(header, kCommonErrorBlock);

  CommonErrorResponse response;
  response.header = header;
  response.response_header = read_response_header(reader);
  response.ret_code = reader.read_le<std::int32_t>();
  reader.seek(8 + header.block_length);
  response.ret_msg = reader.read_var_string16();
  return response;
}

}  // namespace bybit::sbe
