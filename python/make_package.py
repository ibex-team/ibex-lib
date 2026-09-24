#!/usr/bin/env python3
"""Build the standalone `ibexopt-ml` package: executable + Python + benchmarks.

The package is meant to be used as a black box: a prebuilt binary, the Python
client, and the benchmarks to run it on. No sources, no build system.

`python/README.md` is the reference document for both the source tree and the
package, so the package copy is *generated* from it here: the sections about
building and about the C++ internals are replaced by their black-box
equivalents, and the links into the Ibex tree are flattened. Edit
`python/README.md`; never edit the generated copy.

    python3 python/make_package.py [-o ibexopt-ml.zip]
"""

import argparse
import os
import re
import shutil
import subprocess
import sys
import tempfile
import zipfile

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))

SECTION_2 = """## 2. What is in this package

```
ibexopt-ml/
  bin/ibexopt-ml            the executable (links against libstdc++, libm,
                            libgcc and libc only%s)
  python/ibexml.py          client, Python branch & bound, tensor encoding
  python/collect_dataset.py dataset collection over a set of instances
  python/test_admissible.py safety net: a no-information model must terminate
  python/experiment_bisectors.py  compare branching rules over the benchmarks
  python/README.md          this document
  benchs/                   298 Minibex instances, flat, plus manifest.csv
```

There is nothing to compile. `ibexml.py` locates the binary on its own
(`../bin/ibexopt-ml`, relative to the Python package); `$IBEXOPT_ML` overrides
that if you move it elsewhere.

```bash
cd ibexopt-ml
./bin/ibexopt-ml --version
printf '{"cmd":"info"}\\n{"cmd":"quit"}\\n' | ./bin/ibexopt-ml benchs/easy/ex2_1_1.bch
```

numpy is needed only for the optional `encode()` / `labels()` helpers; the
client itself is standard library only.

"""

SECTION_13 = """## 13. Guarantees and side effects

The contract the solver honours, and where it stops honouring it.

**A box from the client is a real node.** A box handed to `contract`,
`features`, `dive` or `sample` becomes a full search node: the bookkeeping every
operator expects is attached to it exactly as IbexOpt attaches it to its own
root. There is no degraded path for boxes that did not come from the search.

**Dives are exactly isolated.** Three things leak between calls and all three
are saved and put back around every dive: the incumbent and the bounds, the
random generator, and the contractor's adaptive shaving state. The last one is
easy to miss — ACID tunes itself from call to call, and before it was restored
the *same* dive repeated five times gave `178, 170, 170, 170, 170`. It now gives
`178` five times.

The consequence is that sampling costs time and nothing else. On
`ex6_1_2.bch`, `--collect` with `--sample-prob` 0, 0.3 and 0.6 all visit 30
nodes and return the same enclosure — the same 30 nodes `ibexopt` visits.

**The candidates of one node are comparable.** Every dive of a node starts from
the same incumbent, the same random stream and the same contractor tuning, so
their sizes differ only because of the variable that was forced. Across nodes
they are not comparable — see [§7.3](#73-labelsk--the-dives).

**`eps` nodes are yours to record.** `contract` hands back `eps_lb`, the
objective lower bound a too-small box certifies, rather than folding it into its
own bookkeeping: in server mode you own the cell buffer, so you own the lower
bound. In `--collect` and `--run` the solver keeps it, because there it owns the
buffer.

**What is *not* guaranteed.** `search()` in `ibexml.py` selects nodes
best-first, which is not what `ibexopt` does; use `--run` when node counts must
be comparable. And `lp_calls` counts the bisector's LP solves only, so use
`time` when you need an unbiased measure of effort.

"""

SECTION_INSTANCES = """### The instance set

`benchs/` holds **298 instances** in Minibex format: the union of the seven
benchmark directories of the Ibex tree worth experimenting on (`easy`,
`blowup`, `medium`, `hard`, `others`, `unsolved`,
`coconutbenchmark-library2`), deduplicated.

The overlap between those directories is not trivial, and the merge had to
handle three different cases:

| case | example | what was done |
|---|---|---|
| same name, same bytes | `ex2_1_7.bch` in `blowup` and `medium` | one copy |
| same name, **different** formulation | `dualc1`, `ex2_1_7`, `ex2_1_8`, `launch` | both kept; the `coconut` one is suffixed `__coconut` |
| different name, same bytes | `ex8_4inf-1.bch` = `test_infinity1.bch` | one copy |

298 is exactly the number of distinct file contents across the seven
directories, and all 298 parse as optimization problems.

`benchs/manifest.csv` maps every instance back to the directory it came from,
which is the only record of how hard it is; the report breaks its counts down
by that column.

"""

