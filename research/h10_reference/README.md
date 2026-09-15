# Historical standalone H10 source â€” reproducible host review export

These seven text source files were selected individually from the maintainer-supplied host-research package, preserving their reviewed bytes and relevant MIT notice. They are not lifted game functions or original game assets. Sources with legacy behavior deliberately expose defects for characterization and must not be installed as a runtime.

The directory now includes the **minimal reviewed header closure** needed to compile the historical Windows host suite from a fresh public clone. Six support headers are byte-pinned to `sp00nznet/ps3recomp` commit `25e5efe1793fffb0e98782f35a6e6a73e552bd8f`; `spurs_taskset.h` is the same MIT source with the small project linkage correction documented in `PROVENANCE.json` (`TM64181_VM_ACCESS_INLINE`). The current upstream head is tracked separately in `SOURCES.md`; this historical build closure is intentionally pinned to the version actually validated with the suite.

The suite is opt-in and Windows-only. Use the `TM_PUBLIC_H10_REFERENCE` CMake option described in `BUILDING.md`. It exercises synthetic guest memory and host threads only. It does not read game files or execute a native game workload.

The historical result is 121 checks. The public closure is designed so CI can rebuild and rerun those checks rather than trusting a private binary. AddressSanitizer/UndefinedBehaviorSanitizer results from earlier private work remain historical unless separately rerun in this public package; they are not implied by a normal hosted run.

`representation.c/.h` are the historical contract-reconstruction candidate; `spurs_taskset.c` and `spurs_pm.c` retain helper behavior; `active_api_extract.inc` and `legacy.c` characterize selected legacy HLE functions; `test_representation.c` is the historical driver. See `PROVENANCE.json` and `LICENSES/ps3recomp-MIT.txt`.

This remains a **reference model**, not a drop-in production scheduler. The private combined-runtime integration, launch handoff, complete external-writer coverage and native-game validation are separate evidence domains.
