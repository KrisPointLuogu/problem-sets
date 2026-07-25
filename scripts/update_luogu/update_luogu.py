#!/usr/bin/env python3
import os
import sys
import time
import yaml

sys.path.insert(0, os.path.join(os.path.dirname(__file__), '..'))
from create_luogu.create_luogu import (
    fetch_page_data,
    get_problem_title,
    get_problem_tags,
    get_problem_difficulty,
    get_problem_description,
    fetch_similar_problems,
    build_problem_md,
    PROBLEM_DIR,
    LUOGU_URL,
)


def load_head_yaml(dirpath):
    path = os.path.join(dirpath, "head.yaml")
    if not os.path.isfile(path):
        return None
    with open(path, encoding="utf-8") as f:
        return yaml.safe_load(f)


def write_head_yaml(dirpath, data):
    path = os.path.join(dirpath, "head.yaml")
    with open(path, "w", encoding="utf-8") as f:
        yaml.dump(data, f, allow_unicode=True, default_flow_style=False, sort_keys=False)
    return path


def write_file(dirpath, filename, content):
    filepath = os.path.join(dirpath, filename)
    with open(filepath, "w", encoding="utf-8") as f:
        f.write(content)
    return filepath


def need_update(value):
    if value is None:
        return True
    if isinstance(value, str) and not value:
        return True
    if isinstance(value, list) and not value:
        return True
    return False


def update_one(name, dirpath, data, dry_run=False):
    problem_id = data.get("problem", name)
    page_data = None

    try:
        page_data = fetch_page_data(problem_id)
        if not page_data:
            return {"ok": False, "error": "抓取页面数据失败", "md_ok": False}
    except Exception as e:
        return {"ok": False, "error": f"抓取异常: {e}", "md_ok": False}

    updates = {}

    if need_update(data.get("title")):
        try:
            title = get_problem_title(problem_id)
            if title:
                updates["title"] = title
        except Exception as e:
            print(f"  [!] title 获取失败: {e}")

    if need_update(data.get("description")):
        try:
            desc = get_problem_description(problem_id)
            if desc:
                updates["description"] = desc
        except Exception as e:
            print(f"  [!] description 获取失败: {e}")

    if need_update(data.get("tags")):
        try:
            tags = get_problem_tags(problem_id)
            if tags:
                updates["tags"] = tags
        except Exception as e:
            print(f"  [!] tags 获取失败: {e}")

    if need_update(data.get("difficulty")):
        try:
            diff = get_problem_difficulty(problem_id)
            if diff:
                updates["difficulty"] = diff
        except Exception as e:
            print(f"  [!] difficulty 获取失败: {e}")

    if need_update(data.get("source")):
        updates["source"] = LUOGU_URL.format(problem_id=problem_id)

    if need_update(data.get("common")):
        try:
            common = fetch_similar_problems(problem_id)
            if common:
                updates["common"] = common
        except Exception as e:
            print(f"  [!] common 获取失败: {e}")

    head_updated = False
    if updates:
        data.update(updates)
        if not dry_run:
            write_head_yaml(dirpath, data)
        head_updated = True

    md_ok = True
    md_path = os.path.join(dirpath, "problem.md")
    md_exists = os.path.isfile(md_path)

    try:
        md_content = build_problem_md(page_data)
        if not dry_run:
            write_file(dirpath, "problem.md", md_content)
    except Exception:
        if md_exists:
            print(f"  [!] problem.md 抓取失败，保留现有文件")
        else:
            marker_path = os.path.join(dirpath, "not_get_problem")
            if not dry_run:
                with open(marker_path, "w") as f:
                    f.write("")
            print(f"  [!!] problem.md 抓取失败且不存在，已标记 not_get_problem")
        md_ok = False

    return {"ok": True, "head_updated": head_updated, "md_ok": md_ok, "updates": updates}


def main():
    dry_run = "--dry-run" in sys.argv
    args = [a for a in sys.argv[1:] if not a.startswith("--")]

    luogu_dir = PROBLEM_DIR
    entries = sorted(os.listdir(luogu_dir))
    problems = []
    skipped = []

    for name in entries:
        if name == "old-code":
            continue
        dirpath = os.path.join(luogu_dir, name)
        if not os.path.isdir(dirpath):
            continue
        if args and name not in args:
            continue
        data = load_head_yaml(dirpath)
        if data is None:
            skipped.append((name, "未找到 head.yaml"))
            continue
        oj = data.get("oj", "")
        if oj != "luogu":
            skipped.append((name, f"跳过 (oj={oj})"))
            continue
        problems.append((name, dirpath, data))

    if not problems:
        print("没有需要处理的 Luogu 题目。")
        return

    print(f"将处理 {len(problems)} 道题目\n")

    not_get_problem_markers = []
    for name, dirpath, data in problems:
        marker_path = os.path.join(dirpath, "not_get_problem")
        if os.path.isfile(marker_path):
            not_get_problem_markers.append(name)

    if not_get_problem_markers:
        print(f"  发现 {len(not_get_problem_markers)} 个 not_get_problem 标记，将重新抓取 problem.md")
        for n in not_get_problem_markers:
            print(f"    - {n}")
        print()

    head_updated_count = 0
    md_updated_count = 0
    errors = []

    for name, dirpath, data in problems:
        print(f"[{name}] 正在处理 ...", end=" ", flush=True)

        marker_path = os.path.join(dirpath, "not_get_problem")
        had_marker = os.path.isfile(marker_path)

        result = update_one(name, dirpath, data, dry_run=dry_run)

        md_marker_cleared = False
        if result["md_ok"] and had_marker:
            if not dry_run:
                os.remove(marker_path)
            md_marker_cleared = True

        if not result["ok"]:
            print(f"失败: {result['error']}")
            errors.append(name)
            print()
            continue

        parts = []
        if result["head_updated"]:
            parts.append(f"head.yaml 更新 {len(result['updates'])} 字段")
            head_updated_count += 1
        else:
            parts.append("head.yaml 无变化")

        if result["md_ok"]:
            parts.append("problem.md 已更新")
            md_updated_count += 1
        else:
            parts.append("problem.md 未更新")

        if md_marker_cleared:
            parts.append("not_get_problem 标记已清除")

        print(" | ".join(parts))
        print()

        time.sleep(1)

    print(f"{'='*40}")
    print(f"处理完成。")
    print(f"  更新 head.yaml: {head_updated_count}/{len(problems)}")
    print(f"  更新 problem.md: {md_updated_count}/{len(problems)}")
    if errors:
        print(f"  失败: {len(errors)} 个: {', '.join(errors)}")
    if skipped:
        print(f"  跳过: {len(skipped)} 个")
        for s, reason in skipped:
            print(f"    - {s}: {reason}")
    if not_get_problem_markers and not dry_run:
        remaining = [n for n in not_get_problem_markers if os.path.isfile(os.path.join(luogu_dir, n, "not_get_problem"))]
        if remaining:
            print(f"  仍有 {len(remaining)} 个 not_get_problem 标记未清除")
        else:
            print(f"  所有 not_get_problem 标记已清除")


if __name__ == "__main__":
    main()
