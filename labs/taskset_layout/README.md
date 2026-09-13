# Public Taskset layout probe

An original, portable, synthetic byte-level diagnostic based on the documented Taskset representation issue. It demonstrates why a 32-bit host counter store at guest offset +4 changes the RUNNING bitset, why endian conversion alone is insufficient, and how counts can be derived without that scalar store.

All arrays/IDs are synthetic. This is **not** the actual HLE CreateTask implementation, a scheduler, context-lifetime fix, concurrency test, combined H10 test or new native-port milestone. It deliberately avoids running the historical 121-check suite. Assertions repeated over 128 indices are reported as assertions, not full-game coverage.

The default CMake target builds this file only. Match its offsets against contracts/taskset_layout.json and the historical reference sources before proposing a runtime change.
