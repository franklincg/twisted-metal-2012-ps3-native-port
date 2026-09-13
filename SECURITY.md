# Security and private data

This public repository must not have access to the maintainer's personal workstation, private development repositories, home network or unrelated projects. Never attach a self-hosted Actions runner from that environment. Public pull requests are untrusted code.

CI uses ephemeral GitHub-hosted runners, read-only contents permissions, no project secrets and no automatic privileged promotion. No pull_request_target workflow, privileged workflow_run consumer, credentialed external checkout or arbitrary issue-comment execution is provided. Do not add such a route in a routine feature PR.

Report sensitive vulnerabilities using GitHub's private vulnerability reporting **only when its Report a vulnerability control is enabled**. SECURITY.md does not itself enable it. If unavailable, open a non-sensitive issue asking the maintainer to establish a private reporting channel; do not post the exploit payload, credentials or private data publicly.

The public-export checker uses an exact file allowlist, hashes, text/size/type gates and basic secret/path checks. It is defense in depth, not a guarantee that all secrets, copyright problems, malicious changes or unsupported claims will be detected. Human review remains mandatory before publication.

After accidental publication, remove exposure where possible and rotate affected credentials; deleting a later commit cannot recall existing clones. Do not publish first and rely on cleanup later.
