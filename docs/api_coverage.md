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
| `POST /v5/order/pre-check` | `pre_check_order(...)` |
| `POST /v5/order/amend` | `amend_order(...)` |
| `POST /v5/order/cancel` | `cancel_order(...)` |
| `GET /v5/order/realtime` | `get_open_orders(...)`, `get_realtime_orders(...)` |
| `POST /v5/order/cancel-all` | `cancel_all(...)`, `cancel_all_orders(...)` |
| `POST /v5/order/disconnected-cancel-all` | `set_disconnect_cancel_all(...)` |
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
| `POST /v5/position/set-auto-add-margin` | `set_auto_add_margin(...)` |
| `POST /v5/position/add-margin` | `add_margin(...)` |
| `GET /v5/position/closed-pnl` | `get_closed_pnl(...)` |
| `GET /v5/position/get-closed-positions` | `get_closed_options_positions(...)` |
| `POST /v5/position/confirm-pending-mmr` | `confirm_pending_mmr(...)` |
| `POST /v5/position/move-positions` | `move_positions(...)` |
| `GET /v5/position/move-history` | `get_move_position_history(...)` |

### Account and User

| Endpoint | Method |
| --- | --- |
| `GET /v5/user/query-api` | `get_query_api_key()` |
| `POST /v5/user/create-demo-member` | `create_demo_account()` |
| `GET /v5/user/query-sub-members` | `get_sub_uid_list()` |
| `POST /v5/user/create-sub-member` | `create_sub_uid(...)` |
| `POST /v5/user/del-submember` | `delete_sub_uid(...)` |
| `POST /v5/user/frozen-sub-member` | `freeze_sub_uid(...)` |
| `POST /v5/user/create-sub-api` | `create_sub_api_key(...)` |
| `GET /v5/user/sub-apikeys` | `get_sub_api_keys(...)` |
| `POST /v5/user/update-api` | `update_master_api_key(...)` |
| `POST /v5/user/delete-api` | `delete_master_api_key()` |
| `POST /v5/user/update-sub-api` | `update_sub_api_key(...)` |
| `POST /v5/user/delete-sub-api` | `delete_sub_api_key(...)` |
| `GET /v5/user/get-member-type` | `get_uid_wallet_type(...)` |
| `GET /v5/user/aff-customer-info` | `get_affiliate_user_info(...)` |
| `GET /v5/account/info` | `get_account_info()` |
| `POST /v5/account/upgrade-to-uta` | `upgrade_to_unified_account()` |
| `POST /v5/account/demo-apply-money` | `request_demo_trading_funds(...)` |
| `GET /v5/account/instruments-info` | `get_account_instruments_info(...)` |
| `GET /v5/account/wallet-balance` | `get_wallet_balance(...)` |
| `GET /v5/account/withdrawal` | `get_transferable_amount(...)` |
| `GET /v5/account/transaction-log` | `get_transaction_log(...)` |
| `GET /v5/account/fee-rate` | `get_fee_rate()` |
| `GET /v5/account/collateral-info` | `get_collateral_info(...)` |
| `POST /v5/account/set-collateral-switch` | `set_collateral_coin(...)` |
| `POST /v5/account/set-collateral-switch-batch` | `batch_set_collateral_coins(...)` |
| `GET /v5/account/borrow-history` | `get_borrow_history(...)` |
| `POST /v5/account/borrow` | `manual_borrow(...)` |
| `POST /v5/account/repay` | `manual_repay(...)` |
| `POST /v5/account/no-convert-repay` | `no_convert_repay(...)` |
| `POST /v5/account/quick-repayment` | `quick_repayment(...)` |
| `GET /v5/asset/coin-greeks` | `get_coin_greeks(...)` |
| `GET /v5/account/query-dcp-info` | `get_dcp_info()` |
| `POST /v5/account/set-margin-mode` | `switch_margin_mode(...)` |
| `POST /v5/account/set-hedging-mode` | `set_spot_hedging(...)` |
| `GET /v5/account/mmp-state` | `get_mmp_state(...)` |
| `POST /v5/account/mmp-reset` | `reset_mmp(...)` |
| `POST /v5/account/mmp-modify` | `set_mmp(...)` |
| `GET /v5/account/smp-group` | `get_smp_group()` |
| `GET /v5/account/user-setting-config` | `get_trade_behaviour_config()` |
| `POST /v5/account/set-delta-mode` | `set_delta_neutral_mode(...)` |
| `POST /v5/account/set-limit-px-action` | `set_price_limit_behaviour(...)` |
| `GET /v5/account/option-asset-info` | `get_option_asset_info()` |
| `GET /v5/account/pay-info` | `get_pay_info(...)` |
| `GET /v5/account/trade-info-for-analysis` | `get_trade_info_for_analysis(...)` |

