#!/usr/bin/env python3
"""Judge user's T1-T5 solutions against the official data in 2026SOLVE/.

Usage:
    python3 judge.py            # judge all problems T1-T5
    python3 judge.py T1 T3      # judge only problems T1 and T3

Each problem's source is Tx/main.cpp; it reads stdin, writes stdout.
Official tests live in 2026SOLVE/Tx/data/*.in (expected output: *.out or *.ans).
"""
import os
import signal
import subprocess
import sys
import tempfile
import time
import zipfile

BASE = os.path.dirname(os.path.abspath(__file__))
PROBS = ["T1", "T2", "T3", "T4", "T5"]
OFFICIAL = "2026SOLVE"
TIMELIMIT = 1.0
COMPILE_FLAGS = ["-O2", "-std=c++17", "-w"]

OUT_EXTS = {
    "T1": "out",
    "T2": "out",
    "T3": "out",
    "T4": "out",
    "T5": "ans",
}


def ensure_data(prob: str) -> str:
    data_dir = os.path.join(BASE, OFFICIAL, prob, "data")
    if os.path.isdir(data_dir) and os.listdir(data_dir):
        return data_dir
    zip_path = os.path.join(BASE, OFFICIAL, prob, "data.zip")
    if os.path.isfile(zip_path):
        os.makedirs(data_dir, exist_ok=True)
        with zipfile.ZipFile(zip_path, "r") as zf:
            zf.extractall(data_dir)
        return data_dir
    return data_dir


def normalize(data: bytes) -> list:
    lines = data.decode(errors="replace").splitlines()
    lines = [line.rstrip() for line in lines]
    while lines and lines[-1] == "":
        lines.pop()
    return lines


def compile_one(prob: str, workdir: str) -> tuple:
    src = os.path.join(BASE, prob, "main.cpp")
    if not os.path.isfile(src):
        return None, f"missing source {src}"
    exe = os.path.join(workdir, f"{prob}.bin")
    ret = subprocess.run(
        ["g++", *COMPILE_FLAGS, "-o", exe, src],
        capture_output=True,
    )
    if ret.returncode != 0:
        return None, ret.stderr.decode(errors="replace")
    return exe, None


def judge_problem(prob: str, exe: str) -> dict:
    data_dir = ensure_data(prob)
    ext = OUT_EXTS[prob]
    cases = sorted(f for f in os.listdir(data_dir) if f.endswith(".in"))
    results = []
    for name in cases:
        expected_name = name[:-3] + "." + ext
        expected_path = os.path.join(data_dir, expected_name)
        if not os.path.isfile(expected_path):
            results.append((name, "NO_ANSWER", 0.0))
            continue
        with open(os.path.join(data_dir, name), "rb") as f:
            stdin = f.read()
        try:
            t0 = time.monotonic()
            proc = subprocess.Popen(
                [exe],
                stdin=subprocess.PIPE,
                stdout=subprocess.PIPE,
                stderr=subprocess.PIPE,
                start_new_session=True,
            )
            try:
                out, err = proc.communicate(input=stdin, timeout=TIMELIMIT)
            except subprocess.TimeoutExpired:
                os.killpg(proc.pid, signal.SIGKILL)
                proc.wait()
                elapsed = time.monotonic() - t0
                results.append((name, "TLE", elapsed))
                continue
            elapsed = time.monotonic() - t0
        except Exception:
            results.append((name, "RE", 0.0))
            continue
        if proc.returncode != 0:
            results.append((name, "RE", elapsed))
            continue
        with open(expected_path, "rb") as f:
            expected = f.read()
        if normalize(out) == normalize(expected):
            results.append((name, "AC", elapsed))
        else:
            results.append((name, "WA", elapsed))
    return {
        "prob": prob,
        "results": results,
        "ac": sum(1 for r in results if r[1] == "AC"),
        "total": len(cases),
    }


def main() -> int:
    probs = [p for p in sys.argv[1:]] or list(PROBS)
    probs = [p for p in probs if p in PROBS]

    all_summaries = []
    with tempfile.TemporaryDirectory(prefix="judge_") as workdir:
        for prob in probs:
            exe, err = compile_one(prob, workdir)
            if exe is None:
                print(f"[{prob}] COMPILE ERROR")
                print(err)
                continue
            summary = judge_problem(prob, exe)
            all_summaries.append(summary)
            print(f"[{prob}] {summary['ac']}/{summary['total']} passed")
            for name, verdict, elapsed in summary["results"]:
                mark = "PASS" if verdict == "AC" else "FAIL"
                extra = "" if verdict == "AC" else f" ({verdict})"
                print(f"    {name}: {mark}{extra}  [{elapsed:.2f}s]")

    if not all_summaries:
        return 0

    total_ac = sum(s["ac"] for s in all_summaries)
    total_cases = sum(s["total"] for s in all_summaries)
    score = 100.0 * total_ac / total_cases if total_cases else 0.0
    print()
    print("=" * 46)
    print(f"TOTAL SCORE: {total_ac}/{total_cases}  =  {score:.1f}/100")
    print("=" * 46)
    return 0


if __name__ == "__main__":
    sys.exit(main())
