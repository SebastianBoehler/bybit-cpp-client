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
| Subaccounts | `GET/POST /v5/user/*sub*` | `get_sub_uid_list()`, `create_sub_uid(...)`, `freeze_sub_uid(...)`, `delete_sub_uid(...)` |
| API key management | `POST /v5/user/*api` | `create_sub_api_key(...)`, `update_master_api_key(...)`, `update_sub_api_key(...)` |
| User wallet and affiliate info | `GET /v5/user/get-member-type`, `GET /v5/user/aff-customer-info` | `get_uid_wallet_type(...)`, `get_affiliate_user_info(...)` |
| Affiliate user list | `GET /v5/affiliate/aff-user-list` | `get_affiliate_user_list(...)` |
| Spot margin state and leverage | `GET/POST /v5/spot-margin-trade/*` | `get_spot_margin_state()`, `switch_spot_margin_mode(...)`, `set_spot_margin_leverage(...)` |
| Spot margin rates and collateral | `GET /v5/spot-margin-trade/data`, `GET /v5/spot-margin-trade/collateral`, `GET /v5/spot-margin-trade/currency-data` | `get_spot_margin_vip_data(...)`, `get_spot_margin_tiered_collateral_ratio(...)`, `get_spot_margin_currency_data(...)` |
| Spot margin borrow and liability | `GET/POST /v5/spot-margin-trade/*borrow*`, `GET /v5/spot-margin-trade/liability` | `get_spot_margin_fixed_borrow_order_quote(...)`, `create_spot_margin_fixed_borrow(...)`, `get_spot_margin_liability(...)` |
| Spot margin repayment data | `GET/POST /v5/spot-margin-trade/*repay*`, `GET /v5/spot-margin-trade/repayment-available-amount` | `get_spot_margin_repayment_available_amount(...)`, `set_spot_margin_auto_repay_mode(...)` |
| Spread Trading | `GET/POST /v5/spread/*` | `get_spread_instruments(...)`, `create_spread_order(...)`, `get_spread_trade_history(...)` |
| RFQ Trading | `GET/POST /v5/rfq/*` | `create_rfq(...)`, `create_rfq_quote(...)`, `get_rfq_trade_history(...)` |
| Exchange Broker | `GET/POST /v5/broker/*` | `get_broker_earnings(...)`, `set_broker_rate_limit(...)`, `issue_broker_voucher(...)` |
| Bybit Card | `POST /v5/card/*` | `get_card_transaction_records(...)`, `get_card_point_balance()`, `get_card_mall_items(...)` |
| Earn / Advanced Earn | `GET/POST /v5/earn/*` | `get_earn_product_info(...)`, `get_advanced_earn_product_quote(...)`, `add_advanced_earn_liquidity_mining_liquidity(...)` |
| Web3 Alpha | `POST /v5/alpha/trade/*` | `get_web3_trade_quote(...)`, `execute_web3_purchase(...)`, `get_web3_order_list(...)` |
| Crypto Loan (New) | `GET/POST /v5/crypto-loan-*` | `get_crypto_loan_position()`, `borrow_flexible_crypto_loan(...)`, `borrow_fixed_crypto_loan(...)` |
| Institutional Loan | `GET/POST /v5/ins-loan/*` | `get_institutional_loan_ltv()`, `get_institutional_loan_orders(...)`, `repay_institutional_loan(...)` |
| Account margin mode | `GET /v5/account/info` | `get_account_info()` |
| Account upgrade and demo trading | `POST /v5/account/upgrade-to-uta`, `POST /v5/account/demo-apply-money`, `POST /v5/user/create-demo-member` | `upgrade_to_unified_account()`, `request_demo_trading_funds(...)`, `create_demo_account()` |
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
| Asset portfolio overview | `GET /v5/asset/asset-overview`, `GET /v5/asset/portfolio-margin` | `get_asset_overview(...)`, `get_portfolio_margin_info(...)` |
| Asset transfers | `POST /v5/asset/transfer/*transfer` | `create_internal_transfer(...)`, `create_universal_transfer(...)` |
| Asset history | `GET /v5/asset/fundinghistory`, `GET /v5/asset/delivery-record`, `GET /v5/asset/settlement-record` | `get_funding_account_history(...)`, `get_delivery_records(...)`, `get_settlement_records(...)` |
| Asset conversion | `GET/POST /v5/asset/exchange/*` | `get_convert_coin_list(...)`, `request_convert_quote(...)`, `confirm_convert_quote(...)`, `get_convert_status(...)`, `get_convert_history(...)` |
| Small-balance and fiat conversion | `GET/POST /v5/asset/covert/*`, `GET/POST /v5/fiat/*` | `request_small_balance_quote(...)`, `confirm_small_balance_quote(...)`, `request_fiat_quote(...)`, `confirm_fiat_quote(...)` |
| Deposit records and settings | `GET/POST /v5/asset/deposit/*` | `get_deposit_records(...)`, `get_sub_deposit_records(...)`, `set_deposit_account(...)` |
| Coin and withdrawal info | `GET /v5/asset/coin/query-info`, `GET /v5/asset/withdraw/*` | `get_coin_info(...)`, `get_withdrawable_amount(...)`, `get_withdrawal_records(...)` |
| Withdrawal address and lifecycle | `GET/POST /v5/asset/withdraw/*` | `get_withdrawal_address_list(...)`, `create_withdrawal(...)`, `cancel_withdrawal(...)` |
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
| Pre-upgrade history | `GET /v5/pre-upgrade/*` | `get_pre_upgrade_order_history(...)`, `get_pre_upgrade_trade_history(...)`, `get_pre_upgrade_closed_pnl(...)` |
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
- Asset endpoints now cover core balances, portfolio overview, transfers, funding/delivery/settlement history, exchange records, crypto convert quote/execute/status/history, small-balance convert, fiat convert, deposit records/settings, deposit addresses, coin metadata, withdrawable amount, VASP metadata, withdrawal addresses, withdrawal records, withdrawal creation, and cancellation.
- User endpoints now cover sub UID management, sub API key management, master API key mutation/deletion, UID wallet type, and affiliate user info.
- Demo trading support now covers demo-account creation and demo-funds requests; use `https://api-demo.bybit.com` for demo-account-key calls.
- Spot Margin Trade now covers UTA margin state, leverage, public VIP data, currency data, historical rates, tiered collateral ratio, position tiers, coin state, max borrowable amount, repayment controls, fixed-rate borrow lifecycle, and liability info.
- Crypto Loan (New) now covers common loan data, collateral adjustment, flexible loans, fixed borrow/supply orders, contract info, renewals, and repayment history.
- Institutional Loan now covers product data, margin coin data, loan and repayment orders, LTV, UID association, and manual repayment.
- Exchange Broker now covers earnings, account info, subaccount deposits, rate-limit setup, and reward vouchers.
- Bybit Card now covers transaction records plus reward points, tiers, mall items, and cashback details.
- Earn now covers core product info, stake/redeem flows, yield history, Advanced Earn structured products, and liquidity-mining actions/history.
- Web3 Alpha now covers quote, purchase, redeem, token metadata, payment-token, order, asset-list, and asset-detail endpoints.

This client covers the core trading wrapper surface plus high-value Account, Asset, User, Affiliate, Exchange Broker, Bybit Card, Earn, Advanced Earn, Web3 Alpha, Spread Trading, RFQ Trading, Crypto Loan (New), Institutional Loan, Spot Margin Trade, Position, Trade, and Market methods. It does not yet wrap every Bybit V5 product category such as legacy Crypto Loan or SBE. See [`docs/api_coverage.md`](./docs/api_coverage.md) for the current coverage map.

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

Withdrawal creation and API-key mutation methods accept raw JSON bodies so callers can pass Bybit's exact typed JSON, including integer fields and nested objects. Use whitelisted addresses, idempotent `requestId` values where supported, and environment-specific keys with the minimum required permissions.

If you find a vulnerability or credential exposure, follow [`SECURITY.md`](./SECURITY.md) and report it privately first.

## License

This repository is licensed under the Apache License 2.0. See [`LICENSE`](./LICENSE).
