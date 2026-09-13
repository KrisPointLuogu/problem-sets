#!/usr/bin/env python3
import argparse
import html.parser
import json
import math
import os
import random
import re
import shutil
import subprocess
import sys
import time
import webbrowser
from http.cookiejar import CookieJar
from urllib.error import URLError
from urllib.parse import urlencode
from urllib.request import HTTPCookieProcessor, Request, build_opener


LUOGU_BASE_URL = "https://www.luogu.com.cn"
DEFAULT_USER = "Rainboy"
CODE_EXTENSIONS = ["py", "cpp", "c", "java", "js", "rs", "hs"]


class LuoguLookupError(RuntimeError):
    pass


class LentilleContextParser(html.parser.HTMLParser):
    def __init__(self):
        super().__init__()
        self.in_context = False
        self.context_parts = []

    def handle_starttag(self, tag, attrs):
        if tag != "script":
            return
        attributes = dict(attrs)
        if attributes.get("id") == "lentille-context":
            self.in_context = True

    def handle_endtag(self, tag):
        if tag == "script" and self.in_context:
            self.in_context = False

    def handle_data(self, data):
        if self.in_context:
            self.context_parts.append(data)


def get_clipboard_command():
    if sys.platform.startswith("darwin"):
        return ["pbcopy"]
    if sys.platform.startswith("linux"):
        if shutil.which("wl-copy"):
            return ["wl-copy"]
        if shutil.which("xclip"):
            return ["xclip", "-selection", "clipboard"]
        if shutil.which("xsel"):
            return ["xsel", "--clipboard", "--input"]
    return None


def copy_to_clipboard(text):
    command = get_clipboard_command()
    if not command:
        return False
    try:
        process = subprocess.Popen(command, stdin=subprocess.PIPE)
        process.communicate(input=text.encode("utf-8"))
        return process.returncode == 0
    except OSError:
        return False


def parse_problem_set(filepath):
    """Parse problem references from a Markdown problem set in source order."""
    with open(filepath, "r", encoding="utf-8") as file:
        content = file.read()

    pattern = r"- \[(.)\] \[\[problem:\s*([^,]+),\s*([^\]]+)\]\]"
    problems = []
    for match in re.finditer(pattern, content):
        problems.append({
            "oj": match.group(2).strip(),
            "pid": match.group(3).strip(),
            "done": match.group(1).strip().lower() == "x",
        })
    return problems


def luogu_real_id(pid):
    """Return the ID used in Luogu URLs, such as P1068 or B2002."""
    pid = pid.strip()
    return f"P{pid}" if pid[:1].isdigit() else pid.upper()


def luogu_dir_id(pid):
    """Map a Luogu ID to this repository's directory convention."""
    real_id = luogu_real_id(pid)
    if real_id.startswith("P"):
        return real_id[1:].lower()
    return real_id.lower()


def unique_paths(paths):
    seen = set()
    result = []
    for path in paths:
        normalized = os.path.normpath(path)
        if normalized in seen:
            continue
        seen.add(normalized)
        result.append(path)
    return result


def candidate_problem_dirs(oj, pid, problems_root="problems"):
    """Return possible local directories, with repository conventions first."""
    oj_dir = oj.lower()
    oj_root = os.path.join(problems_root, oj_dir)
    if oj_dir == "luogu":
        real_id = luogu_real_id(pid)
        return unique_paths([
            os.path.join(oj_root, luogu_dir_id(pid)),
            os.path.join(oj_root, pid),
            os.path.join(oj_root, real_id),
            os.path.join(oj_root, real_id.lower()),
        ])
    return [os.path.join(oj_root, pid)]


def find_code_file(oj, pid, problems_root="problems", code_ext=None):
    """Find the main solution file, optionally restricted to one extension."""
    checked_dirs = []
    directories_without_code = []
    extensions = [code_ext] if code_ext else CODE_EXTENSIONS
    for base_dir in candidate_problem_dirs(oj, pid, problems_root):
        checked_dirs.append(base_dir)
        if not os.path.isdir(base_dir):
            continue

        for extension in extensions:
            code_path = os.path.join(base_dir, f"main.{extension}")
            if os.path.isfile(code_path):
                with open(code_path, "r", encoding="utf-8") as file:
                    return code_path, file.read()
        directories_without_code.append(base_dir)

    if directories_without_code:
        expected_code = f"main.{code_ext}" if code_ext else "main.py / main.cpp 等代码文件"
        return None, f"目录存在但没有 {expected_code}: " + ", ".join(directories_without_code)
    return None, "未找到题目目录，已尝试: " + ", ".join(checked_dirs)


