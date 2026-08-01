#pragma once

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cerrno>
#include <cstring>
#include <stdexcept>
#include <string>
#include <thread>

namespace bybit::test {

class HttpCaptureServer {
 public:
  HttpCaptureServer() {
    socket_ = ::socket(AF_INET, SOCK_STREAM, 0);
    if (socket_ < 0)
      throw std::runtime_error("failed to create capture socket");

    int reuse = 1;
    ::setsockopt(socket_, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    address.sin_port = 0;
    if (::bind(socket_, reinterpret_cast<sockaddr*>(&address), sizeof(address)) != 0 || ::listen(socket_, 1) != 0) {
      ::close(socket_);
      throw std::runtime_error("failed to bind capture socket");
    }

    socklen_t length = sizeof(address);
    if (::getsockname(socket_, reinterpret_cast<sockaddr*>(&address), &length) != 0) {
      ::close(socket_);
      throw std::runtime_error("failed to read capture socket address");
    }
    port_ = ntohs(address.sin_port);
    worker_ = std::thread([this] { serve_one(); });
  }

  ~HttpCaptureServer() {
    if (socket_ >= 0) {
      ::shutdown(socket_, SHUT_RDWR);
      ::close(socket_);
      socket_ = -1;
    }
    if (worker_.joinable())
      worker_.join();
  }

  HttpCaptureServer(const HttpCaptureServer&) = delete;
  HttpCaptureServer& operator=(const HttpCaptureServer&) = delete;

  std::string base_url() const {
    return "http://127.0.0.1:" + std::to_string(port_);
  }

  std::string take_request() {
    if (worker_.joinable())
      worker_.join();
    if (!error_.empty())
      throw std::runtime_error(error_);
    return request_;
  }

 private:
  static std::size_t content_length(const std::string& request, std::size_t header_end) {
    constexpr const char* label = "Content-Length:";
    const auto start = request.find(label);
    if (start == std::string::npos || start > header_end)
      return 0;
    const auto value_start = start + std::strlen(label);
    const auto value_end = request.find("\r\n", value_start);
    return static_cast<std::size_t>(std::stoul(request.substr(value_start, value_end - value_start)));
  }

  void serve_one() noexcept {
    const int client = ::accept(socket_, nullptr, nullptr);
    if (client < 0) {
      if (errno != EINVAL && errno != EBADF)
        error_ = "failed to accept capture connection";
      return;
    }

    char buffer[4096];
    while (true) {
      const auto count = ::recv(client, buffer, sizeof(buffer), 0);
      if (count <= 0)
        break;
      request_.append(buffer, static_cast<std::size_t>(count));
      const auto header_end = request_.find("\r\n\r\n");
      if (header_end != std::string::npos && request_.size() >= header_end + 4 + content_length(request_, header_end))
        break;
    }

    constexpr const char* response =
        "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\nContent-Length: 39\r\nConnection: close\r\n\r\n"
        "{\"retCode\":0,\"retMsg\":\"OK\",\"result\":{}}";
    ::send(client, response, std::strlen(response), 0);
    ::shutdown(client, SHUT_RDWR);
    ::close(client);
  }

  int socket_{-1};
  std::uint16_t port_{};
  std::thread worker_;
  std::string request_;
  std::string error_;
};

}  // namespace bybit::test
