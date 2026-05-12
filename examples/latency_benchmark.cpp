#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <functional>
#include <iomanip>
#include <iostream>
#include <string>

#include "bybit/rest_client.hpp"

namespace {

int env_int(const char* name, int fallback) {
  const char* value = std::getenv(name);
  if (!value)
    return fallback;
  try {
    return std::max(1, std::stoi(value));
  } catch (...) {
    return fallback;
  }
}

double timed_ms(const std::function<void()>& fn) {
  auto start = std::chrono::steady_clock::now();
  fn();
  auto end = std::chrono::steady_clock::now();
  return std::chrono::duration<double, std::milli>(end - start).count();
}

}  // namespace

int main() {
  const int iterations = env_int("BYBIT_BENCH_ITERATIONS", 5);
  const char* base_url_env = std::getenv("BYBIT_BASE_URL");
  const std::string base_url = base_url_env ? base_url_env : "https://api.bybit.com";

  double cold_total = 0.0;
  double warm_total = 0.0;
  bybit::RestClient warm_client{"", "", "linear", base_url, "5000"};

  try {
    for (int i = 0; i < iterations; ++i) {
      cold_total += timed_ms([&] {
        bybit::RestClient client{"", "", "linear", base_url, "5000"};
        (void)client.get_server_time();
      });
      warm_total += timed_ms([&] { (void)warm_client.get_server_time(); });
    }
  } catch (const std::exception& ex) {
    std::cerr << "Benchmark failed: " << ex.what() << "\n";
    return 1;
  }

  std::cout << std::fixed << std::setprecision(2);
  std::cout << "iterations: " << iterations << "\n";
  std::cout << "cold avg ms: " << cold_total / iterations << "\n";
  std::cout << "warm avg ms: " << warm_total / iterations << "\n";
  return 0;
}
