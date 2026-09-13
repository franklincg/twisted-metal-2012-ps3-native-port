# SPURS / Taskset

**Priority:** Coherent shared state, then scheduler decisions.  
**Evidence/status:** Taskset corruption characterized; scheduler values not established.  
**Starting work:** MEM-002, SPURS-001.

## Boundary
Own lifecycle/claim and signaling; scheduler consumes real state. Never fabricate FOUND_WORKLOAD to advance.

## Independent contribution
Actual candidate under host tests, claim isolation, wait/signal and known/unknown state.

## Integration rule
Changes to a shared interface need a small reviewed contract and a dependent-test list. Source may be reviewed before the producer/consumer is integrated, but a mock result cannot certify that integration. Find an existing upstream solution first and record the exact ref and license.

## Out of scope
No proprietary inputs, private workspace access, automatic game runs, forced state, unrelated subsystem rewrite or unsupported menu/gameplay claim. For future tracks see ../MODDING_AND_PLATFORMS.md.
