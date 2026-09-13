# SPU / MFC / reservations

**Priority:** Context lifecycle and coherence.  
**Evidence/status:** Runtime reference known; combined integration pending.  
**Starting work:** SPU-001, SPU-002.

## Boundary
Inputs: captured launch descriptor, live context, reservation line. Outputs: valid lifetime, ordered writes/events and explicit teardown.

## Independent contribution
Sequential/simultaneous contexts, reservation loss, retirement and lock-order tests.

## Integration rule
Changes to a shared interface need a small reviewed contract and a dependent-test list. Source may be reviewed before the producer/consumer is integrated, but a mock result cannot certify that integration. Find an existing upstream solution first and record the exact ref and license.

## Out of scope
No proprietary inputs, private workspace access, automatic game runs, forced state, unrelated subsystem rewrite or unsupported menu/gameplay claim. For future tracks see ../MODDING_AND_PLATFORMS.md.
