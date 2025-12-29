#include <cstdlib>
#include <iostream>
#include <optional>
#include <string>

#include "bybit/rest_client.hpp"

std::string getenv_or(const char* key, const std::string& fallback) {
  if (const char* v = std::getenv(key)) return std::string(v);
  return fallback;
}

int main() {
  const std::string api_key = getenv_or("BYBIT_API_KEY", "");
  const std::string api_secret = getenv_or("BYBIT_API_SECRET", "");
  if (api_key.empty() || api_secret.empty()) {
    std::cerr << "[position-info] BYBIT_API_KEY/BYBIT_API_SECRET required\n";
    return 1;
  }

  const std::string category = getenv_or("BYBIT_CATEGORY", "linear");
  const std::string base_url = getenv_or("BYBIT_BASE_URL", "https://api.bybit.com");
  const std::string symbol = getenv_or("BYBIT_SYMBOL", "BTCUSDT");
  const std::string settle = getenv_or("BYBIT_SETTLE_COIN", "USDT");

  bybit::RestClient client(api_key, api_secret, category, base_url);

  std::cerr << "[position-info] category=" << category << " symbol=" << symbol << " settleCoin=" << settle << "\n";
  // PrivateRestClient supports settleCoin filter (not symbol). For linear USDT perps, settleCoin=USDT.
  auto resp = client.get_position_info(settle);
  std::cout << resp << std::endl;
  return 0;
}
