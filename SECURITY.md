# Security

This repository handles exchange API keys, signed requests, and potentially sensitive account or trading data.

## Please Do Not Report Vulnerabilities Publicly First

If you discover a security issue, credential leak, unsafe signing path, or sensitive-data exposure, do not open a public issue with exploit details.

Instead:

1. Share a private report with the maintainer through GitHub security advisories if enabled.
2. If that is not available, contact the maintainer directly and include enough detail to reproduce the problem safely.
3. Avoid sending live API keys, API secrets, reusable session data, or raw account exports unless explicitly requested through a secure channel.

## Good Reports Include

- affected component or path
- impact
- steps to reproduce
- whether live credentials are required
- suggested mitigation if known

## Sensitive Artifacts

Never commit:

- API keys or API secrets
- `.env` files
- logs containing `X-BAPI-*` headers
- account exports or order-history dumps
- raw private WebSocket payloads
- production trading scripts with live account identifiers

Those files should stay local and ignored.
