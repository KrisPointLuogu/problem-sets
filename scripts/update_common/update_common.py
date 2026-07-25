#!/usr/bin/env python3
import os
import sys
import time
import yaml

sys.path.insert(0, os.path.join(os.path.dirname(__file__), '..'))
from create_luogu.create_luogu import fetch_similar_problems, PROBLEM_DIR

LUOGU_DIR = PROBLEM_DIR


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


def main():
    dry_run = "--dry-run" in sys.argv

    entries = sorted(os.listdir(LUOGU_DIR))
    problems = []

    for name in entries:
        dirpath = os.path.join(LUOGU_DIR, name)
        if not os.path.isdir(dirpath):
            continue

        data = load_head_yaml(dirpath)
        if data is None:
            print(f"! {name}: 未找到 head.yaml，请运行 python3 scripts/create_luogu.py {name}")
            continue

        oj = data.get("oj", "")
        if oj != "luogu":
            print(f"- {name}: 跳过 (oj={oj})")
            continue

        problems.append((name, dirpath, data))

    if not problems:
        print("没有需要处理的 Luogu 题目。")
        return

    print(f"将更新 {len(problems)} 道题目的 common 字段\n")

    for name, dirpath, data in problems:
        print(f"正在处理 {name} ...", end=" ", flush=True)
        try:
            common = fetch_similar_problems(name)
            if common:
                print(f"获取到 {len(common)} 个相似题: {', '.join(common)}")
            else:
                print("获取为空")
        except Exception as e:
            print(f"失败: {e}")
            continue

        if dry_run:
            print(f"  -> 将写入 common: {common}")
        else:
            old_common = data.get("common", [])
            data["common"] = common
            write_head_yaml(dirpath, data)
            if old_common != common:
                print(f"  -> 已更新 common: {common}")
            else:
                print(f"  -> 无变化")

        time.sleep(1)

    print("\n完成。")


if __name__ == "__main__":
    main()
