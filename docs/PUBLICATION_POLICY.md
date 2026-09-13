# Public export policy

## Separate repository, fresh history

Publication starts with a new empty repository and a curated file list. Do not change visibility of the private development repository, fork it publicly, mirror it, push all refs or copy its history. Never use the full personal workspace as the source of a public push.

`publication/allowlist.json` lists every allowed relative file. `publication/manifest.json` records content hashes. The export tool reads those paths only and writes into a new destination. Unknown files, symlinks, unsafe paths, unexpected hashes, binary payloads and obvious secret patterns fail the check. Changing the allowlist requires source/provenance review; regenerating hashes is not a substitute for that review.

## Never export

Original game or firmware executables; ISO/PKG/disc images; extracted textures, meshes, audio or video; original scripts/data of uncertain rights; decryption/license keys; saves; raw memory dumps; opaque archives; generated game-code dumps of uncertain redistributability; credentials; personal identifiers not deliberately approved; machine names; personal absolute paths; unrelated projects; private history and logs.

This first export also excludes the complete historical analysis ZIP, host binaries/objects, compiler logs, raw disassembly and internal dispatch files. It uses a sanitized status summary instead.

## Provenance gate

Each imported source needs origin, exact version, license, modifications and retained notices. References to public ABI facts do not authorize copying differently licensed code. Unknown ownership/license means quarantine, not public release. MIT only covers code the project is entitled to license; it grants no rights to the game or trademarks.

This is an operational publication policy, not a legal opinion or assurance against a takedown. GitHub's DMCA process also addresses some circumvention claims. Seek qualified legal review before publishing material whose rights or circumvention implications are uncertain.

## Untrusted contributions

A public clone cannot browse a workstation merely because this repository exists, but executing untrusted code or attaching a self-hosted runner can create exposure. Use ephemeral hosted CI with no private credentials. Do not automatically execute external PRs on the maintainer's PC.

## Source release stages

1. Collaboration kit and synthetic lab: this export.
2. Audited, licensed runtime helpers and exact dependencies: PUB-001, file by file.
3. Reproducible build instructions using user-owned inputs, after review.
4. Release artifacts only after content and runtime validation.

No stage claims completion of a later one. A source-only repository is not automatically a completed port.
