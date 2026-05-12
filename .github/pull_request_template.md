## Summary

Describe the behavior, endpoint, docs, or infrastructure change.

## Bybit Contract

- Official docs URL:
- Endpoint(s):
- Account mode or category constraints:
- Request/response contract notes:

## Validation

- [ ] `cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug`
- [ ] `cmake --build build`
- [ ] `ctest --test-dir build --output-on-failure`
- [ ] WebSocket build checked if stream code changed:
      `cmake -S . -B build-ws -DCMAKE_BUILD_TYPE=Debug -DBYBIT_ENABLE_WEBSOCKET=ON`
- [ ] Public docs updated if behavior or coverage changed

## Safety

- [ ] No API keys, secrets, signed headers, account exports, private WebSocket payloads, or `.env` files are included.
- [ ] No production fallback or mock data was added outside explicit tests.
- [ ] High-risk trading/account behavior is documented.

## Notes for Reviewers

Mention any residual risk, live-test gap, or follow-up work.
