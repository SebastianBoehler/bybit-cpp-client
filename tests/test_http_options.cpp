#include <iostream>
#include <stdexcept>
#include <string>

#include "bybit/http_client.hpp"
#include "bybit/rest_client.hpp"

int main() {
  bybit::HttpOptions options;
  options.connect_timeout_ms = 1500;
  options.request_timeout_ms = 2500;
  options.dns_cache_timeout_seconds = 120;
  options.max_connections = 16;
  options.share_dns_and_ssl_session_cache = false;
  options.tcp_keepalive = false;
  options.tcp_nodelay = false;
  options.proxy = "http://127.0.0.1:9";
  options.user_agent = "bybit-cpp-client-test";

  bybit::HttpClient http{"key", "secret", "https://api.bybit.com", "5000", options};
  if (http.options().connect_timeout_ms != 1500 || http.options().request_timeout_ms != 2500 ||
      http.options().dns_cache_timeout_seconds != 120 || http.options().max_connections != 16 ||
      http.options().share_dns_and_ssl_session_cache ||
      http.options().tcp_keepalive || http.options().tcp_nodelay ||
      http.options().proxy != "http://127.0.0.1:9" || http.options().user_agent != "bybit-cpp-client-test") {
    std::cerr << "HttpOptions were not retained by HttpClient\n";
    return 1;
  }
  bybit::HttpError error{429, "{\"retCode\":10006,\"retMsg\":\"Too many visits!\"}"};
  if (error.status_code() != 429 || error.body().find("10006") == std::string::npos ||
      !error.ret_code() || *error.ret_code() != 10006 || !error.ret_msg() || *error.ret_msg() != "Too many visits!" ||
      std::string(error.what()).find("HTTP status 429") == std::string::npos) {
    std::cerr << "HttpError did not expose status/body\n";
    return 1;
  }
  bybit::HttpError html_error{502, "<html>bad gateway</html>"};
  if (html_error.ret_code() || html_error.ret_msg()) {
    std::cerr << "HttpError parsed non-JSON Bybit fields\n";
    return 1;
  }

  bybit::RestClient client{"key", "secret", "linear", options};
  try {
    client.move_positions("from", "to", {});
    std::cerr << "move_positions accepted an empty leg list\n";
    return 1;
  } catch (const std::invalid_argument&) {
  }
  try {
    client.set_disconnect_cancel_all(2);
    std::cerr << "set_disconnect_cancel_all accepted a too-small time window\n";
    return 1;
  } catch (const std::invalid_argument&) {
  }
  try {
    client.set_auto_add_margin("BTCUSDT", 2);
    std::cerr << "set_auto_add_margin accepted an invalid flag\n";
    return 1;
  } catch (const std::invalid_argument&) {
  }
  return 0;
}