### Asset

| Endpoint | Method |
| --- | --- |
| `GET /v5/asset/transfer/query-asset-info` | `get_asset_info(...)` |
| `GET /v5/asset/asset-overview` | `get_asset_overview(...)` |
| `GET /v5/asset/portfolio-margin` | `get_portfolio_margin_info(...)` |
| `GET /v5/asset/total-members-assets` | `get_total_members_assets(...)` |
| `GET /v5/asset/fundinghistory` | `get_funding_account_history(...)` |
| `GET /v5/asset/delivery-record` | `get_delivery_records(...)` |
| `GET /v5/asset/settlement-record` | `get_settlement_records(...)` |
| `GET /v5/asset/exchange/order-record` | `get_coin_exchange_records(...)` |
| `GET /v5/asset/exchange/query-coin-list` | `get_convert_coin_list(...)` |
| `POST /v5/asset/exchange/quote-apply` | `request_convert_quote(...)` |
| `POST /v5/asset/exchange/convert-execute` | `confirm_convert_quote(...)` |
| `GET /v5/asset/exchange/convert-result-query` | `get_convert_status(...)` |
| `GET /v5/asset/exchange/query-convert-history` | `get_convert_history(...)` |
| `GET /v5/asset/covert/small-balance-list` | `get_small_balance_coins(...)` |
| `POST /v5/asset/covert/get-quote` | `request_small_balance_quote(...)` |
| `POST /v5/asset/covert/small-balance-execute` | `confirm_small_balance_quote(...)` |
| `GET /v5/asset/covert/small-balance-history` | `get_small_balance_history(...)` |
| `GET /v5/fiat/query-coin-list` | `get_fiat_convert_coin_list(...)` |
| `GET /v5/fiat/reference-price` | `get_fiat_reference_price(...)` |
| `POST /v5/fiat/quote-apply` | `request_fiat_quote(...)` |
| `POST /v5/fiat/trade-execute` | `confirm_fiat_quote(...)` |
| `GET /v5/fiat/trade-query` | `get_fiat_convert_status(...)` |
| `GET /v5/fiat/query-trade-history` | `get_fiat_convert_history(...)` |
| `GET /v5/fiat/balance-query` | `get_fiat_balance(...)` |
| `GET /v5/asset/transfer/query-sub-member-list` | `get_asset_sub_uids()` |
| `GET /v5/asset/transfer/query-account-coins-balance` | `get_all_coin_balances(...)` |
| `GET /v5/asset/transfer/query-account-coin-balance` | `get_single_coin_balance(...)` |
| `GET /v5/asset/transfer/query-transfer-coin-list` | `get_transferable_coins(...)` |
| `POST /v5/asset/transfer/inter-transfer` | `create_internal_transfer(...)` |
| `GET /v5/asset/transfer/query-inter-transfer-list` | `get_internal_transfer_records(...)` |
| `POST /v5/asset/transfer/universal-transfer` | `create_universal_transfer(...)` |
| `GET /v5/asset/transfer/query-universal-transfer-list` | `get_universal_transfer_records(...)` |
| `GET /v5/asset/deposit/query-record` | `get_deposit_records(...)` |
| `GET /v5/asset/deposit/query-sub-member-record` | `get_sub_deposit_records(...)` |
| `GET /v5/asset/deposit/query-internal-record` | `get_internal_deposit_records(...)` |
| `GET /v5/asset/deposit/query-allowed-list` | `get_allowed_deposit_coin_info(...)` |
| `POST /v5/asset/deposit/deposit-to-account` | `set_deposit_account(...)` |
| `GET /v5/asset/deposit/query-address` | `get_master_deposit_address(...)` |
| `GET /v5/asset/deposit/query-sub-member-address` | `get_sub_deposit_address(...)` |
| `GET /v5/asset/coin/query-info` | `get_coin_info(...)` |
| `GET /v5/asset/withdraw/withdrawable-amount` | `get_withdrawable_amount(...)` |
| `GET /v5/asset/withdraw/query-address` | `get_withdrawal_address_list(...)` |
| `GET /v5/asset/withdraw/vasp/list` | `get_available_vasps()` |
| `GET /v5/asset/withdraw/query-record` | `get_withdrawal_records(...)` |
| `POST /v5/asset/withdraw/create` | `create_withdrawal(...)` |
| `POST /v5/asset/withdraw/cancel` | `cancel_withdrawal(...)` |

