# Single-Problem Luogu Submission Design

## Goal

Add a `luogu-submit` command for submitting one local solution to Luogu with
help from a ScriptCat userscript. The command must support both an explicit
problem ID and problem-ID inference from the current problem directory:

```text
luogu-submit 1001
luogu-submit B2002
cd /configured/problems/luogu/1001 && luogu-submit
```

After the user selects a source file and confirms the submission in the
terminal, the command opens the matching Luogu problem page. The userscript
fetches a one-time task from a loopback HTTP server, selects the language,
fills the editor, verifies the inserted code, and clicks the submit button.

This feature handles one problem per command invocation. It does not replace or
change the existing problem-set navigation and Playwright batch workflows.

## Components

The feature adds these files:

- `luogu-submit`: an executable Python script containing the single-problem CLI,
  configuration access, source selection, one-time HTTP server, and browser
  launch.
- `userscripts/luogu-submit-helper.user.js`: a ScriptCat userscript containing
  only the local-task bridge and Luogu page automation.
- `install.sh`: an installer that creates or updates the
  `~/.local/bin/luogu-submit` symbolic link to the repository's `luogu-submit`
  script.
- Focused Python and userscript tests for the new behavior.

The existing `userscripts/luogu-submit-area-fixed.user.js` remains a separate
layout userscript. The new command reuses the problem-ID normalization and
directory conventions from `submit_problem_sets.py`; it does not enter the
problem-set control flow.

The CLI uses only the Python standard library. The userscript uses
`GM_xmlhttpRequest` and declares access only to `127.0.0.1`.

## Configuration

The command stores configuration at:

```text
${XDG_CONFIG_HOME:-~/.config}/luogu-submit-helper/config.json
```

The initial configuration command is:

```text
luogu-submit config /absolute/or/relative/path/to/problems
```

The supplied directory must exist and contain a `luogu/` child directory. The
command resolves the path to an absolute canonical path before writing this
schema:

```json
{
  "problems_root": "/absolute/path/to/problems"
}
```

Configuration writes create the parent directory when necessary and replace the
file atomically. A missing file, invalid JSON, missing key, nonexistent root, or
missing `luogu/` directory produces an actionable error before any browser or
HTTP activity begins.

## Problem Resolution

The normal command accepts zero or one problem ID:

```text
luogu-submit [PROBLEM_ID]
```

An all-digit ID receives a `P` prefix, so `1001` becomes `P1001`. IDs beginning
with a letter are uppercased. IDs may contain only ASCII letters, digits,
underscores, and hyphens, and must begin with an ASCII letter or digit. Path
separators, dots, whitespace, and empty IDs are rejected.

Directory mapping follows the repository's existing convention:

| Luogu ID | Directory below `<problems_root>/luogu` |
|----------|------------------------------------------|
| `P1001`  | `1001`                                   |
| `B2002`  | `b2002`                                  |

When no ID is supplied, the resolved current directory must be a direct child
of the configured `<problems_root>/luogu` directory. Its directory name is
validated with the same character rules and converted back to a Luogu ID: an
all-digit name becomes a `P` problem, while any other valid name is uppercased.
The command does not search ancestors or guess a different problems root.

After normalization, the resolved problem directory must remain a direct child
of the configured Luogu root. This check prevents an ID from escaping the
configured tree.

## Source Discovery And Interaction

Only source files directly inside the resolved problem directory are candidates.
The first version supports these extensions and logical Luogu languages:

| Extension | Language |
|-----------|----------|
| `.py`     | Python 3 |
| `.cpp`    | C++20    |
| `.c`      | C        |
| `.java`   | Java     |
| `.js`     | JavaScript |
| `.rs`     | Rust     |
| `.hs`     | Haskell  |

Discovery is not recursive. Regular files named `main.<extension>` sort before
other candidates; each group then sorts by case-insensitive filename. The CLI
prints a numbered list and asks the user to select a file even when only one
candidate exists. Empty input selects the first candidate, while `q` cancels.
Out-of-range and nonnumeric choices are rejected and prompted again.

After selection, the command prints the normalized problem ID, absolute source
path, and language. A final empty Enter confirms real submission; any nonempty
input cancels. The source file is read as UTF-8 only after selection. An unreadable
or non-UTF-8 file stops the command before the local server starts.

The language names above are fixed in the first version. Each language has an
explicit userscript matcher based on normalized visible labels; a generic
substring match is not allowed. A matcher must yield exactly one option. The
`.cpp` matcher must require a standalone C++20 version token and must not fall
back to another C++ standard. If a fixed language has zero or multiple compatible
options, the task fails without clicking submit.

## Loopback Protocol

