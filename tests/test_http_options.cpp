#include <iostream>

#include "bybit/http_client.hpp"
#include "bybit/rest_client.hpp"

int main() {
  bybit::HttpOptions options;
  options.connect_timeout_ms = 1500;
  options.request_timeout_ms = 2500;
  options.dns_cache_timeout_seconds = 120;
  options.tcp_keepalive = false;
  options.proxy = "http://127.0.0.1:9";
  options.user_agent = "bybit-cpp-client-test";

  bybit::HttpClient http{"key", "secret", "https://api.bybit.com", "5000", options};
  if (http.options().connect_timeout_ms != 1500 || http.options().request_timeout_ms != 2500 ||
      http.options().dns_cache_timeout_seconds != 120 || http.options().tcp_keepalive ||
      http.options().proxy != "http://127.0.0.1:9" || http.options().user_agent != "bybit-cpp-client-test") {
    std::cerr << "HttpOptions were not retained by HttpClient\n";
    return 1;
  }

  bybit::RestClient client{"key", "secret", "linear", options};
  (void)client;
  return 0;
}
