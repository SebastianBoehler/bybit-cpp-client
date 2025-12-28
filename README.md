# Bybit C++ Client (REST v5)

A lightweight C++ wrapper for Bybit REST v5 endpoints. Focused on REST first; websocket integration can follow.

## Features

- REST v5 endpoints: query API key, account info, positions, instruments, submit order, set leverage, historic orders, fee rate.
- Shared HTTP helper with HMAC-SHA256 signing (OpenSSL) and libcurl transport.
- Public/private client split for maintainability; facade `RestClient` preserves simple usage.
- Clang-format hook (git pre-commit) for consistent style.
- CMake build with FetchContent/submodule friendliness and install targets.

## Build & Install

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
cmake --install build --prefix dist
```

## Use via FetchContent

```cmake
include(FetchContent)
FetchContent_Declare(
  bybit_cpp_client
  GIT_REPOSITORY https://github.com/SebastianBoehler/bybit-cpp-client.git
  GIT_TAG main
)
FetchContent_MakeAvailable(bybit_cpp_client)

add_executable(app main.cpp)
target_link_libraries(app PRIVATE Bybit::bybit_client)
```

## Minimal usage

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

## Examples

See `examples/basic_linear.cpp` (USDT-margined) and `examples/basic_spot.cpp` for common flows. Requires `BYBIT_API_KEY`/`BYBIT_API_SECRET` in env; set `BYBIT_CATEGORY` as needed (e.g., `linear` or `spot`).

## Environment & tests

- Set `BYBIT_API_KEY`, `BYBIT_API_SECRET`, `BYBIT_CATEGORY` (e.g., `linear`) to run the readonly test.
- `.env.example` provided; `.env` is gitignored.
- Run tests: `ctest --test-dir build` (after build).

## Formatting hook

- `.git/hooks/pre-commit` runs `clang-format` on staged C/C++ files. Ensure `clang-format` is installed.

## Roadmap

- Optional: migrate HMAC to OpenSSL EVP_MAC to silence deprecation warnings.
- Add websocket support (positions/tickers) after REST parity is stable.
- Broaden REST coverage beyond current endpoints.
- Expand automated tests (unit/integration) across public/private calls.
