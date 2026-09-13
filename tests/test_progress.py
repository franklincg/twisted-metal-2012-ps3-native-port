import copy,json,sys,unittest
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1]
sys.path.insert(0,str(ROOT/'tools'))
from render_progress import render
class ProgressTests(unittest.TestCase):
    def data(self):return {'overall_percent':None,'milestones':[{'id':'X','name':'Fixture','denominator_fixed':True,'gates':[{'id':'A','state':'passed','evidence':'README.md'},{'id':'B','state':'pending','evidence':None}]}]}
    def test_fixed_denominator(self):self.assertIn('50.0%',render(self.data()))
    def test_unknown_denominator(self):
        d=self.data();d['milestones'][0]['denominator_fixed']=False;self.assertIn('Not estimable',render(d))
    def test_rejects_overall_guess(self):
        d=self.data();d['overall_percent']=90
        with self.assertRaises(ValueError):render(d)
    def test_requires_evidence(self):
        d=self.data();d['milestones'][0]['gates'][0]['evidence']=None
        with self.assertRaises(ValueError):render(d)
    def test_rejects_duplicate_id(self):
        d=self.data();d['milestones'][0]['gates'][1]['id']='A'
        with self.assertRaises(ValueError):render(d)
    def test_rejects_empty_denominator(self):
        d=self.data();d['milestones'][0]['gates']=[]
        with self.assertRaises(ValueError):render(d)
    def test_repository_data(self):
        d=json.loads((ROOT/'data/milestones.json').read_text());self.assertIn('not estimable',render(d))
    def test_work_item_ids_and_dependencies(self):
        doc=json.loads((ROOT/'data/work_items.json').read_text());items=doc['items'];ids={i['id'] for i in items}
        self.assertEqual(len(ids),len(items))
        for item in items:
            self.assertTrue(set(item['depends_on'])<=ids)
            self.assertNotIn(item['id'],item['depends_on'])
            self.assertTrue(item['acceptance'])
        numbers=[item['github_issue_number'] for item in items]
        populated=[n for n in numbers if n is not None]
        self.assertEqual(len(populated),len(set(populated)))
        self.assertTrue(all(isinstance(n,int) and n>0 for n in populated))
        if doc.get('publication_state')=='published_issues_seeded': self.assertEqual(len(populated),len(items))
    def test_status_does_not_claim_game_ready(self):
        d=json.loads((ROOT/'data/status.json').read_text());self.assertFalse(d['menu_verified']);self.assertFalse(d['gameplay_verified']);self.assertIsNone(d['overall_completion_percent'])
if __name__=='__main__':unittest.main()
