#pragma once

#include <atomic>
#include <memory>
#include <mutex>

#ifdef BYBIT_ENABLE_WEBSOCKET
#include <IXWebSocket.h>
#else
namespace ix {
class WebSocket;
}
#endif

namespace bybit::detail {

template <typename Owner>
class WebSocketCallbackOwner {
 public:
  using Token = std::shared_ptr<std::atomic<Owner*>>;

  explicit WebSocketCallbackOwner(Owner* owner) : token_(std::make_shared<std::atomic<Owner*>>(owner)) {}

  Token renew(Owner* owner) {
    std::scoped_lock lock(mutex_);
    token_->store(nullptr, std::memory_order_release);
    token_ = std::make_shared<std::atomic<Owner*>>(owner);
    return token_;
  }

  void invalidate() {
    std::scoped_lock lock(mutex_);
    token_->store(nullptr, std::memory_order_release);
  }

  Token current() const {
    std::scoped_lock lock(mutex_);
    return token_;
  }

 private:
  mutable std::mutex mutex_;
  Token token_;
};

inline thread_local const void* active_websocket_callback = nullptr;

class WebSocketCallbackScope {
 public:
  explicit WebSocketCallbackScope(const void* owner) : previous_(active_websocket_callback) {
    active_websocket_callback = owner;
  }

  ~WebSocketCallbackScope() {
    active_websocket_callback = previous_;
  }

  WebSocketCallbackScope(const WebSocketCallbackScope&) = delete;
  WebSocketCallbackScope& operator=(const WebSocketCallbackScope&) = delete;

 private:
  const void* previous_;
};

inline bool in_websocket_callback(const void* owner) {
  return active_websocket_callback == owner;
}

template <typename Transport>
void restart_transport(Transport& transport) {
  transport.stop();
  transport.start();
}

inline void configure_managed_reconnect(ix::WebSocket& transport) {
#ifdef BYBIT_ENABLE_WEBSOCKET
  transport.disableAutomaticReconnection();
#else
  (void)transport;
#endif
}

}  // namespace bybit::detail
