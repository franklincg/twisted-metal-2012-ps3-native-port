# Storage / FIOS / SaveData

**Priority:** Guest path and completion contracts.  
**Evidence/status:** Earlier boot activity reported; complete storage semantics not certified.  
**Starting work:** IO-001.

## Boundary
Guest VFS inputs map to controlled host paths; completions carry exact byte counts/errors.

## Independent contribution
Synthetic filesystem fixtures and traversal/bounds tests; no private saves.

## Integration rule
Changes to a shared interface need a small reviewed contract and a dependent-test list. Source may be reviewed before the producer/consumer is integrated, but a mock result cannot certify that integration. Find an existing upstream solution first and record the exact ref and license.

## Out of scope
No proprietary inputs, private workspace access, automatic game runs, forced state, unrelated subsystem rewrite or unsupported menu/gameplay claim. For future tracks see ../MODDING_AND_PLATFORMS.md.