SECTION_15 = """## 16. File map

| file | role |
|---|---|
| `bin/ibexopt-ml` | the executable |
| `python/ibexml.py` | client, Python branch & bound, tensor encoding |
| `python/collect_dataset.py` | dataset collection over a set of instances |
| `python/experiment_bisectors.py` | run and report a comparison of branching rules |
| `python/test_admissible.py` | safety net: a constant-score model must terminate |
| `python/README.md` | this document |
| `benchs/` | 298 Minibex instances, plus `manifest.csv` |

This package contains no sources and no build system; it is the binary and what
is needed to drive it. The C++ lives in the Ibex tree it was built from.

Reference for the heuristic this imitates: I. Araya, B. Neveu, *lsmear: a
variable selection strategy for interval branch and bound solvers*, Journal of
Global Optimization, 2018.
"""

INTRO_OLD = """It is not a fork of the optimizer: it reuses `DefaultOptimizerConfig`, so the
contractor (HC4 → ACID(HC4) → fix-point of X-Newton + HC4), the loup finder
(`LoupFinderDefault` with inHC4), the cell buffer (`CellBeamSearch`) and the
bisector (`LSmear`, variant `LSMEAR_MG`) are byte-for-byte the ones `ibexopt`
runs. Whatever you measure here is what the real solver would have done."""

INTRO_NEW = """It is not a modified optimizer: it runs IbexOpt's own default configuration —
the same contractor (HC4 → ACID(HC4) → fix-point of X-Newton + HC4), the same
upper bounding, the same node selection and the same LSmear bisector that
`ibexopt` uses. Whatever you measure here is what the real solver would have
done."""


#: The instances ship flat in benchs/, so every path through a class
#: subdirectory has to collapse onto it -- in the prose and in the scripts.
_BENCH_DIRS = ("all", "easy", "medium", "hard", "blowup", "others", "unsolved",
               "coconutbenchmark-library2")


def _flatten_bench_paths(s):
    for base in ("benchs/optim", "benchs"):
        for d in _BENCH_DIRS:
            s = s.replace("%s/%s/" % (base, d), "benchs/")
            s = s.replace("%s/%s" % (base, d), "benchs")
    s = s.replace("benchs/optim/", "benchs/")
    s = s.replace("benchs/optim", "benchs")
    return s


def links_ipopt(binary):
    """True if this build needs libipopt at run time.

    The package is meant to run anywhere with nothing installed, and a binary
    built with -DIBEX_WITH_IPOPT=ON does not: better to say so in the README
    than to let the recipient find out.
    """
    out = subprocess.run(["ldd", binary], capture_output=True, text=True)
    return "libipopt" in out.stdout


def blackbox_readme(text, ipopt_note=""):
    """Turn the source-tree README into the black-box one."""
    s = text

    s = s.replace(INTRO_OLD, INTRO_NEW)

    # section 2: building -> what is in this package
    s = s.replace(s[s.index("## 2. Building"):s.index("## 3. Concepts")],
                  SECTION_2 % (ipopt_note or ""))
    s = s.replace("2. [Building](#2-building)",
                  "2. [What is in this package](#2-what-is-in-this-package)")

    # section 13: C++ internals -> the observable contract
    s = s.replace(s[s.index("## 13. How it works inside"):s.index("## 14. Known limitations")],
                  SECTION_13)
    s = s.replace("13. [How it works inside](#13-how-it-works-inside)",
                  "13. [Guarantees and side effects](#13-guarantees-and-side-effects)")
    s = s.replace("(#13-how-it-works-inside)", "(#13-guarantees-and-side-effects)")

    # the package ships the instances already merged
    s = s.replace(s[s.index("### The instance set"):s.index("### The rules")],
                  SECTION_INSTANCES)

    # section 16: file map of the package
    s = s.replace(s[s.index("## 16. File map"):], SECTION_15)

    # paths are relative to the package root, where the instances are flat
    s = s.replace("./build/bin/ibexopt-ml", "./bin/ibexopt-ml")
    s = _flatten_bench_paths(s)

    # links into the Ibex tree do not resolve here: keep the text, drop the link
    s = re.sub(r"\[(`[^\]]+`)\]\(\.\./[^)]+\)", r"\1", s)
    s = re.sub(r"\[([^\]]+)\]\(\.\./[^)]+\)", r"\1", s)

    return s


