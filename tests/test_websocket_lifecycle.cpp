#include <chrono>
#include <iostream>

#include "bybit/detail/websocket_auth.hpp"
#include "bybit/detail/websocket_lifecycle.hpp"
#include "bybit/detail/websocket_transport.hpp"
#include "bybit/websocket_client.hpp"

namespace {

struct FakeTransport {
  void stop() {
    calls += "stop;";
  }
  void start() {
    calls += "start;";
  }
  std::string calls;
};

}  // namespace

int main() {
  using bybit::detail::AuthAction;
  using bybit::detail::OpenAction;

  int first_owner = 1;
  int second_owner = 2;
  bybit::detail::WebSocketCallbackOwner<int> callback_generation{&first_owner};
  const auto first_generation = callback_generation.current();
  const auto second_generation = callback_generation.renew(&second_owner);
  if (first_generation->load() != nullptr || second_generation->load() != &second_owner) {
    std::cerr << "stale websocket callbacks retained a newer transport owner\n";
    return 1;
  }
  callback_generation.invalidate();
  if (second_generation->load() != nullptr) {
    std::cerr << "closed websocket transport retained its callback owner\n";
    return 1;
  }

  int callback_owner = 0;
  if (bybit::detail::in_websocket_callback(&callback_owner)) {
    std::cerr << "callback state leaked across scopes\n";
    return 1;
  }
  {
    bybit::detail::WebSocketCallbackScope callback_scope{&callback_owner};
    if (!bybit::detail::in_websocket_callback(&callback_owner)) {
      std::cerr << "callback close was not detected\n";
      return 1;
    }
  }
  if (bybit::detail::in_websocket_callback(&callback_owner)) {
    std::cerr << "callback state was not restored\n";
    return 1;
  }

  FakeTransport fake_transport;
  bybit::detail::restart_transport(fake_transport);
  if (fake_transport.calls != "stop;start;") {
    std::cerr << "manual restart did not reap the prior transport thread\n";
    return 1;
  }

  bool rejected_partial_credentials = false;
  try {
    bybit::WebSocketClient client{"", "key", ""};
  } catch (const std::invalid_argument&) {
    rejected_partial_credentials = true;
  }
  if (!rejected_partial_credentials) {
    std::cerr << "partial websocket credentials were accepted\n";
    return 1;
  }

  bybit::detail::WebSocketLifecycle lifecycle;
  lifecycle.configure_reconnect(true, 3);

  bybit::detail::WebSocketLifecycle in_flight_lifecycle;
  in_flight_lifecycle.configure_reconnect(true, 3);
  if (!in_flight_lifecycle.begin_connect()) {
    std::cerr << "initial websocket connect did not start\n";
    return 1;
  }
  in_flight_lifecycle.configure_reconnect(false, 3);
  if (in_flight_lifecycle.on_open(false) != OpenAction::Ready || !in_flight_lifecycle.can_send()) {
    std::cerr << "disabling reconnect cancelled the in-flight initial connection\n";
    return 1;
  }

  bybit::detail::WebSocketLifecycle pending_reconnect_lifecycle;
  pending_reconnect_lifecycle.configure_reconnect(true, 3);
  if (!pending_reconnect_lifecycle.begin_connect() || pending_reconnect_lifecycle.on_open(false) != OpenAction::Ready ||
      !pending_reconnect_lifecycle.on_disconnect()) {
    std::cerr << "reconnect cancellation precondition failed\n";
    return 1;
  }
  pending_reconnect_lifecycle.configure_reconnect(false, 3);
  if (pending_reconnect_lifecycle.on_open(false) != OpenAction::None || pending_reconnect_lifecycle.can_send()) {
    std::cerr << "disabling reconnect did not cancel the pending reconnect\n";
    return 1;
  }

  if (!lifecycle.begin_connect() || lifecycle.begin_connect()) {
    std::cerr << "duplicate connect was not suppressed\n";
    return 1;
  }
  if (lifecycle.on_open(true) != OpenAction::Authenticate || lifecycle.can_send() ||
      lifecycle.on_open(true) != OpenAction::None) {
    std::cerr << "duplicate open would repeat authentication/resubscription\n";
    return 1;
  }
  if (lifecycle.on_auth_result(false) != AuthAction::Close || lifecycle.can_send() ||
      lifecycle.on_auth_result(true) != AuthAction::None) {
    std::cerr << "failed authentication was not rejected without releasing private subscriptions\n";
    return 1;
  }

  const auto reconnect = lifecycle.on_disconnect();
  if (!reconnect || *reconnect != std::chrono::milliseconds{2000} || lifecycle.on_disconnect()) {
    std::cerr << "disconnect did not schedule exactly one reconnect\n";
    return 1;
  }

  lifecycle.reconnect_finished();
  if (lifecycle.on_open(true) != OpenAction::Authenticate || lifecycle.on_auth_result(true) != AuthAction::Ready ||
      !lifecycle.can_send() || lifecycle.on_auth_result(true) != AuthAction::None) {
    std::cerr << "private subscriptions were not released exactly once after auth success\n";
    return 1;
  }

  const auto retry = lifecycle.on_disconnect();
  if (!retry || *retry != std::chrono::milliseconds{2000}) {
    std::cerr << "successful reconnect did not reset retry backoff\n";
    return 1;
  }

  bybit::detail::WebSocketLifecycle failing_private_lifecycle;
  failing_private_lifecycle.configure_reconnect(true, 2);
  if (!failing_private_lifecycle.begin_connect()) {
    std::cerr << "private retry-budget precondition failed\n";
    return 1;
  }
  for (int attempt = 0; attempt < 3; ++attempt) {
    if (failing_private_lifecycle.on_open(true) != OpenAction::Authenticate ||
        failing_private_lifecycle.on_auth_result(false) != AuthAction::Close) {
      std::cerr << "private authentication failure did not stay in the retry lifecycle\n";
      return 1;
    }
    const auto delay = failing_private_lifecycle.on_disconnect();
    if ((attempt < 2) != delay.has_value()) {
      std::cerr << "private authentication failures bypassed the reconnect retry budget\n";
      return 1;
    }
    if (delay)
      failing_private_lifecycle.reconnect_finished();
  }

  lifecycle.close_intentionally();
  if (lifecycle.on_disconnect()) {
    std::cerr << "intentional close scheduled a reconnect\n";
    return 1;
  }
  if (!lifecycle.begin_connect() || lifecycle.on_open(false) != OpenAction::Ready || !lifecycle.can_send()) {
    std::cerr << "client could not reconnect after an intentional close\n";
    return 1;
  }

  bybit::detail::WebSocketLifecycle manual_lifecycle;
  if (!manual_lifecycle.begin_connect() || manual_lifecycle.on_open(false) != OpenAction::Ready ||
      manual_lifecycle.on_disconnect() || !manual_lifecycle.begin_connect()) {
    std::cerr << "terminal disconnect did not allow a later manual connect\n";
    return 1;
  }

  const auto stream_success =
      bybit::detail::parse_websocket_auth_result(R"({"success": true, "ret_msg":"", "op":"auth"})");
  const auto stream_failure =
      bybit::detail::parse_websocket_auth_result(R"({"success":false,"ret_msg":"bad sign","op":"auth"})");
  const auto trade_success = bybit::detail::parse_websocket_auth_result(R"({"retCode":0,"retMsg":"OK","op":"auth"})");
  const auto unrelated =
      bybit::detail::parse_websocket_auth_result(R"({"success":true,"ret_msg":"auth","op":"subscribe"})");
  if (!stream_success || !*stream_success || !stream_failure || *stream_failure || !trade_success || !*trade_success ||
      unrelated) {
    std::cerr << "auth acknowledgement envelope detection drifted\n";
    return 1;
  }
#ifdef BYBIT_ENABLE_WEBSOCKET
  ix::WebSocket transport;
  if (!transport.isAutomaticReconnectionEnabled()) {
    std::cerr << "IXWebSocket default reconnect precondition changed\n";
    return 1;
  }
  bybit::detail::configure_managed_reconnect(transport);
  if (transport.isAutomaticReconnectionEnabled()) {
    std::cerr << "IXWebSocket reconnect competes with the wrapper lifecycle\n";
    return 1;
  }
#endif
  return 0;
}
