# Build the public host probes

## Scope

The default build produces **one synthetic C executable**, not a game, emulator, lifted workload or replacement production runtime. It reads and writes only synthetic in-process byte arrays. The Python tooling similarly operates only on this public package or explicitly selected temporary test directories.

Requirements: Python 3.10+; CMake 3.20+; a C11 compiler. Linux GCC/Clang and Windows MSVC/Clang are intended host targets for the small portable probe.

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

Sanitizers do not prove the absence of races. The portable layout probe is not a concurrency test.

## Historical H10 Windows host reference

`research/h10_reference/` contains the reviewed H10 host-model sources plus the exact redistributable header closure required by that historical suite. The support closure is pinned and documented in `research/h10_reference/PROVENANCE.json`; it contains no game binaries, lifted game functions, assets, keys or private traces.

This target is **Windows-only** because the historical suite deliberately exercises Win32 thread/SRW-lock behavior. It remains a reference-model test, not the combined production runtime and not a game execution. Build it with Visual Studio/MSVC (the target enables MSVC C11 atomics explicitly):

```powershell
cmake -S . -B build-h10 -DTM_PUBLIC_H10_REFERENCE=ON
cmake --build build-h10 --config Release --target tm2012_h10_reference
ctest --test-dir build-h10 -C Release -R h10_reference_host --output-on-failure
```

The expected historical contract is 121 host checks. A passing public build proves only that this exported reference suite is reproducible from the repository. It does **not** prove H11 scheduler integration, a native PS3 workload run, menu rendering or gameplay.

## Game build

Not delivered in this export. No command above requires a disc, game asset, firmware, private key, network service or user account. An eventual game pipeline needs an independently reviewed source export and input/provenance contract; do not guess build commands or ask contributors to upload proprietary inputs.
