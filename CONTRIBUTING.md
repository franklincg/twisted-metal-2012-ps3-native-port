# Contributing

## Choose a bounded task

Read docs/STATUS.md and docs/WORK_ITEMS.md. Comment on the matching issue with your intended scope, expected files and evidence. If issues have not yet been seeded, use the stable work-item ID in the PR title. A claim coordinates work; it does not grant repository write access. Updates every seven days help avoid duplicate work; inactivity is discussed, not treated as a silent transfer of authorship.

Documentation, tests, code review and ABI research are welcome alongside code. English and Portuguese are accepted. Do not require voice/video calls or disclosure of a real name.

## Reuse before writing

Check the same-title project, the pinned/current ps3recomp implementation, relevant open PRs, RPCS3 semantics and related ports. Record exact refs, file/function, what is reusable and what remains incompatible. Reuse a validated result when inputs are unchanged; re-run only tests affected by the proposed change. Do not import an entire newer runtime merely to obtain one fix.

## Work in isolation

Fork this public repository and create a focused branch. Do not point an automated agent at a personal workstation or private development directory. Public build targets run only synthetic host tests. No game files are required or accepted.

Keep representation, context lifetime, synchronization, scheduler semantics, rendering and filesystem changes reviewable separately. Write an interface contract before work touches another area. Claims of integrated behavior require the integrated sources to be the actual code under test, not a second mock implementation.

## Evidence and review

A useful PR contains: linked work-item ID; a minimal causal explanation; exact input refs and patch; reproducible commands; positive and negative tests; test environment; limitations; license/provenance; and AI-use disclosure if applicable. Name checks accurately: characterization may confirm a defect; a component PASS is not a Windows integration PASS, menu, gameplay or release PASS.

Review all file changes, including CI, metadata, dependencies and generated manifests. Update the exact export allowlist only for deliberately reviewed files. Do not use broad folder copies. A workflow result alone never replaces source review.

## Acceptance

Maintainer/code-owner review, relevant synthetic tests, provenance and publication checks must pass. One independent technical review is the target for memory/ABI/concurrency changes; initially that review slot may be unfilled, so those changes remain unmerged. No automatic merge of outside contributions.

An integration runner or private-game experiment requires its own explicit authorization, approved binary/input hashes and bounded execution plan. A public PR does not grant access to the maintainer's computer or permission to repeat historical native campaigns.

## Prohibited content

No proprietary game/firmware bytes, keys, binaries, assets, audio, videos, saves, dumps, credentials, personal paths or unrelated project data. Do not attach full logs unless they have been sanitized. Do not send third-party code under a new license without permission. Use synthetic fixtures and source links.

## Attribution

Choose the name/handle you want credited; preserve upstream copyright and accurate authorship. See CREDITS.md and THIRD_PARTY_NOTICES.md. Existing licenses govern contributed derivatives; the root MIT license applies only where this project can grant it.
