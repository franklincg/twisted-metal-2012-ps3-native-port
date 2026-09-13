# Graphics / RSX

**Priority:** Resource ownership and visual evidence.  
**Evidence/status:** Presentation reported historically; menu image not confirmed.  
**Starting work:** GFX-001.

## Boundary
Command/resource/presentation interfaces; one owner of each host device/swap-chain.

## Independent contribution
Synthetic formats and ownership tests; define visual acceptance evidence, no copied game screenshots.

## Integration rule
Changes to a shared interface need a small reviewed contract and a dependent-test list. Source may be reviewed before the producer/consumer is integrated, but a mock result cannot certify that integration. Find an existing upstream solution first and record the exact ref and license.

## Out of scope
No proprietary inputs, private workspace access, automatic game runs, forced state, unrelated subsystem rewrite or unsupported menu/gameplay claim. For future tracks see ../MODDING_AND_PLATFORMS.md.
