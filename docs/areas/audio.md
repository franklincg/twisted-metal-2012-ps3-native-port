# Audio

**Priority:** Buffer/format/timing contract.  
**Evidence/status:** Independent fixture work available; game integration unverified.  
**Starting work:** AUDIO-001.

## Boundary
PCM/stream buffers, timestamps and lifecycle; no original sound banks in fixtures.

## Independent contribution
Generated tones, ring-buffer bounds, stop/restart and underrun tests.

## Integration rule
Changes to a shared interface need a small reviewed contract and a dependent-test list. Source may be reviewed before the producer/consumer is integrated, but a mock result cannot certify that integration. Find an existing upstream solution first and record the exact ref and license.

## Out of scope
No proprietary inputs, private workspace access, automatic game runs, forced state, unrelated subsystem rewrite or unsupported menu/gameplay claim. For future tracks see ../MODDING_AND_PLATFORMS.md.
