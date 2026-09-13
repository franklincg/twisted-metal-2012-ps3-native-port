# Mods / map editor / localization

**Priority:** Requirements, later SDK.  
**Evidence/status:** Future design track.  
**Starting work:** Future roadmap only.

## Boundary
Versioned packages, new/permissioned assets, reversible overrides, tools separate from runtime.

## Independent contribution
Synthetic interchange/manifest schemas after baseline and interfaces stabilize.

## Integration rule
Changes to a shared interface need a small reviewed contract and a dependent-test list. Source may be reviewed before the producer/consumer is integrated, but a mock result cannot certify that integration. Find an existing upstream solution first and record the exact ref and license.

## Out of scope
No proprietary inputs, private workspace access, automatic game runs, forced state, unrelated subsystem rewrite or unsupported menu/gameplay claim. For future tracks see ../MODDING_AND_PLATFORMS.md.
