#include <IXGetFreePort.h>
#include <IXNetSystem.h>
#include <IXWebSocketServer.h>

#include <chrono>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>

#include "bybit/websocket_client.hpp"

int main() {
  ix::initNetSystem();

  const int port = ix::getFreePort();
  ix::WebSocketServer server{port, "127.0.0.1"};
  server.setOnClientMessageCallback(
      [](const std::shared_ptr<ix::ConnectionState>&, ix::WebSocket& socket, const ix::WebSocketMessagePtr& message) {
        if (message->type == ix::WebSocketMessageType::Open)
          socket.send("close-race", false);
      });
  const auto listen_result = server.listen();
  if (!listen_result.first) {
    std::cerr << listen_result.second << '\n';
    ix::uninitNetSystem();
    return 1;
  }
  server.start();

  std::mutex mutex;
  std::condition_variable cv;
  bool handler_entered = false;
  bool closer_started = false;
  bool send_allowed = false;
  bool handler_finished = false;

  bybit::WebSocketClient client{"ws://127.0.0.1:" + std::to_string(port)};
  client.set_message_handler([&](const std::string&) {
    {
      std::unique_lock lock(mutex);
      handler_entered = true;
      cv.notify_all();
      cv.wait(lock, [&] { return send_allowed; });
    }
    try {
      client.ping("close-race");
    } catch (const std::runtime_error&) {
      // The concurrent close may remove the transport before this send.
    }
    {
      std::scoped_lock lock(mutex);
      handler_finished = true;
    }
    cv.notify_all();
  });
  client.connect();

  {
    std::unique_lock lock(mutex);
    if (!cv.wait_for(lock, std::chrono::seconds{5}, [&] { return handler_entered; })) {
      std::cerr << "websocket handler did not start\n";
      lock.unlock();
      client.close();
      server.stop();
      ix::uninitNetSystem();
      return 1;
    }
  }

  std::thread closer{[&] {
    {
      std::scoped_lock lock(mutex);
      closer_started = true;
    }
    cv.notify_all();
    client.close();
  }};
  {
    std::unique_lock lock(mutex);
    cv.wait(lock, [&] { return closer_started; });
  }
  std::this_thread::sleep_for(std::chrono::milliseconds{100});
  {
    std::scoped_lock lock(mutex);
    send_allowed = true;
  }
  cv.notify_all();

  {
    std::unique_lock lock(mutex);
    if (!cv.wait_for(lock, std::chrono::seconds{5}, [&] { return handler_finished; })) {
      std::cerr << "close and callback send deadlocked\n";
      return 1;
    }
  }
  closer.join();
  server.stop();
  ix::uninitNetSystem();
  return 0;
}
