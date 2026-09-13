# Architecture: boundaries before parallel implementation

## Current versus intended

The current public code consists of host diagnostic scaffolding and historical reference sources. The complete native runtime is not exported yet. The layers below are the **target module boundaries and contribution contracts**, not claims that each layer is implemented.

| Layer | Owns | Must not own |
|---|---|---|
| Build/input preparation | Versioned manifests, provenance, reproducible source generation contract | Distribution of game/firmware inputs or secrets |
| PPU runtime | ABI calls, memory-access conventions, thread/runtime boundary | Renderer-specific state or host pointers in guest ABI |
| SPU/MFC runtime | Local store, task launch identity, reservations, DMA contracts | Fake task completion or unbounded image-ID workarounds |
| SPURS/taskset services | Taskset representation, lifecycle, signaling, scheduler-facing state | Made-up scheduler results to leave a loop |
| Storage/FIOS | Guest VFS path resolution, async I/O requests, completion | Host absolute paths returned as guest paths |
| Graphics | RSX command semantics, resource ownership, presentation/backend interface | Duplicated swap-chain ownership or UI-state forcing |
| Audio | Guest stream/mixer contracts, buffering, host output abstraction | Publishing original sound banks |
| Input/front-end | Controller state, user actions, front-end observations | Inventing a reached menu based on present counts |
| Validation/tooling | Synthetic fixtures, manifests, test evidence, host integration gates | Automatically launching private-game experiments |
| Modding/portability | Versioned extension interfaces after baseline stability | Making unfinished enhancements a dependency of first boot |

## Shared-state contract

Representation, lifetime and scheduler policy are separate responsibilities. Memory must not be overlaid with an incompatible host struct. An object identity must include its owning instance and a valid lifetime; any generation counter is host-side unless the guest ABI defines one. Reservation lifetime is not context lifetime. Capture launch identity consistently before asynchronous handoff.

A private mutex protecting one helper is not proof that all writers are synchronized. For each writer family specify the range, ownership, lock order, visibility, possible overlap and evidence. Unknown dynamic overlap is not proof of safety and not a mandate to rebuild all memory management.

## Evidence boundary

A standalone model validates its fixtures. A combined host test must link and execute the actual integrated candidate path. A native milestone requires an authorized game run and observable evidence; a visible milestone requires a frame attributable to that run. Documentation and compiler success do not cross those boundaries.

## Parallel work protocol

Before changing a shared interface, publish a small contract: input/output types, ownership/lifetime, synchronization order, error behavior, test fixture, version and dependent work items. Consumers can implement against a fixture without pretending the producer exists. Integrate vertically in small steps rather than collecting a huge patch from every area.

## Delivery sequence

Public source/export and reproducible host lab → coherent task state and launch/teardown → bounded integration proof → scheduler decision backed by observed state → authorized boot progression → verified visible menu → input/navigation → gameplay and stability. Audio, graphics, input and tools can produce independent tests in parallel. Their production integration still follows dependency gates.
