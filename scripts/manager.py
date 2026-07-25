#!/usr/bin/env python3
import os
import sys
import yaml
import subprocess

SCRIPTS_DIR = os.path.dirname(os.path.abspath(__file__))

def discover_tools():
    tools = {}
    for name in os.listdir(SCRIPTS_DIR):
        dirpath = os.path.join(SCRIPTS_DIR, name)
        head_path = os.path.join(dirpath, "head.yaml")
        if os.path.isdir(dirpath) and os.path.isfile(head_path):
            with open(head_path) as f:
                data = yaml.safe_load(f)
            if data:
                data["_dir"] = name
                tools[name] = data
    return tools

def cmd_list(tools, args):
    if not tools:
        print("  没有发现任何工具。")
        return
    print(f"  共 {len(tools)} 个工具:\n")
    for name, info in sorted(tools.items()):
        lang = info.get("language", "")
        desc = info.get("description", "")
        print(f"  {name:<30} [{lang:<6}] {desc}")

def cmd_info(tools, args):
    if not args:
        print("  用法: info <工具名>")
        return
    name = args[0]
    info = tools.get(name)
    if not info:
        print(f"  未找到工具 '{name}'")
        return
    print(f"  名称:        {info.get('name', '')}")
    print(f"  语言:        {info.get('language', '')}")
    print(f"  文件:        {info.get('file', '')}")
    print(f"  描述:        {info.get('description', '')}")
    print(f"  用法:        {info.get('usage', '')}")
    deps = info.get("dependencies", [])
    if deps:
        print(f"  依赖:        {', '.join(deps)}")

def cmd_run(tools, args):
    if not args:
        print("  用法: run <工具名> [参数...]")
        return
    name = args[0]
    info = tools.get(name)
    if not info:
        print(f"  未找到工具 '{name}'")
        return
    filename = info.get("file", "")
    dirpath = os.path.join(SCRIPTS_DIR, info["_dir"])
    filepath = os.path.join(dirpath, filename)
    if not os.path.isfile(filepath):
        print(f"  错误: 文件不存在 {filepath}")
        return
    lang = info.get("language", "")
    extra_args = args[1:]
    cmd = []
    if lang == "python":
        cmd = [sys.executable, filepath] + extra_args
    elif lang == "bash":
        cmd = ["bash", filepath] + extra_args
    else:
        print(f"  不支持的语言: {lang}")
        return
    print(f"  运行: {' '.join(cmd)}\n")
    try:
        subprocess.run(cmd, cwd=dirpath)
    except KeyboardInterrupt:
        print("\n  已中断")

def cmd_search(tools, args):
    if not args:
        print("  用法: search <关键词>")
        return
    keyword = args[0].lower()
    matches = []
    for name, info in sorted(tools.items()):
        if keyword in name.lower() or keyword in info.get("description", "").lower():
            matches.append((name, info))
    if not matches:
        print(f"  没有匹配 '{keyword}' 的工具")
        return
    print(f"  找到 {len(matches)} 个匹配:\n")
    for name, info in matches:
        print(f"  {name:<30} {info.get('description', '')}")

def cmd_help(tools, args):
    print("""  可用命令:
    list                     列出所有工具
    info <工具名>             查看工具详情
    run <工具名> [参数...]    运行工具
    search <关键词>           搜索工具
    retry                    重新抓取 problem.md 失败的题目
    help                     显示此帮助
    exit                     退出
""")

def check_incomplete():
    root = os.path.dirname(SCRIPTS_DIR)
    problem_dir = os.path.join(root, "problem")
    incomplete = []
    if os.path.isdir(problem_dir):
        for cat in os.listdir(problem_dir):
            catpath = os.path.join(problem_dir, cat)
            if not os.path.isdir(catpath):
                continue
            for prob in os.listdir(catpath):
                if os.path.isfile(os.path.join(catpath, prob, "not_complete")):
                    incomplete.append(f"{cat}/{prob}")
    return incomplete


def check_not_get_problem():
    root = os.path.dirname(SCRIPTS_DIR)
    luogu_dir = os.path.join(root, "problem", "luogu")
    markers = []
    if os.path.isdir(luogu_dir):
        for name in os.listdir(luogu_dir):
            marker_path = os.path.join(luogu_dir, name, "not_get_problem")
            if os.path.isfile(marker_path):
                markers.append(name)
    return sorted(markers)

def cmd_retry(tools, args):
    root = os.path.dirname(SCRIPTS_DIR)
    luogu_dir = os.path.join(root, "problem", "luogu")
    markers = check_not_get_problem()
    if not markers:
        print("  没有发现 not_get_problem 标记。")
        return
    print(f"  将重新抓取以下 {len(markers)} 道题的 problem.md: {', '.join(markers)}")
    ans = input("  确认? (Y/n): ").strip().lower()
    if ans == "n":
        print("  已取消")
        return
    script_path = os.path.join(SCRIPTS_DIR, "update_luogu", "update_luogu.py")
    if not os.path.isfile(script_path):
        print(f"  错误: 未找到 {script_path}")
        return
    cmd = [sys.executable, script_path] + markers
    print(f"  运行: {' '.join(cmd)}\n")
    try:
        subprocess.run(cmd, cwd=SCRIPTS_DIR)
    except KeyboardInterrupt:
        print("\n  已中断")


def main():
    tools = discover_tools()
    incomplete = check_incomplete()
    not_get = check_not_get_problem()
    print(f"Manager — 共发现 {len(tools)} 个工具，输入 help 查看帮助，exit 退出。\n")
    if incomplete:
        print(f"  ⚠ 以下 {len(incomplete)} 个题目标记为未完成:\n")
        for item in incomplete:
            print(f"    - {item}")
        print()
    if not_get:
        print(f"  ⚠ 以下 {len(not_get)} 道题的 problem.md 抓取失败 (标记 not_get_problem):\n")
        for item in not_get:
            print(f"    - {item}")
        print(f"  输入 retry 重新抓取\n")
    while True:
        try:
            line = input("manager> ").strip()
        except (EOFError, KeyboardInterrupt):
            print()
            break
        if not line:
            continue
        parts = line.split()
        cmd = parts[0]
        args = parts[1:]
        if cmd == "exit":
            break
        elif cmd == "list":
            cmd_list(tools, args)
        elif cmd == "info":
            cmd_info(tools, args)
        elif cmd == "run":
            cmd_run(tools, args)
        elif cmd == "search":
            cmd_search(tools, args)
        elif cmd == "retry":
            cmd_retry(tools, args)
        elif cmd == "help":
            cmd_help(tools, args)
        else:
            print(f"  未知命令: {cmd}，输入 help 查看帮助。")

if __name__ == "__main__":
    main()
