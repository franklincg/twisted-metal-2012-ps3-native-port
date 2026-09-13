# Governance and review

**Founder/coordinator:** Franklin Wilster (FrankCG), GitHub @franklincg. Technical area-maintainer positions are initially open; no outside person is appointed without agreeing.

The coordinator sets priorities, manages scope and approves publication. Specialist reviewers validate the technical claims they can actually assess. AI tools assist these roles but do not create an independent human review or a second GitHub identity.

Contributors work through forks and PRs. Newcomers do not receive direct-write, administrator, secret or workstation access. Trusted area maintainers may be appointed later with least-privilege access. Escalate competing designs through a short ADR containing alternatives, dependencies, evidence and a reversible decision.

Canonical runtime work, public collaboration and release publication are separate tracks. The public repository never auto-writes the private workspace. Only an explicitly reviewed patch may cross that boundary; source provenance and the local integration checkpoint must be reconciled first.

Main should require PR review, CODEOWNERS review, passing host/publication checks, resolved conversations and no force pushes. These are actual repository settings, not effects of this document. Until verified after creation, treat them as **required configuration, not enabled protection**. See docs/REPOSITORY_SETUP.md.

No calendar release promises, inflated progress percentages or credit claims over upstream work. Decisions and technical results must remain inspectable in issue/PR history.
