# Roadmap

This project should become a complete, serious C++ Bybit Open API V5 library: broad endpoint coverage, low-overhead transport, clear documentation, and a contribution path that makes outside help practical.

## Near Term

- Add transport options for timeouts, keepalive, DNS cache TTL, proxy, and user agent.
- Add current-shape support for `POST /v5/position/move-positions`.
- Add richer order placement options without making simple market orders harder to use.
- Add structured request builders for high-risk endpoints while preserving raw JSON response access.
- Expand public tests that do not require credentials.

## API Coverage

- Complete the remaining high-value Trade and Position endpoints first.
- Add Account endpoints needed for collateral, borrow state, repayment, and Greeks.
- Add Asset transfer, deposit, and withdrawal endpoints behind clear safety docs.
- Add User and subaccount management endpoints.
- Add product-specific APIs only when they have a clear caller or example.

See [`docs/api_coverage.md`](./docs/api_coverage.md) for the detailed map.

## Performance

- Keep HTTP connections warm by default.
- Benchmark cold versus warm request latency.
- Add optional shared libcurl state for multi-client deployments.
- Evaluate HTTP/2 support when the upstream behavior and libcurl build make it useful.
- Keep route methods thin so performance work happens in shared transport and signing code.

## Project Quality

- Add issue templates and a pull request template.
- Add generated API reference docs.
- Add sanitizer CI jobs.
- Add clang-format installation to CI and document local setup.
- Add release checklist automation.

## Design Principle

Prefer a small, predictable core over clever wrappers. The library should expose Bybit behavior faithfully, improve the transport and ergonomics around it, and avoid hidden fallbacks or mock data in production paths.
