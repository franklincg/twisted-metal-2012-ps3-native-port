#!/usr/bin/env python3
"""Render fixed-denominator acceptance progress without inventing port completion."""
from __future__ import annotations
import argparse,json,sys
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1]
VALID={'passed','pending','not_verified','blocked'}

def render(data: dict) -> str:
    if data.get('overall_percent') is not None:
        raise ValueError('Overall port percentage must remain unknown until an approved complete model exists')
    rows=[]; ids=set()
    for m in data['milestones']:
        gates=m['gates']
        if not gates: raise ValueError('Empty acceptance denominator')
        for g in gates:
            if g['id'] in ids or g['state'] not in VALID: raise ValueError('Invalid/duplicate gate')
            ids.add(g['id'])
            if g['state']=='passed' and not g.get('evidence'): raise ValueError('Passed gate lacks evidence')
        passed=sum(g['state']=='passed' for g in gates)
        percent=f'{100*passed/len(gates):.1f}%' if m.get('denominator_fixed') else 'Not estimable'
        rows.append(f"| {m['id']} — {m['name']} | {passed}/{len(gates)} | {percent} |")
    return ('# Evidence-based progress\n\n'
            '**Overall port completion: not estimable. Menu and gameplay are not verified.**\n\n'
            'P0 measures preparation/publication of the collaboration repository, **not the game**. '
            'Other percentages are withheld because their full acceptance scope remains open.\n\n'
            '| Milestone | Verified gates / currently listed gates | Checklist percentage |\n'
            '|---|---:|---:|\n'+'\n'.join(rows)+'\n\n'
            'Historical test counts and sanitizer variants are not completion percentages. '
            'A source-review export is not full runtime integration. See [policy](PROGRESS_POLICY.md), '
            '[status](STATUS.md) and `data/milestones.json` for evidence and pending gates.\n')

def main() -> int:
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--check',action='store_true');a=p.parse_args()
    try: content=render(json.loads((ROOT/'data/milestones.json').read_text(encoding='utf-8')))
    except (KeyError,ValueError,TypeError) as e: print('Invalid progress data:',e,file=sys.stderr);return 1
    dest=ROOT/'docs/PROGRESS.md'
    if a.check:
        if not dest.exists() or dest.read_text(encoding='utf-8')!=content: print('Progress document is stale',file=sys.stderr);return 1
        print('Progress document matches its explicit evidence gates');return 0
    dest.write_text(content,encoding='utf-8',newline='\n');print('Updated docs/PROGRESS.md');return 0

if __name__=='__main__':sys.exit(main())
