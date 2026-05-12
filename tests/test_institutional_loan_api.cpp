#include "bybit/rest_client.hpp"

int main() {
  using RestClient = bybit::RestClient;

  auto product_info = &RestClient::get_institutional_loan_product_info;
  auto margin_coin_info = &RestClient::get_institutional_loan_margin_coin_info;
  auto loan_orders = &RestClient::get_institutional_loan_orders;
  auto repayment_orders = &RestClient::get_institutional_loan_repayment_orders;
  auto ltv = &RestClient::get_institutional_loan_ltv;
  auto bind_uid = &RestClient::bind_or_unbind_institutional_loan_uid;
  auto repay = &RestClient::repay_institutional_loan;

  (void)product_info;
  (void)margin_coin_info;
  (void)loan_orders;
  (void)repayment_orders;
  (void)ltv;
  (void)bind_uid;
  (void)repay;

  return 0;
}
