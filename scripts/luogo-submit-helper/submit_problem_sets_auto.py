#!/usr/bin/env python3
"""
洛谷自动提交助手（基于 submit_problem_sets.py 的题单/AC 过滤逻辑）。

安全设计：
- 默认 --dry-run：只打开页面、填代码、不点提交
- 真正提交需要显式 --execute
- 每题之间随机等待，默认 45~120 秒
- 默认最多提交 20 题/次
- 复用公开 AC 列表，跳过已通过题目
- 使用持久化 Chromium 配置目录保存登录状态

示例：
  # 1) 先登录一次（会打开浏览器，手动登录后关闭）
  python3 submit_problem_sets_auto.py \\
    problem-sets/luogu-official-basic-training.md --login-only

  # 2) 预演（不提交）
  python3 submit_problem_sets_auto.py \\
    problem-sets/luogu-official-basic-training.md --skip-done --dry-run --limit 5

  # 3) 真正慢速提交
  python3 submit_problem_sets_auto.py \\
    problem-sets/luogu-official-basic-training.md --skip-done --execute \\
    --wait-min 60 --wait-max 150 --limit 20
"""

from __future__ import annotations

import argparse
import os
import random
import sys
import time
from pathlib import Path

import submit_problem_sets

DEFAULT_PROFILE_DIR = Path.home() / ".cache" / "luogo-submit-helper" / "chromium"
LANG_MAP = {
    ".py": "Python3",
    ".cpp": "C++",
    ".c": "C",
    ".java": "Java",
    ".js": "JavaScript",
    ".rs": "Rust",
    ".hs": "Haskell",
}


def detect_lang_label(code_path: str) -> str:
    ext = Path(code_path).suffix.lower()
    return LANG_MAP.get(ext, "C++")


def wait_human(min_seconds: float, max_seconds: float, label: str = "等待") -> None:
    seconds = random.uniform(min_seconds, max_seconds)
    end = time.time() + seconds
    while True:
        left = int(end - time.time())
        if left <= 0:
            break
        mins, secs = divmod(left, 60)
        print(f"\r{label} {mins:02d}:{secs:02d} ...", end="", flush=True)
        time.sleep(1)
    print(f"\r{label}结束。                ")


def ensure_playwright():
    try:
        from playwright.sync_api import sync_playwright
    except ImportError as error:
        raise RuntimeError(
            "未安装 playwright。请先执行: pip install playwright && playwright install chromium"
        ) from error
    return sync_playwright


def open_browser(user_data_dir: Path, headless: bool = False):
    sync_playwright = ensure_playwright()
    playwright = sync_playwright().start()
    user_data_dir.mkdir(parents=True, exist_ok=True)
    context = playwright.chromium.launch_persistent_context(
        user_data_dir=str(user_data_dir),
        channel="chromium",
        headless=headless,
        viewport={"width": 1400, "height": 900},
        locale="zh-CN",
        args=["--disable-blink-features=AutomationControlled"],
    )
    page = context.pages[0] if context.pages else context.new_page()
    return playwright, context, page


def is_logged_in(page) -> bool:
    try:
        page.goto(submit_problem_sets.LUOGU_BASE_URL, wait_until="domcontentloaded", timeout=60000)
        page.wait_for_timeout(2000)
        cookies = page.context.cookies()
        names = {c.get("name") for c in cookies}
        # 洛谷登录后通常有 _uid
        if "_uid" in names:
            return True
        # 兜底：页面出现用户中心/退出
        content = page.content()
        if "退出登录" in content or "个人主页" in content:
            return True
        return False
    except Exception:
        return False


def ask_continue(prompt: str, default_no: bool = True) -> bool:
    if not sys.stdin.isatty():
        return not default_no
    answer = input(prompt).strip().lower()
    return answer in {"c", "y", "yes"}


