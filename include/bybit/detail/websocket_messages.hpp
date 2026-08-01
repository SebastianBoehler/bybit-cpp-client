#pragma once

#include <string>
#include <vector>

namespace bybit::detail {

std::string make_subscription_message(const std::string& operation, const std::vector<std::string>& topics,
                                      const std::string& request_id);
std::string make_ping_message(const std::string& request_id);
std::string make_auth_message(const std::string& api_key, const std::string& expires_ms, const std::string& signature);

}  // namespace bybit::detail
