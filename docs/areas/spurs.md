# SPURS / Taskset

**Priority:** Preserve H10/H11 semantics while moving from host proof to native validation.
**Evidence/status:** Guest-state coherence and scheduler/POLL host semantics are proven; native validation has not yet run because N6 stopped at build.
**Starting work:** PUB-001 for redistributable build closure; follow-on native work remains separately authorized.

## Proven host contract
- `FOUND_TASK` is driven independently by runnable task state.
- `FOUND_WORKLOAD` requires a real eligible selector result and is never forced merely to leave a loop.
- `selected_wid` is a transient selector result; it is not persisted into the guest Taskset layout.
- `current_wid` is host kernel state scoped by owner/logical-SPU and is committed only after eligible admission, before the task leaf executes.
- Priority zero and `maxContention == 0` admit no task leaf; active-count capacity is released on completion/yield/reschedule.
- System-service WID 32 is distinct from UNKNOWN.

## Reference pins
The closed H11 proof and N6 delta check used:
- `sp00nznet/twistedmetal` `7858472a75ddba7b527242723a9443003759a614`
- `sp00nznet/ps3recomp` `fddf580d48b4c71b2fa2a5ac6356a706bf873c35`
- `RPCS3/rpcs3` `1b569fde4d2dc8a57531bef8a310bf113e2c1324`

## Native boundary
The first N6 attempt consumed one native build authorization but failed before link/run because the build closure omitted an already-existing diagnostic include dependency. That is a tooling/build-closure blocker, not evidence that H11 passed or failed natively.

## Integration rule
Use one real state producer on the launch/claim path; do not add a parallel selector. Changes to shared interfaces require a reviewed contract, dependent-test list and provenance-pinned reuse classification. Preserve the guest layout: no scalar task count over `taskset + 0x04`.

## Out of scope
No proprietary inputs, private workspace disclosure, automatic native reruns, forced state, synthetic completion, unrelated subsystem rewrite or unsupported menu/gameplay claim. For future tracks see ../MODDING_AND_PLATFORMS.md.
