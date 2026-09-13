# 洛谷提交助手

从外部题库读取源码，辅助完成洛谷单题或 Markdown 题单提交。
项目提供三种工作流：

| 脚本 | 工作方式 | 风险 |
|------|----------|------|
| `luogu-submit` | 交互选择单题源码，由 ScriptCat 自动填入并点击提交 | 中 |
| `submit_problem_sets.py` | 复制代码并打开提交页，由你手动提交 | 低 |
| `submit_problem_sets_auto.py` | Playwright 自动填入代码，可选自动提交 | 较高 |

自动版的安全限制和完整用法见 [docs/automatic.md](docs/automatic.md)。

## 数据约定

工具不保存题单和题目代码，而是读取一个 rbook 兼容的外部目录：

```text
/path/to/rbook/
├── problem-sets/
│   └── training.md
└── problems/
    └── luogu/
        ├── 1001/main.py
        └── b2002/main.cpp
```

题单中识别以下格式：

```markdown
- [ ] [[problem: luogu,P1001]]
- [x] [[problem: luogu,B2002]]
```

## 依赖

单题 `luogu-submit` 命令只需要 Python 3，并需要浏览器安装 ScriptCat。

手动版只需要 Python 3 和一个系统剪贴板工具：

- Wayland：`wl-copy`
- X11：`xclip` 或 `xsel`
- macOS：`pbcopy`

自动版还需要 Playwright：

```bash
python3 -m pip install -r requirements.txt
python3 -m playwright install chromium
```

## 手动版

从本项目目录运行时，同时给出外部题单和代码根目录：

```bash
python3 submit_problem_sets.py \
  /path/to/rbook/problem-sets/training.md \
  --problems-root /path/to/rbook/problems \
  --skip-done
```

如果当前目录就是 rbook 根目录，`--problems-root` 可省略，默认读取
`./problems`：

```bash
cd /path/to/rbook
python3 /path/to/luogo-submit-helper/submit_problem_sets.py \
  problem-sets/training.md \
  --skip-done
```

启动后，当前题代码会复制到剪贴板。

| 按键 | 作用 |
|------|------|
| `o` | 打开当前题的洛谷提交页 |
| `n` | 立即切到下一题 |
| `Enter` | 随机等待后切到下一题 |
| `p` | 返回上一题 |
| `q` | 退出 |

### 只提交 Python 代码

```bash
python3 submit_problem_sets.py \
  /path/to/rbook/problem-sets/training.md \
  --problems-root /path/to/rbook/problems \
  --code-ext py
```

这时只收集存在 `main.py` 的题目，仅有 `main.cpp` 的题目会被跳过。
不传 `--code-ext` 时，默认优先级为：

```text
py -> cpp -> c -> java -> js -> rs -> hs
```

### 常用参数

| 参数 | 含义 | 默认 |
|------|------|------|
| `problem_set` | 题单 Markdown 路径 | 必填 |
| `--problems-root` | 外部题目代码根目录 | `problems` |
| `--skip-done` | 跳过题单 `[x]` 和洛谷公开 AC | 关 |
| `--user` | 查询公开 AC 的洛谷用户名 | `Rainboy` |
| `--code-ext` | 只选择指定扩展名的 `main.*` | 不过滤 |
| `--wait-min` | Enter 最少等待分钟数 | `1` |
| `--wait-max` | Enter 最多等待分钟数 | `5` |
| `--no-auto-open` | 切题时不自动打开浏览器 | 关 |

`--skip-done` 会在启动时读取一次指定用户的公开 AC 列表，并与题单中
已经勾选的条目合并过滤。查询失败时程序会报错退出，不会静默忽略。

## 目录映射

洛谷题号按以下规则寻找本地目录：

| 题号 | 目录 |
|------|------|
| `P1001` 或 `1001` | `<problems-root>/luogu/1001/` |
| `B2002` | `<problems-root>/luogu/b2002/` |

每个目录中使用 `main.<扩展名>` 作为正式提交代码。

## `luogu-submit` 使用

### 安装

运行安装脚本，把仓库中的 `luogu-submit` 软链接到 `~/.local/bin`：

```bash
./install.sh
```

如果脚本提示 `~/.local/bin` 不在 `PATH`，按提示更新 shell 配置并重新打开
终端。

在 ScriptCat 中导入并启用：

```text
userscripts/luogu-submit-helper.user.js
```

这个 userscript 只在命令打开的、带一次性任务参数的洛谷题目页执行提交；
普通浏览题目页时不会连接本地服务或点击按钮。

首次使用时配置包含 `luogu/` 的固定题库根目录：

```bash
luogu-submit config /path/to/rbook/problems
```

`gen.py` 和 `gen.cpp` 是默认源码黑名单，永远不会出现在提交候选中。可以在
配置题库时追加其他文件名：

```bash
luogu-submit config /path/to/rbook/problems \
  --blacklist brute.py debug.cpp
```

黑名单按题目目录直属文件名精确匹配，不支持路径或通配符。配置写入
`source_blacklist`，旧配置没有该字段时仍会自动排除 `gen.py` 和 `gen.cpp`。

配置保存在：

```text
${XDG_CONFIG_HOME:-~/.config}/luogu-submit-helper/config.json
```

查看完整命令帮助：

```bash
luogu-submit --help
```

### 提交一题

题号可以写纯数字、完整的 `P` 题号或其他洛谷题号：

```bash
luogu-submit 1001
luogu-submit P1001
luogu-submit B2002
```

也可以进入已配置题库中的题目目录，不输入题号：

```bash
cd /path/to/rbook/problems/luogu/1001
luogu-submit
```

命令只列出题目目录直属的以下源码，不递归扫描子目录：

```text
py cpp c java js rs hs
```

黑名单文件会先被移除；剩余候选中 `main.*` 排在前面，其余文件按名称
排序。`.cpp` 固定选择洛谷的 C++20。选择源码后，终端会再次显示题号、
绝对路径和语言；只有按空 Enter 确认，才会启动本地服务并打开浏览器。

浏览器中的 userscript 会依次校验页面题号、选择语言、填入并回读代码，
最后点击一次提交按钮。终端显示“已点击提交按钮”只表示点击动作完成，不
表示代码已经 AC；最终结果仍以洛谷记录为准。

### 单题提交故障排查

终端等待两分钟后超时，通常表示 ScriptCat 脚本没有导入或启用，或浏览器
没有成功打开带一次性任务参数的页面。自动打开失败时，终端会打印完整
地址，可以手动打开。

如果 userscript 报告语言、编辑器或提交按钮不唯一/不存在，它不会点击
提交。这通常意味着尚未登录、遇到验证码，或者洛谷页面结构已经变化。

## 测试

```bash
python3 -m unittest discover -s tests
node --check userscripts/luogu-submit-helper.user.js
bash -n install.sh
```

测试不访问洛谷、不打开用户的默认浏览器，也不会提交代码。userscript 测试
会启动本地无头 Chromium 并加载模拟提交页。

## 常见问题

找不到本地代码时，先确认 `--problems-root` 指向的是包含 `luogu/` 的
`problems` 目录，而不是 rbook 项目根目录。

剪贴板复制失败时，安装当前桌面环境对应的工具。即使复制失败，终端仍会
显示当前题目和提交页地址。
