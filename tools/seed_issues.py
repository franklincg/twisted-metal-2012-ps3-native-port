#!/usr/bin/env python3
"""Prepare or explicitly publish this kit's issues to its one designated public repository."""
from __future__ import annotations
import argparse,json,shutil,subprocess,sys
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1]
EXPECTED='franklincg/twisted-metal-2012-ps3-native-port'

def gh(*args: str) -> str:
    p=subprocess.run(['gh',*args],cwd=ROOT,text=True,capture_output=True,check=False)
    if p.returncode: raise RuntimeError('GitHub CLI command failed; no credentials printed. Check authentication and repository access locally.')
    return p.stdout.strip()

def main() -> int:
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--apply',action='store_true',help='Create missing labels/issues in the designated existing public repository.')
    a=p.parse_args()
    items=json.loads((ROOT/'data/work_items.json').read_text())['items']
    if not a.apply:
        print(json.dumps({'mode':'DRY_RUN','repository':EXPECTED,'issue_count':len(items),'titles':[i['issue_title'] for i in items]},indent=2));return 0
    if not shutil.which('gh'): print('GitHub CLI is required; no automatic installation or token request.',file=sys.stderr);return 1
    try:
        if gh('api','user','--jq','.login')!='franklincg': raise RuntimeError('Wrong authenticated account; no changes made')
        repo=json.loads(gh('api','repos/'+EXPECTED))
        if repo.get('full_name')!=EXPECTED or repo.get('private') or repo.get('fork'):
            raise RuntimeError('Target must be the designated independent PUBLIC repository, not a fork')
        current=json.loads(gh('issue','list','--repo',EXPECTED,'--state','all','--limit','500','--json','title,number,url'))
        if len(current)>=500: raise RuntimeError('Issue listing may be incomplete; aborting before writes')
        existing={i['title']:i for i in current}
        labels=sorted({l for i in items for l in i['labels']})
        existing_labels={i['name'] for i in json.loads(gh('label','list','--repo',EXPECTED,'--limit','500','--json','name'))}
        if len(existing_labels)>=500: raise RuntimeError('Label listing may be incomplete; aborting before writes')
        for label in labels:
            if label not in existing_labels:
                gh('label','create',label,'--repo',EXPECTED,'--color','586069','--description','Contribution-kit classification')
        receipt=[]
        for item in items:
            title=item['issue_title']
            if title in existing: receipt.append({'id':item['id'],'status':'EXISTS','url':existing[title]['url']});continue
            # Body is passed as one argument, never interpolated as shell code.
            url=gh('issue','create','--repo',EXPECTED,'--title',title,'--body',item['body'],'--label',','.join(item['labels']))
            receipt.append({'id':item['id'],'status':'CREATED','url':url})
        print(json.dumps({'repository':EXPECTED,'issues':receipt},indent=2));return 0
    except (RuntimeError,ValueError,OSError) as e: print(str(e),file=sys.stderr);return 1

if __name__=='__main__':sys.exit(main())
