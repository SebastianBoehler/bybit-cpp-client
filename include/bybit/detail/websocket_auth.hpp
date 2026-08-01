#pragma once

#include <optional>
#include <string_view>

namespace bybit::detail {

std::optional<bool> parse_websocket_auth_result(std::string_view message);

}  // namespace bybit::detail
