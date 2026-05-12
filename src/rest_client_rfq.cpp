#include "bybit/rest_client.hpp"

namespace bybit {

std::string RestClient::create_rfq(const std::string& json_body) {
  return private_.create_rfq(json_body);
}

std::string RestClient::get_rfq_config() {
  return private_.get_rfq_config();
}

std::string RestClient::cancel_rfq(const std::string& json_body) {
  return private_.cancel_rfq(json_body);
}

std::string RestClient::cancel_all_rfqs() {
  return private_.cancel_all_rfqs();
}

std::string RestClient::accept_non_lp_quote(const std::string& json_body) {
  return private_.accept_non_lp_quote(json_body);
}

std::string RestClient::create_rfq_quote(const std::string& json_body) {
  return private_.create_rfq_quote(json_body);
}

std::string RestClient::execute_rfq_quote(const std::string& json_body) {
  return private_.execute_rfq_quote(json_body);
}

std::string RestClient::cancel_rfq_quote(const std::string& json_body) {
  return private_.cancel_rfq_quote(json_body);
}

std::string RestClient::cancel_all_rfq_quotes() {
  return private_.cancel_all_rfq_quotes();
}

std::string RestClient::get_rfqs_realtime(const QueryParams& filters) {
  return private_.get_rfqs_realtime(filters);
}

std::string RestClient::get_rfqs(const QueryParams& filters) {
  return private_.get_rfqs(filters);
}

std::string RestClient::get_rfq_quotes_realtime(const QueryParams& filters) {
  return private_.get_rfq_quotes_realtime(filters);
}

std::string RestClient::get_rfq_quotes(const QueryParams& filters) {
  return private_.get_rfq_quotes(filters);
}

std::string RestClient::get_rfq_trade_history(const QueryParams& filters) {
  return private_.get_rfq_trade_history(filters);
}

std::string RestClient::get_rfq_public_trades(const QueryParams& filters) {
  return private_.get_rfq_public_trades(filters);
}

}  // namespace bybit