### Spot Margin Trade

| Endpoint | Method |
| --- | --- |
| `GET /v5/spot-margin-trade/data` | `get_spot_margin_vip_data(...)` |
| `GET /v5/spot-margin-trade/currency-data` | `get_spot_margin_currency_data(...)` |
| `GET /v5/spot-margin-trade/state` | `get_spot_margin_state()` |
| `POST /v5/spot-margin-trade/switch-mode` | `switch_spot_margin_mode(...)` |
| `POST /v5/spot-margin-trade/set-leverage` | `set_spot_margin_leverage(...)` |
| `GET /v5/spot-margin-trade/max-borrowable` | `get_spot_margin_max_borrowable(...)` |
| `GET /v5/spot-margin-trade/position-tiers` | `get_spot_margin_position_tiers(...)` |
| `GET /v5/spot-margin-trade/coinstate` | `get_spot_margin_coin_state(...)` |
| `GET /v5/spot-margin-trade/repayment-available-amount` | `get_spot_margin_repayment_available_amount(...)` |
| `POST /v5/spot-margin-trade/set-auto-repay-mode` | `set_spot_margin_auto_repay_mode(...)` |
| `GET /v5/spot-margin-trade/get-auto-repay-mode` | `get_spot_margin_auto_repay_mode(...)` |
| `GET /v5/spot-margin-trade/interest-rate-history` | `get_spot_margin_interest_rate_history(...)` |
| `GET /v5/spot-margin-trade/collateral` | `get_spot_margin_tiered_collateral_ratio(...)` |
| `GET /v5/spot-margin-trade/fixedborrow-order-quote` | `get_spot_margin_fixed_borrow_order_quote(...)` |
| `POST /v5/spot-margin-trade/fixedborrow` | `create_spot_margin_fixed_borrow(...)` |
| `POST /v5/spot-margin-trade/fixedborrow-renew` | `renew_spot_margin_fixed_borrow(...)` |
| `GET /v5/spot-margin-trade/fixedborrow-order-info` | `get_spot_margin_fixed_borrow_order_info(...)` |
| `GET /v5/spot-margin-trade/fixedborrow-contract-info` | `get_spot_margin_fixed_borrow_contract_info(...)` |
| `GET /v5/spot-margin-trade/liability` | `get_spot_margin_liability(...)` |

### Pre-upgrade

