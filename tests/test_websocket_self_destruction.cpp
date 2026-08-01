#include <IXGetFreePort.h>
#include <IXNetSystem.h>
#include <IXWebSocketServer.h>

#include <chrono>
#include <condition_variable>
#include <iostream>
#include <memory>
#include <mutex>
#include <string>

#include "bybit/websocket_client.hpp"

int main() {
  ix::initNetSystem();

  std::mutex mutex;
  std::condition_variable cv;
  bool destroyed_in_handler = false;

  const int port = ix::getFreePort();
  ix::WebSocketServer server{port, "127.0.0.1"};
  server.setOnClientMessageCallback(
      [](const std::shared_ptr<ix::ConnectionState>&, ix::WebSocket& socket, const ix::WebSocketMessagePtr& message) {
        if (message->type == ix::WebSocketMessageType::Open)
          socket.send("destroy", false);
      });
  const auto listen_result = server.listen();
  if (!listen_result.first) {
    std::cerr << listen_result.second << '\n';
    ix::uninitNetSystem();
    return 1;
  }
  server.start();

  std::unique_ptr<bybit::WebSocketClient> client =
      std::make_unique<bybit::WebSocketClient>("ws://127.0.0.1:" + std::to_string(port));
  client->set_message_handler([&](const std::string&) {
    client.reset();
    {
      std::scoped_lock lock(mutex);
      destroyed_in_handler = true;
    }
    cv.notify_all();
  });
  client->connect();

  {
    std::unique_lock lock(mutex);
    if (!cv.wait_for(lock, std::chrono::seconds{5}, [&] { return destroyed_in_handler; })) {
      std::cerr << "websocket callback did not destroy its client\n";
      lock.unlock();
      client.reset();
      server.stop();
      ix::uninitNetSystem();
      return 1;
    }
  }

  server.stop();
  ix::uninitNetSystem();
  return 0;
}
