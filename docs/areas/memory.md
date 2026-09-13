# Guest ABI / memory

**Priority:** Representation before scheduling.  
**Evidence/status:** Host-model evidence; production integration pending.  
**Starting work:** PUB-001, MEM-001, MEM-002.

## Boundary
Inputs: guest bytes, typed bounds, owner identity. Outputs: validated BE fields and errors. No host pointer may leak into guest ABI.

## Independent contribution
Layout/byte-order probes, bounded reads/writes, canaries, negative invalid-state cases.

## Integration rule
Changes to a shared interface need a small reviewed contract and a dependent-test list. Source may be reviewed before the producer/consumer is integrated, but a mock result cannot certify that integration. Find an existing upstream solution first and record the exact ref and license.

## Out of scope
No proprietary inputs, private workspace access, automatic game runs, forced state, unrelated subsystem rewrite or unsupported menu/gameplay claim. For future tracks see ../MODDING_AND_PLATFORMS.md.
