# Input / front-end

**Priority:** Input contract before playable claims.  
**Evidence/status:** Main menu/navigation not verified.  
**Starting work:** INPUT-001.

## Boundary
Controller state/events and deterministic mapping, without forcing UI state.

## Independent contribution
Synthetic button/axis sequences, connect/disconnect and focus behavior.

## Integration rule
Changes to a shared interface need a small reviewed contract and a dependent-test list. Source may be reviewed before the producer/consumer is integrated, but a mock result cannot certify that integration. Find an existing upstream solution first and record the exact ref and license.

## Out of scope
No proprietary inputs, private workspace access, automatic game runs, forced state, unrelated subsystem rewrite or unsupported menu/gameplay claim. For future tracks see ../MODDING_AND_PLATFORMS.md.
