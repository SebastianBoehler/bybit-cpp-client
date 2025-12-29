#include <chrono>
#include <cstdlib>
#include <iostream>
#include <string>
#include <thread>

#include "bybit/rest_client.hpp"

std::string getenv_or(const char* key, const std::string& fallback) {
  if (const char* v = std::getenv(key)) return std::string(v);
  return fallback;
}

int main() {
  const std::string api_key = getenv_or("BYBIT_API_KEY", "");
  const std::string api_secret = getenv_or("BYBIT_API_SECRET", "");
  if (api_key.empty() || api_secret.empty()) {
    std::cerr << "[limit-order] BYBIT_API_KEY/BYBIT_API_SECRET required\n";
    return 1;
  }

  const std::string category = getenv_or("BYBIT_CATEGORY", "linear");
  const std::string base_url = getenv_or("BYBIT_BASE_URL", "https://api.bybit.com");
  const std::string symbol = getenv_or("BYBIT_SYMBOL", "BTCUSDT");
  const std::string price = getenv_or("BYBIT_PRICE", "10000");
  const std::string qty = getenv_or("BYBIT_QTY", "0.001");
  const std::string side = getenv_or("BYBIT_SIDE", "Buy");
  const std::string tif = getenv_or("BYBIT_TIF", "GTC");
  const int position_idx = std::atoi(getenv_or("BYBIT_POSITION_IDX", "1").c_str());

  std::string order_link_id = "bybit-cpp-" + std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>(
                                                                std::chrono::system_clock::now().time_since_epoch())
                                                                .count());

  bybit::RestClient client(api_key, api_secret, category, base_url);

  std::cerr << "[limit-order] placing limit order symbol=" << symbol << " side=" << side << " qty=" << qty
            << " price=" << price << " tif=" << tif << " pos_idx=" << position_idx << " link_id=" << order_link_id
            << "\n";
  auto resp = client.submit_order(symbol, side, "Limit", qty, order_link_id, position_idx, price, tif);
  std::cout << "[limit-order] submit response: " << resp << std::endl;

  // Optional cancel: cancel all open orders for the symbol (simple demo).
  std::cerr << "[limit-order] cancel_all for symbol=" << symbol << "\n";
  auto cancel_resp = client.cancel_all(symbol);
  std::cout << "[limit-order] cancel_all response: " << cancel_resp << std::endl;

  return 0;
}
