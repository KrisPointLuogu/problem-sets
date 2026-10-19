---
name: gitmananger
description: 使用 Angular commit 规范生成符合本项目风格的 git commit 消息
---

## 流程

### 1. 了解变更

运行以下命令：

```
git status
git diff --stat
git log --oneline -5
```

### 2. 将变更按逻辑分组

每组应是一个独立且内聚的改动。例如：
- 所有脚本文件的重构 → 一组
- 所有 problem 目录的移动或添加 → 一组
- Bug 修复 → 单独一组
- 文档修改 → 单独一组

### 3. 为每组生成 `<type>(<scope>): <subject>`

**type** — 从以下中选择：
- `feat` — 新功能
- `fix` — 修复 bug
- `docs` — 文档
- `style` — 格式调整
- `refactor` — 重构
- `perf` — 性能优化
- `test` — 测试
- `chore` — 构建/工具
- `revert` — 回滚
- `merge` — 合并
- `sync` — 同步

**scope** — 描述影响范围，参考历史 commit 风格（如 `scripts/*`、`problem/luogu/*`）。如果涉及多个子范围，使用 `*`。

**subject** — 用 **Basic English 850** 写，≤50 字符，结尾不加句号。

### 4. 合并成一条 commit message

每组一行，不留空行：

```
<type>(<scope>): <subject>
<type>(<scope>): <subject>
```

**注意：所有改动放在同一次 commit 中提交，不要拆成多个 commit。**

### 5. 展示给用户确认

将生成的完整 message 展示给用户，等用户确认后再执行。

### 6. 执行

```
git add -A && git commit -s -m "..."
```

## 约束

- 必须等用户确认才能执行 git 命令
- subject 用 Basic English 850，不加句号
- 不要推送到远程（除非用户要求）
