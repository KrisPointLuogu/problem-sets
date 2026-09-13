# CodeMirror 6 Editor Fix Design

## Problem

Running `luogu-submit 1001` successfully starts the loopback task, opens the
Luogu submission page, and selects the requested language, but the userscript
then reports:

```text
自动提交失败: 等待代码编辑器超时
```

The final browser URL is `https://www.luogu.com.cn/problem/P1001#submit`. This
URL is expected: the userscript removes the one-time port and token query
parameters with `history.replaceState` after reading them.

The July 12, 2026 Luogu Columba frontend uses a Vue CodeMirror 6 component for
the submission editor. Its observable DOM contains exactly one:

```text
.cm-editor .cm-content[contenteditable="true"]
```

The current userscript recognizes CodeMirror 5, Ace, Monaco, and visible native
textareas, but not CodeMirror 6. It therefore waits until the editor timeout
even though the editor is present.

## Fix

Add a CodeMirror 6 adapter before the existing editor adapters. It searches the
submission root for `.cm-editor .cm-content[contenteditable="true"]` and
requires exactly one visible match.

The adapter must update CodeMirror state through its public DOM event contract,
not by assigning `textContent` or depending on Vue private properties:

1. Focus the `.cm-content` element.
2. Dispatch a bubbling, cancelable `keydown` for the platform's select-all
   shortcut: `Ctrl+A` on Linux/Windows or `Cmd+A` on macOS.
3. Require CodeMirror to consume the shortcut, then allow selection state to
   settle.
4. Put the task code into a `DataTransfer` as `text/plain` and dispatch a
   bubbling, cancelable `ClipboardEvent("paste")` on `.cm-content`.
5. Require CodeMirror to consume the paste event, then allow its Vue model and
   rendered state to settle.

Requiring event consumption prevents the adapter from claiming success when a
lookalike contenteditable element does not have CodeMirror handlers.

## Exact Readback

CodeMirror 6 virtualizes long documents, so DOM `textContent` or visible
`.cm-line` nodes are not a reliable representation of the complete source.
Readback must also use the editor event contract:

1. Dispatch the select-all shortcut again.
2. Dispatch a `ClipboardEvent("copy")` with a fresh `DataTransfer`.
3. Require CodeMirror to consume the copy event.
4. Read `text/plain` from the transfer and require strict JavaScript string
   equality with the task code.

Any select-all, paste, copy, or equality failure returns an `error` result to
the CLI. The submit button remains untouched.

## Compatibility

The editor detection order becomes:

1. CodeMirror 6 contenteditable
2. CodeMirror 5 instance
3. Ace instance
4. Monaco model
5. visible native textarea

Existing language selection, task validation, submit-button uniqueness,
loopback HTTP protocol, and CLI behavior do not change. The userscript version
increments from `1.0.1` to `1.0.2`.

## Error Handling

The adapter reports stage-specific messages when:

- no unique CodeMirror 6 content element is available;
- the editor does not consume select-all;
- `DataTransfer` or synthetic `ClipboardEvent` construction is unavailable;
- the editor does not consume paste or copy;
- copied text differs from the submitted source.

Unsupported event APIs do not trigger a direct DOM-mutation fallback. Existing
adapters remain available only when no CodeMirror 6 editor is detected.

## Verification

Extend the Playwright fixture with a CodeMirror 6-compatible event harness. The
harness owns an internal document string and handles select-all, paste, and copy
events in the same observable way as CodeMirror 6.

The success case verifies multiline source replacement, exact clipboard
readback, one submit click, and a `submit_clicked` result. Failure cases disable
paste or copy handling and verify an `error` result with zero submit clicks.

Run the complete Python suite, `node --check` for the userscript, and existing
Bash/Python syntax checks. Automated tests remain local and never submit to
Luogu.
