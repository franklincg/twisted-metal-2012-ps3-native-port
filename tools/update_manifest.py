#!/usr/bin/env python3
"""Refresh hashes only for a deliberately reviewed exact allowlist; not a scan approval."""
from __future__ import annotations
import argparse, json
from pathlib import Path
from audit_public import ROOT, META, safe_relative, content_errors, digest

def main() -> None:
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--reviewed',action='store_true',help='Acknowledge manual content/provenance review.')
    a=p.parse_args()
    if not a.reviewed: p.error('requires --reviewed; adding files is a manual review decision')
    doc=json.loads((ROOT/'publication/allowlist.json').read_text())
    origins=doc['origins']; records={}
    for name in doc['paths']:
        if not safe_relative(name): raise SystemExit('Unsafe path in allowlist')
        if name in META: continue
        path=ROOT/name
        if path.is_symlink() or not path.is_file(): raise SystemExit('Not an ordinary source file: '+name)
        data=path.read_bytes(); errors=content_errors(name,data)
        if errors: raise SystemExit(name+': '+'; '.join(errors))
        origin=origins.get(name)
        if not origin or not origin.get('origin') or not origin.get('license'):
            raise SystemExit('Missing provenance: '+name)
        records[name]={**origin,'sha256':digest(data),'bytes':len(data)}
    (ROOT/'publication/manifest.json').write_text(json.dumps({'schema_version':1,'self_hash_policy':'manifest is allowlisted but not self-hashed','files':records},ensure_ascii=False,indent=2)+'\n',encoding='utf-8')
    print('Recorded',len(records),'reviewed file hashes; no files discovered or published.')

if __name__=='__main__': main()
