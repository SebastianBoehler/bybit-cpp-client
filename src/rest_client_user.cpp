#include "bybit/rest_client.hpp"

#include <optional>
#include <string>

namespace bybit {

std::string RestClient::get_sub_uid_list() {
  return private_.get_sub_uid_list();
}

std::string RestClient::create_sub_uid(const std::string& json_body) {
  return private_.create_sub_uid(json_body);
}

std::string RestClient::delete_sub_uid(const std::string& sub_member_id) {
  return private_.delete_sub_uid(sub_member_id);
}

std::string RestClient::freeze_sub_uid(int subuid, int frozen) {
  return private_.freeze_sub_uid(subuid, frozen);
}

std::string RestClient::create_sub_api_key(const std::string& json_body) {
  return private_.create_sub_api_key(json_body);
}

std::string RestClient::get_sub_api_keys(const QueryParams& filters) {
  return private_.get_sub_api_keys(filters);
}

std::string RestClient::update_master_api_key(const std::string& json_body) {
  return private_.update_master_api_key(json_body);
}

std::string RestClient::delete_master_api_key() {
  return private_.delete_master_api_key();
}

std::string RestClient::update_sub_api_key(const std::string& json_body) {
  return private_.update_sub_api_key(json_body);
}

std::string RestClient::delete_sub_api_key(const std::optional<std::string>& api_key) {
  return private_.delete_sub_api_key(api_key);
}

std::string RestClient::get_uid_wallet_type(const std::optional<std::string>& member_ids) {
  return private_.get_uid_wallet_type(member_ids);
}

std::string RestClient::get_affiliate_user_info(const QueryParams& filters) {
  return private_.get_affiliate_user_info(filters);
}

}  // namespace bybit
