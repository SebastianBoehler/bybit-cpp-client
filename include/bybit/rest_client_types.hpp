#pragma once

#include <string>
#include <utility>
#include <vector>

namespace bybit {

using QueryParams = std::vector<std::pair<std::string, std::string>>;

struct MovePositionLeg {
  std::string category, symbol, price, side, qty;
};

struct CollateralCoinSwitch {
  std::string coin, collateral_switch;
};

}  // namespace bybit
