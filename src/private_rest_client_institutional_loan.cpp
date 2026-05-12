#include "bybit/http_client.hpp"
#include "bybit/rest_client.hpp"

namespace bybit {

std::string PrivateRestClient::get_institutional_loan_product_info(const QueryParams& filters,
                                                                   bool authenticated) {
  return http_.get("/v5/ins-loan/product-infos", filters, authenticated);
}

std::string PrivateRestClient::get_institutional_loan_margin_coin_info(const QueryParams& filters,
                                                                       bool authenticated) {
  return http_.get("/v5/ins-loan/ensure-tokens-convert", filters, authenticated);
}

std::string PrivateRestClient::get_institutional_loan_orders(const QueryParams& filters) {
  return http_.get("/v5/ins-loan/loan-order", filters, true);
}

std::string PrivateRestClient::get_institutional_loan_repayment_orders(const QueryParams& filters) {
  return http_.get("/v5/ins-loan/repaid-history", filters, true);
}

std::string PrivateRestClient::get_institutional_loan_ltv() {
  return http_.get("/v5/ins-loan/ltv-convert", {}, true);
}

std::string PrivateRestClient::bind_or_unbind_institutional_loan_uid(const std::string& json_body) {
  return http_.post("/v5/ins-loan/association-uid", json_body, true);
}

std::string PrivateRestClient::repay_institutional_loan(const std::string& json_body) {
  return http_.post("/v5/ins-loan/repay-loan", json_body, true);
}

}  // namespace bybit