def check_readme(s):
    """Every internal anchor must resolve and no source-tree path may survive."""
    def slug(t):
        t = re.sub(r"[^a-z0-9 \-]", "", t.strip().lower())
        return t.replace(" ", "-")

    heads = {slug(l.lstrip("#")) for l in s.splitlines() if l.startswith("#")}
    bad = [a for a in sorted(set(re.findall(r"\]\(#([\w-]+)\)", s))) if a not in heads]
    if bad:
        raise SystemExit("broken anchors in the generated README: %s" % bad)

    leftovers = (re.findall(r"\]\(\.\./[^)]*\)", s)
                 + re.findall(r"benchs/optim", s)
                 + re.findall(r"benchs/(?:%s)\b" % "|".join(_BENCH_DIRS), s))
    if leftovers:
        raise SystemExit("source-tree paths left in the generated README: %s" % leftovers)


def main():
    p = argparse.ArgumentParser(description=__doc__,
                                formatter_class=argparse.RawDescriptionHelpFormatter)
    p.add_argument("-o", "--output",
                   default=os.path.join(os.path.dirname(ROOT), "ibexopt-ml.zip"),
                   help="output zip (default: next to the repository, so that "
                        "it does not land in the working tree)")
    p.add_argument("--binary", default=os.path.join(ROOT, "build", "bin", "ibexopt-ml"))
    p.add_argument("--benchs", default=os.path.join(ROOT, "benchs", "optim", "all"),
                   help="directory of .bch files to ship (default: the merged set "
                        "built by benchs/optim/make_all.py)")
    args = p.parse_args()

    if not os.path.isfile(args.binary):
        raise SystemExit("no binary at %s -- build it first:\n"
                         "  mkdir -p build && cd build && cmake .. -DLP_LIB=soplex && make -j8"
                         % args.binary)

    # refuse to ship a binary that cannot run
    out = subprocess.run([args.binary, "--version"], capture_output=True, text=True)
    if out.returncode != 0 or "IbexOpt-ML" not in out.stdout:
        raise SystemExit("the binary does not run: %s%s" % (out.stdout, out.stderr))

    with tempfile.TemporaryDirectory() as tmp:
        pkg = os.path.join(tmp, "ibexopt-ml")
        os.makedirs(os.path.join(pkg, "bin"))
        os.makedirs(os.path.join(pkg, "python"))

        shutil.copy2(args.binary, os.path.join(pkg, "bin", "ibexopt-ml"))
        os.chmod(os.path.join(pkg, "bin", "ibexopt-ml"), 0o755)

        for f in ("ibexml.py", "collect_dataset.py", "test_admissible.py",
                  "experiment_bisectors.py"):
            shutil.copy2(os.path.join(ROOT, "python", f), os.path.join(pkg, "python", f))

        note = ("" if not links_ipopt(args.binary) else
                ", plus libipopt:\n                            this build has --loup ipopt* and needs Ipopt installed")
        readme = blackbox_readme(
            open(os.path.join(ROOT, "python", "README.md")).read(), note)
        check_readme(readme)
        open(os.path.join(pkg, "python", "README.md"), "w").write(readme)

        front = _flatten_bench_paths(open(os.path.join(ROOT, "python", "PACKAGE.md")).read())
        open(os.path.join(pkg, "README.md"), "w").write(front)

        # Only the instances and their manifest: the source tree also holds
        # .nl/.int copies and large archives that have no use here.
        if not os.path.isdir(args.benchs):
            raise SystemExit("no %s -- build it with benchs/optim/make_all.py"
                             % args.benchs)
        os.makedirs(os.path.join(pkg, "benchs"))
        nb = 0
        for f in sorted(os.listdir(args.benchs)):
            if f.endswith(".bch") or f == "manifest.csv":
                shutil.copy2(os.path.join(args.benchs, f),
                             os.path.join(pkg, "benchs", f))
                nb += 1
        if nb < 2:
            raise SystemExit("%s holds no instances" % args.benchs)

        # the docstrings and defaults ship with the package, so their paths
        # must match its layout
        for f in ("ibexml.py", "collect_dataset.py", "test_admissible.py",
                  "experiment_bisectors.py"):
            path = os.path.join(pkg, "python", f)
            t = _flatten_bench_paths(open(path).read())
            open(path, "w").write(t)

        n = 0
        with zipfile.ZipFile(args.output, "w", zipfile.ZIP_DEFLATED) as z:
            for base, _, files in os.walk(pkg):
                for f in sorted(files):
                    full = os.path.join(base, f)
                    rel = os.path.relpath(full, tmp)
                    info = zipfile.ZipInfo.from_file(full, rel)
                    info.compress_type = zipfile.ZIP_DEFLATED
                    if os.access(full, os.X_OK) and not f.endswith((".md", ".bch")):
                        info.external_attr = 0o755 << 16
                    with open(full, "rb") as fh:
                        z.writestr(info, fh.read())
                    n += 1

    print("%s  (%d files, %.1f MB)"
          % (args.output, n, os.path.getsize(args.output) / 1e6), file=sys.stderr)


if __name__ == "__main__":
    main()
