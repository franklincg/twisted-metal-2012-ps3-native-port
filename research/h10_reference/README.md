# Historical standalone H10 source — review export

These seven text source files were selected individually from the maintainer-supplied host-research package, preserving their bytes and relevant MIT notice. They are not lifted game functions or original game assets. Sources with legacy behavior deliberately expose defects for characterization and must not be installed as a runtime.

**This directory is excluded from the default build and CI.** It depends on the historical Windows runtime headers (`cellSpurs.h`, `spurs_taskset.h`, `ppu_memory.h` and their dependencies), which are not exported as a verified build closure in this first package. Publishing that closure with provenance is PUB-001. Do not pretend that it is a self-contained clone-and-run H10 suite.

The reported historical result was 121 distinct checks in each of normal, AddressSanitizer and UndefinedBehaviorSanitizer variants. The publication preparation did not rerun this historical suite, did not test a combined runtime and did not run the game. Its private lock domain, allocator adaptation, launch handoff and external-writer integration remain limitations. A compile or source read here cannot close those gaps.

`representation.c/.h` are the historical contract-reconstruction candidate; `spurs_taskset.c` and `spurs_pm.c` retain upstream helper behavior; `active_api_extract.inc` and `legacy.c` characterize selected legacy HLE functions; `test_representation.c` is the historical driver. See PROVENANCE.json and LICENSES/ps3recomp-MIT.txt.

Use these sources to prepare focused reviews and synthetic reproductions. The portable `labs/taskset_layout` probe is a separate small diagnostic, not a replacement for this suite or the production candidate.
