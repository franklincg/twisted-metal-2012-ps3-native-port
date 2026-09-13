#!/usr/bin/env python3
"""Audit an exact, source-only export. No network or private-tree discovery."""
from __future__ import annotations
import argparse
import hashlib
import json
import os
from pathlib import Path, PurePosixPath
import re
import stat
import sys

ROOT = Path(__file__).resolve().parents[1]
META = {"publication/manifest.json"}
IGNORED_DIRS = {".git", "__pycache__", "build"}
TEXT_SUFFIXES = {".md", ".txt", ".json", ".py", ".c", ".h", ".inc", ".yml", ".yaml", ".toml"}
TEXT_NAMES = {"LICENSE", "CMakeLists.txt", "CODEOWNERS", ".gitignore", ".gitattributes"}
FORBIDDEN_PARTS = {".git", ".env", "node_modules", "private", "dumps", "saves", "local-inputs", "autopilot"}
MAX_BYTES = 512 * 1024
SECRET_PATTERNS = [
    ("GitHub credential", re.compile(r"\bgh[pousr]_[A-Za-z0-9]{30,}\b")),
    ("GitHub fine-grained credential", re.compile(r"\bgithub_pat_[A-Za-z0-9_]{40,}\b")),
    ("cloud access credential", re.compile(r"\b(?:AKIA|ASIA)[A-Z0-9]{16}\b")),
    ("private-key block", re.compile(r"-----BEGIN (?:RSA |EC |OPENSSH )?PRIVATE KEY-----")),
    ("personal Windows path", re.compile(r"(?i)\b[A-Z]:[\\/](?:Users|Games|MONEY_AGENT)[\\/]")),
    ("personal machine identifier", re.compile(r"\bDESKTOP-[A-Z0-9]{5,}\b")),
    ("personal Unix home path", re.compile(r"/(?:home|Users)/[A-Za-z0-9_.-]+/")),
]

def digest(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()

def safe_relative(name: str) -> bool:
    if not isinstance(name, str) or not name or "\\" in name or ":" in name or "\x00" in name:
        return False
    p = PurePosixPath(name)
    if p.is_absolute() or any(x in {"", ".", ".."} for x in name.split("/")):
        return False
    return not any(x.lower() in FORBIDDEN_PARTS or x.lower().startswith(".env.") for x in p.parts)

def content_errors(name: str, data: bytes) -> list[str]:
    errors = []
    p = PurePosixPath(name)
    if p.suffix.lower() not in TEXT_SUFFIXES and p.name not in TEXT_NAMES:
        errors.append("file type is not approved text")
    if len(data) > MAX_BYTES:
        errors.append("file exceeds reviewed text size limit")
    if b"\x00" in data or data.startswith((b"MZ", b"\x7fELF", b"PK\x03\x04")):
        errors.append("binary/archive payload is not allowed")
    try:
        text = data.decode("utf-8-sig")
    except UnicodeDecodeError:
        return errors + ["not valid UTF-8 text"]
    for label, pattern in SECRET_PATTERNS:
        if pattern.search(text):
            errors.append(label + " detected; matched content withheld")
    if name.startswith(".github/workflows/"):
        for bad in (r"(?m)^\s*pull_request_target\s*:", r"(?m)^\s*workflow_run\s*:",
                    r"(?m)^\s*runs-on\s*:.*self-hosted", r"\bsecrets\."):
            if re.search(bad, text):
                errors.append("privileged or private workflow route is forbidden")
        for match in re.finditer(r"(?m)^\s*-?\s*uses:\s*([^\s#]+)", text):
            action = match.group(1)
            if not re.fullmatch(r"[A-Za-z0-9_.-]+/[A-Za-z0-9_.-]+@[0-9a-f]{40}", action):
                errors.append("external action must use a full commit SHA")
    return errors

def audit(root: Path) -> dict:
    root = root.resolve()
    errors: list[str] = []
    try:
        allowed_doc = json.loads((root / "publication/allowlist.json").read_text(encoding="utf-8"))
        allowed = allowed_doc["paths"]
        manifest = json.loads((root / "publication/manifest.json").read_text(encoding="utf-8"))
        records = manifest["files"]
    except (OSError, ValueError, KeyError, TypeError) as exc:
        return {"status": "FAIL", "file_count": 0, "errors": ["invalid publication metadata: " + type(exc).__name__]}
    if not isinstance(allowed, list) or not all(isinstance(x, str) for x in allowed):
        return {"status":"FAIL", "file_count":0, "errors":["invalid allowlist"]}
    if len(set(allowed)) != len(allowed) or len({x.casefold() for x in allowed}) != len(allowed):
        errors.append("duplicate or Windows case-colliding allowlist path")
    if not isinstance(records, dict) or set(records) != set(allowed) - META:
        errors.append("manifest paths must equal allowlist except manifest itself")
        records = records if isinstance(records, dict) else {}
    for name in allowed:
        if not safe_relative(name):
            errors.append("unsafe allowlist path")
            continue
        path = root / name
        if any((root / Path(*PurePosixPath(name).parts[:i])).is_symlink()
               for i in range(1, len(PurePosixPath(name).parts) + 1)):
            errors.append(name + ": symlink rejected")
            continue
        try:
            if not stat.S_ISREG(path.stat().st_mode):
                errors.append(name + ": nonregular file")
                continue
            data = path.read_bytes()
        except OSError:
            errors.append(name + ": missing or unreadable")
            continue
        errors.extend(name + ": " + error for error in content_errors(name, data))
        if name not in META:
            rec = records.get(name, {})
            if rec.get("sha256") != digest(data) or rec.get("bytes") != len(data):
                errors.append(name + ": content differs from reviewed manifest")
            if not rec.get("origin") or not rec.get("license"):
                errors.append(name + ": missing provenance/license record")
    # Build products are not export candidates. No symlinks are followed.
    seen = set()
    for base, dirs, names in os.walk(root, followlinks=False):
        keep = []
        for d in dirs:
            path = Path(base) / d
            if path.is_symlink():
                errors.append(path.relative_to(root).as_posix() + ": symlink directory rejected")
            elif d not in IGNORED_DIRS and not d.startswith("build-"):
                keep.append(d)
        dirs[:] = keep
        for n in names:
            if n.endswith((".pyc", ".pyo")):
                continue
            seen.add((Path(base) / n).relative_to(root).as_posix())
    for name in sorted(seen - set(allowed)):
        errors.append(name + ": not in reviewed allowlist")
    return {"status": "PASS" if not errors else "FAIL", "file_count": len(allowed),
            "scanned_scope":"exact source export; build outputs and Git history are never exported",
            "security_guarantee":"none; source and license review still required", "errors": errors}

def main() -> int:
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--root",type=Path,default=ROOT)
    args=parser.parse_args()
    result=audit(args.root)
    print(json.dumps(result,ensure_ascii=False,indent=2))
    return 0 if result["status"]=="PASS" else 1

if __name__=="__main__":
    sys.exit(main())
