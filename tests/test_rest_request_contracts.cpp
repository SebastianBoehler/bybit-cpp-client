#include <iostream>
#include <optional>
#include <string>

#include "bybit/rest_client.hpp"
#include "bybit/signing.hpp"
#include "http_capture_server.hpp"

namespace {

int failures = 0;

void expect_equal(const std::string& actual, const std::string& expected, const std::string& context) {
  if (actual == expected)
    return;
  std::cerr << context << "\nexpected: " << expected << "\nactual:   " << actual << "\n";
  ++failures;
}

void expect_contains(const std::string& actual, const std::string& expected, const std::string& context) {
  if (actual.find(expected) != std::string::npos)
    return;
  std::cerr << context << "\nmissing: " << expected << "\nactual:  " << actual << "\n";
  ++failures;
}

void expect_not_contains(const std::string& actual, const std::string& unexpected, const std::string& context) {
  if (actual.find(unexpected) == std::string::npos)
    return;
  std::cerr << context << "\nunexpected: " << unexpected << "\nactual:     " << actual << "\n";
  ++failures;
}

std::string header_value(const std::string& request, const std::string& name) {
  const auto start = request.find(name + ": ");
  if (start == std::string::npos)
    return {};
  const auto value_start = start + name.size() + 2;
  const auto end = request.find("\r\n", value_start);
  return request.substr(value_start, end - value_start);
}

std::string body(const std::string& request) {
  const auto start = request.find("\r\n\r\n");
  return start == std::string::npos ? std::string{} : request.substr(start + 4);
}

template <typename Invoke>
std::string capture(Invoke invoke) {
  bybit::test::HttpCaptureServer server;
  bybit::RestClient client{"key", "secret", "linear", server.base_url(), "5000"};
  invoke(client);
  return server.take_request();
}

}  // namespace

