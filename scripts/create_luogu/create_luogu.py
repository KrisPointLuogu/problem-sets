#!/usr/bin/env python3
import os
import re
import sys
import time
import argparse
import json
import yaml
import requests

PROBLEM_DIR = os.path.join(os.path.dirname(__file__), "..", "..", "problem", "luogu")
LUOGU_URL = "https://www.luogu.com.cn/problem/{problem_id}"
TAG_API_URL = "https://www.luogu.com.cn/_lfe/tags/zh-CN"

DIFFICULTY_MAP = {
    0: "暂无评定", 1: "入门", 2: "普及-", 3: "普及/提高-",
    4: "普及+/提高", 5: "提高+/省选-", 6: "省选/NOI-",
    7: "NOI", 8: "NOI+/CTSC",
}

_SESSION = None
_CACHED_PAGE = {}


def _get_session():
    global _SESSION
    if _SESSION is None:
        _SESSION = requests.Session()
        _SESSION.headers.update({
            "User-Agent": "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/120.0.0.0 Safari/537.36",
        })
    return _SESSION


def _request_with_retry(url, retries=3, timeout=15):
    session = _get_session()
    last_err = None
    for attempt in range(retries):
        try:
            r = session.get(url, timeout=timeout)
            r.raise_for_status()
            return r
        except requests.RequestException as e:
            last_err = e
            if attempt < retries - 1:
                time.sleep(1 * (attempt + 1))
    raise last_err


def fetch_tag_map():
    try:
        r = _request_with_retry(TAG_API_URL, timeout=10)
        data = r.json()
        return {t["id"]: t for t in data.get("tags", [])}
    except Exception:
        return {}


def fetch_page_data(problem_id, retries=3):
    if problem_id in _CACHED_PAGE:
        return _CACHED_PAGE[problem_id]

    for attempt in range(retries):
        try:
            r = _request_with_retry(LUOGU_URL.format(problem_id=problem_id), retries=1, timeout=15)
            m = re.search(r'<script id="lentille-context" type="application/json">(.*?)</script>', r.text, re.DOTALL)
            if m:
                data = json.loads(m.group(1))
                _CACHED_PAGE[problem_id] = data
                return data
        except Exception:
            pass
        if attempt < retries - 1:
            time.sleep(1.5 * (attempt + 1))
    return None


def get_problem_title(problem_id):
    data = fetch_page_data(problem_id)
    return data["data"]["problem"].get("name", "") if data else ""


def get_problem_tags(problem_id):
    data = fetch_page_data(problem_id)
    if not data:
        return []
    tag_ids = data["data"]["problem"].get("tags", [])
    if not tag_ids:
        return []

    tag_map = fetch_tag_map()
    if not tag_map:
        return []
    return [
        tag_map[t]["name"]
        for t in tag_ids
        if t in tag_map and tag_map[t].get("type") == 2
    ]


def get_problem_difficulty(problem_id):
    data = fetch_page_data(problem_id)
    if not data:
        return ""
    diff_num = data["data"]["problem"].get("difficulty")
    if isinstance(diff_num, int):
        return DIFFICULTY_MAP.get(diff_num, "")
    return str(diff_num) if diff_num else ""


def get_problem_description(problem_id):
    data = fetch_page_data(problem_id)
    if not data:
        return ""
    problem = data["data"]["problem"]
    contenu = problem.get("contenu") or problem.get("content", {})
    return (contenu.get("description") or "").strip()


def fetch_similar_problems(problem_id):
    data = fetch_page_data(problem_id)
    if not data:
        return []
    recommendations = data["data"].get("recommendations", [])
    return [r["pid"] for r in recommendations if r.get("pid")]


def build_problem_md(data):
    problem = data["data"]["problem"]
    contenu = problem.get("contenu") or problem.get("content", {})
    title = contenu.get("name") or problem.get("name") or ""
    background = (contenu.get("background") or "").strip()
    description = (contenu.get("description") or "").strip()
    formatI = (contenu.get("formatI") or "").strip()
    formatO = (contenu.get("formatO") or "").strip()
    hint = (contenu.get("hint") or "").strip()
    samples = problem.get("samples", [])

    lines = []
    lines.append(f"# {title}")
    lines.append("")

    if background:
        lines.append("## 题目背景")
        lines.append("")
        lines.append(background)
        lines.append("")

    lines.append("## 题目描述")
    lines.append("")
    lines.append(description)
    lines.append("")

    lines.append("## 输入格式")
    lines.append("")
    lines.append(formatI)
    lines.append("")

    lines.append("## 输出格式")
    lines.append("")
    lines.append(formatO)
    lines.append("")

    for i, (inp, out) in enumerate(samples, 1):
        lines.append(f"## 输入输出样例 #{i}")
        lines.append("")
        lines.append("**输入**")
        lines.append("```")
        lines.append(inp.rstrip("\n"))
        lines.append("```")
        lines.append("")
        lines.append("**输出**")
        lines.append("```")
        lines.append(out.rstrip("\n"))
        lines.append("```")
        lines.append("")

    if hint:
        lines.append("## 提示")
        lines.append("")
        lines.append(hint)
        lines.append("")

    return "\n".join(lines).strip() + "\n"


