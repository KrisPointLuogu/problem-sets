#!/usr/bin/env python3
import os
import re
import fnmatch
import yaml

PROBLEM_DIR = os.path.join(os.path.dirname(__file__), "..", "..", "problem")


def load_problems():
    problems = []
    for root, dirs, files in os.walk(PROBLEM_DIR):
        if "head.yaml" in files:
            path = os.path.join(root, "head.yaml")
            with open(path, encoding="utf-8") as f:
                data = yaml.safe_load(f)
            if data:
                data["_path"] = os.path.relpath(root, PROBLEM_DIR)
                problems.append(data)
    return problems


def compile_pattern(pattern):
    if any(c in pattern for c in "*?"):
        return re.compile(fnmatch.translate(pattern), re.IGNORECASE)
    return re.compile(re.escape(pattern), re.IGNORECASE)


def match_field(field_value, pattern):
    if field_value is None:
        return False
    if isinstance(field_value, list):
        return any(pattern.search(str(v)) for v in field_value)
    return bool(pattern.search(str(field_value)))


def search(problems, cmd):
    cmd = cmd.strip()
    if not cmd:
        return problems

    prefixes = {
        "id:": "problem",
        "title:": "title",
        "tag:": "tags",
        "desc:": "description",
        "diff:": "difficulty",
        "oj:": "oj",
        "common:": "common",
    }

    for prefix, field in prefixes.items():
        if cmd.startswith(prefix):
            pat = compile_pattern(cmd[len(prefix):].strip())
            return [p for p in problems if match_field(p.get(field), pat)]

    pat = compile_pattern(cmd)
    return [
        p for p in problems
        if any(match_field(p.get(f), pat) for f in ["problem", "title", "tags", "description", "difficulty", "oj"])
    ]


def display(results):
    if not results:
        print("  (无匹配结果)")
        return

    print(f"  搜索结果（共 {len(results)} 条）")

    id_w = max(len(r.get("problem", "")) for r in results)
    id_w = max(id_w, 4)
    title_w = max(len(r.get("title", "")) for r in results)
    title_w = min(max(title_w, 5), 60)
    tags_w = max(len(", ".join(r.get("tags", []))) for r in results)
    tags_w = min(max(tags_w, 4), 40)

    sep = f"+{'-'*(id_w+2)}+{'-'*(title_w+2)}+{'-'*(tags_w+2)}+{'-'*7}+------+"
    hdr = f"| {'ID':^{id_w}} | {'Title':^{title_w}} | {'Tags':^{tags_w}} | {'OJ':^5} | Diff |"
    print(sep)
    print(hdr)
    print(sep)

    for r in results:
        pid = r.get("problem", "")
        title = r.get("title", "")
        if len(title) > title_w:
            title = title[:title_w-3] + "..."
        tags = ", ".join(r.get("tags", []))
        if len(tags) > tags_w:
            tags = tags[:tags_w-3] + "..."
        oj = r.get("oj", "")
        diff = r.get("difficulty", "")
        print(f"| {pid:<{id_w}} | {title:<{title_w}} | {tags:<{tags_w}} | {oj:^5} | {diff:<4} |")

    print(sep)
    print()
    print("路径:")
    for r in results:
        print(f"  problem/{r['_path']}/")


def show_help():
    print("""  搜索语法:
    <关键词>           跨所有字段模糊搜索
    id:<关键词>        按题目ID搜索 (e.g. id:P1002)
    title:<关键词>     按标题搜索
    tag:<关键词>       按标签搜索 (e.g. tag:DP)
    desc:<关键词>      按描述搜索
    diff:<关键词>      按难度搜索
    oj:<关键词>        按OJ搜索
    common:<关键词>    按相似题目搜索 (e.g. common:P1003)
    list / all        列出所有题目
    help              显示此帮助
    exit / quit       退出

  支持 * 和 ? 通配符，也支持正则表达式。""")


def main():
    problems = load_problems()
    if not problems:
        print("未找到任何 head.yaml，请确认 problem/ 目录下有题目。")
        return

    print(f"共加载 {len(problems)} 道题目，输入 help 查看帮助，exit 退出。")

    while True:
        try:
            cmd = input("find> ").strip()
        except (EOFError, KeyboardInterrupt):
            print()
            break

        if not cmd:
            continue
        if cmd in ("exit", "quit"):
            break
        if cmd in ("help",):
            show_help()
            continue
        if cmd in ("list", "all"):
            display(problems)
            continue

        results = search(problems, cmd)
        display(results)


if __name__ == "__main__":
    main()
