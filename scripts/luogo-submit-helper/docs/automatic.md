# 洛谷题单自动提交助手

`submit_problem_sets_auto.py` 复用手动版的题单解析、本地代码查找和公开
AC 过滤逻辑，通过 Playwright + Chromium 打开洛谷提交页并填入代码。

## 安全行为

- 默认是 `dry-run`：填入代码，但不点击提交。
- 真正提交必须显式传入 `--execute`，并在终端输入 `yes`。
- 默认每次最多处理 20 题，题间等待 45 到 120 秒。
- 登录状态保存在 `~/.cache/luogo-submit-helper/chromium`。

自动化可能触发网站风控。首次使用和洛谷页面改版后，都应先用少量题目预演。

## 安装

```bash
python3 -m pip install -r requirements.txt
python3 -m playwright install chromium
```

## 登录

```bash
python3 submit_problem_sets_auto.py --login-only
```

Chromium 打开后手动登录洛谷，再回到终端按 Enter。后续命令默认复用同一
`--profile-dir`。

## 预演

```bash
python3 submit_problem_sets_auto.py \
  /path/to/rbook/problem-sets/training.md \
  --problems-root /path/to/rbook/problems \
  --skip-done \
  --dry-run \
  --limit 3
```

预演会打开页面、选择语言并填入代码，但不会点击提交。

## 真实提交

```bash
python3 submit_problem_sets_auto.py \
  /path/to/rbook/problem-sets/training.md \
  --problems-root /path/to/rbook/problems \
  --skip-done \
  --execute \
  --limit 5 \
  --wait-min 90 \
  --wait-max 180
```

终端要求输入 `yes` 后才开始。建议先完成预演，并确认代码、语言和题号都正确。

## 常用参数

| 参数 | 含义 | 默认 |
|------|------|------|
| `problem_set` | 题单 Markdown 路径 | 登录模式可省略 |
| `--problems-root` | 外部题目代码根目录 | `problems` |
| `--login-only` | 只打开浏览器供手动登录 | 关 |
| `--skip-done` | 跳过题单 `[x]` 和洛谷公开 AC | 关 |
| `--user` | 查询公开 AC 的用户名 | `Rainboy` |
| `--dry-run` | 只填代码，不点击提交 | 默认模式 |
| `--execute` | 允许点击提交 | 关 |
| `--limit` | 本轮最多处理题数 | `20` |
| `--wait-min` | 题间最少等待秒数 | `45` |
| `--wait-max` | 题间最多等待秒数 | `120` |
| `--start-from` | 从指定洛谷题号开始 | 空 |
| `--profile-dir` | Chromium 持久化目录 | `~/.cache/luogo-submit-helper/chromium` |
| `--headless` | 使用无头浏览器 | 关 |

## 工作流程

1. 用 `--login-only` 建立登录状态。
2. 用 `--dry-run --limit 3` 检查页面和语言选择。
3. 用 `--execute --limit 5` 做小批量提交。
4. 确认无异常后再逐步增加 `--limit`。

代码文件选择顺序与手动版一致：`main.py`、`main.cpp`、`main.c`、
`main.java`、`main.js`、`main.rs`、`main.hs`。

## 常见问题

提示未登录时，重新执行 `--login-only`，并确认登录与提交使用了相同的
`--profile-dir`。

无法填入代码或切换语言时，先用 `--dry-run --limit 1` 查看浏览器页面。
洛谷前端结构变化、验证码或登录墙都需要人工处理。

`--execute` 与 `--dry-run` 不能同时使用；等待时间不能为负数，且最小值
不能大于最大值。
