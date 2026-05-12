#include "bybit/http_client.hpp"
#include "bybit/rest_client.hpp"

namespace bybit {

std::string PrivateRestClient::create_rfq(const std::string& json_body) {
  return http_.post("/v5/rfq/create-rfq", json_body, true);
}

std::string PrivateRestClient::get_rfq_config() {
  return http_.get("/v5/rfq/config", {}, true);
}

std::string PrivateRestClient::cancel_rfq(const std::string& json_body) {
  return http_.post("/v5/rfq/cancel-rfq", json_body, true);
}

std::string PrivateRestClient::cancel_all_rfqs() {
  return http_.post("/v5/rfq/cancel-all-rfq", "{}", true);
}

std::string PrivateRestClient::accept_non_lp_quote(const std::string& json_body) {
  return http_.post("/v5/rfq/accept-other-quote", json_body, true);
}

std::string PrivateRestClient::create_rfq_quote(const std::string& json_body) {
  return http_.post("/v5/rfq/create-quote", json_body, true);
}

std::string PrivateRestClient::execute_rfq_quote(const std::string& json_body) {
  return http_.post("/v5/rfq/execute-quote", json_body, true);
}

std::string PrivateRestClient::cancel_rfq_quote(const std::string& json_body) {
  return http_.post("/v5/rfq/cancel-quote", json_body, true);
}

std::string PrivateRestClient::cancel_all_rfq_quotes() {
  return http_.post("/v5/rfq/cancel-all-quotes", "{}", true);
}

std::string PrivateRestClient::get_rfqs_realtime(const QueryParams& filters) {
  return http_.get("/v5/rfq/rfq-realtime", filters, true);
}

std::string PrivateRestClient::get_rfqs(const QueryParams& filters) {
  return http_.get("/v5/rfq/rfq-list", filters, true);
}

std::string PrivateRestClient::get_rfq_quotes_realtime(const QueryParams& filters) {
  return http_.get("/v5/rfq/quote-realtime", filters, true);
}

std::string PrivateRestClient::get_rfq_quotes(const QueryParams& filters) {
  return http_.get("/v5/rfq/quote-list", filters, true);
}

std::string PrivateRestClient::get_rfq_trade_history(const QueryParams& filters) {
  return http_.get("/v5/rfq/trade-list", filters, true);
}

std::string PrivateRestClient::get_rfq_public_trades(const QueryParams& filters) {
  return http_.get("/v5/rfq/public-trades", filters, true);
}

}  // namespace bybit
