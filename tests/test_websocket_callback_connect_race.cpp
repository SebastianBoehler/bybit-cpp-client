#include <IXGetFreePort.h>
#include <IXNetSystem.h>
#include <IXWebSocketServer.h>

#include <atomic>
#include <chrono>
#include <iostream>
#include <string>
#include <thread>

#include "bybit/websocket_client.hpp"

class TestWebSocketServer : public ix::WebSocketServer {
 public:
  using ix::WebSocketServer::WebSocketServer;

  void stop_accepting_connections() {
    stopAcceptingConnections();
  }
};

int main() {
  ix::initNetSystem();

  const int port = ix::getFreePort();
  TestWebSocketServer server{port, "127.0.0.1"};
  server.setOnClientMessageCallback(
      [](const std::shared_ptr<ix::ConnectionState>&, ix::WebSocket& socket, const ix::WebSocketMessagePtr& message) {
        if (message->type == ix::WebSocketMessageType::Open)
          socket.send("callback-close", false);
      });
  const auto listen_result = server.listen();
  if (!listen_result.first) {
    std::cerr << listen_result.second << '\n';
    ix::uninitNetSystem();
    return 1;
  }
  server.start();

  std::atomic<bool> stopping{false};
  std::atomic<int> callbacks{0};
  bybit::WebSocketClient client{"ws://127.0.0.1:" + std::to_string(port)};
  client.set_message_handler([&](const std::string&) {
    callbacks.fetch_add(1, std::memory_order_relaxed);
    client.close();
  });
  client.connect();

  std::thread connector{[&] {
    while (!stopping.load(std::memory_order_acquire)) {
      client.connect();
      std::this_thread::yield();
    }
  }};

  const auto deadline = std::chrono::steady_clock::now() + std::chrono::seconds{5};
  while (callbacks.load(std::memory_order_relaxed) < 200 && std::chrono::steady_clock::now() < deadline)
    std::this_thread::sleep_for(std::chrono::milliseconds{5});

  stopping.store(true, std::memory_order_release);
  connector.join();

  bybit::WebSocketClient external_race_client{"ws://127.0.0.1:" + std::to_string(port)};
  std::thread external_connector{[&] {
    for (int iteration = 0; iteration < 1000; ++iteration)
      external_race_client.connect();
  }};
  std::thread external_closer{[&] {
    for (int iteration = 0; iteration < 1000; ++iteration)
      external_race_client.close();
  }};
  external_connector.join();
  external_closer.join();

  server.stop_accepting_connections();
  client.close();
  external_race_client.close();
  const auto server_drain_deadline = std::chrono::steady_clock::now() + std::chrono::seconds{5};
  while (!server.getClients().empty() && std::chrono::steady_clock::now() < server_drain_deadline)
    std::this_thread::sleep_for(std::chrono::milliseconds{5});
  server.stop();
  ix::uninitNetSystem();

  if (callbacks.load(std::memory_order_relaxed) < 200) {
    std::cerr << "callback-close/connect race did not complete enough iterations\n";
    return 1;
  }
  return 0;
}
