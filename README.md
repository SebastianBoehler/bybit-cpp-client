<div align="center">

# bybit-cpp-client

Modern C++ client for Bybit Open API V5 REST and optional WebSocket flows.

[![Build](https://github.com/SebastianBoehler/bybit-cpp-client/actions/workflows/c-cpp.yml/badge.svg)](https://github.com/SebastianBoehler/bybit-cpp-client/actions/workflows/c-cpp.yml)
[![Tests](https://github.com/SebastianBoehler/bybit-cpp-client/actions/workflows/tests.yml/badge.svg)](https://github.com/SebastianBoehler/bybit-cpp-client/actions/workflows/tests.yml)
![C++](https://img.shields.io/badge/C%2B%2B-20-00599C?logo=cplusplus&logoColor=white)
![CMake](https://img.shields.io/badge/CMake-3.16%2B-064F8C?logo=cmake&logoColor=white)
![Bybit](https://img.shields.io/badge/Bybit-Open%20API%20V5-F7A600)
![License](https://img.shields.io/badge/license-Apache--2.0-D22128.svg)

[Install](#install) | [Usage](#minimal-rest-usage) | [API Coverage](#api-coverage) | [Contributing](#contributing)

</div>

`bybit-cpp-client` is a lightweight C++ wrapper for Bybit Open API V5. The current focus is a stable REST wrapper for the endpoints used by the TypeScript trading integration, with optional WebSocket support kept behind a build flag.

The library does not ship mock data, hidden fallbacks, or trading opinions. It signs requests, dispatches them to Bybit, and returns upstream JSON so applications can decide how to parse, store, and act on responses.

## Features

- REST V5 facade with public/private client separation.
- HMAC-SHA256 request signing through OpenSSL and HTTP transport through libcurl.
- Persistent libcurl easy handle per client for connection reuse, TLS session reuse, and per-handle DNS caching.
- Optional WebSocket V5 client behind `-DBYBIT_ENABLE_WEBSOCKET=ON`.
- CMake install targets for package consumers, submodules, and `FetchContent`.
- Small examples for market data, positions, wallet balance, orders, and WebSocket streams.
- Focused tests for public REST calls and signing behavior.

## API Coverage

The wrapper targets [Bybit Open API V5](https://bybit-exchange.github.io/docs/v5/intro). The [official V5 changelog](https://bybit-exchange.github.io/docs/changelog/v5) was last checked on 2026-05-12; V5 remains the current public API family, with active schema additions rather than a newer major version.

| Capability | Bybit endpoint | Client method |
| --- | --- | --- |
| Server time | `GET /v5/market/time` | `get_server_time()` |
| API key validation | `GET /v5/user/query-api` | `get_query_api_key()` |
| Account margin mode | `GET /v5/account/info` | `get_account_info()` |
| Account instruments | `GET /v5/account/instruments-info` | `get_account_instruments_info(...)` |
| Transferable amount | `GET /v5/account/withdrawal` | `get_transferable_amount(...)` |
| Collateral info | `GET /v5/account/collateral-info` | `get_collateral_info(...)` |
| Set collateral coin | `POST /v5/account/set-collateral-switch` | `set_collateral_coin(...)` |
| Batch collateral coins | `POST /v5/account/set-collateral-switch-batch` | `batch_set_collateral_coins(...)` |
| Manual borrow and repay | `POST /v5/account/borrow`, `POST /v5/account/repay` | `manual_borrow(...)`, `manual_repay(...)` |
| No-convert repay | `POST /v5/account/no-convert-repay` | `no_convert_repay(...)` |
| Repay liability | `POST /v5/account/quick-repayment` | `quick_repayment(...)` |
| Coin Greeks | `GET /v5/asset/coin-greeks` | `get_coin_greeks(...)` |
| MMP and SMP account state | `GET/POST /v5/account/mmp-*`, `GET /v5/account/smp-group` | `get_mmp_state(...)`, `set_mmp(...)`, `reset_mmp(...)`, `get_smp_group()` |
| Account trade behaviour | `GET /v5/account/user-setting-config` | `get_trade_behaviour_config()` |
| Delta Neutral mode | `POST /v5/account/set-delta-mode` | `set_delta_neutral_mode(...)` |
| Limit price behaviour | `POST /v5/account/set-limit-px-action` | `set_price_limit_behaviour(...)` |
| Option asset and pay info | `GET /v5/account/option-asset-info`, `GET /v5/account/pay-info` | `get_option_asset_info()`, `get_pay_info(...)` |
| Spot trade analysis | `GET /v5/account/trade-info-for-analysis` | `get_trade_info_for_analysis(...)` |
| Asset balances | `GET /v5/asset/transfer/query-*balance` | `get_all_coin_balances(...)`, `get_single_coin_balance(...)` |
| Asset transfers | `POST /v5/asset/transfer/*transfer` | `create_internal_transfer(...)`, `create_universal_transfer(...)` |
| Deposit records and addresses | `GET /v5/asset/deposit/*` | `get_deposit_records(...)`, `get_master_deposit_address(...)` |
| Coin and withdrawal info | `GET /v5/asset/coin/query-info`, `GET /v5/asset/withdraw/*` | `get_coin_info(...)`, `get_withdrawable_amount(...)`, `get_withdrawal_records(...)` |
| Create withdrawal | `POST /v5/asset/withdraw/create` | `create_withdrawal(...)` |
| Open positions | `GET /v5/position/list` | `get_position_info(...)` |
| Instrument specs and lot size | `GET /v5/market/instruments-info` | `get_instruments_info(...)` |
| Risk tiers | `GET /v5/market/risk-limit` | `get_risk_limit(...)` |
| Place order | `POST /v5/order/create` | `submit_order(...)` |
| Pre-check order | `POST /v5/order/pre-check` | `pre_check_order(...)` |
| Open and recent closed orders | `GET /v5/order/realtime` | `get_open_orders(...)`, `get_realtime_orders(...)` |
| Cancel all orders | `POST /v5/order/cancel-all` | `cancel_all(...)`, `cancel_all_orders(...)` |
| Disconnect cancel all | `POST /v5/order/disconnected-cancel-all` | `set_disconnect_cancel_all(...)` |
| DCP info | `GET /v5/account/query-dcp-info` | `get_dcp_info()` |
| Set leverage | `POST /v5/position/set-leverage` | `set_leverage(...)` |
| Order history | `GET /v5/order/history` | `get_historic_orders(...)` |
| Filtered order history | `GET /v5/order/history` | `get_order_history(...)` |
| Execution history | `GET /v5/execution/list` | `get_trade_history(...)` |
| Closed PnL | `GET /v5/position/closed-pnl` | `get_closed_pnl(...)` |
| Closed option positions | `GET /v5/position/get-closed-positions` | `get_closed_options_positions(...)` |
| Move positions | `POST /v5/position/move-positions` | `move_positions(...)` |
| Move position history | `GET /v5/position/move-history` | `get_move_position_history(...)` |
| Auto add margin | `POST /v5/position/set-auto-add-margin` | `set_auto_add_margin(...)` |
| Confirm pending MMR | `POST /v5/position/confirm-pending-mmr` | `confirm_pending_mmr(...)` |
| Fee rates | `GET /v5/account/fee-rate` | `get_fee_rate()` |
| Spot borrow quota | `GET /v5/order/spot-borrow-check` | `get_borrow_quota(...)` |

Recent Bybit changes to keep in mind:

- `Get Instruments Info` now returns `symbolId` for spot, futures, and options.
- `Get Position Info` now includes `openTime`.
- `Get API Key Information` includes `FiatBitPay` while the older `FiatBybitPay` field remains during transition.
- `Place Order` supports BBO order parameters `bboSideType` and `bboLevel`; this client already exposes optional parameters for them.
- Manual borrow/repay, Delta Neutral mode, pay info, option asset info, and spot trade analysis are active Account endpoints and are now wrapped.
- Asset endpoints now cover core balances, transfers, deposit records, deposit addresses, coin metadata, withdrawable amount, withdrawal records, and withdrawal creation.

This client covers the core trading wrapper surface plus high-value Account, Asset, Position, Trade, and Market methods. It does not yet wrap every Bybit V5 product category such as User management, Spot Margin Trade, Spread Trading, RFQ, Crypto Loan, Broker, Finance, Bybit Card, Web3, or SBE. See [`docs/api_coverage.md`](./docs/api_coverage.md) for the current coverage map.

Because responses are returned as raw JSON, additive response fields usually do not require a client release. Breaking request-contract changes should be tracked in issues and covered by tests before release.

## Install

Build and install from a local checkout:

```bash
git clone https://github.com/SebastianBoehler/bybit-cpp-client.git
cd bybit-cpp-client
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
cmake --install build --prefix dist
```

Enable the optional WebSocket client:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DBYBIT_ENABLE_WEBSOCKET=ON
cmake --build build
```

## Use via FetchContent

```cmake
include(FetchContent)

FetchContent_Declare(
  bybit_cpp_client
  GIT_REPOSITORY https://github.com/SebastianBoehler/bybit-cpp-client.git
  GIT_TAG v0.1.0
)
FetchContent_MakeAvailable(bybit_cpp_client)

add_executable(app main.cpp)
target_link_libraries(app PRIVATE bybit_client)
```

Pin `GIT_TAG` to a release, commit SHA, or `main`, depending on how much change you want to accept.

## Minimal REST Usage

```cpp
#include <bybit/rest_client.hpp>

#include <iostream>

int main() {
  bybit::RestClient client{"YOUR_KEY", "YOUR_SECRET", "linear"};

  try {
    std::cout << client.get_instruments_info() << "\n";
    std::cout << client.get_trade_history({{"symbol", "BTCUSDT"}, {"limit", "100"}}) << "\n";
    std::cout << client.get_realtime_orders({{"orderLinkId", "my-order-link-id"}}) << "\n";
    std::cout << client.get_order_history({{"orderLinkId", "my-order-link-id"}}) << "\n";
  } catch (const std::exception& ex) {
    std::cerr << "Bybit request failed: " << ex.what() << "\n";
    return 1;
  }
}
```

Configure transport behavior when latency budgets or infrastructure require it:

```cpp
bybit::HttpOptions options;
options.connect_timeout_ms = 1000;
options.request_timeout_ms = 3000;
options.dns_cache_timeout_seconds = 600;
options.tcp_keepalive = true;
options.user_agent = "my-trading-service/1.0";

bybit::RestClient client{"YOUR_KEY", "YOUR_SECRET", "linear", options};
```

## Minimal WebSocket Usage

```cpp
#include <bybit/websocket_client.hpp>

#include <iostream>

int main() {
  bybit::WebSocketClient ws{"wss://stream.bybit.com/v5/public/linear"};

  ws.set_message_handler([](const std::string& message) {
    std::cout << message << "\n";
  });

  ws.connect();
  ws.subscribe({"tickers.BTCUSDT"}, "sub-1");
}
```

## Examples

| Example | Purpose |
| --- | --- |
| `examples/basic_linear.cpp` | Basic authenticated linear REST flow |
| `examples/basic_spot.cpp` | Basic spot REST flow |
| `examples/instruments_info.cpp` | Instrument metadata and lot specs |
| `examples/market_kline.cpp` | Kline retrieval |
| `examples/position_info.cpp` | Position query |
| `examples/wallet_balance.cpp` | Wallet balance query |
| `examples/limit_order.cpp` | Order placement example |
| `examples/ws_market_data.cpp` | Public WebSocket market stream |
| `examples/ws_orderbook.cpp` | Public WebSocket orderbook stream |
| `examples/ws_private_positions.cpp` | Private WebSocket position stream |

Private REST and WebSocket examples expect credentials in the environment:

```bash
export BYBIT_API_KEY="..."
export BYBIT_API_SECRET="..."
export BYBIT_CATEGORY="linear"
```

Use `.env.example` as a local reference. Do not commit `.env` files or account data.

## Development

Run the normal build and test cycle:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
ctest --test-dir build --output-on-failure
```

Run the WebSocket build when touching stream code:

```bash
cmake -S . -B build-ws -DCMAKE_BUILD_TYPE=Debug -DBYBIT_ENABLE_WEBSOCKET=ON
cmake --build build-ws
ctest --test-dir build-ws --output-on-failure
```

Keep route methods thin. Shared signing, encoding, request dispatch, and category-specific details should live in reusable helpers rather than being duplicated across endpoints.

See [`docs/performance.md`](./docs/performance.md) for the current transport-performance model and [`ROADMAP.md`](./ROADMAP.md) for the path toward broader coverage and stronger library ergonomics.

## Repository Layout

| Path | Purpose |
| --- | --- |
| `include/bybit/` | Public C++ headers |
| `src/` | REST, signing, HTTP, and WebSocket implementations |
| `examples/` | Small runnable usage examples |
| `tests/` | Focused C++ tests |
| `cmake/` | CMake package config template |
| `docs/` | Generated or static project docs |

## Contributing

Contributions are welcome, especially small pull requests that improve endpoint coverage, request safety, tests, examples, or documentation.

Please read [`CONTRIBUTING.md`](./CONTRIBUTING.md), [`CODE_OF_CONDUCT.md`](./CODE_OF_CONDUCT.md), and [`SECURITY.md`](./SECURITY.md) before opening larger changes. For API additions, link the relevant Bybit documentation, keep files modular, and add tests that can run without live credentials where possible.

## Security

This project handles exchange API keys and signed trading requests. Never commit secrets, `.env` files, logs containing headers, account exports, or raw private WebSocket payloads.

Withdrawal creation is exposed as `create_withdrawal(json_body)` so callers can pass Bybit's exact typed JSON, including integer fields and nested travel-rule objects. Use whitelisted addresses, idempotent `requestId` values, and environment-specific keys with the minimum required permissions.

If you find a vulnerability or credential exposure, follow [`SECURITY.md`](./SECURITY.md) and report it privately first.

## License

This repository is licensed under the Apache License 2.0. See [`LICENSE`](./LICENSE).
