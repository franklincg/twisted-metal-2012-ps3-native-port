# Repository setup — administrative steps still required

Target: `franklincg/twisted-metal-2012-ps3-native-port`. Create a **new empty repository**; do not convert an existing development repository to public. The preparation session could read GitHub but could not create this repository. These settings are a checklist, not a claim they are active.

| Setting | Required value | Current verified state |
|---|---|---|
| Repository created / public | Separate target only | Verified: new public repository created; initial source push pending |
| Default branch | main | Pending |
| Main protection | Require PR, review, code-owner review and resolved threads | Pending |
| Status checks | host-linux, host-windows, publication-audit once first runs exist | Pending |
| Force pushes / branch deletion | Disallowed for main | Pending |
| Outside-collaborator Actions | Require approval for all outside collaborators | Pending |
| Workflow token | Read repository contents; do not permit Actions to approve PRs | Pending |
| CI runners | GitHub-hosted only; no personal/self-hosted runner | Workflow template prepared |
| Private vulnerability reporting | Enable free public-repository feature where available | Pending |
| Secret scanning / push protection | Enable supported free controls; no paid upgrade | Pending |
| Wiki | Disabled to avoid a second uncontrolled source of project status | Pending |
| Issues | Enabled; seed work items using stable IDs | Prepared, not created |
| Discussions / Projects board | Optional manual organization later | Not created |

CODEOWNERS only names reviewers; it does not enable branch protection. A SECURITY.md file does not enable private reporting. Until verified, state these limitations publicly rather than claiming protection.

## Minimal path with the connected GitHub tool

The owner creates the empty target repository once, then authorizes the connector for that new repository if its installation is scoped. The prepared allowlisted files and work items can then be uploaded without importing the private repository. Do not supply passwords or tokens in chat.

## Reproducible package path

Use `tools/export_public.py` to create a fresh audited source tree. A maintainer may initialize a **fresh Git repository in that export only**, inspect it, and push it to the new target. Never use `git push --mirror`, force-push, or the private workspace history.

After first CI runs, select the actual reported check names in branch protection. The default branch should be protected before accepting third-party changes. If independent reviewers are not yet available, keep ABI/concurrency changes in review rather than faking approval.
