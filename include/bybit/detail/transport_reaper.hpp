#pragma once

#include <condition_variable>
#include <deque>
#include <memory>
#include <mutex>
#include <thread>

namespace bybit::detail {

template <typename Transport>
class TransportReaper {
 public:
  TransportReaper() : worker_([this] { run(); }) {}

  ~TransportReaper() {
    {
      std::scoped_lock lock(mutex_);
      stopping_ = true;
    }
    cv_.notify_all();
    if (worker_.joinable())
      worker_.join();
  }

  TransportReaper(const TransportReaper&) = delete;
  TransportReaper& operator=(const TransportReaper&) = delete;

  void enqueue(std::unique_ptr<Transport> transport) {
    if (!transport)
      return;
    {
      std::scoped_lock lock(mutex_);
      transports_.push_back(std::move(transport));
    }
    cv_.notify_one();
  }

 private:
  void run() {
    while (true) {
      std::unique_ptr<Transport> transport;
      {
        std::unique_lock lock(mutex_);
        cv_.wait(lock, [this] { return stopping_ || !transports_.empty(); });
        if (transports_.empty()) {
          if (stopping_)
            return;
          continue;
        }
        transport = std::move(transports_.front());
        transports_.pop_front();
      }
      transport->stop();
    }
  }

  std::mutex mutex_;
  std::condition_variable cv_;
  std::deque<std::unique_ptr<Transport>> transports_;
  bool stopping_{false};
  std::thread worker_;
};

template <typename Transport>
TransportReaper<Transport>& transport_reaper() {
  static TransportReaper<Transport> reaper;
  return reaper;
}

template <typename Transport>
void ensure_transport_reaper() {
  (void)transport_reaper<Transport>();
}

template <typename Transport>
void defer_transport_cleanup(std::unique_ptr<Transport> transport) {
  transport_reaper<Transport>().enqueue(std::move(transport));
}

}  // namespace bybit::detail
