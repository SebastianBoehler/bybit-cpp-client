#include <algorithm>
#include <optional>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "bybit/http_client.hpp"
#include "bybit/rest_client.hpp"

namespace bybit {
namespace {

bool has_param(const QueryParams& params, const std::string& key) {
  return std::any_of(params.begin(), params.end(), [&key](const auto& param) { return param.first == key; });
}

QueryParams with_category(const std::string& category, const QueryParams& params) {
  QueryParams with_category_params;
  if (!has_param(params, "category")) {
    with_category_params.emplace_back("category", category);
  }
  with_category_params.insert(with_category_params.end(), params.begin(), params.end());
  return with_category_params;
}

std::string json_string(const std::string& value) {
  std::ostringstream oss;
  oss << "\"";
  for (char c : value) {
    if (c == '"' || c == '\\') oss << "\\";
    oss << c;
  }
  oss << "\"";
  return oss.str();
}

std::string build_dcp_body(int time_window, const std::optional<std::string>& product) {
  if (time_window < 3 || time_window > 300) {
    throw std::invalid_argument("set_disconnect_cancel_all time_window must be between 3 and 300 seconds");
  }

  std::ostringstream oss;
  oss << "{\"timeWindow\":" << time_window;
  if (product) {
    oss << ",\"product\":" << json_string(*product);
  }
  oss << "}";
  return oss.str();
}

}  // namespace

std::string PrivateRestClient::pre_check_order(const QueryParams& order_params) {
  return http_.post("/v5/order/pre-check", to_json_object(with_category(category_, order_params)), true);
}

std::string PrivateRestClient::cancel_all_orders(const QueryParams& filters) {
  return http_.post("/v5/order/cancel-all", to_json_object(with_category(category_, filters)), true);
}

std::string PrivateRestClient::set_disconnect_cancel_all(int time_window, const std::optional<std::string>& product) {
  return http_.post("/v5/order/disconnected-cancel-all", build_dcp_body(time_window, product), true);
}

std::string PrivateRestClient::get_dcp_info() {
  return http_.get("/v5/account/query-dcp-info", {}, true);
}

}  // namespace bybit
