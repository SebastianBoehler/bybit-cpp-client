# Bybit V5 API Coverage

Checked against the official Bybit Open API V5 documentation on 2026-08-01.

## Covered

### Market

| Endpoint | Method |
| --- | --- |
| `GET /v5/market/time` | `get_server_time()` |
| `GET /v5/market/instruments-info` with symbol, symbolType, status, baseCoin, limit, and cursor filters | `get_instruments_info(...)` |
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
| Additional `GET /v5/market/*`, `GET /v5/announcements/index`, `GET /v5/system/status` data | `get_rpi_orderbook(...)`, `get_insurance(...)`, `get_announcements(...)`, `get_system_status(...)` |

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
| `GET/POST /v5/strategy/*` | `create_strategy_order(...)`, `get_strategy_list(...)`, `get_strategy_order_list(...)`, `stop_strategy(...)` |

### Position

| Endpoint | Method |
| --- | --- |
| `GET /v5/position/list` | `get_position_info(...)` |
| `POST /v5/position/set-leverage` | `set_leverage(...)` |
| `POST /v5/position/switch-mode` | `switch_position_mode(...)` |
| `POST /v5/position/trading-stop` | `set_trading_stop(...)` |
| `POST /v5/position/set-risk-limit` | `set_risk_limit(...)` (Bybit marks this endpoint abandoned and without effect) |
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
| `GET /v5/user/query-sub-members`, `GET /v5/user/submembers` | `get_sub_uid_list()`, `get_sub_uid_list_unlimited(...)` |
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
| `GET /v5/user/aff-customer-info`, `GET /v5/user/invitation/referrals` | `get_affiliate_user_info(...)`, `get_friend_referrals(...)` |
| `GET /v5/account/info` | `get_account_info()` |
| `POST /v5/account/upgrade-to-uta` | `upgrade_to_unified_account()` |
| `POST /v5/account/demo-apply-money` | `request_demo_trading_funds(...)` |
| `POST /v5/user/agreement` | `sign_agreement(...)` |
| `GET /v5/account/instruments-info` | `get_account_instruments_info(...)` |
| `GET /v5/account/wallet-balance` | `get_wallet_balance(...)` |
| `GET /v5/account/withdrawal` | `get_transferable_amount(...)` |
| `GET /v5/account/transaction-log` | `get_transaction_log(...)` |
| `GET /v5/account/fee-rate` | `get_fee_rate(...)` |
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
| `GET /v5/account/user-setting-config` | `get_trade_behaviour_config()`, `get_limit_price_behaviour()` |
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

| Area | Endpoints | Representative methods |
| --- | --- | --- |
| UTA state, risk, and repay | `GET/POST /v5/spot-margin-trade/*` | `get_spot_margin_state()`, `set_spot_margin_leverage(...)`, `get_spot_margin_max_borrowable(...)`, `set_spot_margin_auto_repay_mode(...)` |
| UTA rates and fixed borrow | `GET/POST /v5/spot-margin-trade/*` | `get_spot_margin_interest_rate_history(...)`, `get_spot_margin_tiered_collateral_ratio(...)`, `create_spot_margin_fixed_borrow(...)`, `get_spot_margin_liability(...)` |
| Normal account spot cross margin | `GET/POST /v5/spot-cross-margin-trade/*` | `get_spot_cross_margin_vip_data(...)`, `borrow_spot_cross_margin(...)`, `repay_spot_cross_margin(...)`, `switch_spot_cross_margin(...)` |

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

### Crypto Loan

| Area | Endpoints | Representative methods |
| --- | --- | --- |
| Common data | `GET /v5/crypto-loan-common/loanable-data`, `GET /v5/crypto-loan-common/collateral-data` | `get_crypto_loan_borrowable_coins(...)`, `get_crypto_loan_collateral_coins(...)` |
| Collateral and position | `GET/POST /v5/crypto-loan-common/*` | `adjust_crypto_loan_collateral(...)`, `get_crypto_loan_position()`, `get_crypto_loan_max_loan_amount(...)` |
| Flexible loans | `GET/POST /v5/crypto-loan-flexible/*` | `borrow_flexible_crypto_loan(...)`, `repay_flexible_crypto_loan(...)`, `get_flexible_crypto_loans(...)` |
| Fixed and legacy loans | `GET/POST /v5/crypto-loan-fixed/*`, `GET/POST /v5/crypto-loan/*` | `borrow_fixed_crypto_loan(...)`, `borrow_legacy_crypto_loan(...)`, `get_legacy_crypto_loan_unpaid_loans(...)` |

