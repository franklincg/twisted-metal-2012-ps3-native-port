# PPU / ABI integration

**Priority:** Coverage of real access paths.  
**Evidence/status:** Preserved native objects; no new compilation authorized here.  
**Starting work:** PPU-001.

## Boundary
Document inline/out-of-line calls, symbol owner, calling convention and lock participation.

## Independent contribution
Static owner/provenance map and synthetic accessor coverage; no relift.

## Integration rule
Changes to a shared interface need a small reviewed contract and a dependent-test list. Source may be reviewed before the producer/consumer is integrated, but a mock result cannot certify that integration. Find an existing upstream solution first and record the exact ref and license.

## Out of scope
No proprietary inputs, private workspace access, automatic game runs, forced state, unrelated subsystem rewrite or unsupported menu/gameplay claim. For future tracks see ../MODDING_AND_PLATFORMS.md.
