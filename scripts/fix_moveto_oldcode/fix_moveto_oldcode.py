#!/usr/bin/env python3
import os
import shutil

LUOGU_DIR = os.path.join(os.path.dirname(__file__), "..", "..", "problem", "luogu")
OLD_CODE_DIR = os.path.join(LUOGU_DIR, "old-code")

ORIGINAL_15 = {
    "P1002", "P1057", "P1115", "P1216", "P1824",
    "P1970", "P2678", "P2985", "P6625", "P7909",
    "P8177", "P8814", "P9748", "P9749", "P9750",
}
OVERLAP_6 = {"P1002", "P1216", "P7909", "P8814", "P9748", "P9749"}

dry_run = "--dry-run" in __import__("sys").argv

all_dirs = sorted(os.listdir(LUOGU_DIR))
imported = [d for d in all_dirs if d not in ORIGINAL_15 and d != "old-code" and os.path.isdir(os.path.join(LUOGU_DIR, d))]

print(f"待移至 old-code/ 的目录: {len(imported)} 个\n")

# Handle 6 overlap originals first (move old/ contents to old-code/)
print("--- 重叠题目的 old/ 子目录处理 ---")
os.makedirs(OLD_CODE_DIR, exist_ok=True)
overlap_merged_count = 0
for name in OVERLAP_6:
    old_src = os.path.join(LUOGU_DIR, name, "old")
    if os.path.isdir(old_src):
        dest = os.path.join(OLD_CODE_DIR, name)
        print(f"[重叠] {name}: old/ → old-code/{name}/")
        if not dry_run:
            os.makedirs(dest, exist_ok=True)
            for item in os.listdir(old_src):
                shutil.move(os.path.join(old_src, item), os.path.join(dest, item))
            os.rmdir(old_src)
        overlap_merged_count += 1
    else:
        print(f"[重叠] {name}: 无 old/ 子目录，跳过")

print()

# Handle regular imported problems
print("--- 导入题目移至 old-code/ ---")
moved_count = 0
for name in imported:
    src = os.path.join(LUOGU_DIR, name)
    dest = os.path.join(OLD_CODE_DIR, name)

    print(f"[导入] {name} → old-code/{name}/")
    if not dry_run:
        shutil.move(src, dest)
    moved_count += 1

print(f"\n=== 结果 ===")
print(f"  整体移动至 old-code/: {moved_count}")
print(f"  重叠 old/ 合并至 old-code/: {overlap_merged_count}")

# Final state
remaining = sorted(os.listdir(LUOGU_DIR))
remaining_original = [d for d in remaining if d in ORIGINAL_15]
remaining_other = [d for d in remaining if d not in ORIGINAL_15]
print(f"\n  problem/luogu/ 剩余原始题目: {len(remaining_original)}")
print(f"  problem/luogu/ 剩余其他: {len(remaining_other)}")
if remaining_other:
    print(f"  未清理: {remaining_other}")
