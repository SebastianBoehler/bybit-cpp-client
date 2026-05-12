#include "bybit/rest_client.hpp"

int main() {
  using RestClient = bybit::RestClient;

  auto product_info = &RestClient::get_advanced_earn_product_info;
  auto product_quote = &RestClient::get_advanced_earn_product_quote;
  auto place_order = &RestClient::place_advanced_earn_order;
  auto position = &RestClient::get_advanced_earn_position;
  auto order_history = &RestClient::get_advanced_earn_order_history;
  auto lm_product_info = &RestClient::get_advanced_earn_liquidity_mining_product_info;
  auto lm_add_liquidity = &RestClient::add_advanced_earn_liquidity_mining_liquidity;
  auto lm_remove_liquidity = &RestClient::remove_advanced_earn_liquidity_mining_liquidity;
  auto lm_reinvest = &RestClient::reinvest_advanced_earn_liquidity_mining;
  auto lm_add_margin = &RestClient::add_advanced_earn_liquidity_mining_margin;
  auto lm_claim_interest = &RestClient::claim_advanced_earn_liquidity_mining_interest;
  auto lm_position = &RestClient::get_advanced_earn_liquidity_mining_position;
  auto lm_order_history = &RestClient::get_advanced_earn_liquidity_mining_order_history;
  auto lm_yield_records = &RestClient::get_advanced_earn_liquidity_mining_yield_records;
  auto lm_liquidation_records = &RestClient::get_advanced_earn_liquidity_mining_liquidation_records;
  (void)product_info;
  (void)product_quote;
  (void)place_order;
  (void)position;
  (void)order_history;
  (void)lm_product_info;
  (void)lm_add_liquidity;
  (void)lm_remove_liquidity;
  (void)lm_reinvest;
  (void)lm_add_margin;
  (void)lm_claim_interest;
  (void)lm_position;
  (void)lm_order_history;
  (void)lm_yield_records;
  (void)lm_liquidation_records;

  return 0;
}
