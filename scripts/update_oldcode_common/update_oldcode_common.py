#!/usr/bin/env python3
import os
import sys
import time
import yaml

sys.path.insert(0, os.path.join(os.path.dirname(__file__), '..'))
from create_luogu.create_luogu import fetch_page_data, fetch_similar_problems, LUOGU_URL

OLD_CODE_DIR = os.path.join(os.path.dirname(__file__), "..", "..", "problem", "luogu", "old-code")

DIFFICULTY_MAP = {
    0: "暂无评定", 1: "入门", 2: "普及-", 3: "普及/提高-",
    4: "普及+/提高", 5: "提高+/省选-", 6: "省选/NOI-",
    7: "NOI", 8: "NOI+/CTSC",
}

TAG_MAP = {
    1: "模拟", 2: "字符串", 3: "DP", 4: "搜索",
    5: "数学", 6: "图论", 7: "贪心", 8: "计算几何",
    9: "数论", 10: "高精度", 11: "树形数据结构", 12: "递推",
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

dry_run = "--dry-run" in sys.argv

entries = sorted(os.listdir(OLD_CODE_DIR))
to_update = []

for name in entries:
    path = os.path.join(OLD_CODE_DIR, name, "head.yaml")
    if not os.path.isfile(path):
        continue
    with open(path, encoding="utf-8") as f:
        data = yaml.safe_load(f)
    if not data.get("common") or not data.get("description", "").strip():
        to_update.append((name, path, data))

print(f"需要更新的题目: {len(to_update)} 个\n")

for name, path, old_data in to_update:
    print(f"  {name} ...", end=" ", flush=True)

    if dry_run:
        print("(dry-run 跳过)")
        continue

    page_data = fetch_page_data(name)
    if not page_data:
        print("抓取失败，跳过")
        continue

    p = page_data["data"]["problem"]
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

    similar = fetch_similar_problems(name)

    old_data["title"] = title
    old_data["tags"] = tags
    old_data["description"] = description
    old_data["difficulty"] = difficulty
    old_data["source"] = LUOGU_URL.format(problem_id=name)
    old_data["common"] = similar

    with open(path, "w", encoding="utf-8") as f:
        yaml.dump(old_data, f, allow_unicode=True, default_flow_style=False, sort_keys=False)

    print(f"title={len(title)>0} tags={len(tags)} desc={len(description)>0} diff={difficulty} common={len(similar)}")
    time.sleep(1)

if not dry_run:
    print(f"\n完成。更新了 {len(to_update)} 道题。")