int main() {
  const auto position_request =
      capture([](bybit::RestClient& client) { client.switch_position_mode(3, std::string{"BTCUSDT"}); });
  expect_contains(position_request, "POST /v5/position/switch-mode HTTP/1.1", "position mode path drifted");
  expect_equal(body(position_request), R"({"category":"linear","symbol":"BTCUSDT","mode":3})",
               "position mode body must use symbol/coin and an integer mode");

  const auto coin_position_request =
      capture([](bybit::RestClient& client) { client.switch_position_mode(0, std::nullopt, std::string{"USDT"}); });
  expect_equal(body(coin_position_request), R"({"category":"linear","coin":"USDT","mode":0})",
               "coin-scoped position mode body drifted");

  try {
    bybit::RestClient client{"key", "secret", "linear", "http://127.0.0.1:1", "5000"};
    client.switch_position_mode(3);
    std::cerr << "position mode accepted a request without symbol or coin\n";
    ++failures;
  } catch (const std::invalid_argument&) {
  }

  const auto margin_request = capture([](bybit::RestClient& client) { client.switch_margin_mode("PORTFOLIO_MARGIN"); });
  expect_contains(margin_request, "POST /v5/account/set-margin-mode HTTP/1.1", "margin mode path drifted");
  expect_equal(body(margin_request), R"({"setMarginMode":"PORTFOLIO_MARGIN"})",
               "margin mode must use the account-level setMarginMode shape");

  const auto order_request = capture([](bybit::RestClient& client) {
    client.submit_order("BTCUSDT", "Buy", "Limit", "1", "link-1", 0, "25000", "GTC", false, std::nullopt, std::nullopt,
                        true);
  });
  expect_equal(
      body(order_request),
      R"({"category":"linear","symbol":"BTCUSDT","side":"Buy","orderType":"Limit","qty":"1","orderLinkId":"link-1","positionIdx":0,"price":"25000","timeInForce":"GTC","reduceOnly":false,"rpiTakerAccess":true})",
      "typed order scalars or rpiTakerAccess drifted");

  const auto batch_request = capture([](bybit::RestClient& client) {
    bybit::JsonObject order{{"symbol", "BTCUSDT"},  {"side", "Buy"},
                            {"orderType", "Limit"}, {"qty", "1"},
                            {"positionIdx", 0},     {"reduceOnly", false},
                            {"isLeverage", 1},      {"note", std::string{"line\b\f\n\r\t\x01"}}};
    client.batch_submit_orders({order});
  });
  expect_equal(
      body(batch_request),
      R"({"category":"linear","request":[{"symbol":"BTCUSDT","side":"Buy","orderType":"Limit","qty":"1","positionIdx":0,"reduceOnly":false,"isLeverage":1,"note":"line\b\f\n\r\t\u0001"}]})",
      "batch order scalars or JSON string escaping drifted");

  const auto transaction_request =
      capture([](bybit::RestClient& client) { client.get_transaction_log(25, std::string{"cursor-1"}); });
  expect_contains(transaction_request,
                  "GET /v5/account/transaction-log?accountType=UNIFIED&category=linear&cursor=cursor-1&limit=25 "
                  "HTTP/1.1",
                  "transaction log accountType/category query drifted");
  expect_not_contains(transaction_request, "recvWindow", "recvWindow leaked into a private GET query");
  expect_equal(header_value(transaction_request, "X-BAPI-RECV-WINDOW"), "5000",
               "private GET omitted the recv-window header");
  const auto timestamp = header_value(transaction_request, "X-BAPI-TIMESTAMP");
  const auto expected_signature = bybit::Signer::sign_with_timestamp(
      "key", "secret", "accountType=UNIFIED&category=linear&cursor=cursor-1&limit=25", timestamp, "5000");
  expect_equal(header_value(transaction_request, "X-BAPI-SIGN"), expected_signature.signature,
               "private GET signed a payload other than the endpoint query");

  const auto fee_request = capture([](bybit::RestClient& client) { client.get_fee_rate(std::string{"BTCUSDT"}); });
  expect_contains(fee_request, "GET /v5/account/fee-rate?category=linear&symbol=BTCUSDT HTTP/1.1",
                  "fee rate did not target the configured symbol");
  expect_not_contains(fee_request, "recvWindow", "recvWindow leaked into the fee-rate query");

  const auto post_query_request = capture([](bybit::RestClient& client) { client.get_card_point_balance(); });
  expect_contains(post_query_request, "POST /v5/card/reward/points/balance HTTP/1.1",
                  "private query POST path drifted");
  expect_not_contains(post_query_request, "recvWindow", "recvWindow leaked into a private POST query");

  const auto leverage_request = capture([](bybit::RestClient& client) { client.set_leverage("BTCUSDT", "2", "3"); });
  expect_equal(body(leverage_request),
               R"({"category":"linear","symbol":"BTCUSDT","buyLeverage":"2","sellLeverage":"3"})",
               "set leverage body contains undocumented fields");

  const auto cancel_request = capture([](bybit::RestClient& client) { client.cancel_order("BTCUSDT", "order-1"); });
  expect_equal(body(cancel_request), R"({"category":"linear","symbol":"BTCUSDT","orderId":"order-1"})",
               "cancel order body contains undocumented fields");

  const auto amend_request = capture([](bybit::RestClient& client) {
    client.amend_order("BTCUSDT", "order-1", std::string{"2"}, std::string{"26000"});
  });
  expect_equal(body(amend_request),
               R"({"category":"linear","symbol":"BTCUSDT","orderId":"order-1","qty":"2","price":"26000"})",
               "amend order body contains undocumented fields");

  const auto stop_request = capture([](bybit::RestClient& client) {
    client.set_trading_stop("BTCUSDT", 2, std::string{"27000"}, std::string{"24000"}, std::string{"500"});
  });
  expect_equal(
      body(stop_request),
      R"({"category":"linear","symbol":"BTCUSDT","positionIdx":2,"takeProfit":"27000","stopLoss":"24000","trailingStop":"500"})",
      "trading stop body contains undocumented or stringified fields");

  const auto risk_request = capture([](bybit::RestClient& client) { client.set_risk_limit("BTCUSDT", 7, 2); });
  expect_equal(body(risk_request), R"({"category":"linear","symbol":"BTCUSDT","riskId":7,"positionIdx":2})",
               "risk limit body contains undocumented or stringified fields");

  const auto margin_request_body = capture([](bybit::RestClient& client) { client.add_margin("BTCUSDT", "25", 2); });
  expect_equal(body(margin_request_body), R"({"category":"linear","symbol":"BTCUSDT","margin":"25","positionIdx":2})",
               "add margin body contains undocumented or stringified fields");

  const auto auto_margin_request =
      capture([](bybit::RestClient& client) { client.set_auto_add_margin("BTCUSDT", 1, 2); });
  expect_equal(body(auto_margin_request),
               R"({"category":"linear","symbol":"BTCUSDT","autoAddMargin":1,"positionIdx":2})",
               "auto-add-margin body stringified integer fields");

  return failures == 0 ? 0 : 1;
}
