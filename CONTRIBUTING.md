# Contributing

Thanks for considering a contribution.

This repository is a C++ client for Bybit Open API V5. The best contributions are small, testable, and explicit about which endpoint, request contract, or user-facing behavior they change.

## Before You Start

Please read:

- [`README.md`](./README.md)
- [`LICENSE`](./LICENSE)
- [`CODE_OF_CONDUCT.md`](./CODE_OF_CONDUCT.md)
- [`SECURITY.md`](./SECURITY.md)

If your change touches a live Bybit endpoint, include a link to the relevant official documentation and mention whether you verified the flow locally, in testnet, or by unit tests only.

## Ways to Contribute

- add support for a stable Bybit V5 REST endpoint
- improve request signing, encoding, transport behavior, or error handling
- add focused unit tests around request construction and signing
- add integration examples that use testnet-safe flows
- improve CMake packaging and consumer ergonomics
- update documentation when Bybit changes an endpoint contract

## License and Contributor Terms

This repository is open source under the Apache License 2.0.

Unless you explicitly state otherwise, any contribution intentionally submitted for inclusion in the project is provided under the Apache License 2.0 terms, as described in Section 5 of [`LICENSE`](./LICENSE). No separate CLA is required for normal pull requests.

## Development Setup

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
ctest --test-dir build --output-on-failure
```

Build with optional WebSocket support when touching stream code:

```bash
cmake -S . -B build-ws -DCMAKE_BUILD_TYPE=Debug -DBYBIT_ENABLE_WEBSOCKET=ON
cmake --build build-ws
ctest --test-dir build-ws --output-on-failure
```

## Credentials and Local Data

- never commit API keys, API secrets, `.env` files, account exports, private WebSocket payloads, or logs containing signed headers
- prefer Bybit testnet for manual verification
- keep live trading examples opt-in and clearly documented
- return clear errors for missing credentials or upstream failures
- do not add mock fallback data unless a test explicitly needs local fixtures

## Change Guidelines

Please keep pull requests:

- focused on one logical change
- backed by tests when behavior changes
- consistent with the existing public/private REST split
- careful with category-specific behavior
- small enough to review without reconstructing unrelated context

When adding or changing an endpoint:

- link the official Bybit V5 docs
- keep route methods thin and delegate signing/encoding/dispatch to shared helpers
- preserve deterministic request behavior
- document any required credentials or account modes
- add tests near the changed client or helper

## PR Checklist

Before opening a PR, make sure:

- relevant tests pass locally
- new behavior is documented when needed
- secrets and local artifacts are not included
- the PR description explains the problem, approach, and validation

Useful PR descriptions usually answer:

1. What endpoint or behavior changed?
2. What request or response contract matters?
3. How was the change validated?

## Review Expectations

Review will generally focus on:

- correctness against Bybit Open API V5
- regression risk for existing endpoint wrappers
- test coverage
- maintainability and modularity
- safe handling of credentials and signed requests
