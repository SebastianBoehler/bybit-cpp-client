#pragma once

#include <algorithm>
#include <chrono>
#include <optional>

namespace bybit::detail {

enum class OpenAction { None, Authenticate, Ready };
enum class AuthAction { None, Ready, Close };

class WebSocketLifecycle {
 public:
  void configure_reconnect(bool enabled, int max_retries) {
    auto_reconnect_ = enabled;
    max_retries_ = std::max(0, max_retries);
    reconnect_attempts_ = 0;
    if (!enabled) {
      const bool was_pending = reconnect_pending_;
      reconnect_pending_ = false;
      if (was_pending)
        active_ = false;
    }
  }

  bool begin_connect() {
    if (active_)
      return false;
    active_ = true;
    open_ = false;
    ready_ = false;
    awaiting_auth_ = false;
    intentional_close_ = false;
    reconnect_pending_ = false;
    reconnect_attempts_ = 0;
    return true;
  }

  OpenAction on_open(bool requires_auth) {
    if (!active_ || intentional_close_ || open_)
      return OpenAction::None;
    open_ = true;
    ready_ = !requires_auth;
    awaiting_auth_ = requires_auth;
    reconnect_pending_ = false;
    if (!requires_auth)
      reconnect_attempts_ = 0;
    return requires_auth ? OpenAction::Authenticate : OpenAction::Ready;
  }

  AuthAction on_auth_result(bool success) {
    if (!active_ || !open_ || !awaiting_auth_)
      return AuthAction::None;
    awaiting_auth_ = false;
    ready_ = success;
    if (success)
      reconnect_attempts_ = 0;
    return success ? AuthAction::Ready : AuthAction::Close;
  }

  bool can_send() const {
    return active_ && open_ && ready_;
  }

  std::optional<std::chrono::milliseconds> on_disconnect() {
    open_ = false;
    ready_ = false;
    awaiting_auth_ = false;
    if (!active_ || intentional_close_ || reconnect_pending_)
      return std::nullopt;
    if (!auto_reconnect_ || reconnect_attempts_ >= max_retries_) {
      active_ = false;
      return std::nullopt;
    }

    reconnect_pending_ = true;
    const int attempt = ++reconnect_attempts_;
    return std::chrono::milliseconds{std::min(1000 * (1 << std::min(attempt, 6)), 10000)};
  }

  void reconnect_finished() {
    reconnect_pending_ = false;
  }

  void close_intentionally() {
    active_ = false;
    open_ = false;
    ready_ = false;
    awaiting_auth_ = false;
    intentional_close_ = true;
    reconnect_pending_ = false;
  }

 private:
  bool active_{false};
  bool open_{false};
  bool ready_{false};
  bool awaiting_auth_{false};
  bool intentional_close_{true};
  bool auto_reconnect_{false};
  bool reconnect_pending_{false};
  int max_retries_{5};
  int reconnect_attempts_{0};
};

}  // namespace bybit::detail