def login_only(user_data_dir: Path) -> int:
    print(f"使用配置目录: {user_data_dir}")
    print("将打开 Chromium。请在浏览器中手动登录洛谷，完成后回到终端按 Enter。")
    playwright, context, page = open_browser(user_data_dir, headless=False)
    try:
        page.goto(f"{submit_problem_sets.LUOGU_BASE_URL}/auth/login", wait_until="domcontentloaded")
        input("登录完成后按 Enter 继续检查登录状态...")
        if is_logged_in(page):
            print("登录状态看起来正常，cookie 已写入配置目录。")
            return 0
        print("未能确认登录状态。请重新运行 --login-only。")
        return 1
    finally:
        context.close()
        playwright.stop()


def set_language(page, lang_label: str) -> None:
    # 洛谷前端会变，尽量多兜底
    candidates = [
        page.get_by_role("combobox"),
        page.locator("select"),
        page.locator(".lang-select, .language-select, [class*='lang']"),
    ]
    for locator in candidates:
        try:
            if locator.count() == 0:
                continue
            first = locator.first
            tag = first.evaluate("el => el.tagName.toLowerCase()")
            if tag == "select":
                # 尝试按可见文本选择
                options = first.locator("option")
                n = options.count()
                for i in range(n):
                    text = options.nth(i).inner_text().strip()
                    if lang_label.lower() in text.lower() or text.lower() in lang_label.lower():
                        value = options.nth(i).get_attribute("value")
                        if value is not None:
                            first.select_option(value=value)
                        else:
                            first.select_option(label=text)
                        return
            else:
                first.click(timeout=2000)
                page.get_by_text(lang_label, exact=False).first.click(timeout=2000)
                return
        except Exception:
            continue
    print(f"警告: 未能自动切换语言到 {lang_label}，将沿用页面默认语言。")


def fill_code(page, code: str) -> None:
    # 先点开“提交”相关区域，等待编辑器渲染
    for text in ("提交答案", "提交", "提交代码"):
        try:
            loc = page.get_by_text(text, exact=False)
            if loc.count() > 0:
                loc.first.click(timeout=1500)
                page.wait_for_timeout(800)
                break
        except Exception:
            pass

    tried = []

    # 0) 全局 JS 兜底：CodeMirror / Ace / Monaco / textarea
    try:
        ok = page.evaluate(
            """(code) => {
                // CodeMirror
                const cm = document.querySelector('.CodeMirror');
                if (cm && cm.CodeMirror) {
                    cm.CodeMirror.setValue(code);
                    return 'CodeMirror';
                }
                // Ace
                const aceEl = document.querySelector('.ace_editor');
                if (aceEl) {
                    if (aceEl.env && aceEl.env.editor) {
                        aceEl.env.editor.setValue(code, -1);
                        return 'ace-env';
                    }
                    if (window.ace) {
                        try {
                            window.ace.edit(aceEl).setValue(code, -1);
                            return 'ace-global';
                        } catch (e) {}
                    }
                }
                // Monaco
                if (window.monaco && window.monaco.editor) {
                    const models = window.monaco.editor.getModels();
                    if (models && models.length) {
                        models[0].setValue(code);
                        return 'monaco';
                    }
                }
                // textarea
                const areas = Array.from(document.querySelectorAll('textarea'));
                for (const ta of areas) {
                    if (ta.offsetParent === null && ta.getAttribute('aria-hidden') === 'true') {
                        // still try hidden editors used by CM
                    }
                    ta.focus();
                    ta.value = code;
                    ta.dispatchEvent(new Event('input', { bubbles: true }));
                    ta.dispatchEvent(new Event('change', { bubbles: true }));
                    if (ta.value === code || ta.value.length > 0) return 'textarea';
                }
                return '';
            }""",
            code,
        )
        if ok:
            return
        tried.append(f"js-global({ok or 'empty'})")
    except Exception as error:
        tried.append(f"js-global-failed:{error}")

    # 1) 点击编辑器区域后键盘输入
    for selector in (
        ".CodeMirror",
        ".ace_editor",
        ".monaco-editor",
        "[class*='editor']",
        "textarea",
    ):
        try:
            loc = page.locator(selector)
            if loc.count() == 0:
                tried.append(selector)
                continue
            loc.first.click(timeout=2000)
            page.keyboard.press("Control+A")
            page.keyboard.insert_text(code)
            return
        except Exception:
            tried.append(f"{selector}-failed")

    raise RuntimeError(
        "无法填入代码。常见原因：未登录 / 页面未加载提交区 / 选择器失效。"
        f" 已尝试: {', '.join(tried)}"
    )