def parse_luogu_payload(text):
    """Parse either a JSON response or lentille-context embedded in HTML."""
    stripped = text.strip()
    try:
        payload = json.loads(stripped)
    except json.JSONDecodeError:
        parser = LentilleContextParser()
        parser.feed(text)
        context = "".join(parser.context_parts).strip()
        if not context:
            raise LuoguLookupError("洛谷响应中没有找到 lentille-context 数据")
        try:
            payload = json.loads(context)
        except json.JSONDecodeError as error:
            raise LuoguLookupError("洛谷页面中的用户数据不是有效 JSON") from error

    if not isinstance(payload, dict):
        raise LuoguLookupError("洛谷返回了无法识别的数据格式")
    return payload


def extract_user_uid(payload, username):
    containers = [payload, payload.get("data"), payload.get("currentData")]
    users = None
    for container in containers:
        if isinstance(container, dict) and "users" in container:
            users = container["users"]
            break
    if not isinstance(users, list):
        raise LuoguLookupError("洛谷用户搜索结果中缺少 users 列表")

    expected = username.casefold()
    for user in users:
        if not isinstance(user, dict):
            continue
        if str(user.get("name", "")).casefold() == expected:
            try:
                return int(user["uid"])
            except (KeyError, TypeError, ValueError) as error:
                raise LuoguLookupError("匹配的洛谷用户缺少有效 UID") from error
    raise LuoguLookupError(f"没有找到洛谷用户 {username}")


def extract_accepted_problem_ids(payload):
    containers = [payload, payload.get("data"), payload.get("currentData")]
    passed = None
    for container in containers:
        if isinstance(container, dict) and "passed" in container:
            passed = container["passed"]
            break
    if not isinstance(passed, list):
        raise LuoguLookupError("洛谷练习页中缺少已通过题目列表")

    accepted = set()
    for problem in passed:
        if not isinstance(problem, dict) or not problem.get("pid"):
            continue
        accepted.add(luogu_real_id(str(problem["pid"])))
    return accepted


def make_luogu_opener():
    return build_opener(HTTPCookieProcessor(CookieJar()))


def fetch_text(opener, url, timeout=20):
    request = Request(
        url,
        headers={
            "Accept": "application/json,text/html;q=0.9,*/*;q=0.8",
            "Accept-Encoding": "identity",
            "User-Agent": "luogu-problem-set-submit-helper/1.0",
        },
    )
    with opener.open(request, timeout=timeout) as response:
        charset = response.headers.get_content_charset() or "utf-8"
        return response.read().decode(charset, errors="replace")


def fetch_accepted_problem_ids(username, opener=None):
    """Read one user's public accepted-problem list without login cookies."""
    opener = opener or make_luogu_opener()
    search_url = f"{LUOGU_BASE_URL}/api/user/search?{urlencode({'keyword': username})}"
    try:
        search_payload = parse_luogu_payload(fetch_text(opener, search_url))
        uid = extract_user_uid(search_payload, username)
        practice_url = f"{LUOGU_BASE_URL}/user/{uid}/practice?_contentOnly=1"
        practice_payload = parse_luogu_payload(fetch_text(opener, practice_url))
        return extract_accepted_problem_ids(practice_payload), uid
    except LuoguLookupError:
        raise
    except (OSError, TimeoutError, URLError) as error:
        raise LuoguLookupError(f"连接洛谷失败: {error}") from error


def build_navigable_problems(problem_refs, skip_done=False, accepted_ids=None,
                              problems_root="problems", code_ext=None):
    accepted_ids = accepted_ids or set()
    navigable = []
    stats = {"non_luogu": 0, "done": 0, "missing_code": 0}

    for problem in problem_refs:
        if problem["oj"].lower() != "luogu":
            stats["non_luogu"] += 1
            continue

        real_id = luogu_real_id(problem["pid"])
        if skip_done and (problem["done"] or real_id in accepted_ids):
            stats["done"] += 1
            continue

        code_path, code_content = find_code_file(
            problem["oj"], problem["pid"], problems_root, code_ext=code_ext
        )
        if not code_path:
            stats["missing_code"] += 1
            continue

        navigable.append({
            **problem,
            "real_id": real_id,
            "code_path": code_path,
            "code_content": code_content,
        })
    return navigable, stats


