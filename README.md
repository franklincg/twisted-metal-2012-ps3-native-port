# Twisted Metal (2012) — PS3 Native PC Port

**Project initiated and coordinated by Franklin Wilster (FrankCG) — Brazil 🇧🇷**

[Português brasileiro](README.pt-BR.md) · [Current status](docs/STATUS.md) · [Choose an area](docs/HOW_TO_HELP.md) · [Roadmap](docs/ROADMAP.md) · [Credits](CREDITS.md)

> **Research / pre-menu stage. Not a playable release.** This independent initiative targets the **2012 PlayStation 3 game**, not the 1995 PlayStation title. Menu rendering and gameplay have not been verified for this initiative. No download here installs or runs the original game.

## What is being built?

A community research effort toward a faithful native Windows PC port, with a reproducible development pipeline and small, reviewable runtime improvements. Work starts from existing open-source research rather than claiming a first-ever port or rebuilding every subsystem.

This first public export contains the collaboration structure, a portable synthetic Taskset layout probe, selected historical host-model sources for review, publication safeguards, and concrete contribution tasks. **It is not yet the complete redistributable runtime or a self-contained game build.** Exporting the remaining source and its exact build inputs is tracked as work item PUB-001. The private development workspace and its Git history are not mirrored here.

## Where help is useful now

| Area | Current contribution | Can start independently? |
|---|---|---|
| SPURS / shared state | Review context lifetime, cleanup, lock order and launch identity | Yes, through host-only cases |
| Guest ABI | Byte-order, layout and ownership regression tests | Yes |
| PPU / SPU memory | Review actual writer coverage and reservation contracts | Yes, no game execution |
| Build / source export | Audit permissive source dependencies and reproduce a public host build | Yes; highest onboarding priority |
| Graphics, audio, input, storage | Design interfaces and synthetic fixtures; do not claim game integration | Yes, with an agreed scope |
| Scheduler | Define semantics from references, without forcing a progress bit | Research only; integration blocked by coherent state |
| Modding / editor / PT-BR / online / other platforms | Record requirements and interfaces | Future track, not a current runtime dependency |

Pick a task from [the work board](docs/WORK_ITEMS.md), comment with your proposed scope, then open a focused pull request. Major architecture changes need a short proposal first. Contributions in Portuguese or English are welcome. Text-based collaboration is sufficient; calls are not required.

## Run something without owning the game

Python checks (Python 3.10+):

```sh
python -m unittest discover -s tests -v
python tools/render_progress.py --check
python tools/audit_public.py
```

Synthetic C host probe (CMake 3.20+ and a C compiler):

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
ctest --test-dir build -C Release --output-on-failure
```

These commands **do not build, link, launch or emulate Twisted Metal**, access a game installation, download assets, or rerun the private port's historical experiments. See [BUILDING.md](BUILDING.md) for the exact scope.

## Honest progress

The current development milestone is guest Taskset state coherence, followed by integration evidence and then a decision on scheduler work. A passing component test is not a passing port. [The dashboard](docs/PROGRESS.md) distinguishes host models, integration, visual evidence and playable milestones. There is **no defensible overall port-completion percentage or release date yet**.

## Credits and AI transparency

Franklin initiated and coordinates **this independent initiative**. Development has made extensive use of **OpenAI ChatGPT and Codex** for engineering assistance, research, debugging, test design and documentation. AI-generated output is not automatically accepted or proof of correctness. Review, evidence, license provenance and tests are required. See [CREDITS.md](CREDITS.md) and [AI_USE.md](AI_USE.md).

We acknowledge **ps3recomp**, the separate **sp00nznet/twistedmetal** project, their contributors, and **RPCS3** research. Existing projects are not being rebranded as Franklin's work, and no contributor or vendor is represented as endorsing this initiative.

## What is never included

No original game executables, disc images, extracted commercial assets, firmware, keys, saves, private memory dumps, personal files or unrelated projects. A future game build will require the user's own lawfully obtained inputs and a reviewed workflow; that workflow is not delivered in this first export. Do not upload such inputs to issues or pull requests.

## Independence and license

Unofficial fan research; no affiliation with or endorsement by Sony Interactive Entertainment, the original developers, OpenAI, Meta, NVIDIA or any other vendor. Original game IP, names and marks remain with their respective rights holders. Naming the target is identification, not a grant of rights. A disclaimer and absence of assets do not guarantee immunity from legal claims.

Original repository material is under [MIT](LICENSE). Retained upstream material keeps its notices and provenance in [THIRD_PARTY_NOTICES.md](THIRD_PARTY_NOTICES.md). Game content is not licensed by this repository. See [publication policy](docs/PUBLICATION_POLICY.md).