### Institutional Loan

| Endpoint | Method |
| --- | --- |
| `GET /v5/ins-loan/product-infos` | `get_institutional_loan_product_info(...)` |
| `GET /v5/ins-loan/ensure-tokens-convert` | `get_institutional_loan_margin_coin_info(...)` |
| `GET /v5/ins-loan/loan-order` | `get_institutional_loan_orders(...)` |
| `GET /v5/ins-loan/repaid-history` | `get_institutional_loan_repayment_orders(...)` |
| `GET /v5/ins-loan/ltv-convert` | `get_institutional_loan_ltv()` |
| `POST /v5/ins-loan/association-uid` | `bind_or_unbind_institutional_loan_uid(...)` |
| `POST /v5/ins-loan/repay-loan` | `repay_institutional_loan(...)` |

### Exchange Broker

| Area | Endpoints | Representative methods |
| --- | --- | --- |
| Account and deposits | `GET /v5/broker/earnings-info`, `GET /v5/broker/account-info`, `GET /v5/broker/asset/query-sub-member-deposit-record` | `get_broker_earnings(...)`, `get_broker_account_info()`, `get_broker_sub_deposit_records(...)` |
| Rate limits and rewards | `GET/POST /v5/broker/apilimit/*`, `POST /v5/broker/award/*` | `set_broker_rate_limit(...)`, `get_broker_rate_limits(...)`, `issue_broker_voucher(...)` |

### Bybit Card

| Area | Endpoints | Representative methods |
| --- | --- | --- |
| Transaction records | `POST /v5/card/transaction/query-asset-records` | `get_card_transaction_records(...)` |
| Reward points | `POST /v5/card/reward/points/*` | `get_card_point_balance()`, `get_card_point_records(...)`, `get_card_tier_info()` |
| Reward mall and cashback | `POST /v5/card/reward/mall/item/list`, `POST /v5/card/reward/point/cashback/detail` | `get_card_mall_items(...)`, `get_card_cashback_detail(...)` |

### Leveraged Tokens
Covered via `get_leverage_token_info(...)`, `get_leverage_token_market(...)`, `purchase_leverage_token(...)`, `redeem_leverage_token(...)`, and `get_leverage_token_order_records(...)`.

### Earn

| Area | Endpoints | Representative methods |
| --- | --- | --- |
| Core Earn | `GET /v5/earn/product`, `POST /v5/earn/place-order`, `GET /v5/earn/order` | `get_earn_product_info(...)`, `place_earn_order(...)`, `get_earn_order_history(...)` |
| Positions and yield | `GET /v5/earn/position`, `GET /v5/earn/yield`, `GET /v5/earn/hourly-yield` | `get_earn_position(...)`, `get_earn_yield_history(...)`, `get_earn_hourly_yield_history(...)` |
| Advanced Earn structured products | `GET/POST /v5/earn/advance/*` | `get_advanced_earn_product_info(...)`, `get_advanced_earn_product_quote(...)`, `place_advanced_earn_order(...)` |
| Advanced Earn liquidity mining | `GET/POST /v5/earn/liquidity-mining/*` | `get_advanced_earn_liquidity_mining_product_info(...)`, `add_advanced_earn_liquidity_mining_liquidity(...)`, `get_advanced_earn_liquidity_mining_yield_records(...)` |

### Web3 Alpha

| Endpoints | Representative methods |
| --- | --- |
| `POST /v5/alpha/trade/*` | `get_web3_trade_quote(...)`, `execute_web3_purchase(...)`, `get_web3_order_list(...)` |

## Partially Covered or Needs Review
Public/private WebSocket support exists behind `BYBIT_ENABLE_WEBSOCKET`, including current all-liquidation public topic helpers, private order/execution/position/wallet/Greeks topic helpers, SBE market topic helpers, binary send/payload delivery, market-data decoding for BBO, level-50 orderbook, public trades, explicit auth-acknowledgement gating for private subscriptions, SBE schema version 2 auth/ping/create/replace/cancel order encoders, batch create/replace/cancel order encoders, and auth/pong/single-order/batch/common-error response decoders.

Pre-1.0 contract migration: batch order inputs use `JsonObject` for native scalar types; risk IDs and position-mode values are integers; account margin mode uses only `setMarginMode`; the invalid singular move-position route is not exposed; and the removed SBE batch `created_at` field is not decoded.

## Not Yet Covered
Known gaps: no specific V5 REST account-mode endpoint gap is known after the 2026-08-01 account/API-explorer audit; keep monitoring the Bybit changelog for additive fields and new endpoints.