| Endpoint | Method |
| --- | --- |
| `GET /v5/pre-upgrade/order/history` | `get_pre_upgrade_order_history(...)` |
| `GET /v5/pre-upgrade/execution/list` | `get_pre_upgrade_trade_history(...)` |
| `GET /v5/pre-upgrade/position/closed-pnl` | `get_pre_upgrade_closed_pnl(...)` |
| `GET /v5/pre-upgrade/account/transaction-log` | `get_pre_upgrade_transaction_log(...)` |
| `GET /v5/pre-upgrade/asset/delivery-record` | `get_pre_upgrade_delivery_records(...)` |
| `GET /v5/pre-upgrade/asset/settlement-record` | `get_pre_upgrade_settlement_records(...)` |

### Affiliate

| Endpoint | Method |
| --- | --- |
| `GET /v5/affiliate/aff-user-list` | `get_affiliate_user_list(...)` |
| `GET /v5/user/aff-customer-info` | `get_affiliate_user_info(...)` |

### Spread Trading

| Endpoint | Method |
| --- | --- |
| `GET /v5/spread/instrument` | `get_spread_instruments(...)` |
| `GET /v5/spread/orderbook` | `get_spread_orderbook(...)` |
| `GET /v5/spread/tickers` | `get_spread_tickers(...)` |
| `GET /v5/spread/recent-trade` | `get_spread_recent_trades(...)` |
| `POST /v5/spread/order/create` | `create_spread_order(...)` |
| `POST /v5/spread/order/amend` | `amend_spread_order(...)` |
| `POST /v5/spread/order/cancel` | `cancel_spread_order(...)` |
| `POST /v5/spread/order/cancel-all` | `cancel_all_spread_orders(...)` |
| `GET /v5/spread/order/realtime` | `get_spread_open_orders(...)` |
| `GET /v5/spread/order/history` | `get_spread_order_history(...)` |
| `GET /v5/spread/execution/list` | `get_spread_trade_history(...)` |
| `GET /v5/spread/max-qty` | `get_spread_max_order_qty(...)` |

### RFQ Trading

| Endpoint | Method |
| --- | --- |
| `POST /v5/rfq/create-rfq` | `create_rfq(...)` |
| `GET /v5/rfq/config` | `get_rfq_config()` |
| `POST /v5/rfq/cancel-rfq` | `cancel_rfq(...)` |
| `POST /v5/rfq/cancel-all-rfq` | `cancel_all_rfqs()` |
| `POST /v5/rfq/accept-other-quote` | `accept_non_lp_quote(...)` |
| `POST /v5/rfq/create-quote` | `create_rfq_quote(...)` |
| `POST /v5/rfq/execute-quote` | `execute_rfq_quote(...)` |
| `POST /v5/rfq/cancel-quote` | `cancel_rfq_quote(...)` |
| `POST /v5/rfq/cancel-all-quotes` | `cancel_all_rfq_quotes()` |
| `GET /v5/rfq/rfq-realtime` | `get_rfqs_realtime(...)` |
| `GET /v5/rfq/rfq-list` | `get_rfqs(...)` |
| `GET /v5/rfq/quote-realtime` | `get_rfq_quotes_realtime(...)` |
| `GET /v5/rfq/quote-list` | `get_rfq_quotes(...)` |
| `GET /v5/rfq/trade-list` | `get_rfq_trade_history(...)` |
| `GET /v5/rfq/public-trades` | `get_rfq_public_trades(...)` |

## Partially Covered or Needs Review

| Area | Status |
| --- | --- |
| Legacy Move Position | `move_position(...)` is retained for compatibility with the old singular path. Prefer `move_positions(...)` for the current Bybit V5 endpoint. |
| WebSocket | Public/private WebSocket support exists behind `BYBIT_ENABLE_WEBSOCKET`, but this coverage file tracks REST first. |

## Not Yet Covered

High-value REST gaps:

- remaining Asset endpoints for broker-specific transfer controls

Product areas not in scope yet:
- Spot Margin normal-account endpoints and remaining niche account-mode operations
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
