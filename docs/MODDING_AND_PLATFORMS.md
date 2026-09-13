# Future architecture: mods and additional platforms

These are design intentions after a faithful, stable PC baseline, not implemented features or time estimates.

## Extension boundary

A future Mod SDK should use a versioned manifest, capabilities, dependency versions and reversible overrides. Keep original user-owned assets outside a mod package; distribute newly created or permissioned replacement content. Default to data-only mods. Native-code plugins require a separate trust model, explicit opt-in and compatibility checks; they are not safe merely because they are mods.

## Art and map pipeline

Separate format research, import, authoring and export/validation. An interchange layer may support geometry, material bindings, textures, skeletons, LODs, collision and coordinate/unit conventions. Blender/Cinema 4D adapters consume that documented layer rather than embedding runtime assumptions. Export must validate resource budgets and identifiers. No promise of effortless conversion from every proprietary format.

A map editor is a separate tool with a scene document, object placement, collision, navigation/AI data, spawn rules, lighting and a validator. A visual scene is not yet a playable level. Deterministic versioned packages and rollback are prerequisites for community map exchange.

## PT-BR localization and voices

Separate string tables, UI constraints, subtitles, font/encoding support and voice-event mappings. Record new performances with permission; do not imply existing actors consent to voice cloning. Store audio-language replacements independently and document timing/loudness requirements. Legal review of redistributed content remains necessary.

## Community online

First define the offline simulation and serialization boundaries. A later independent networking layer needs version negotiation, map/mod hashes, ownership/authority, reconciliation, input validation, hosting/discovery and abuse controls. Do not assume original servers or authentication can be reused or bypassed. No online compatibility claim before tests.

## Graphics enhancements

Preserve a faithful baseline and make enhancements optional. A renderer interface can later admit upscaling or frame-generation integrations, but correct motion vectors, depth, jitter, exposure, UI composition, SDK licensing and hardware support need their own validation. Do not advertise a specific DLSS version or universal support before confirming and implementing it.

## Linux, Steam Deck and Quest/VR

Keep platform I/O, threading, graphics and input behind explicit interfaces. A Windows result does not establish Android/ARM64 performance, Vulkan support, memory budgets or VR rendering. Standalone Quest and PC-VR are different targets. OpenXR integration, stereo cameras, latency, interaction and performance are separate engineering work; not an automatic conversion after PC completion.
