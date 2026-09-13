# Progress calculations

Only a fixed, versioned acceptance checklist can have a numerical completion rate: `100 * passed_gates / defined_gates`. A passed gate needs an evidence path. Skipped, blocked, historical-model-only and unknown gates are not automatically passed.

P0 measures the launch of the **collaboration repository**, not completion of the game. Other milestones have incomplete scope, so their percentage is deliberately `not estimable`. No weighted whole-port estimate is manufactured from loosely chosen component weights.

The dashboard shows verified gates alongside pending ones, and preserves `overall_percent: null`. Added/removed gates require review and a rationale; splitting a test or repeating sanitizer variants must not inflate progress. Compare scope versions before claiming a percentage increased.

Update data/milestones.json only after a reviewable artifact or confirmed GitHub action supports the change, then run `python tools/render_progress.py`. The generator is local; no workflow has credentials to edit the repository automatically.
