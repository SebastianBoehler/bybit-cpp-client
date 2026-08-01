#pragma once

#include <string>
#include <vector>

#include "bybit/rest_client_types.hpp"

namespace bybit {

std::string serialize_json_string(const std::string& value);
std::string serialize_json_string_array(const std::vector<std::string>& values);
std::string serialize_json_object(const JsonObject& values);

}  // namespace bybit