def click_submit(page) -> None:
    candidates = [
        page.get_by_role("button", name="提交"),
        page.get_by_role("button", name="提交代码"),
        page.locator("button:has-text('提交')"),
        page.locator("a:has-text('提交')"),
        page.locator("input[type='submit']"),
    ]
    for locator in candidates:
        try:
            if locator.count() == 0:
                continue
            locator.first.click(timeout=4000)
            return
        except Exception:
            continue
    raise RuntimeError("找不到提交按钮")


def submit_one(page, problem, dry_run: bool) -> str:
    real_id = problem["real_id"]
    url = f"{submit_problem_sets.LUOGU_BASE_URL}/problem/{real_id}#submit"
    page.goto(url, wait_until="domcontentloaded", timeout=90000)
    page.wait_for_timeout(random.randint(1200, 2500))

    lang = detect_lang_label(problem["code_path"])
    set_language(page, lang)
    page.wait_for_timeout(random.randint(400, 900))
    fill_code(page, problem["code_content"])
    page.wait_for_timeout(random.randint(600, 1400))

    if dry_run:
        return f"dry-run: 已打开并填入 {real_id}（语言偏好 {lang}），未点击提交"

    click_submit(page)
    page.wait_for_timeout(random.randint(1500, 3000))
    return f"submitted: {real_id}"


def parse_arguments(argv=None):
    parser = argparse.ArgumentParser(
        description="洛谷自动提交助手（默认 dry-run，需 --execute 才真正提交）"
    )
    parser.add_argument("problem_set", nargs="?", help="题单 Markdown 文件路径")
    parser.add_argument(
        "--problems-root",
        default="problems",
        help="题目代码根目录（默认: problems）",
    )
    parser.add_argument("--login-only", action="store_true", help="仅打开浏览器供手动登录")
    parser.add_argument(
        "--skip-done",
        action="store_true",
        help="跳过 Markdown 已勾选及洛谷公开 AC 记录中的题目",
    )
    parser.add_argument(
        "--user",
        default=submit_problem_sets.DEFAULT_USER,
        help=f"查询公开 AC 的洛谷用户名（默认: {submit_problem_sets.DEFAULT_USER}）",
    )
    parser.add_argument(
        "--profile-dir",
        default=str(DEFAULT_PROFILE_DIR),
        help=f"Chromium 持久化配置目录（默认: {DEFAULT_PROFILE_DIR}）",
    )
    parser.add_argument("--dry-run", action="store_true", help="只填代码不提交（默认行为）")
    parser.add_argument("--execute", action="store_true", help="真正点击提交")
    parser.add_argument("--limit", type=int, default=20, help="本轮最多处理题数（默认 20）")
    parser.add_argument(
        "--wait-min",
        type=float,
        default=45.0,
        help="两题之间最少等待秒数（默认 45）",
    )
    parser.add_argument(
        "--wait-max",
        type=float,
        default=120.0,
        help="两题之间最多等待秒数（默认 120）",
    )
    parser.add_argument("--headless", action="store_true", help="无头模式（不推荐，登录难）")
    parser.add_argument("--start-from", default="", help="从某个题号开始，如 P1001")
    args = parser.parse_args(argv)

    if not args.login_only and not args.problem_set:
        parser.error("请提供题单路径，或使用 --login-only")
    if args.wait_min < 0 or args.wait_max < 0:
        parser.error("等待时间不能为负")
    if args.wait_min > args.wait_max:
        parser.error("--wait-min 不能大于 --wait-max")
    if args.limit <= 0:
        parser.error("--limit 必须为正整数")
    if args.execute and args.dry_run:
        parser.error("--execute 与 --dry-run 不能同时使用")
    if not args.execute:
        # 默认 dry-run
        args.dry_run = True
    return args


