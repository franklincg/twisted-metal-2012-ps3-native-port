"""Tests use temporary synthetic trees only. They never publish or read a game."""
import importlib.util,json,sys,tempfile,unittest
from pathlib import Path
TOOLS=Path(__file__).resolve().parents[1]/'tools'
sys.path.insert(0,str(TOOLS))
from audit_public import audit,safe_relative,content_errors,digest
from export_public import export

class PublicationTests(unittest.TestCase):
    def fixture(self,root:Path):
        (root/'publication').mkdir();(root/'README.md').write_text('# Public fixture\n')
        names=['README.md','publication/allowlist.json','publication/manifest.json']
        (root/'publication/allowlist.json').write_text(json.dumps({'paths':names}))
        records={n:{'sha256':digest((root/n).read_bytes()),'bytes':len((root/n).read_bytes()),'origin':'synthetic test','license':'MIT'} for n in names if n!='publication/manifest.json'}
        (root/'publication/manifest.json').write_text(json.dumps({'files':records}))
    def test_valid_relative(self):self.assertTrue(safe_relative('labs/probe.c'))
    def test_traversal(self):self.assertFalse(safe_relative('../private/data'))
    def test_absolute(self):self.assertFalse(safe_relative('/tmp/data'))
    def test_windows_absolute(self):self.assertFalse(safe_relative('Z:'+chr(92)+'fixture'))
    def test_empty_segment(self):self.assertFalse(safe_relative('a//b'))
    def test_env(self):self.assertFalse(safe_relative('.env.local'))
    def test_binary(self):self.assertTrue(content_errors('x.c',b'MZ'+bytes(10)))
    def test_archive(self):self.assertTrue(content_errors('x.txt',b'PK'+bytes([3,4])+b'x'))
    def test_large(self):self.assertTrue(content_errors('x.md',b'a'*(512*1024+1)))
    def test_utf8(self):self.assertTrue(content_errors('x.md',bytes([255,254])))
    def test_token(self):self.assertTrue(content_errors('x.md',('gh'+'p_'+'A'*36).encode()))
    def test_personal_path(self):self.assertTrue(content_errors('x.md',('Z:'+chr(92)+'Users'+chr(92)+'fixture').encode()))
    def test_workflow_pin(self):self.assertTrue(content_errors('.github/workflows/x.yml',b'jobs:\n  - uses: actions/checkout@main\n'))
    def test_valid_tree(self):
        with tempfile.TemporaryDirectory() as t:
            r=Path(t);self.fixture(r);self.assertEqual(audit(r)['status'],'PASS')
    def test_changed_source(self):
        with tempfile.TemporaryDirectory() as t:
            r=Path(t);self.fixture(r);(r/'README.md').write_text('changed');self.assertEqual(audit(r)['status'],'FAIL')
    def test_unlisted(self):
        with tempfile.TemporaryDirectory() as t:
            r=Path(t);self.fixture(r);(r/'other.txt').write_text('unexpected');self.assertEqual(audit(r)['status'],'FAIL')
    def test_missing(self):
        with tempfile.TemporaryDirectory() as t:
            r=Path(t);self.fixture(r);(r/'README.md').unlink();self.assertEqual(audit(r)['status'],'FAIL')
    def test_symlink(self):
        with tempfile.TemporaryDirectory() as t:
            r=Path(t);self.fixture(r);(r/'README.md').unlink()
            try:(r/'README.md').symlink_to(r/'publication/allowlist.json')
            except OSError:self.skipTest('symlink creation not permitted')
            self.assertEqual(audit(r)['status'],'FAIL')
    def test_export_only_allowlist(self):
        with tempfile.TemporaryDirectory() as t:
            parent=Path(t);r=parent/'source';r.mkdir();self.fixture(r)
            (r/'.git').mkdir();(r/'.git/config').write_text('excluded history')
            dest=parent/'export';result=export(r,dest)
            self.assertEqual(result['file_count'],3);self.assertFalse((dest/'.git').exists());self.assertEqual(audit(dest)['status'],'PASS')
    def test_export_refuses_existing(self):
        with tempfile.TemporaryDirectory() as t:
            p=Path(t);r=p/'s';r.mkdir();self.fixture(r);d=p/'d';d.mkdir();(d/'keep').write_text('safe')
            with self.assertRaises(ValueError):export(r,d)
            self.assertEqual((d/'keep').read_text(),'safe')
    def test_export_refuses_nested(self):
        with tempfile.TemporaryDirectory() as t:
            r=Path(t);self.fixture(r)
            with self.assertRaises(ValueError):export(r,r/'nested')
    def test_export_refuses_changed(self):
        with tempfile.TemporaryDirectory() as t:
            p=Path(t);r=p/'s';r.mkdir();self.fixture(r);(r/'README.md').write_text('changed')
            with self.assertRaises(ValueError):export(r,p/'d')
            self.assertFalse((p/'d').exists())

if __name__=='__main__':unittest.main()
