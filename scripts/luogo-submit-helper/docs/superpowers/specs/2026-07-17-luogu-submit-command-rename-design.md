# Luogu Submit Command Rename Design

## Goal

Rename the single-problem command from `luogu` to `luogu-submit` everywhere it
is presented or installed. The old `luogu` command is removed rather than kept
as an alias.

The submission workflow, configuration schema, local HTTP protocol, ScriptCat
automation, supported languages, and problem-directory conventions do not
change.

## File And Command Rename

Rename the executable at the repository root with Git history preservation:

```text
luogu -> luogu-submit
```

The executable's argument parser uses `luogu-submit` as its program name. All
CLI help, configuration guidance, and usage errors use these forms:

```text
luogu-submit config /path/to/problems
luogu-submit 1001
luogu-submit B2002
luogu-submit
```

The test module is renamed from `test_luogu_command.py` to
`test_luogu_submit_command.py`. Its source loader points to `luogu-submit`, and
its internal module variable is renamed for clarity.

## Installer Migration

`install.sh` installs this link:

```text
~/.local/bin/luogu-submit -> <repository>/luogu-submit
```

It retains the existing protection against overwriting a non-symbolic-link
target.

After the new link is installed successfully, the installer checks the old
`~/.local/bin/luogu` path. It removes that path only when both conditions hold:

- the old path is a symbolic link; and
- its raw link target is exactly `<repository>/luogu`, which is the absolute
  target written by the previous installer.

A regular file, directory, or symbolic link with any other target remains
untouched. Failure to install the new command must not remove the old link.

## Names That Remain Unchanged

Only the executable command is renamed. These existing identifiers retain their
current spelling for compatibility and domain meaning:

- the configured `luogu/` problem directory;
- `${XDG_CONFIG_HOME:-~/.config}/luogu-submit-helper/config.json`;
- `userscripts/luogu-submit-helper.user.js`;
- `luogu-submit-port` and `luogu-submit-token` URL parameters;
- internal problem helpers such as `luogu_real_id` and `luogu_dir_id`;
- Luogu website URLs and problem-set `oj: luogu` values.

The userscript changes only user-facing text that names the local command. Its
protocol and DOM identifiers remain unchanged.

## Documentation

README installation instructions, workflow tables, dependency text, examples,
and troubleshooting use `luogu-submit`. The prior single-problem design document
is updated so it does not advertise an obsolete command or installer target.
This rename design remains the record of why that terminology changed.

## Error Handling

Command and submission error behavior remains unchanged. Installer migration is
conservative: an unexpected old path is reported or left alone, never removed.
The installer continues to report when `~/.local/bin` is absent from `PATH`.

## Verification

Tests and checks cover:

- loading the renamed executable and running the existing command/protocol
  suite;
- installing the new `luogu-submit` symbolic link in a temporary home;
- removing the exact old project-owned `luogu` symbolic link;
- preserving an unrelated old `luogu` symbolic link;
- refusing to overwrite a non-symbolic-link `luogu-submit` target;
- `./luogu-submit --help` and rejection of the old root executable path;
- the complete Python suite plus Python, userscript, and Bash syntax checks;
- a focused repository search proving that operational command examples no
  longer use `luogu`.

The existing untracked `userscripts/luogu-submit-area-fixed.user.js` is outside
this rename and remains untouched.
