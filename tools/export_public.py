#!/usr/bin/env python3
"""Copy only audited source into a NEW directory. Never copy Git history or a workspace."""
from __future__ import annotations
import argparse, json, sys
from pathlib import Path
from audit_public import ROOT, audit, digest

def export(root: Path,destination: Path) -> dict:
    root=root.resolve(); destination=destination.absolute()
    if destination.exists() or destination.is_symlink():
        raise ValueError('Destination must not exist; no overwrites are allowed')
    if root==destination.resolve() or root in destination.resolve().parents:
        raise ValueError('Destination must be outside the source tree')
    if any(p.is_symlink() for p in [destination.parent,*destination.parents]):
        raise ValueError('Destination parent must not be a symlink')
    result=audit(root)
    if result['status']!='PASS': raise ValueError('Publication audit failed; run audit_public.py for details')
    allowed=json.loads((root/'publication/allowlist.json').read_text())['paths']
    manifest=json.loads((root/'publication/manifest.json').read_text())['files']
    # Re-read and verify before creating the destination; no tree walking/copying.
    payload={}
    for name in allowed:
        p=root/name
        if p.is_symlink(): raise ValueError('Source changed into symlink')
        data=p.read_bytes()
        if name in manifest and digest(data)!=manifest[name]['sha256']:
            raise ValueError('Source changed after audit: '+name)
        payload[name]=data
    destination.mkdir(parents=False,exist_ok=False)
    for name,data in payload.items():
        p=destination/name;p.parent.mkdir(parents=True,exist_ok=True)
        with p.open('xb') as f: f.write(data)
    return {'status':'PASS','file_count':len(payload),'git_history_copied':False,'game_run':False}

def main() -> int:
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--destination',required=True,type=Path)
    a=p.parse_args()
    try: result=export(ROOT,a.destination)
    except (ValueError,OSError) as exc: print(str(exc),file=sys.stderr);return 1
    print(json.dumps(result,indent=2));return 0

if __name__=='__main__': sys.exit(main())
