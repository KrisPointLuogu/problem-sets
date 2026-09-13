# Submit Button Label Fix Design

## Problem

After the userscript selects C++20 and fills and verifies the CodeMirror 6
editor, it reports:

```text
洛谷提交助手: 找不到可用的提交按钮
```

The logged-in Luogu page contains one relevant button:

```json
{
  "text": "提交评测",
  "disabled": false,
  "className": "solid lform-size-middle"
}
```

The July 12, 2026 Columba frontend confirms that
`problem_submit.submit_to_judge` renders as `提交评测` in Chinese and
`Submit to Judge` in English. The userscript currently accepts only `提交`,
`提交答案`, and `提交代码`, so the current button is excluded.

## Fix

Keep exact label matching and extend the normalized submit-button label set to:

```text
提交
提交答案
提交代码
提交评测
SUBMIT
SUBMIT ANSWER
SUBMIT CODE
SUBMIT TO JUDGE
```

The English entries are shown in normalized uppercase because the existing
`normalizeLabel` function uppercases labels before comparison.

Do not replace this set with prefix or substring matching. Labels such as
`提交文件` and `选择文件` must not become valid submit-button candidates.

## Safety Conditions

All existing button constraints remain mandatory. A candidate must:

- be a `button` or `input[type="submit"]` inside the submission root;
- be visible;
- not have the native `disabled` state;
- not have `aria-disabled="true"`;
- have an exact normalized label from the approved set.

Exactly one candidate must remain. Zero candidates return the existing missing
button error. Multiple candidates return the existing ambiguity error. Neither
failure path clicks a button.

The `solid` CSS class is useful diagnostic evidence but is not used as the
primary selector because it expresses presentation rather than submit meaning.

## Scope

Only the userscript button-label set and browser fixtures change. The CLI,
loopback HTTP protocol, language selection, CodeMirror 6 adapter, code readback,
and URL cleanup remain unchanged. The userscript version increments from
`1.0.2` to `1.0.3`.

## Verification

Update the primary Playwright fixtures to use the real `提交评测` label. Add a
successful `Submit to Judge` fixture to cover the English locale. Retain the
ambiguous-button test and add or retain a nonmatching `提交文件` distractor to
prove that exact matching does not broaden into unsafe prefix matching.

Run the complete Python suite and the existing JavaScript, Python, and Bash
syntax checks. Tests remain local and do not submit to Luogu.
