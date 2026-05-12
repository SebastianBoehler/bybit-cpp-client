#include <optional>
#include <sstream>
#include <string>

#include "bybit/http_client.hpp"
#include "bybit/rest_client.hpp"

namespace bybit {
namespace {

std::string json_string(const std::string& value) {
  std::ostringstream oss;
  oss << "\"";
  for (char c : value) {
    if (c == '"' || c == '\\')
      oss << "\\";
    oss << c;
  }
  oss << "\"";
  return oss.str();
}

}  // namespace

std::string PrivateRestClient::get_sub_uid_list() {
  return http_.get("/v5/user/query-sub-members", {}, true);
}

std::string PrivateRestClient::get_sub_uid_list_unlimited(const QueryParams& filters) {
  return http_.get("/v5/user/submembers", filters, true);
}

std::string PrivateRestClient::get_fund_custodial_sub_accounts(const QueryParams& filters) {
  return http_.get("/v5/user/escrow_sub_members", filters, true);
}

std::string PrivateRestClient::sign_agreement(const std::string& json_body) {
  return http_.post("/v5/user/agreement", json_body, true);
}

std::string PrivateRestClient::create_demo_account() {
  return http_.post("/v5/user/create-demo-member", "{}", true);
}

std::string PrivateRestClient::create_sub_uid(const std::string& json_body) {
  return http_.post("/v5/user/create-sub-member", json_body, true);
}

std::string PrivateRestClient::delete_sub_uid(const std::string& sub_member_id) {
  return http_.post("/v5/user/del-submember", to_json_object({{"subMemberId", sub_member_id}}), true);
}

std::string PrivateRestClient::freeze_sub_uid(int subuid, int frozen) {
  std::ostringstream body;
  body << "{\"subuid\":" << subuid << ",\"frozen\":" << frozen << "}";
  return http_.post("/v5/user/frozen-sub-member", body.str(), true);
}

std::string PrivateRestClient::create_sub_api_key(const std::string& json_body) {
  return http_.post("/v5/user/create-sub-api", json_body, true);
}

std::string PrivateRestClient::get_sub_api_keys(const QueryParams& filters) {
  return http_.get("/v5/user/sub-apikeys", filters, true);
}

std::string PrivateRestClient::update_master_api_key(const std::string& json_body) {
  return http_.post("/v5/user/update-api", json_body, true);
}

std::string PrivateRestClient::delete_master_api_key() {
  return http_.post("/v5/user/delete-api", "{}", true);
}

std::string PrivateRestClient::update_sub_api_key(const std::string& json_body) {
  return http_.post("/v5/user/update-sub-api", json_body, true);
}

std::string PrivateRestClient::delete_sub_api_key(const std::optional<std::string>& api_key) {
  std::string body = api_key ? "{\"apikey\":" + json_string(*api_key) + "}" : "{}";
  return http_.post("/v5/user/delete-sub-api", body, true);
}

std::string PrivateRestClient::get_uid_wallet_type(const std::optional<std::string>& member_ids) {
  QueryParams params;
  if (member_ids)
    params.emplace_back("memberIds", *member_ids);
  return http_.get("/v5/user/get-member-type", params, true);
}

std::string PrivateRestClient::get_friend_referrals(const QueryParams& filters) {
  return http_.get("/v5/user/invitation/referrals", filters, true);
}

std::string PrivateRestClient::get_affiliate_user_list(const QueryParams& filters) {
  return http_.get("/v5/affiliate/aff-user-list", filters, true);
}

std::string PrivateRestClient::get_affiliate_user_info(const QueryParams& filters) {
  return http_.get("/v5/user/aff-customer-info", filters, true);
}

}  // namespace bybit
