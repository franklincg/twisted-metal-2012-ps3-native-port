# Choose where to help

You do not need to solve the whole game. Start with one review, fixture, documentation fix or bounded component change. The [work board](WORK_ITEMS.md) contains stable IDs, starting paths, dependencies and acceptance criteria. An item marked ready can begin as research/review/fixture work even when its later integration depends on another item.

The highest onboarding priority is PUB-001: export the remaining redistributable source and its exact dependencies so contributors can build more than the small public probe. Avoid parallel reimplementation of a component still being safely exported.

Choose an area below. Each page defines ownership boundaries and tests so contributors can work independently without merging unrelated runtime behavior.

- [Guest ABI / memory](areas/memory.md)
- [SPU / MFC / reservations](areas/spu.md)
- [SPURS / Taskset](areas/spurs.md)
- [PPU / ABI integration](areas/ppu.md)
- [Storage / FIOS / SaveData](areas/storage.md)
- [Graphics / RSX](areas/graphics.md)
- [Audio](areas/audio.md)
- [Input / front-end](areas/input.md)
- [Build / provenance / publication](areas/build.md)
- [Mods / map editor / localization](areas/modding.md)
- [Online / additional platforms](areas/online-platforms.md)

## Workflow

Discuss/claim a work item → create a fork/branch → agree on interface when needed → submit a focused PR with provenance and positive/negative tests → review → merge only after relevant gates. Blocked integration is not a reason to fabricate the missing producer; use a labeled fixture and keep its result separate.

Tasks are voluntary. You may use a handle, communicate in Portuguese or English and work without calls. Area maintainer roles are initially open; nobody is granted write access automatically.
