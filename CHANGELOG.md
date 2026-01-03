# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

## [0.1.0] - 2024-01-04

### Added

- Initial release
- REST v5 API wrapper with public and private endpoints
- WebSocket v5 client (optional, behind `-DBYBIT_ENABLE_WEBSOCKET=ON`)
- HMAC-SHA256 signing with OpenSSL
- CMake build with FetchContent/submodule support
- Version header (`bybit/version.hpp`)
- GitHub Actions CI/CD with automated releases

### Features

- `RestClient` facade for simple usage
- `PublicRestClient` for unauthenticated endpoints
- `PrivateRestClient` for authenticated endpoints
- `WebSocketClient` with auto-reconnect and ping/pong
- Support for linear, spot, and other Bybit categories

[Unreleased]: https://github.com/SebastianBoehler/bybit-cpp-client/compare/v0.1.0...HEAD
[0.1.0]: https://github.com/SebastianBoehler/bybit-cpp-client/releases/tag/v0.1.0
