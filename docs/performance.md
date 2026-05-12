# Performance Notes

This client aims to be more than a direct endpoint wrapper. The transport layer should make good default choices for low-latency trading services while staying predictable and easy to audit.

## Current Behavior

`HttpClient` owns one persistent libcurl easy handle for the lifetime of the client.

That matters because libcurl can reuse state attached to an easy handle between requests:

- warm per-client TCP connections
- TLS session state
- shared DNS and TLS session cache entries
- socket keepalive settings

The handle is protected by a mutex. A single `RestClient` instance is therefore safe to call from multiple threads, but requests through that instance are serialized. This avoids unsafe concurrent use of one libcurl easy handle and keeps private request signing close to the actual network dispatch so `recvWindow` timestamps do not become stale while waiting for the handle.

`HttpOptions` configures the transport without replacing the simple constructor:

```cpp
bybit::HttpOptions options;
options.connect_timeout_ms = 1000;
options.request_timeout_ms = 3000;
options.dns_cache_timeout_seconds = 600;
options.max_connections = 16;
options.share_dns_and_ssl_session_cache = true;
options.tcp_keepalive = true;
options.tcp_nodelay = true;
options.tcp_keepidle_seconds = 30;
options.tcp_keepintvl_seconds = 10;
options.proxy = "http://127.0.0.1:8080";
options.user_agent = "my-trading-service/1.0";

bybit::RestClient client{"KEY", "SECRET", "linear", options};
```

## Practical Guidance

- Reuse a `RestClient` instead of constructing one per request.
- Use separate `RestClient` instances if you need parallel HTTP requests.
- Prefer one client per API key, base URL, and category.
- Keep `recvWindow` tight for trading systems, but large enough for your network conditions.

## Planned Work

- Add explicit multi-handle support for concurrent request pipelines where it is worth the extra API surface.
- Add benchmark examples for cold versus warm request latency.
- Add richer error types with HTTP status, Bybit `retCode`, and response body access.
- Add testnet integration tests that can be enabled explicitly with credentials.