def main(argv=None) -> int:
    args = parse_arguments(argv)
    profile_dir = Path(args.profile_dir).expanduser()

    if args.login_only:
        return login_only(profile_dir)

    try:
        problem_refs = submit_problem_sets.parse_problem_set(args.problem_set)
    except OSError as error:
        print(f"读取题单失败: {error}", file=sys.stderr)
        return 2

    print(f"解析题单: {args.problem_set}")
    print(f"共找到 {len(problem_refs)} 个题目引用。")

    accepted_ids = set()
    if args.skip_done:
        print(f"正在查询洛谷用户 {args.user} 的公开 AC 记录...")
        try:
            accepted_ids, uid = submit_problem_sets.fetch_accepted_problem_ids(args.user)
        except submit_problem_sets.LuoguLookupError as error:
            print(f"无法使用 --skip-done: {error}", file=sys.stderr)
            return 2
        print(f"已找到用户 UID {uid}，公开 AC 题目 {len(accepted_ids)} 道。")

    problems, stats = submit_problem_sets.build_navigable_problems(
        problem_refs,
        skip_done=args.skip_done,
        accepted_ids=accepted_ids,
        problems_root=args.problems_root,
    )
    print(
        f"候选 {len(problems)} 道；"
        f"跳过已完成 {stats['done']} 道，"
        f"缺少代码 {stats['missing_code']} 道。"
    )

    if args.start_from:
        start_id = submit_problem_sets.luogu_real_id(args.start_from)
        idx = next(
            (i for i, p in enumerate(problems) if p["real_id"] == start_id),
            None,
        )
        if idx is None:
            print(f"未在候选列表中找到起点 {start_id}")
            return 2
        problems = problems[idx:]
        print(f"从 {start_id} 开始，剩余 {len(problems)} 道。")

    problems = problems[: args.limit]
    if not problems:
        print("没有可处理的题目。")
        return 0

    mode = "EXECUTE 真实提交" if args.execute else "DRY-RUN 预演"
    print(f"模式: {mode}")
    print(f"本轮处理: {len(problems)} 道")
    print(f"题间等待: {args.wait_min:.0f}~{args.wait_max:.0f} 秒")
    print(f"浏览器配置: {profile_dir}")

    if args.execute:
        print("\n警告: 真实提交可能触发洛谷风控。建议小批量、慢速。")
        if sys.stdin.isatty():
            confirm = input("确认开始真实提交？输入 yes 继续: ").strip()
            if confirm != "yes":
                print("已取消。")
                return 0
        else:
            print("非交互终端：已带 --execute，将直接开始。")

    playwright, context, page = open_browser(profile_dir, headless=args.headless)
    try:
        if not is_logged_in(page):
            print("未检测到登录状态。请先运行:")
            print(
                "  python3 submit_problem_sets_auto.py --login-only"
            )
            return 1

        for i, problem in enumerate(problems, start=1):
            print("\n" + "=" * 60)
            print(f"[{i}/{len(problems)}] {problem['real_id']}")
            print(f"代码: {problem['code_path']}")
            print(f"链接: {submit_problem_sets.submission_url(problem['real_id'])}")
            try:
                msg = submit_one(page, problem, dry_run=args.dry_run)
                print(msg)
            except Exception as error:
                print(f"失败: {error}")
                if not ask_continue("本题失败。输入 c 继续下一题，其它键退出: "):
                    break

            if i < len(problems):
                wait_human(args.wait_min, args.wait_max, label="题间等待")

        print("\n本轮结束。")
        return 0
    finally:
        context.close()
        playwright.stop()


if __name__ == "__main__":
    raise SystemExit(main())
