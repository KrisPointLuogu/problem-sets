#!/usr/bin/env python3
import os
import sys
import time
import json
import shutil
import subprocess
import yaml
import re
import requests
from bs4 import BeautifulSoup

sys.path.insert(0, os.path.join(os.path.dirname(__file__), '..'))
from create_luogu.create_luogu import (
    fetch_page_data,
    fetch_similar_problems,
    build_problem_md,
    write_file,
    LUOGU_URL,
    PROBLEM_DIR,
)

OLD_DIR = os.path.join(os.path.dirname(__file__), "..", "..", "luogu-old")
LUOGU_DIR = PROBLEM_DIR  # problem/luogu/
OLD_CODE_DIR = os.path.join(LUOGU_DIR, "old-code")

DIFFICULTY_MAP = {
    0: "暂无评定", 1: "入门", 2: "普及-", 3: "普及/提高-",
    4: "普及+/提高", 5: "提高+/省选-", 6: "省选/NOI-",
    7: "NOI", 8: "NOI+/CTSC",
}

TAG_MAP = {
    1: "模拟", 2: "字符串", 3: "DP", 4: "搜索",
    5: "数学", 6: "图论", 7: "贪心", 8: "计算几何",
    9: "数论", 10: "高精度", 11: "树形数据结构", 12: "递推",
    13: "图论", 14: "计算几何",
    42: "线段树", 45: "二分", 53: "树状数组", 54: "递归",
    78: "离散化", 107: "Special Judge", 108: "O2优化",
    111: "枚举", 112: "分治", 113: "排序",
    126: "BFS", 127: "DFS", 128: "剪枝", 129: "记忆化搜索",
    139: "背包 DP", 144: "区间 DP", 152: "树形 DP",
    159: "拓扑排序", 160: "最短路",
    204: "费用流", 239: "素数判断", 241: "gcd",
    244: "进制", 254: "前缀和",
    260: "Fibonacci 数列", 261: "Catalan 数",
    287: "栈", 364: "Dilworth 定理",
    380: "折半搜索", 444: "线性 DP",
    464: "状压 DP", 476: "Floyd 算法",
    524: "反悔贪心",
}

IGNORE_TAG_IDS = {15, 16, 17, 18, 19, 20, 22, 24, 25, 28, 82, 83, 104}


def is_elf(path):
    try:
        result = subprocess.run(["file", "-b", path], capture_output=True, text=True, timeout=5)
        return "ELF" in result.stdout
    except Exception:
        return False


def clean_directory(dirpath):
    for root, dirs, files in os.walk(dirpath, topdown=False):
        for f in files:
            fpath = os.path.join(root, f)
            if ":Zone.Identifier" in f:
                os.remove(fpath)
            elif is_elf(fpath):
                os.remove(fpath)
        for d in dirs:
            dpath = os.path.join(root, d)
            try:
                os.rmdir(dpath)
            except OSError:
                pass


def move_all(src, dest):
    os.makedirs(dest, exist_ok=True)
    for item in os.listdir(src):
        shutil.move(os.path.join(src, item), os.path.join(dest, item))
    try:
        os.rmdir(src)
    except OSError:
        shutil.rmtree(src, ignore_errors=True)


def fetch_problem_meta(problem_id):
    data = fetch_page_data(problem_id)
    if not data:
        return None
    p = data["data"]["problem"]
    contenu = p.get("contenu") or p.get("content", {})
    title = p.get("name", "")
    description = contenu.get("description", "").strip()

    raw_tags = p.get("tags", [])
    tags = [TAG_MAP.get(t, "") for t in raw_tags if t not in IGNORE_TAG_IDS]
    tags = [t for t in tags if t]

    diff_num = p.get("difficulty")
    if isinstance(diff_num, int):
        difficulty = DIFFICULTY_MAP.get(diff_num, "")
    else:
        difficulty = str(diff_num) if diff_num else ""

    return {"title": title, "tags": tags, "description": description, "difficulty": difficulty}


