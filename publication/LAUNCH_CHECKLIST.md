# Launch checklist

Prepared source files are not a live repository.

- [x] Separate target named; founder/AI/upstream credits written.
- [x] Areas, dependencies and issue seeds prepared.
- [x] Exact source-only allowlist and hash audit tooling prepared.
- [x] Local public-tool/probe test receipt recorded (see local_validation.json).
- [ ] New empty target repository created; not a visibility change to an old repo.
- [ ] Reviewed source export uploaded and contents verified by readback.
- [ ] Actual repository protections, private reporting and CI permissions verified.
- [ ] First hosted CI runs verified; no claim based only on workflow YAML.
- [ ] Initial issues created and actual URLs recorded.
- [ ] Outreach text updated to the live URL and posted only with authorization.

Do not publish original workspace history, private logs or opaque archives. Never attach the personal PC as an automatic public PR runner. Seed issues with `python tools/seed_issues.py` in dry-run mode first, and `--apply` only after target/account verification and explicit publication authorization.