def submission_url(pid):
    return f"{LUOGU_BASE_URL}/problem/{luogu_real_id(pid)}#submit"


def problem_window_bounds(index, total, size=5):
    size = min(size, total)
    start = max(0, index - size // 2)
    start = min(start, total - size)
    return start, start + size


def format_problem_window(problems, index, status_message=""):
    total = len(problems)
    start, end = problem_window_bounds(index, total)
    lines = ["洛谷手动提交助手", ""]
    for position in range(start, end):
        problem = problems[position]
        marker = ">" if position == index else " "
        suffix = f"  {problem['code_path']}" if position == index else ""
        lines.append(
            f"{marker} [{position + 1}/{total}] {problem['real_id']}{suffix}"
        )
    lines.extend([
        "",
        f"当前提交页: {submission_url(problems[index]['real_id'])}",
        f"状态: {status_message}" if status_message else "状态:",
        "",
        "o 重新打开  n 下一题(自动打开)  Enter 倒计时后下一题  p 上一题  q 退出",
    ])
    return "\n".join(lines)


def read_single_key(stream=None):
    stream = stream or sys.stdin
    if os.name == "nt":
        import msvcrt
        key = msvcrt.getwch()
    elif stream.isatty():
        import termios
        import tty
        descriptor = stream.fileno()
        original = termios.tcgetattr(descriptor)
        try:
            tty.setraw(descriptor)
            key = os.read(descriptor, 1).decode("utf-8", errors="ignore")
        finally:
            termios.tcsetattr(descriptor, termios.TCSADRAIN, original)
    else:
        line = stream.readline()
        if line == "":
            return "q"
        key = "\n" if line in ("\n", "\r\n") else line[0]

    if key == "\x03":
        raise KeyboardInterrupt
    if key in ("\r", "\n"):
        return "\n"
    return key.lower()


def clear_terminal(output):
    if hasattr(output, "isatty") and output.isatty():
        output.write("\033[2J\033[H")


def countdown(seconds, sleep_func=time.sleep, output=None):
    output = output or sys.stdout
    remaining_seconds = max(0, math.ceil(seconds))
    try:
        while remaining_seconds > 0:
            minutes, seconds_part = divmod(remaining_seconds, 60)
            output.write(
                f"\r等待 {minutes:02d}:{seconds_part:02d} 后进入下一题..."
            )
            output.flush()
            sleep_func(1)
            remaining_seconds -= 1
    except KeyboardInterrupt:
        output.write("\r倒计时已取消，停留在当前题。          \n")
        output.flush()
        return False

    output.write("\r等待结束，进入下一题。                \n")
    output.flush()
    return True


def copy_current_problem(problem, copy_func=copy_to_clipboard):
    if copy_func(problem["code_content"]):
        return f"已复制 {problem['code_path']}"
    return f"未能复制 {problem['code_path']}，请检查剪贴板工具"


def run_interactive(problems, wait_min, wait_max, auto_open=True,
                    key_reader=None, browser_open=None, copy_func=None,
                    random_uniform=None, sleep_func=None, output=None):
    key_reader = key_reader or read_single_key
    browser_open = browser_open or webbrowser.open_new_tab
    copy_func = copy_func or copy_to_clipboard
    random_uniform = random_uniform or random.uniform
    sleep_func = sleep_func or time.sleep
    output = output or sys.stdout

    def open_current():
        url = submission_url(problems[index]["real_id"])
        if browser_open(url):
            return f"已复制代码并打开 {url}"
        return f"已复制代码，但浏览器未能打开 {url}"

    index = 0
    status_message = copy_current_problem(problems[index], copy_func)
    if auto_open:
        url = submission_url(problems[index]["real_id"])
        if browser_open(url):
            status_message = f"已复制代码并打开 {url}"
    while True:
        clear_terminal(output)
        output.write(format_problem_window(problems, index, status_message) + "\n")
        output.flush()
        try:
            key = key_reader()
        except KeyboardInterrupt:
            output.write("\n已退出提交流程。\n")
            return 0

        if key == "q":
            output.write("\n已退出提交流程。\n")
            return 0
        if key == "o":
            url = submission_url(problems[index]["real_id"])
            if browser_open(url):
                status_message = f"已在浏览器打开 {url}"
            else:
                status_message = f"浏览器未能打开，请手动访问 {url}"
            continue
        if key == "p":
            if index == 0:
                status_message = "已经是第一题"
            else:
                index -= 1
                status_message = copy_current_problem(problems[index], copy_func)
                if auto_open:
                    url = submission_url(problems[index]["real_id"])
                    if browser_open(url):
                        status_message = f"已复制代码并打开 {url}"
            continue
        if key not in ("n", "\n"):
            status_message = f"未知按键: {repr(key)}"
            continue
        if index + 1 >= len(problems):
            status_message = "已经是最后一题"
            continue

        if key == "\n":
            wait_seconds = random_uniform(wait_min * 60, wait_max * 60)
            if not countdown(wait_seconds, sleep_func=sleep_func, output=output):
                status_message = "倒计时已取消"
                continue

        index += 1
        status_message = copy_current_problem(problems[index], copy_func)
        if auto_open:
            url = submission_url(problems[index]["real_id"])
            if browser_open(url):
                status_message = f"已复制代码并打开 {url}"


def parse_arguments(argv=None):
    parser = argparse.ArgumentParser(
        description="按题单顺序辅助进行洛谷手动提交"
    )
    parser.add_argument("problem_set", help="题单 Markdown 文件路径")
    parser.add_argument(
        "--problems-root",
        default="problems",
        help="题目代码根目录（默认: problems）",
    )
    parser.add_argument(
        "--skip-done",
        action="store_true",
        help="跳过 Markdown 已勾选及洛谷账号已通过的题目",
    )
    parser.add_argument(
        "--user",
        default=DEFAULT_USER,
        help=f"用于查询公开 AC 记录的洛谷用户名（默认: {DEFAULT_USER}）",
    )
    parser.add_argument(
        "--wait-min",
        type=float,
        default=1.0,
        help="Enter 随机等待的最少分钟数（默认: 1）",
    )
    parser.add_argument(
        "--wait-max",
        type=float,
        default=5.0,
        help="Enter 随机等待的最多分钟数（默认: 5）",
    )
    parser.add_argument(
        "--no-auto-open",
        action="store_true",
        help="不自动打开浏览器（默认会自动打开提交页）",
    )
    parser.add_argument(
        "--code-ext",
        choices=CODE_EXTENSIONS,
        help="只选择对应的 main.<扩展名> 文件，例如 --code-ext py",
    )
    args = parser.parse_args(argv)
    if args.wait_min < 0 or args.wait_max < 0:
        parser.error("等待时间不能为负数")
    if args.wait_min > args.wait_max:
        parser.error("--wait-min 不能大于 --wait-max")
    return args


def main(argv=None):
    args = parse_arguments(argv)
    try:
        problem_refs = parse_problem_set(args.problem_set)
    except OSError as error:
        print(f"读取题单失败: {error}", file=sys.stderr)
        return 2

    print(f"解析题单: {args.problem_set}")
    print(f"共找到 {len(problem_refs)} 个题目引用。")
    if args.code_ext:
        print(f"代码过滤: 只使用 main.{args.code_ext}")

    accepted_ids = set()
    if args.skip_done:
        print(f"正在查询洛谷用户 {args.user} 的公开 AC 记录...")
        try:
            accepted_ids, uid = fetch_accepted_problem_ids(args.user)
        except LuoguLookupError as error:
            print(f"无法使用 --skip-done: {error}", file=sys.stderr)
            return 2
        print(f"已找到用户 UID {uid}，公开 AC 题目 {len(accepted_ids)} 道。")

    problems, stats = build_navigable_problems(
        problem_refs,
        skip_done=args.skip_done,
        accepted_ids=accepted_ids,
        problems_root=args.problems_root,
        code_ext=args.code_ext,
    )
    print(
        f"本次可浏览 {len(problems)} 道；"
        f"跳过已完成 {stats['done']} 道，"
        f"缺少代码 {stats['missing_code']} 道。"
    )
    if not problems:
        expected_code = (
            f"main.{args.code_ext}"
            if args.code_ext
            else "main.py / main.cpp 等代码文件"
        )
        print(f"没有可提交且含 {expected_code} 的洛谷题目。")
        return 0

    return run_interactive(
        problems,
        wait_min=args.wait_min,
        wait_max=args.wait_max,
        auto_open=not args.no_auto_open,
    )


if __name__ == "__main__":
    raise SystemExit(main())
