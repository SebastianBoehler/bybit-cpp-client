#include "bybit/http_client.hpp"

#include <sstream>
#include <utility>

namespace bybit {
namespace {

std::string http_error_message(long status, const std::string& body) {
  std::ostringstream oss;
  oss << "HTTP status " << status << " body: " << body;
  return oss.str();
}

}  // namespace

HttpError::HttpError(long status_code, std::string body)
    : std::runtime_error(http_error_message(status_code, body)), status_code_(status_code), body_(std::move(body)) {}

}  // namespace bybit
