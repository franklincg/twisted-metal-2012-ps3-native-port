# Roadmap and dependency gates

No dates or overall completion estimate are promised. The initial target is a faithful Windows baseline; enhancements must not conceal missing original behavior.

| Milestone | Exit condition | Parallel work |
|---|---|---|
| P0 — public collaboration | Audited source kit, real repo/issue setup, safe CI and reproducible public tests | Docs, provenance, reviews |
| P1 — coherent host state | Representation, ownership, lifecycle, wait/signal and relevant writers combined against exact Windows candidate | Context tests, ABI/accessor reviews |
| P2 — justified boot progression | Scheduler/producer behavior supported by state evidence and authorized integration | Storage, graphics/audio/input fixtures |
| P3 — visible menu | Native run reaches front-end naturally and produces a readable attributable menu frame | Renderer correctness review |
| P4 — usable menu | Real input navigation, settings and required storage work without forced state | Input/SaveData tests |
| P5 — playable slice | One validated arena/vehicle slice, simulation, audio/input and repeatability | Gameplay regression coverage |
| P6 — stable game baseline | Broader content coverage, longer sessions, crash/performance regression tracking | Compatibility matrix |
| P7 — extension tracks | Mod SDK, tools/editor, localization, online and platforms with separate reviews | Independent proposals after interfaces stabilize |

P3 does not imply P4, and P5 does not mean the full game is complete. Tests that pass on a standalone model do not close P1. Production source export and valid reproduction inputs are explicit prerequisites, not hidden assumptions.
