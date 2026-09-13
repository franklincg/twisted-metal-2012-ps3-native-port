# Build / provenance / publication

**Priority:** Reproducible safe source export.  
**Evidence/status:** Public kit prepared; full runtime export pending.  
**Starting work:** PUB-001, BUILD-001, DOC-001.

## Boundary
Explicit inputs, dependencies, licenses, compiler flags and narrow artifacts.

## Independent contribution
Fresh hosted environments, allowlist audit, source-only package verification.

## Integration rule
Changes to a shared interface need a small reviewed contract and a dependent-test list. Source may be reviewed before the producer/consumer is integrated, but a mock result cannot certify that integration. Find an existing upstream solution first and record the exact ref and license.

## Out of scope
No proprietary inputs, private workspace access, automatic game runs, forced state, unrelated subsystem rewrite or unsupported menu/gameplay claim. For future tracks see ../MODDING_AND_PLATFORMS.md.
