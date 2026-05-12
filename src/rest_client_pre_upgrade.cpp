#include "bybit/rest_client.hpp"

namespace bybit {

std::string RestClient::get_pre_upgrade_order_history(const QueryParams& filters) {
  return private_.get_pre_upgrade_order_history(filters);
}

std::string RestClient::get_pre_upgrade_trade_history(const QueryParams& filters) {
  return private_.get_pre_upgrade_trade_history(filters);
}

std::string RestClient::get_pre_upgrade_closed_pnl(const QueryParams& filters) {
  return private_.get_pre_upgrade_closed_pnl(filters);
}

std::string RestClient::get_pre_upgrade_transaction_log(const QueryParams& filters) {
  return private_.get_pre_upgrade_transaction_log(filters);
}

std::string RestClient::get_pre_upgrade_delivery_records(const QueryParams& filters) {
  return private_.get_pre_upgrade_delivery_records(filters);
}

std::string RestClient::get_pre_upgrade_settlement_records(const QueryParams& filters) {
  return private_.get_pre_upgrade_settlement_records(filters);
}

}  // namespace bybit
