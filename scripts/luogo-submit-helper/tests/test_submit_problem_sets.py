#!/usr/bin/env python3
import io
import json
import sys
import tempfile
import unittest
from contextlib import redirect_stderr, redirect_stdout
from pathlib import Path
from unittest.mock import patch


PROJECT_ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(PROJECT_ROOT))

import submit_problem_sets
import submit_problem_sets_auto


class FakeHeaders:
    def get_content_charset(self):
        return "utf-8"


class FakeResponse:
    def __init__(self, text):
        self.data = text.encode("utf-8")
        self.headers = FakeHeaders()

    def __enter__(self):
        return self

    def __exit__(self, exc_type, exc_value, traceback):
        return False

    def read(self):
        return self.data


class FakeOpener:
    def __init__(self, responses):
        self.responses = iter(responses)
        self.urls = []

    def open(self, request, timeout):
        self.urls.append(request.full_url)
        return FakeResponse(next(self.responses))


class SubmitProblemSetsTest(unittest.TestCase):
    def test_parse_problem_set_and_id_mapping(self):
        markdown = "\n".join([
            "- [ ] [[problem: luogu,B2002]]",
            "- [x] [[problem: luogu,P1068]]",
        ])
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "set.md"
            path.write_text(markdown, encoding="utf-8")
            problems = submit_problem_sets.parse_problem_set(path)

        self.assertEqual([problem["pid"] for problem in problems], ["B2002", "P1068"])
        self.assertFalse(problems[0]["done"])
        self.assertTrue(problems[1]["done"])
        self.assertEqual(submit_problem_sets.luogu_dir_id("B2002"), "b2002")
        self.assertEqual(submit_problem_sets.luogu_dir_id("P1068"), "1068")
        self.assertEqual(submit_problem_sets.luogu_real_id("1068"), "P1068")

    def test_find_code_file_prefers_python_and_uses_luogu_directories(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            b2002 = root / "luogu" / "b2002"
            p1068 = root / "luogu" / "1068"
            b2002.mkdir(parents=True)
            p1068.mkdir(parents=True)
            (b2002 / "main.cpp").write_text("cpp", encoding="utf-8")
            (b2002 / "main.py").write_text("python", encoding="utf-8")
            (p1068 / "main.py").write_text("sort", encoding="utf-8")

            path, content = submit_problem_sets.find_code_file(
                "luogu", "B2002", str(root)
            )
            p_path, p_content = submit_problem_sets.find_code_file(
                "luogu", "P1068", str(root)
            )

        self.assertTrue(path.endswith("luogu/b2002/main.py"))
        self.assertEqual(content, "python")
        self.assertTrue(p_path.endswith("luogu/1068/main.py"))
        self.assertEqual(p_content, "sort")

    def test_fetch_public_accepted_problems_from_local_responses(self):
        search = json.dumps({
            "users": [{"uid": 3157, "name": "Rainboy"}],
        })
        practice_payload = {
            "data": {
                "passed": [
                    {"pid": "P1001", "name": "A+B"},
                    {"pid": "B2002", "name": "Hello"},
                ]
            }
        }
        practice = (
            '<script id="lentille-context" type="application/json">'
            + json.dumps(practice_payload)
            + "</script>"
        )
        opener = FakeOpener([search, practice])

        accepted, uid = submit_problem_sets.fetch_accepted_problem_ids(
            "rainboy", opener=opener
        )

        self.assertEqual(uid, 3157)
        self.assertEqual(accepted, {"P1001", "B2002"})
        self.assertIn("keyword=rainboy", opener.urls[0])
        self.assertIn("/user/3157/practice", opener.urls[1])

    def test_build_problem_list_filters_public_ac_and_markdown_done(self):
        references = [
            {"oj": "luogu", "pid": "P1001", "done": False},
            {"oj": "luogu", "pid": "B2002", "done": True},
            {"oj": "luogu", "pid": "P1068", "done": False},
        ]
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            for problem_dir in ["1001", "b2002", "1068"]:
                path = root / "luogu" / problem_dir
                path.mkdir(parents=True)
                (path / "main.py").write_text(problem_dir, encoding="utf-8")

            problems, stats = submit_problem_sets.build_navigable_problems(
                references,
                skip_done=True,
                accepted_ids={"P1001"},
                problems_root=str(root),
            )

        self.assertEqual([problem["real_id"] for problem in problems], ["P1068"])
        self.assertEqual(stats["done"], 2)

    def test_build_problem_list_filters_code_extension(self):
        references = [
            {"oj": "luogu", "pid": "P1001", "done": False},
            {"oj": "luogu", "pid": "P1002", "done": False},
            {"oj": "luogu", "pid": "P1003", "done": False},
        ]
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            for problem_dir in ["1001", "1002", "1003"]:
                (root / "luogu" / problem_dir).mkdir(parents=True)
            (root / "luogu" / "1001" / "main.py").write_text(
                "python only", encoding="utf-8"
            )
            (root / "luogu" / "1002" / "main.cpp").write_text(
                "cpp only", encoding="utf-8"
            )
            (root / "luogu" / "1003" / "main.py").write_text(
                "python preferred", encoding="utf-8"
            )
            (root / "luogu" / "1003" / "main.cpp").write_text(
                "cpp fallback", encoding="utf-8"
            )

            python_problems, python_stats = submit_problem_sets.build_navigable_problems(
                references,
                problems_root=str(root),
                code_ext="py",
            )
            default_problems, default_stats = submit_problem_sets.build_navigable_problems(
                references,
                problems_root=str(root),
            )

        self.assertEqual(
            [problem["real_id"] for problem in python_problems],
            ["P1001", "P1003"],
        )
        self.assertTrue(
            all(problem["code_path"].endswith("main.py") for problem in python_problems)
        )
        self.assertEqual(python_stats["missing_code"], 1)
        self.assertEqual(
            [problem["real_id"] for problem in default_problems],
            ["P1001", "P1002", "P1003"],
        )
        self.assertTrue(default_problems[1]["code_path"].endswith("main.cpp"))
        self.assertEqual(default_stats["missing_code"], 0)

    def test_main_uses_custom_problems_root(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            problem_set = root / "set.md"
            problems_root = root / "external-problems"
            solution_dir = problems_root / "luogu" / "1001"
            solution_dir.mkdir(parents=True)
            problem_set.write_text(
                "- [ ] [[problem: luogu,P1001]]\n",
                encoding="utf-8",
            )
            (solution_dir / "main.py").write_text(
                "print('ok')\n",
                encoding="utf-8",
            )

            with redirect_stdout(io.StringIO()):
                with patch.object(
                    submit_problem_sets,
                    "run_interactive",
                    return_value=0,
                ) as run_interactive:
                    result = submit_problem_sets.main([
                        str(problem_set),
                        "--problems-root", str(problems_root),
                        "--no-auto-open",
                    ])

        self.assertEqual(result, 0)
        problems = run_interactive.call_args.args[0]
        self.assertEqual(len(problems), 1)
        self.assertEqual(problems[0]["real_id"], "P1001")
        self.assertEqual(problems[0]["code_content"], "print('ok')\n")

    def test_automatic_helper_accepts_custom_problems_root(self):
        args = submit_problem_sets_auto.parse_arguments([
            "set.md",
            "--problems-root", "/tmp/external-problems",
        ])

        self.assertEqual(args.problems_root, "/tmp/external-problems")
        self.assertTrue(args.dry_run)

    def test_five_problem_window_moves_at_boundaries(self):
        problems = [
            {
                "real_id": f"P{index}",
                "code_path": f"problems/luogu/{index}/main.py",
            }
            for index in range(1, 8)
        ]

        first = submit_problem_sets.format_problem_window(problems, 0)
        middle = submit_problem_sets.format_problem_window(problems, 3)
        last = submit_problem_sets.format_problem_window(problems, 6)

        self.assertIn("> [1/7] P1", first)
        self.assertNotIn("[6/7]", first)
        self.assertIn("> [4/7] P4", middle)
        self.assertIn("[2/7] P2", middle)
        self.assertIn("[6/7] P6", middle)
        self.assertIn("> [7/7] P7", last)
        self.assertNotIn("[2/7]", last)

    def test_interactive_keys_open_and_navigate_without_extra_enter(self):
        problems = [
            {
                "real_id": "B2002",
                "code_path": "problems/luogu/b2002/main.py",
                "code_content": "first",
            },
            {
                "real_id": "P1068",
                "code_path": "problems/luogu/1068/main.py",
                "code_content": "second",
            },
        ]
        keys = iter(["o", "n", "p", "\n", "q"])
        opened = []
        copied = []
        output = io.StringIO()

        result = submit_problem_sets.run_interactive(
            problems,
            wait_min=0,
            wait_max=0,
            auto_open=True,
            key_reader=lambda: next(keys),
            browser_open=lambda url: opened.append(url) or True,
            copy_func=lambda code: copied.append(code) or True,
            output=output,
        )

        self.assertEqual(result, 0)
        # auto_open=True: opens on start, after n/p/Enter, plus 'o' press
        self.assertEqual(
            opened,
            [
                "https://www.luogu.com.cn/problem/B2002#submit",  # auto start
                "https://www.luogu.com.cn/problem/B2002#submit",  # 'o' key
                "https://www.luogu.com.cn/problem/P1068#submit",  # 'n' key
                "https://www.luogu.com.cn/problem/B2002#submit",  # 'p' key
                "https://www.luogu.com.cn/problem/P1068#submit",  # Enter countdown
            ],
        )
        self.assertEqual(copied, ["first", "second", "first", "second"])
        self.assertIn("等待结束", output.getvalue())

    def test_auto_open_disabled(self):
        problems = [
            {
                "real_id": "B2002",
                "code_path": "problems/luogu/b2002/main.py",
                "code_content": "first",
            },
            {
                "real_id": "P1068",
                "code_path": "problems/luogu/1068/main.py",
                "code_content": "second",
            },
        ]
        keys = iter(["n", "q"])
        opened = []
        copied = []
        output = io.StringIO()

        result = submit_problem_sets.run_interactive(
            problems,
            wait_min=0,
            wait_max=0,
            auto_open=False,
            key_reader=lambda: next(keys),
            browser_open=lambda url: opened.append(url) or True,
            copy_func=lambda code: copied.append(code) or True,
            output=output,
        )

        self.assertEqual(result, 0)
        # auto_open=False: no browser opens on navigation
        self.assertEqual(opened, [])
        self.assertEqual(copied, ["first", "second"])

    def test_countdown_can_be_cancelled(self):
        output = io.StringIO()

        def interrupt(_seconds):
            raise KeyboardInterrupt

        completed = submit_problem_sets.countdown(
            60,
            sleep_func=interrupt,
            output=output,
        )

        self.assertFalse(completed)
        self.assertIn("倒计时已取消", output.getvalue())

    def test_wait_range_validation(self):
        with redirect_stderr(io.StringIO()):
            with self.assertRaises(SystemExit):
                submit_problem_sets.parse_arguments([
                    "set.md",
                    "--wait-min", "5",
                    "--wait-max", "1",
                ])

    def test_code_extension_argument_validation(self):
        args = submit_problem_sets.parse_arguments([
            "set.md",
            "--code-ext", "py",
        ])
        self.assertEqual(args.code_ext, "py")

        with redirect_stderr(io.StringIO()):
            with self.assertRaises(SystemExit):
                submit_problem_sets.parse_arguments([
                    "set.md",
                    "--code-ext", "txt",
                ])


if __name__ == "__main__":
    unittest.main()
