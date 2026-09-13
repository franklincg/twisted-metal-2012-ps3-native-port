# Build the public host probe

## Scope

The default build produces **one synthetic C executable**, not a game, emulator, lifted workload or replacement production runtime. It reads and writes only synthetic in-process byte arrays. The Python tooling similarly operates only on this public package or explicitly selected temporary test directories.

Requirements: Python 3.10+; CMake 3.20+; a C11 compiler. Linux GCC/Clang and Windows MSVC/Clang are intended host targets for the small probe. This publication preparation validates the Linux environment only; do not claim a Windows CI result until the hosted job actually runs.

```sh
python -m unittest discover -s tests -v
python tools/render_progress.py --check
python tools/audit_public.py
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
ctest --test-dir build -C Release --output-on-failure
```

Optional sanitizer build on compilers supporting address/undefined sanitizers:

```sh
cmake -S . -B build-sanitized -DTM_PUBLIC_SANITIZERS=ON
cmake --build build-sanitized
ctest --test-dir build-sanitized --output-on-failure
```

Sanitizers do not prove the absence of races. This probe is not a concurrency test.

## Historical H10 model

`research/h10_reference/` contains selectively audited source from a prior standalone Windows contract reconstruction. It is **review material**, excluded from default CMake and CI. The exact production headers, allocator, wake protocol and combined Windows integration are not supplied as a public drop-in build. See its README and PUB-001. Do not manufacture new PASS counts from those historical results.

## Game build

Not delivered in this export. No command above requires a disc, game asset, firmware, private key, network service or user account. An eventual game pipeline needs an independently reviewed source export and input/provenance contract; do not guess build commands or ask contributors to upload proprietary inputs.
