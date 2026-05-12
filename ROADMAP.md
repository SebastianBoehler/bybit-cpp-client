# Roadmap

This project should become a complete, serious C++ Bybit Open API V5 library: broad endpoint coverage, low-overhead transport, clear documentation, and a contribution path that makes outside help practical.

## Near Term

- Add structured request builders for high-risk endpoints while preserving raw JSON response access.
- Expand public tests that do not require credentials.

## API Coverage

- Complete the remaining high-value Trade and Position endpoints first.
- Add remaining Account and Spot Margin endpoints for upgrade/demo flows, portfolio margin info, normal-account margin flows, and account-mode-specific operations.
- Add remaining Asset endpoints for small-balance convert, fiat-convert, and broker-specific transfer controls.
- Add any newly released User endpoints beyond subaccount and API-key management.
- Add product-specific APIs only when they have a clear caller or example.

See [`docs/api_coverage.md`](./docs/api_coverage.md) for the detailed map.

## Performance

- Keep HTTP connections warm by default.
- Benchmark cold versus warm request latency.
- Add optional shared libcurl state for multi-client deployments.
- Evaluate HTTP/2 support when the upstream behavior and libcurl build make it useful.
- Keep route methods thin so performance work happens in shared transport and signing code.

## Project Quality

- Add generated API reference docs.
- Add sanitizer CI jobs.
- Add clang-format installation to CI and document local setup.
- Add release checklist automation.

## Design Principle

Prefer a small, predictable core over clever wrappers. The library should expose Bybit behavior faithfully, improve the transport and ergonomics around it, and avoid hidden fallbacks or mock data in production paths.
