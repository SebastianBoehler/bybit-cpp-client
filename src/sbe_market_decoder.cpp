#include "bybit/sbe_market_decoder.hpp"

#include <cstring>
#include <stdexcept>
#include <type_traits>

namespace bybit::sbe {
namespace {

constexpr std::uint16_t kBestObRpiTemplate = 20000;
constexpr std::uint16_t kOrderbookL50Template = 20001;
constexpr std::uint16_t kPublicTradeTemplate = 20002;
constexpr std::size_t kHeaderBytes = 8;
constexpr std::size_t kBestObRpiKnownBlock = 98;
constexpr std::size_t kOrderbookL50KnownBlock = 35;
constexpr std::size_t kPublicTradeKnownBlock = 10;
constexpr std::size_t kBookLevelKnownBlock = 16;
constexpr std::size_t kTradeItemKnownBlock = 35;

class Reader {
 public:
  explicit Reader(std::string_view data) : data_(data) {}

  template <typename T>
  T read() {
    static_assert(std::is_integral_v<T>);
    if (pos_ + sizeof(T) > data_.size())
      throw std::runtime_error("truncated SBE payload");
    using U = std::make_unsigned_t<T>;
    U value = 0;
    for (std::size_t i = 0; i < sizeof(T); ++i) {
      value |= static_cast<U>(static_cast<unsigned char>(data_[pos_ + i])) << (i * 8);
    }
    pos_ += sizeof(T);
    if constexpr (std::is_signed_v<T>) {
      T signed_value{};
      std::memcpy(&signed_value, &value, sizeof(T));
      return signed_value;
    } else {
      return value;
    }
  }

  std::string read_var_string8() {
    const auto length = read<std::uint8_t>();
    if (pos_ + length > data_.size())
      throw std::runtime_error("truncated SBE varString8");
    std::string value{data_.substr(pos_, length)};
    pos_ += length;
    return value;
  }

  void skip_to(std::size_t pos) {
    if (pos > data_.size())
      throw std::runtime_error("truncated SBE block");
    pos_ = pos;
  }

  std::size_t pos() const {
    return pos_;
  }

 private:
  std::string_view data_;
  std::size_t pos_{0};
};

MessageHeader read_header(Reader& reader) {
  MessageHeader header;
  header.block_length = reader.read<std::uint16_t>();
  header.template_id = reader.read<std::uint16_t>();
  header.schema_id = reader.read<std::uint16_t>();
  header.version = reader.read<std::uint16_t>();
  return header;
}

void require_block(const MessageHeader& header, std::size_t bytes) {
  if (header.block_length < bytes)
    throw std::runtime_error("unexpected SBE block length");
}

std::vector<BookLevel> read_book_levels(Reader& reader) {
  const auto block_length = reader.read<std::uint16_t>();
  const auto count = reader.read<std::uint16_t>();
  if (block_length < kBookLevelKnownBlock)
    throw std::runtime_error("unexpected SBE book level block length");
  std::vector<BookLevel> levels;
  levels.reserve(count);
  for (std::uint16_t i = 0; i < count; ++i) {
    const auto entry_start = reader.pos();
    BookLevel level;
    level.price = reader.read<std::int64_t>();
    level.size = reader.read<std::int64_t>();
    reader.skip_to(entry_start + block_length);
    levels.push_back(level);
  }
  return levels;
}

BestObRpiEvent read_best_ob_rpi(Reader& reader, const MessageHeader& header) {
  require_block(header, kBestObRpiKnownBlock);
  BestObRpiEvent event;
  event.header = header;
  event.ts = reader.read<std::int64_t>();
  event.seq = reader.read<std::int64_t>();
  event.cts = reader.read<std::int64_t>();
  event.update_id = reader.read<std::int64_t>();
  event.ask_normal_price = reader.read<std::int64_t>();
  event.ask_normal_size = reader.read<std::int64_t>();
  event.ask_rpi_price = reader.read<std::int64_t>();
  event.ask_rpi_size = reader.read<std::int64_t>();
  event.bid_normal_price = reader.read<std::int64_t>();
  event.bid_normal_size = reader.read<std::int64_t>();
  event.bid_rpi_price = reader.read<std::int64_t>();
  event.bid_rpi_size = reader.read<std::int64_t>();
  event.price_exponent = reader.read<std::int8_t>();
  event.size_exponent = reader.read<std::int8_t>();
  reader.skip_to(kHeaderBytes + header.block_length);
  event.symbol = reader.read_var_string8();
  return event;
}

OrderbookL50Event read_orderbook_l50(Reader& reader, const MessageHeader& header) {
  require_block(header, kOrderbookL50KnownBlock);
  OrderbookL50Event event;
  event.header = header;
  event.ts = reader.read<std::int64_t>();
  event.seq = reader.read<std::int64_t>();
  event.cts = reader.read<std::int64_t>();
  event.update_id = reader.read<std::int64_t>();
  event.price_exponent = reader.read<std::int8_t>();
  event.size_exponent = reader.read<std::int8_t>();
  event.package_type = reader.read<std::uint8_t>();
  reader.skip_to(kHeaderBytes + header.block_length);
  event.asks = read_book_levels(reader);
  event.bids = read_book_levels(reader);
  event.symbol = reader.read_var_string8();
  return event;
}

PublicTradeEvent read_public_trade(Reader& reader, const MessageHeader& header) {
  require_block(header, kPublicTradeKnownBlock);
  PublicTradeEvent event;
  event.header = header;
  event.ts = reader.read<std::int64_t>();
  event.price_exponent = reader.read<std::int8_t>();
  event.size_exponent = reader.read<std::int8_t>();
  reader.skip_to(kHeaderBytes + header.block_length);
  const auto block_length = reader.read<std::uint16_t>();
  const auto count = reader.read<std::uint16_t>();
  if (block_length < kTradeItemKnownBlock)
    throw std::runtime_error("unexpected SBE trade item block length");
  event.trade_items.reserve(count);
  for (std::uint16_t i = 0; i < count; ++i) {
    const auto entry_start = reader.pos();
    TradeItem item;
    item.fill_time = reader.read<std::int64_t>();
    item.price = reader.read<std::int64_t>();
    item.size = reader.read<std::int64_t>();
    item.seq = reader.read<std::int64_t>();
    item.side = reader.read<std::uint8_t>();
    item.is_block_trade = reader.read<std::uint8_t>() == 1;
    item.is_rpi = reader.read<std::uint8_t>() == 1;
    reader.skip_to(entry_start + block_length);
    item.exec_id = reader.read_var_string8();
    event.trade_items.push_back(item);
  }
  event.symbol = reader.read_var_string8();
  return event;
}

}  // namespace

MessageHeader decode_message_header(std::string_view payload) {
  Reader reader(payload);
  return read_header(reader);
}

MarketMessage decode_market_message(std::string_view payload) {
  Reader reader(payload);
  const auto header = read_header(reader);
  switch (header.template_id) {
    case kBestObRpiTemplate:
      return read_best_ob_rpi(reader, header);
    case kOrderbookL50Template:
      return read_orderbook_l50(reader, header);
    case kPublicTradeTemplate:
      return read_public_trade(reader, header);
    default:
      throw std::runtime_error("unsupported SBE market template");
  }
}

}  // namespace bybit::sbe
