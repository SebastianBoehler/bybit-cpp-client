# Bybit C++ Client (REST v5 + WS)

[![Build](https://github.com/SebastianBoehler/bybit-cpp-client/actions/workflows/c-cpp.yml/badge.svg)](https://github.com/SebastianBoehler/bybit-cpp-client/actions/workflows/c-cpp.yml)
[![Tests](https://github.com/SebastianBoehler/bybit-cpp-client/actions/workflows/tests.yml/badge.svg)](https://github.com/SebastianBoehler/bybit-cpp-client/actions/workflows/tests.yml)

A lightweight C++ wrapper for Bybit REST v5 endpoints. Focused on REST first; websocket integration can follow.

## Features

- General Bybit REST v5 C++ API wrapper; optional WebSocket v5 client (public/private) behind
  `-DBYBIT_ENABLE_WEBSOCKET=ON`.
- Shared HTTP helper with HMAC-SHA256 signing (OpenSSL) and libcurl transport.
- Public/private client split for maintainability; facade `RestClient` preserves simple usage.
- Clang-format hook (git pre-commit) for consistent style.
- CMake build with FetchContent/submodule friendliness and install targets.

## Build & Install

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
cmake --install build --prefix dist

# Enable websocket client (fetches ixwebsocket)
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DBYBIT_ENABLE_WEBSOCKET=ON
cmake --build build
```

## Use via FetchContent

```cmake
include(FetchContent)
FetchContent_Declare(
  bybit_cpp_client
  GIT_REPOSITORY https://github.com/SebastianBoehler/bybit-cpp-client.git
  GIT_TAG v0.1.0  # Pin to a specific release version
)
FetchContent_MakeAvailable(bybit_cpp_client)

add_executable(app main.cpp)
target_link_libraries(app PRIVATE bybit_client)
```

### Version Pinning

You can pin to:

- **Specific release**: `GIT_TAG v0.1.0`
- **Latest main**: `GIT_TAG main`
- **Specific commit**: `GIT_TAG <commit-sha>`

Check the [Releases](https://github.com/SebastianBoehler/bybit-cpp-client/releases) page for available versions.

### Check Version at Runtime

```cpp
#include <bybit/version.hpp>
#include <iostream>

int main() {
    std::cout << "bybit-cpp-client version: " << bybit::version_string << "\n";
    // Or use macros: BYBIT_VERSION_MAJOR, BYBIT_VERSION_MINOR, BYBIT_VERSION_PATCH
}
```

## Minimal REST usage

```cpp
#include <bybit/rest_client.hpp>
#include <iostream>

int main() {
  bybit::RestClient client{"YOUR_KEY", "YOUR_SECRET", "linear"};
  try {
    auto instruments = client.get_instruments_info();
    std::cout << instruments << "\n";
  } catch (const std::exception& ex) {
    std::cerr << "Error: " << ex.what() << "\n";
  }
}
```

## Minimal WebSocket usage

```cpp
#include <bybit/websocket_client.hpp>
#include <iostream>

int main() {
  // public linear ticker stream
  bybit::WebSocketClient ws{"wss://stream.bybit.com/v5/public/linear"};
  ws.set_message_handler([](const std::string& msg) { std::cout << msg << "\n"; });
  ws.connect();
  ws.subscribe({"tickers.BTCUSDT"}, "sub-1");
  // keep process alive in your app loop; this example omits cleanup for brevity
}
```

## Examples

- `examples/basic_linear.cpp` (USDT-margined), `examples/basic_spot.cpp`: REST flows.
- `examples/ws_market_data.cpp`: WebSocket market data demo (tickers, orderbook, klines, public trades). Build with
  `-DBYBIT_ENABLE_WEBSOCKET=ON`.
- `examples/ws_orderbook.cpp`: WebSocket order book depth-50 stream demo. Build with `-DBYBIT_ENABLE_WEBSOCKET=ON`.

Requires `BYBIT_API_KEY`/`BYBIT_API_SECRET` in env for private REST tests; set `BYBIT_CATEGORY` as needed (e.g., `linear`
or `spot`).

## Environment & tests

- Set `BYBIT_API_KEY`, `BYBIT_API_SECRET`, `BYBIT_CATEGORY` (e.g., `linear`) to run the readonly test.
- `.env.example` provided; `.env` is gitignored.
- Run tests: `ctest --test-dir build` (after build).

## Formatting hook

- `.git/hooks/pre-commit` runs `clang-format` on staged C/C++ files. Ensure `clang-format` is installed.

## Roadmap

- Optional: migrate HMAC to OpenSSL EVP_MAC to silence deprecation warnings.
- Add websocket coverage (positions/tickers) — initial dedicated client shipped behind optional flag.
- Broaden REST coverage beyond current endpoints.
- Expand automated tests (unit/integration) across public/private calls.
