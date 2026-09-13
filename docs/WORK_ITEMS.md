# Initial contribution board

These entries are prepared issue seeds. **They are not claimed to exist as GitHub issues until the publication receipt confirms creation.** The JSON source is `data/work_items.json`.

| ID | Area | Priority | Start / dependencies | Task |
|---|---|---|---|---|
| PUB-001 | build | P0 | ready; independent | Export the exact redistributable host-source build closure |
| SPU-001 | spu | P1 | ready; independent | Review reservation-context retirement and slot reuse |
| MEM-001 | memory | P1 | ready; independent | Review guest Taskset layout and extend the public byte probe |
| MEM-002 | memory | P1 | blocked; PUB-001 (source review may begin) | Audit instance ownership and immutable launch identity |
| SPU-002 | spu | P1 | ready; independent | Map shared-state writers and lock order without a full-memory rewrite |
| SPURS-001 | spurs | P2 | blocked; MEM-002, SPU-002 | Specify Taskset POLL return-state producers |
| PPU-001 | ppu | P2 | ready; independent | Document accessor ABI and compatibility of preserved object code |
| IO-001 | storage | P2 | ready; independent | Design guest VFS path and callback test fixtures |
| GFX-001 | graphics | P2 | ready; independent | Define renderer ownership and visible-menu acceptance checklist |
| AUDIO-001 | audio | P2 | ready; independent | Define a synthetic audio-buffer contract and fixtures |
| INPUT-001 | input | P2 | ready; independent | Define controller-state and menu-navigation test fixtures |
| BUILD-001 | build | P1 | ready; independent | Review public CI, allowlist and provenance gates |
| DOC-001 | build | P2 | ready; independent | Maintain bilingual status and evidence-based progress |

Detailed acceptance criteria and ready-to-publish issue bodies are in the JSON. Future modding/editor/online/platform work belongs to the roadmap, not the current critical-path implementation.