After confirmation, the CLI binds an operating-system-assigned port on
`127.0.0.1` and creates a cryptographically random task token. It opens this
shape of URL in the default browser:

```text
https://www.luogu.com.cn/problem/P1001?luogu-submit-port=PORT&luogu-submit-token=TOKEN#submit
```

The query values are one-time coordination data. The userscript removes them
from the visible browser URL with `history.replaceState` immediately after
parsing them.

The server exposes two task-scoped operations:

- `GET /task?token=TOKEN` returns JSON containing the problem ID, source
  filename, language, and code. The first valid request atomically changes the
  task from `pending` to `claimed`; later claims receive a conflict response.
- `POST /result?token=TOKEN` accepts a small JSON result containing a fixed
  status (`submit_clicked` or `error`) and a human-readable message. It is
  accepted only for the claimed task.

All other paths, methods, missing tokens, and incorrect tokens are rejected. The
server listens only on loopback, compares tokens without leaking partial-match
information, limits request-body size, emits JSON responses, and never serves a
directory or arbitrary file. It handles exactly one task and exits after a
result or a two-minute timeout.

If the default browser cannot be opened, the CLI prints the complete URL and
continues waiting so the user can open it manually.

## Userscript Behavior

The userscript matches Luogu problem pages but remains inert unless both helper
query parameters are present and valid. It performs these steps once per page:

1. Parse the loopback port and token, remove them from the displayed URL, and
   fetch the task with `GM_xmlhttpRequest`.
2. Compare the problem ID in `window.location.pathname` with the task's
   normalized ID. Report an error on any mismatch.
3. Open or locate the submission panel and wait for its controls to render.
4. Select exactly one compatible language option, including an explicit C++20
   match for `.cpp` tasks.
5. Fill the supported Luogu editor or textarea using its native API or native
   value setter plus bubbling input/change events.
6. Read the editor value back and require exact equality with the task code.
7. Locate one visible, enabled submit button and click it exactly once.
8. Report `submit_clicked` to the CLI. On any earlier failure, report `error`
   with the failed stage and do not click.

A page-local guard prevents duplicate execution if ScriptCat invokes the script
more than once. The terminal message says that the submit button was clicked;
it does not claim that Luogu accepted the solution or that the result was AC.

## Failure Behavior

Failures before terminal confirmation never start the server or browser.
Failures after task creation are shown both on the page through a small userscript
notification and in the terminal when result reporting remains possible.

Expected failures include a missing or disabled userscript, a login wall,
captcha, a changed Luogu DOM, missing submission controls, an unavailable or
ambiguous language, code verification mismatch, and an unavailable submit
button. These conditions do not fall back to keyboard pasting, the current page
language, or an unverified button.

If no userscript result arrives within two minutes, the CLI exits nonzero with a
message directing the user to check ScriptCat installation, permissions, and the
opened page. Cancellation is a successful no-op; configuration, resolution,
protocol, browser, and automation failures return a nonzero exit status.

## Installation And Documentation

`install.sh` resolves its own repository path, ensures `~/.local/bin` exists,
and creates or replaces only the `~/.local/bin/luogu-submit` symbolic link. It
refuses to overwrite a non-symbolic-link file. It also reports when
`~/.local/bin` is not on `PATH`.

The README will document:

- running `install.sh`;
- importing and enabling `userscripts/luogu-submit-helper.user.js` in ScriptCat;
- running `luogu-submit config PATH` once;
- explicit `luogu-submit 1001` and `luogu-submit B2002` examples;
- no-argument use from a configured problem directory;
- the source selection and final confirmation behavior;
- timeout and common userscript troubleshooting.

## Verification

Python unit tests use temporary configuration and problem trees. They cover:

- XDG configuration location, canonical path storage, validation, and atomic
  replacement;
- numeric, `P`, `B`, lowercase, and invalid problem IDs;
- exact current-directory inference and rejection outside the configured tree;
- direct-file filtering, supported extensions, `main.*` priority, alphabetical
  ordering, and no recursion;
- selection defaults, invalid choices, cancellation, and final confirmation;
- valid task claims, incorrect tokens, duplicate claims, result validation,
  request-size limits, and timeout;
- browser-open success and failure without launching a real browser.

Userscript verification includes a JavaScript syntax check and a Playwright-driven
local simulated submission page, using the Playwright dependency already present
in this project. The simulation supplies fake language controls, editor state,
submit button, and `GM_xmlhttpRequest`, then verifies successful C++20 selection,
exact code insertion, a single click, and result reporting. Separate cases prove
that a problem mismatch, language mismatch, editor mismatch, or ambiguous submit
button reports an error and performs zero clicks.

The complete verification runs the existing Python suite as well. No automated
test contacts Luogu, opens the user's browser, or submits real code.
