#pragma once

#include <cstdint>
#include <string>
#include <type_traits>
#include <utility>
#include <variant>
#include <vector>

namespace bybit {

using QueryParams = std::vector<std::pair<std::string, std::string>>;

class JsonValue {
 public:
  using Storage = std::variant<std::string, bool, std::int64_t>;

  JsonValue(const char* value) : value_(std::string(value)) {}
  JsonValue(std::string value) : value_(std::move(value)) {}
  JsonValue(bool value) : value_(value) {}

  template <typename T, std::enable_if_t<std::is_integral_v<T> && !std::is_same_v<std::decay_t<T>, bool>, int> = 0>
  JsonValue(T value) : value_(static_cast<std::int64_t>(value)) {}

  const Storage& storage() const {
    return value_;
  }

 private:
  Storage value_;
};

using JsonObject = std::vector<std::pair<std::string, JsonValue>>;

struct MovePositionLeg {
  std::string category, symbol, price, side, qty;
};

struct CollateralCoinSwitch {
  std::string coin, collateral_switch;
};

}  // namespace bybit