def main():
    dry_run = "--dry-run" in sys.argv

    if not os.path.isdir(OLD_DIR):
        print(f"错误: 未找到 luogu-old 目录 ({OLD_DIR})")
        sys.exit(1)

    entries = sorted(os.listdir(OLD_DIR))
    entries = [e for e in entries if os.path.isdir(os.path.join(OLD_DIR, e))]

    print(f"luogu-old 中共有 {len(entries)} 个题目目录\n")

    existing = set(os.listdir(LUOGU_DIR)) if os.path.isdir(LUOGU_DIR) else set()
    existing.discard("old-code")

    ORIGINAL_15 = {"P1002", "P1057", "P1115", "P1216", "P1824", "P1970", "P2678", "P2985", "P6625", "P7909", "P8177", "P8814", "P9748", "P9749", "P9750"}
    overlap_originals = {"P1002", "P1216", "P7909", "P8814", "P9748", "P9749"}
    overlap = [e for e in entries if e in existing and e in overlap_originals]
    already_done = [e for e in entries if e in existing and e not in overlap_originals]
    new_ones = [e for e in entries if e not in existing]

    print(f"  - 重叠原始题目 (→ old-code/): {len(overlap)} 个")
    if already_done:
        print(f"  - 上次已处理完的残留目录 (→ 直接清理): {len(already_done)} 个")
    print(f"  - 全新题目 (→ old-code/ + head.yaml + problem.md): {len(new_ones)} 个\n")

    if dry_run:
        print("=== DRY RUN 模式 — 不会执行任何实际操作 ===\n")

    os.makedirs(OLD_CODE_DIR, exist_ok=True)
    stats = {"moved": 0, "cleaned": 0, "created": 0, "failed": 0}

    for idx, name in enumerate(entries, 1):
        src = os.path.join(OLD_DIR, name)
        print(f"[{idx}/{len(entries)}] {name} ", end="", flush=True)

        if not dry_run:
            clean_directory(src)

        if name in overlap:
            dest = os.path.join(OLD_CODE_DIR, name)
            if not dry_run:
                move_all(src, dest)
            print("重叠 → 已移至 old-code/")
            stats["moved"] += 1

        elif name in already_done:
            if not dry_run:
                shutil.rmtree(src, ignore_errors=True)
            print("已处理 → 清理残留源目录")
            stats["cleaned"] += 1

        else:
            dest = os.path.join(OLD_CODE_DIR, name)
            if not dry_run:
                move_all(src, dest)

            print("新建...", end=" ", flush=True)

            if dry_run:
                print("(dry-run 跳过抓取)")
                stats["created"] += 1
                continue

            meta = fetch_problem_meta(name)
            if meta is None:
                print("抓取失败，跳过")
                stats["failed"] += 1
                continue

            similar = fetch_similar_problems(name)

            head_data = {
                "oj": "luogu",
                "problem": name,
                "title": meta["title"],
                "tags": meta["tags"],
                "description": meta["description"],
                "difficulty": meta["difficulty"],
                "source": LUOGU_URL.format(problem_id=name),
                "common": similar,
            }

            yaml_path = os.path.join(dest, "head.yaml")
            with open(yaml_path, "w", encoding="utf-8") as f:
                yaml.dump(head_data, f, allow_unicode=True, default_flow_style=False, sort_keys=False)

            try:
                page_data = fetch_page_data(name)
                if page_data:
                    md = build_problem_md(page_data)
                    write_file(dest, "problem.md", md)
                    print(f"head.yaml + problem.md (tags={len(meta['tags'])}, common={len(similar)})")
                else:
                    print(f"head.yaml (problem.md 抓取失败)")
            except Exception as e:
                print(f"head.yaml (problem.md 抓取失败: {e})")

            stats["created"] += 1
            time.sleep(1)

    print("\n=== 统计 ===")
    print(f"  重叠移至 old-code/: {stats['moved']}")
    print(f"  清理已处理残留: {stats['cleaned']}")
    print(f"  新建 (head.yaml + problem.md): {stats['created']}")
    print(f"  抓取失败跳过: {stats['failed']}")

    # Check for leftover items in luogu-old
    leftovers = [f for f in os.listdir(OLD_DIR) if f != "WebAddress"]
    if leftovers and not dry_run:
        print(f"\n注意: luogu-old 中仍有 {len(leftovers)} 个残留条目（非目录文件），请检查")
    elif leftovers and dry_run:
        print(f"\n注意: luogu-old 中除 WebAddress 外还有 {len(leftovers)} 个条目")


if __name__ == "__main__":
    main()
