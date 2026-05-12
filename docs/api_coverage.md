# Bybit V5 API Coverage

Checked against the official Bybit Open API V5 documentation on 2026-05-12.

This repository intentionally focuses on the trading wrapper surface first. It is not a complete mirror of every Bybit product API.

## Covered

### Market

| Endpoint | Method |
| --- | --- |
| `GET /v5/market/time` | `get_server_time()` |
| `GET /v5/market/instruments-info` | `get_instruments_info(...)` |
| `GET /v5/market/tickers` | `get_tickers(...)` |
| `GET /v5/market/orderbook` | `get_orderbook(...)` |
| `GET /v5/market/kline` | `get_kline(...)` |
| `GET /v5/market/mark-price-kline` | `get_mark_price_kline(...)` |
| `GET /v5/market/index-price-kline` | `get_index_price_kline(...)` |
| `GET /v5/market/premium-index-price-kline` | `get_premium_index_price_kline(...)` |
| `GET /v5/market/recent-trade` | `get_recent_trades(...)` |
| `GET /v5/market/funding/history` | `get_funding_history(...)` |
| `GET /v5/market/open-interest` | `get_open_interest(...)` |
| `GET /v5/market/account-ratio` | `get_long_short_ratio(...)` |
| `GET /v5/market/risk-limit` | `get_risk_limit(...)` |

### Trade

| Endpoint | Method |
| --- | --- |
| `POST /v5/order/create` | `submit_order(...)` |
| `POST /v5/order/amend` | `amend_order(...)` |
| `POST /v5/order/cancel` | `cancel_order(...)` |
| `GET /v5/order/realtime` | `get_open_orders(...)`, `get_realtime_orders(...)` |
| `POST /v5/order/cancel-all` | `cancel_all(...)` |
| `GET /v5/order/history` | `get_historic_orders(...)`, `get_order_history(...)` |
| `GET /v5/execution/list` | `get_trade_history(...)` |
| `POST /v5/order/create-batch` | `batch_submit_orders(...)` |
| `POST /v5/order/amend-batch` | `batch_amend_orders(...)` |
| `POST /v5/order/cancel-batch` | `batch_cancel_orders(...)` |
| `GET /v5/order/spot-borrow-check` | `get_borrow_quota(...)` |

### Position

| Endpoint | Method |
| --- | --- |
| `GET /v5/position/list` | `get_position_info(...)` |
| `POST /v5/position/set-leverage` | `set_leverage(...)` |
| `POST /v5/position/switch-mode` | `switch_position_mode(...)` |
| `POST /v5/position/trading-stop` | `set_trading_stop(...)` |
| `POST /v5/position/set-risk-limit` | `set_risk_limit(...)` |
| `POST /v5/position/add-margin` | `add_margin(...)` |
| `GET /v5/position/closed-pnl` | `get_closed_pnl(...)` |
| `POST /v5/position/move-positions` | `move_positions(...)` |
| `GET /v5/position/move-history` | `get_move_position_history(...)` |

### Account and User

| Endpoint | Method |
| --- | --- |
| `GET /v5/user/query-api` | `get_query_api_key()` |
| `GET /v5/account/info` | `get_account_info()` |
| `GET /v5/account/wallet-balance` | `get_wallet_balance(...)` |
| `GET /v5/account/transaction-log` | `get_transaction_log(...)` |
| `GET /v5/account/fee-rate` | `get_fee_rate()` |
| `POST /v5/account/set-margin-mode` | `switch_margin_mode(...)` |

## Partially Covered or Needs Review

| Area | Status |
| --- | --- |
| Legacy Move Position | `move_position(...)` is retained for compatibility with the old singular path. Prefer `move_positions(...)` for the current Bybit V5 endpoint. |
| WebSocket | Public/private WebSocket support exists behind `BYBIT_ENABLE_WEBSOCKET`, but this coverage file tracks REST first. |

## Not Yet Covered

High-value REST gaps:

- `POST /v5/order/pre-check`
- `POST /v5/order/disconnected-cancel-all`
- `POST /v5/position/set-auto-add-margin`
- `POST /v5/position/confirm-pending-mmr`
- broader Account endpoints such as collateral, borrow history, coin greeks, MMP, and repayment
- Asset transfer, deposit, and withdrawal endpoints

Product areas not in scope yet:

- Spot Margin Trade
- Asset
- full User and subaccount management
- Spread Trading
- RFQ Trading
- Affiliate
- Crypto Loan and Institutional Loan
- Broker
- Finance
- Bybit Card
- Web3
- SBE

## Selection Rule

Prefer adding endpoints that help trading clients make safer decisions:

1. account state and permissions
2. order lifecycle
3. execution history and fees
4. position lifecycle and PnL
5. instrument metadata and risk limits

Add narrower product APIs only when there is a concrete caller or example that needs them.