def prompt(label, default=None):
    if default is not None:
        prompt_text = f"{label} (default={default}): "
    else:
        prompt_text = f"{label}: "
    val = input(prompt_text).strip()
    if not val and default is not None:
        return default
    return val


def write_yaml(dirpath, data):
    os.makedirs(dirpath, exist_ok=True)
    filepath = os.path.join(dirpath, "head.yaml")
    with open(filepath, "w", encoding="utf-8") as f:
        yaml.dump(data, f, allow_unicode=True, default_flow_style=False, sort_keys=False)
    return filepath


def write_file(dirpath, filename, content):
    filepath = os.path.join(dirpath, filename)
    with open(filepath, "w", encoding="utf-8") as f:
        f.write(content)
    return filepath


def main():
    parser = argparse.ArgumentParser(description="创建 Luogu 题目目录及 head.yaml、problem.md")
    parser.add_argument("problem_id", help="题目 ID，如 P1002")
    parser.add_argument("--title", "-t", help="题目标题")
    parser.add_argument("--tags", "-T", help="标签，逗号分隔")
    parser.add_argument("--desc", "-d", help="题目描述")
    parser.add_argument("--difficulty", "-D", help="难度")
    parser.add_argument("--common", "-c", help="相似题目 ID，逗号分隔")
    parser.add_argument("--force", "-f", action="store_true", help="目录存在时直接覆盖，不询问")
    parser.add_argument("--no-problem-md", action="store_true", help="不抓取 problem.md")
    args = parser.parse_args()

    problem_id = args.problem_id.upper()
    if not re.match(r"^P\d+$", problem_id):
        print("错误：题目 ID 格式应为 P 加数字，如 P1002")
        sys.exit(1)

    dirpath = os.path.join(PROBLEM_DIR, problem_id)

    if os.path.exists(dirpath):
        if args.force:
            print("目录已存在，因 -f 参数直接覆盖")
        else:
            ans = input(f"目录 {os.path.relpath(dirpath, os.path.join(os.path.dirname(__file__), '..'))} 已存在，覆盖? (y/N): ").strip().lower()
            if ans != "y":
                print("已取消")
                return

    auto_title = get_problem_title(problem_id)
    auto_tags = get_problem_tags(problem_id)
    auto_desc = get_problem_description(problem_id)
    auto_diff = get_problem_difficulty(problem_id)
    auto_common = fetch_similar_problems(problem_id)

    if auto_title:
        print(f"自动获取标题: {auto_title}")
    if auto_tags:
        print(f"自动获取标签: {', '.join(auto_tags)}")
    if auto_diff:
        print(f"自动获取难度: {auto_diff}")
    if auto_common:
        print(f"自动获取相似题目: {', '.join(auto_common)}")
    if auto_desc:
        print(f"自动获取描述: {auto_desc[:60]}{'...' if len(auto_desc) > 60 else ''}")

    has_cli_flag = any(getattr(args, f) is not None for f in ["title", "tags", "desc", "difficulty"])
    interactive = not has_cli_flag

    data = {"oj": "luogu", "problem": problem_id}

    if interactive:
        title = prompt("title", default=auto_title or "")
        raw_tags = prompt("tags (逗号分隔)", default=",".join(auto_tags))
        desc = prompt("description", default=auto_desc or "")
        diff = prompt("difficulty", default=auto_diff or "")
    else:
        title = args.title if args.title is not None else (auto_title or "")
        raw_tags = args.tags if args.tags is not None else (",".join(auto_tags) if auto_tags else "")
        desc = args.desc if args.desc is not None else (auto_desc or "")
        diff = args.difficulty if args.difficulty is not None else (auto_diff or "")

    data["title"] = title
    tags = [t.strip() for t in raw_tags.split(",") if t.strip()] if raw_tags else []
    data["tags"] = tags
    data["description"] = desc
    data["difficulty"] = diff
    data["source"] = LUOGU_URL.format(problem_id=problem_id)

    if interactive:
        common_raw = prompt("common (逗号分隔)", default=",".join(auto_common) if auto_common else "")
        common = [c.strip() for c in common_raw.split(",") if c.strip()] if common_raw else []
    else:
        if args.common is not None:
            common_raw = args.common
        elif auto_common:
            common_raw = ",".join(auto_common)
        else:
            print("错误: 无法自动获取相似题目，请手动输入")
            common_raw = input("common (逗号分隔): ").strip()
        common = [c.strip() for c in common_raw.split(",") if c.strip()] if common_raw else []
    data["common"] = common

    os.makedirs(dirpath, exist_ok=True)
    yaml_path = write_yaml(dirpath, data)
    print(f"已创建: {os.path.relpath(yaml_path, os.path.join(os.path.dirname(__file__), '..'))}")

    if not args.no_problem_md:
        try:
            page_data = fetch_page_data(problem_id)
            if page_data:
                md = build_problem_md(page_data)
                md_path = write_file(dirpath, "problem.md", md)
                print(f"已创建: {os.path.relpath(md_path, os.path.join(os.path.dirname(__file__), '..'))}")
            else:
                print("警告: 无法解析页面数据，跳过 problem.md")
        except Exception as e:
            print(f"警告: 抓取 problem.md 失败 ({e})，跳过")


if __name__ == "__main__":
    main()
