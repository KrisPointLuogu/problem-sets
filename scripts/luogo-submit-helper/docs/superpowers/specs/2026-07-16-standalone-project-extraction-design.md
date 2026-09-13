# Standalone Project Extraction Design

## Goal

Extract the Luogu problem-set submission helpers from
`/home/rainboy/mycode/rbook_new_problem_solutions` into the existing empty Git
repository at `/home/rainboy/mycode/luogo-submit-helper`.

The result remains a lightweight script project. It must run without being
installed as a Python package, preserve the existing manual and browser-assisted
workflows, and accept an external rbook-compatible `problems/` directory.

## Project Structure

```text
luogo-submit-helper/
├── README.md
├── requirements.txt
├── .gitignore
├── submit_problem_sets.py
├── submit_problem_sets_auto.py
├── docs/
│   ├── automatic.md
│   └── superpowers/specs/...
└── tests/
    └── test_submit_problem_sets.py
```

`submit_problem_sets.py` contains the shared problem-set parsing, local solution
lookup, public accepted-problem lookup, and terminal navigation logic.
`submit_problem_sets_auto.py` imports that module directly and adds Playwright
browser automation. The project does not add a package or console-entry-point
layer.

## Command-Line Compatibility

Existing behavior and options remain available, including `--skip-done`, wait
ranges, browser controls, and the manual helper's `--code-ext` filter.

Both scripts add:

```text
--problems-root PATH
```

The option defaults to `problems`, preserving current behavior when the command
is launched from an rbook repository root. An absolute path allows either script
to be launched from the standalone project or any other working directory.

The positional problem-set Markdown path remains explicit. The scripts do not
assume that problem sets or solution files live inside the standalone project.

## Documentation And Dependencies

`README.md` becomes the main entry point and documents installation, the manual
workflow, external repository paths, language filtering, and links to the
automatic workflow. The detailed automatic-submission documentation moves to
`docs/automatic.md`.

The manual helper uses only the Python standard library. `requirements.txt`
declares Playwright for the automatic helper, and the README separately documents
the Chromium installation command.

The project `.gitignore` covers Python bytecode, test caches, virtual
environments, and local browser state if it is placed under the repository.

## Source Repository Cleanup

After the target files are complete and verified, remove these source files:

- `scripts/problem-analysis-tools/submit_helper.py`
- `scripts/problem-analysis-tools/submit_helper.md`
- `scripts/problem-analysis-tools/submit_helper_auto.py`
- `scripts/problem-analysis-tools/submit_helper_auto.md`
- `scripts/problem-analysis-tools/test_submit_helper.py`

Remove only the four matching ignore rules from the source `.gitignore`, leaving
all unrelated user changes intact. Remove the old tracked helper design document
from the source repository; this standalone-project specification supersedes it,
so the rbook repository no longer presents the extracted tool as a local
component.

All executable examples, dynamic imports, headings, comparison tables, user-agent
labels, and test paths are updated to the new `submit_problem_sets` names. A
repository-wide no-ignore search verifies that no operational old-name reference
remains.

## Error Handling

The existing clear error for an unreadable problem-set file remains. A missing or
incorrect `--problems-root` produces no navigable local solutions through the
existing missing-code statistics rather than a traceback. Unsupported
`--code-ext` values remain command-line errors.

The automatic helper continues to import Playwright lazily and reports the
installation command when the dependency is unavailable. Merely requesting help
or importing the automatic module does not require Playwright or launch a
browser.

## Verification

Move and update the current focused unit tests, then add coverage proving that a
custom problems root reaches solution lookup. Verification consists of:

- `python3 -m unittest discover -s tests`
- Python syntax compilation for both scripts and the test module
- `--help` smoke tests for both scripts
- importing the automatic helper without opening a browser
- a no-ignore search for stale `submit_helper` operational references

No verification step logs into Luogu, opens a submission page, or submits code.
