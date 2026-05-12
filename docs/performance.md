# Performance Notes

This client aims to be more than a direct endpoint wrapper. The transport layer should make good default choices for low-latency trading services while staying predictable and easy to audit.

## Current Behavior

`HttpClient` owns one persistent libcurl easy handle for the lifetime of the client.

That matters because libcurl can reuse state attached to an easy handle between requests:

- warm TCP connections
- TLS session state
- DNS cache entries
- socket keepalive settings

The handle is protected by a mutex. A single `RestClient` instance is therefore safe to call from multiple threads, but requests through that instance are serialized. This avoids unsafe concurrent use of one libcurl easy handle and keeps private request signing close to the actual network dispatch so `recvWindow` timestamps do not become stale while waiting for the handle.

## Practical Guidance

- Reuse a `RestClient` instead of constructing one per request.
- Use separate `RestClient` instances if you need parallel HTTP requests.
- Prefer one client per API key, base URL, and category.
- Keep `recvWindow` tight for trading systems, but large enough for your network conditions.

## Planned Work

- Add a configurable transport options object for timeouts, TCP keepalive, DNS cache TTL, proxy, and user agent.
- Add optional shared DNS/connection state across multiple clients where libcurl share handles are appropriate.
- Add benchmark examples for cold versus warm request latency.
- Add richer error types with HTTP status, Bybit `retCode`, and response body access.
- Add testnet integration tests that can be enabled explicitly with credentials.
