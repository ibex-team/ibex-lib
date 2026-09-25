#!/usr/bin/env python3
"""Compare branching rules on a set of instances, and report the comparison.

Every run is `ibexopt-ml --solve` on one instance with one rule: same
contractor, same loup finder, same node selection, same seed, same precision --
only the bisector changes. A rule may be one of the hand-written bisectors or a
learned model (``name=path/to.model``).

    # run
    python3 python/experiment_bisectors.py run -o results/bisectors.csv \\
        --dir benchs/optim/all --timeout 20 --jobs 8

    # report (re-runnable on a partial csv)
    python3 python/experiment_bisectors.py report results/bisectors.csv

The run is resumable: results are appended as they land and an existing csv is
read back, so an interrupted sweep continues where it stopped.

Three numbers are reported, because no single one is honest on its own:

  solved       how many instances the rule closed inside the timeout. This is
               the primary number: a rule that solves more is better, whatever
               its node counts look like.
  common       total nodes and time over the instances *every* rule solved.
               Comparing means over a set that includes timeouts would reward a
               rule for giving up early, since a timeout truncates its own cost.
  geomean      geometric mean, over the common set, of the ratio to the
               baseline. Node counts span orders of magnitude, so the arithmetic
               mean of ratios is dominated by a handful of instances.

PAR2 is also given: a timeout is charged twice the limit. It is the usual way to
fold "solved" and "time" into one number, and unlike the common-set totals it
uses every instance.

A note on time: Ibex measures CPU seconds (getrusage), not wall clock, and
enforces --timeout in those same units. Node counts are deterministic and
unaffected by how many runs go in parallel; times are affected only through
cache and memory contention. The default --jobs leaves half the cores free for
that reason.
"""

import argparse
import csv
import json
import math
import os
import subprocess
import sys
import time
from concurrent.futures import ThreadPoolExecutor

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from ibexml import (solve, _env, BISECTORS, RELAXATIONS,          # noqa: E402
                    LOUP_FINDERS, DEFAULT_BINARY)

FIELDS = ["instance", "set", "rule", "relax", "ub", "status", "nodes", "time",
          "loup", "uplo", "timeout"]

#: A run in one of these states is finished whatever the limit: re-running it
#: with more time cannot change the answer. "killed" is not among them -- it
#: means the wall-clock guard fired, which says nothing about the instance.
TERMINAL = ("complete", "unbounded", "unbounded_obj", "infeasible",
            "no_feasible_found", "unreached_prec")

#: ...but only "complete" means the optimum was actually enclosed to the
#: required precision. The others end the search without an answer, and a rule
#: must not be credited for them.
SOLVED = ("complete",)

#: Anything at or above this is "no incumbent": +oo, or the DBL_MAX the solver
#: sometimes carries instead.
NO_INCUMBENT = 1e300


def solved(r):
    """Whether a run actually enclosed the optimum.

    Status alone is not enough on results written before the solver told the
    outcomes apart: a search whose buffer simply emptied was reported
    "complete" even when it never found a feasible point. Requiring a finite
    incumbent recovers the right answer from the recorded values, so old and
    new rows can be compared without re-running either.
    """
    return r["status"] in SOLVED and float(r["loup"]) < NO_INCUMBENT

#: Runs in parallel. Half of a 16-core machine by default: Ibex reports CPU time
#: (getrusage), so concurrency does not inflate the numbers the way wall-clock
#: timing would, but saturating every core still costs a few percent through
#: cache and memory-bandwidth contention -- and leaves nothing for anything else.
DEFAULT_JOBS = min(8, os.cpu_count() or 4)


# --------------------------------------------------------------------- running

def read_manifest(d):
    """instance -> source set, when the directory was built by make_all.py."""
    path = os.path.join(d, "manifest.csv")
    if not os.path.exists(path):
        return {}
    with open(path) as f:
        return {r["instance"]: r["set"] for r in csv.DictReader(f)}


#: The strategy ported from Bertrand Neveu's fork, run through its own binary
#: (src/bin/ibexopt-ipopt.cpp) rather than through ibexopt-ml. It is not a
#: bisector: it is a whole hand-built strategy -- its own contractor assembly,
#: its own node selection -- and it is in the comparison to answer whether the
#: configuration ibexopt-ml builds gives anything away against the one the Ipopt
#: finder was written for.
REF_RULE = "ref-ipopt"

#: The same strategy with ibexopt's node selection instead of its own. The two
#: differ in one argument, and running both is what turns "the copied strategy
#: closes more instances and costs more time" into a statement about which part
#: of it does that: everything else it does is held identical between them.
REF_RULE_DH = "ref-ipopt-dh"

#: rule name -> the node selection passed to the binary
_REF_BUFFER = {REF_RULE: "bfs", REF_RULE_DH: "dh"}

REF_BINARY = os.path.join(os.path.dirname(DEFAULT_BINARY), "ibexopt-ipopt")

#: How this runner's names for the two other axes are spelled there.
_REF_RELAX = {"xtaylor": "xn", "affine": "art", "both": "compo"}
#: "default" is ibexopt's own upper bounding, which has inHC4 *on*
#: (DefaultOptimizerConfig::default_inHC4), so it is "xninhc4" there, not "xn".
#: The ipopt* names mean the same thing on both sides: ipoptxn is probing plus
#: X-Taylor with inHC4 off, ipoptxninhc4 is inHC4 plus X-Taylor.
_REF_UB = {"default": "xninhc4", "ipoptprob": "ipoptprob", "ipoptxn": "ipoptxn",
           "ipoptxninhc4": "ipoptxninhc4"}


def solve_ref(path, relax, ub, timeout, random_seed, wall_timeout, binary=None,
              rule=REF_RULE):
    """Run the copied strategy, and answer in the shape `solve()` answers.

    Everything that can be held equal with the ibexopt-ml arms is: the same
    relaxation, the same upper bounding, the same seed, and the same stopping
    criterion (eps_x 0, 1e-3 relative and 1e-7 absolute on the objective, eps_h
    1e-8 -- ibexopt's defaults). What cannot be held equal is the point: this
    arm is acidhc4 + LSmear(MG) + its own hand-built contractor, which is what
    makes it worth running. `rule` picks the node selection: REF_RULE is the
    strategy as written (best-first on a plain CellHeap), REF_RULE_DH swaps in
    the CellDoubleHeap ibexopt builds.
    """
    argv = [binary or REF_BINARY, path, "acidhc4", _REF_RELAX[relax], "lsmearmg",
            _REF_UB[ub]]
    if ub.startswith("ipopt"):
        argv += ["100", "0"]              # ipopt frequency, "this is a QP"
    argv += [_REF_BUFFER[rule], "0", "1e-3/1e-7", "1e-8", repr(float(timeout)),
             str(int(random_seed)), "--json"]
    try:
        out = subprocess.run(argv, capture_output=True, text=True,
                             timeout=wall_timeout, env=_env())
    except subprocess.TimeoutExpired:
        return {"status": "killed", "loup": float("inf"), "uplo": float("-inf"),
                "nodes": 0, "time": wall_timeout, "rule": rule}
    line = [l for l in out.stdout.strip().splitlines() if l.startswith("{")]
    if not line:
        raise RuntimeError("no JSON from %s: %s"
                           % (rule, " ".join((out.stdout + out.stderr).split())[:200]))
    return json.loads(line[-1])


def parse_rules(specs):
    """Each spec is a bisector name, `ref-ipopt[-dh]`, or ``label=model.file``."""
    rules = []
    for s in specs:
        label, _, model = s.partition("=")
        if model:
            if not os.path.exists(model):
                raise SystemExit("no model file %r" % model)
            rules.append((label, None, model))
        elif label in _REF_BUFFER:
            if not os.path.isfile(REF_BINARY):
                raise SystemExit(
                    "no %s -- it is only built with -DIBEX_WITH_IPOPT=ON" % REF_BINARY)
            rules.append((label, label, None))
        else:
            if label not in BISECTORS:
                raise SystemExit("unknown bisector %r (one of: %s, %s, %s)"
                                 % (label, ", ".join(BISECTORS), REF_RULE, REF_RULE_DH))
            rules.append((label, label, None))
    return rules


def row_relax(r):
    """The relaxation a row was produced under (rows predating the column)."""
    return r.get("relax") or "xtaylor"


def row_ub(r):
    """The upper bounding a row was produced under (rows predating the column).

    Every result recorded before the Ipopt loup finder existed was produced with
    the default one, so that is what a missing column means.
    """
    return r.get("ub") or "default"


def key(r):
    """What identifies a run. The relaxation and the upper bounding are part of
    it: the same bisector under a different contraction, or against a different
    incumbent, is a different experiment, not a re-run."""
    return (r["instance"], r["rule"], row_relax(r), row_ub(r))


def row_limit(r):
    """The limit a row was produced under (rows predating the column: 0)."""
    try:
        return float(r.get("timeout") or 0)
    except ValueError:
        return 0.0


def retryable(r):
    """Whether a recorded run should be attempted again.

    An `error:` row means the run did not produce an answer -- usually because
    something killed it from outside -- so it carries no information about the
    instance and is always worth redoing. A `killed` row does: the wall-clock
    guard fired, and it will fire again at the same limit.
    """
    return r["status"].startswith("error:")


def better(a, b):
    """Of two rows for the same (instance, rule), the one that says more.

    A finished run beats an unfinished one; between two unfinished ones, the
    one that was given more time. The results file is append-only so that an
    interrupted sweep loses nothing, which means a re-run leaves both rows in
    it; every reader resolves them through here.
    """
    if a is None:
        return b
    if retryable(a) != retryable(b):
        return b if retryable(a) else a
    fa = a["status"] in TERMINAL
    fb = b["status"] in TERMINAL
    if fa != fb:
        return a if fa else b
    return a if row_limit(a) >= row_limit(b) else b


def count_attempts(path):
    """(instance, rule, relax, ub) -> how many times the run produced no answer.

    An `error:` row carries no information about the instance, so it is worth
    retrying -- but only so often. A run that fails the same way every time
    would otherwise be retried on every resume, forever, and the sweep would
    never report itself finished.
    """
    paths = [path] if isinstance(path, str) else list(path)
    n = {}
    for p in paths:
        if not os.path.exists(p):
            continue
        with open(p) as f:
            for r in csv.DictReader(f):
                if retryable(r):
                    n[key(r)] = n.get(key(r), 0) + 1
    return n


def load_done(path):
    """(instance, rule, relax, ub) -> the most informative row recorded for it.

    Accepts one path or several; several are merged, which is how a run under
    one relaxation is compared with a run under another when they were written
    to different files.
    """
    paths = [path] if isinstance(path, str) else list(path)
    best = {}
    for p in paths:
        if not os.path.exists(p):
            continue
        with open(p) as f:
            for r in csv.DictReader(f):
                k = key(r)
                best[k] = better(best.get(k), r)
    return best


def run(args):
    files = sorted(f for f in os.listdir(args.dir) if f.endswith(".bch"))
    if args.limit and args.limit < len(files):
        # evenly spaced rather than the first N: the directory is alphabetical,
        # so a prefix is a biased sample of one benchmark family
        step = len(files) / float(args.limit)
        files = [files[int(k * step)] for k in range(args.limit)]
    if not files:
        raise SystemExit("no .bch under %s" % args.dir)

    sets = read_manifest(args.dir)
    rules = parse_rules(args.rules)
    done = load_done(args.output)
    attempts = count_attempts(args.output)

    todo, kept, redo, gaveup = [], 0, 0, 0
    for f in files:
        for r in rules:
            prev = done.get((f, r[0], args.relax, args.ub))
            if prev is None:
                todo.append((f, r))
            elif retryable(prev):
                if attempts.get((f, r[0], args.relax, args.ub), 0) >= args.max_retries:
                    gaveup += 1                # fails the same way every time
                else:
                    todo.append((f, r))        # produced no answer at all
                    redo += 1
            elif prev["status"] in TERMINAL or row_limit(prev) >= args.timeout:
                kept += 1                      # nothing more to learn from it
            else:
                todo.append((f, r))            # unfinished under a smaller limit
                redo += 1

    print("%d instances x %d rules = %d runs" % (len(files), len(rules),
                                                 len(files) * len(rules)),
          file=sys.stderr)
    print("  %d kept, %d re-run with the larger limit, %d never run"
          % (kept, redo, len(todo) - redo), file=sys.stderr)
    if gaveup:
        print("  %d given up on after %d failed attempts (see the error: rows)"
              % (gaveup, args.max_retries), file=sys.stderr)
    if todo:
        print("  worst case %.1f h at %gs and %d jobs"
              % (len(todo) * args.timeout / args.jobs / 3600.0, args.timeout, args.jobs),
              file=sys.stderr)
    if not todo:
        return

    d = os.path.dirname(os.path.abspath(args.output))
    if d:
        os.makedirs(d, exist_ok=True)

    new = not os.path.exists(args.output)
    out = open(args.output, "a", newline="")
    writer = csv.DictWriter(out, FIELDS)
    if new:
        writer.writeheader()
        out.flush()

    counter = [0]
    t0 = time.time()

    def one(job):
        fname, (label, bisector, model) = job
        path = os.path.join(args.dir, fname)
        try:
            wall = args.timeout * args.wall_factor + 60.0
            if label in _REF_BUFFER:
                r = solve_ref(path, args.relax, args.ub, args.timeout,
                              args.random_seed, wall, rule=label)
            else:
                r = solve(path, model=model, bisector=bisector, relax=args.relax,
                          loup=args.ub, binary=args.binary, timeout=args.timeout,
                          random_seed=args.random_seed, wall_timeout=wall)
        except Exception as e:
            # keep why it failed: "error:IbexError" alone cannot be diagnosed
            # from the results file, and these runs are the ones worth looking at
            why = " ".join(str(e).split())[:120].replace(",", ";")
            r = {"status": "error: %s" % (why or type(e).__name__), "nodes": 0,
                 "time": 0.0, "loup": float("inf"), "uplo": float("-inf")}
        return {"instance": fname, "set": sets.get(fname, "?"), "rule": label,
                "relax": args.relax, "ub": args.ub,
                "status": r["status"], "nodes": r["nodes"], "time": r["time"],
                "loup": r["loup"], "uplo": r["uplo"], "timeout": args.timeout}

    with ThreadPoolExecutor(max_workers=args.jobs) as pool:
        for row in pool.map(one, todo):
            writer.writerow(row)
            out.flush()
            counter[0] += 1
            if counter[0] % 10 == 0 or counter[0] == len(todo):
                el = time.time() - t0
                print("\r  %d/%d runs  (%.0fs elapsed, ~%.0fs left)     "
                      % (counter[0], len(todo), el,
                         el / counter[0] * (len(todo) - counter[0])),
                      end="", file=sys.stderr)
    out.close()
    print("\n-> %s" % args.output, file=sys.stderr)


# -------------------------------------------------------------------- reporting

def geomean(xs):
    xs = [x for x in xs if x > 0]
    return math.exp(sum(math.log(x) for x in xs) / len(xs)) if xs else float("nan")


def report(args):
    by = load_done(args.csv)          # resolves re-runs to the best row
    drop = set()
    if args.exclude and os.path.exists(args.exclude):
        drop = {l.strip() for l in open(args.exclude) if l.strip()}
    keep = None
    if args.only and os.path.exists(args.only):
        keep = {l.strip() for l in open(args.only) if l.strip()}
    rows = [r for r in by.values()
            if row_relax(r) == args.relax and row_ub(r) == args.ub
            and r["instance"] not in drop
            and (keep is None or r["instance"] in keep)]
    if not rows:
        have = sorted({"%s/%s" % (row_relax(r), row_ub(r)) for r in by.values()})
        raise SystemExit("no runs with --relax %s --ub %s in %s (it holds: %s)"
                         % (args.relax, args.ub, args.csv, ", ".join(have)))
    by = {key(r): r for r in rows}
    if not rows:
        raise SystemExit("%s is empty" % args.csv)

    for r in rows:
        r["nodes"] = int(float(r["nodes"]))
        r["time"] = float(r["time"])
        r["loup"] = float(r["loup"])
        r["uplo"] = float(r["uplo"])
        r["solved"] = solved(r)

    rules = []
    for r in rows:
        if r["rule"] not in rules:
            rules.append(r["rule"])
    instances = sorted({r["instance"] for r in rows})
    sets = {r["instance"]: r["set"] for r in rows}
    by = {(r["instance"], r["rule"]): r for r in rows}

    baseline = args.baseline if args.baseline in rules else rules[0]

    limits = sorted({row_limit(r) for r in rows})
    limit = args.timeout if args.timeout else (limits[-1] if limits else 0)
    # Only unfinished runs are charged against their limit, so a mix of limits
    # among runs that all finished changes nothing and is not worth a warning.
    unfinished = sorted({row_limit(r) for r in rows if not r["solved"]})
    if len(unfinished) > 1:
        print("warning: unfinished runs in this file carry different limits (%s); "
              "PAR2 charges each one twice its own"
              % ", ".join("%gs" % x for x in unfinished))

    # only instances every rule was actually run on
    full = [i for i in instances if all((i, u) in by for u in rules)]
    common = [i for i in full if all(by[(i, u)]["solved"] for u in rules)]

    print("instances: %d run, %d with every rule, %d solved by every rule"
          % (len(instances), len(full), len(common)))
    if not common:
        print("  (no instance was solved by every rule: the per-instance ratios and\n"
              "   the common-set totals are undefined -- raise --timeout, or compare\n"
              "   fewer rules)")
    print("rules: %s   (baseline: %s)" % (", ".join(rules), baseline))
    print("relaxation: %s   upper bounding: %s%s%s" % (args.relax, args.ub,
          ("   (%d excluded)" % len(drop)) if drop else "",
          ("   (restricted to %d named instances)" % len(keep)) if keep else ""))
    print("time limit: %s\n" % (", ".join("%gs" % x for x in limits) if limits else "?"))

    hdr = ("%-16s %7s %8s %12s %10s %9s %9s %10s"
           % ("rule", "solved", "timeout", "nodes(com)", "time(com)",
              "geo.nodes", "geo.time", "PAR2"))
    print(hdr)
    print("-" * len(hdr))

    for u in rules:
        rs = [by[(i, u)] for i in full]
        n_solved = sum(1 for r in rs if r["solved"])
        to = len(rs) - n_solved
        cn = sum(by[(i, u)]["nodes"] for i in common)
        ct = sum(by[(i, u)]["time"] for i in common)
        gn = geomean([max(by[(i, u)]["nodes"], 1) / max(by[(i, baseline)]["nodes"], 1)
                      for i in common])
        gt = geomean([max(by[(i, u)]["time"], 1e-6) / max(by[(i, baseline)]["time"], 1e-6)
                      for i in common])
        par2 = (sum(r["time"] if r["solved"] else 2 * (row_limit(r) or limit)
                    for r in rs) / max(len(rs), 1))
        fmt = lambda x: "-" if (x != x) else "%9.2f" % x      # NaN when common is empty
        print("%-16s %7d %8d %12d %10.1f %9s %9s %10.2f"
              % (u, n_solved, to, cn, ct, fmt(gn).strip(), fmt(gt).strip(), par2))

    # Pairwise against the baseline. The common set above is the intersection
    # over *every* rule, so one weak rule shrinks it to what that rule can do
    # and the ratios stop describing the others. Comparing two rules at a time,
    # on the instances both of them closed, is the number that answers "is this
    # rule better than the baseline".
    # A ratio inside the deadband is a tie: counting a one-node difference as a
    # win turns rounding into a result. The band is symmetric in ratio space,
    # which is the space the geometric means live in.
    m = args.margin
    lo_band, hi_band = 1.0 / (1.0 + m), 1.0 + m

    print("\nhead to head against %s, on the instances both closed" % baseline)
    print("  a win needs at least %.0f%% fewer nodes; anything inside that band is a tie"
          % (m * 100))
    hdr = ("%-16s %7s %10s %10s %8s %8s %6s"
           % ("rule", "both", "geo.nodes", "geo.time", "wins", "losses", "ties"))
    print(hdr)
    print("-" * len(hdr))
    for u in rules:
        if u == baseline:
            continue
        both = [i for i in full if by[(i, u)]["solved"] and by[(i, baseline)]["solved"]]
        if not both:
            print("%-16s %7d %10s %10s %8s %8s %6s" % (u, 0, "-", "-", "-", "-", "-"))
            continue
        gn = geomean([max(by[(i, u)]["nodes"], 1) / max(by[(i, baseline)]["nodes"], 1)
                      for i in both])
        gt = geomean([max(by[(i, u)]["time"], 1e-6) / max(by[(i, baseline)]["time"], 1e-6)
                      for i in both])
        ratios = [max(by[(i, u)]["nodes"], 1) / max(by[(i, baseline)]["nodes"], 1)
                  for i in both]
        w = sum(1 for r in ratios if r < lo_band)
        l = sum(1 for r in ratios if r > hi_band)
        print("%-16s %7d %10.2f %10.2f %8d %8d %6d"
              % (u, len(both), gn, gt, w, l, len(both) - w - l))
    print("  (geo < 1 means fewer nodes / less time than the baseline;")
    print("   wins/losses count instances, so they weigh a 2x and a 100x the same)")

    # instances only one side solves say more than any ratio
    print("\nsolved by one and not the other, vs %s" % baseline)
    for u in rules:
        if u == baseline:
            continue
        only_u = [i for i in full if by[(i, u)]["solved"] and not by[(i, baseline)]["solved"]]
        only_b = [i for i in full if by[(i, baseline)]["solved"] and not by[(i, u)]["solved"]]
        print("  %-16s +%-3d  -%-3d" % (u, len(only_u), len(only_b)))

    # per difficulty class
    classes = []
    for i in full:
        if sets[i] not in classes:
            classes.append(sets[i])
    if len(classes) > 1:
        print("\nsolved / run, by source set")
        print("%-16s %s" % ("rule", "".join("%14s" % c[:13] for c in classes)))
        for u in rules:
            cells = []
            for c in classes:
                sub = [i for i in full if sets[i] == c]
                cells.append("%14s" % ("%d/%d" % (sum(1 for i in sub if by[(i, u)]["solved"]),
                                                  len(sub))))
            print("%-16s %s" % (u, "".join(cells)))

    # a rule that returns a different optimum is a bug, not a win
    bad = []
    for i in common:
        los = [by[(i, u)]["loup"] for u in rules]
        ups = [by[(i, u)]["uplo"] for u in rules]
        lo, hi = max(ups), min(los)
        if lo > hi + args.tol * max(1.0, abs(hi)):
            bad.append((i, lo, hi))
    print("\nenclosures disagree on %d of the %d commonly solved instances%s"
          % (len(bad), len(common), ":" if bad else ""))
    for i, lo, hi in bad[:15]:
        print("   %-34s no value in [%.10g, %.10g]" % (i, lo, hi))

    if args.per_instance:
        print("\n%-34s %s" % ("instance", "".join("%12s" % u[:11] for u in rules)))
        for i in full:
            cells = []
            for u in rules:
                r = by[(i, u)]
                cells.append("%12s" % (r["nodes"] if r["solved"] else "t/o"))
            print("%-34s %s" % (i[:34], "".join(cells)))


def status(args):
    """What is left to do, and roughly how long it will take.

    The progress line of a running sweep extrapolates from the average so far,
    which is badly pessimistic early on: the re-runs are scheduled first and
    they are precisely the runs already known to exhaust the limit. This
    weights each pending run by what is known about it instead, and works on a
    csv that is still being appended to.
    """
    files = sorted(f for f in os.listdir(args.dir) if f.endswith(".bch"))
    rules = [r.partition("=")[0] for r in args.rules]
    done = load_done(args.csv)
    sets = read_manifest(args.dir)

    at_limit, redo, new, kept = [], 0, 0, 0
    for f in files:
        for u in rules:
            p = done.get((f, u, args.relax, args.ub))
            if p is None:
                new += 1
            elif retryable(p):
                redo += 1
            elif p["status"] in TERMINAL:
                kept += 1
            elif row_limit(p) >= args.timeout:
                at_limit.append(p)
            else:
                redo += 1

    total = len(files) * len(rules)
    finished = kept + len(at_limit)
    print("%d/%d runs settled at a limit of %gs (%.0f%%)"
          % (finished, total, args.timeout, 100.0 * finished / max(total, 1)))
    print("  %d finished, %d ran out at the full limit" % (kept, len(at_limit)))
    print("  %d to re-run (ran out under a smaller limit), %d never run"
          % (redo, new))

    # A re-run is a known hard case, so it very likely burns the whole limit.
    # For a run never attempted there is no direct evidence: the runs already
    # done at the full limit are all re-runs, so their timeout rate is 1 by
    # construction and useless as a prior. The rate observed across *every*
    # settled run is an upper bound instead -- some of those that ran out under
    # a smaller limit would finish under this one -- so the estimate is a range.
    settled = [r for r in done.values()
               if row_relax(r) == args.relax and row_ub(r) == args.ub]
    rate = ((sum(1 for r in settled if r["status"] not in TERMINAL) / len(settled))
            if settled else 0.4)
    cheap = 20.0
    lo = redo * (0.9 * args.timeout + 0.1 * cheap) + new * (rate * args.timeout
                                                            + (1 - rate) * cheap)
    hi = (redo + new) * args.timeout
    print("  estimated %.0f-%.0f h of wall clock left at %d jobs "
          "(%.0f%% of runs so far did not finish)"
          % (lo / max(args.jobs, 1) / 3600.0, hi / max(args.jobs, 1) / 3600.0,
             args.jobs, 100 * rate))

    if args.by_set:
        per = {}
        for f in files:
            c = sets.get(f, "?")
            for u in rules:
                p = done.get((f, u, args.relax, args.ub))
                a, b = per.get(c, (0, 0))
                per[c] = (a + 1, b + (1 if (p is not None and
                          (p["status"] in TERMINAL or row_limit(p) >= args.timeout)) else 0))
        print("\nsettled, by source set")
        for c in sorted(per, key=lambda k: -per[k][0]):
            n, d = per[c]
            print("  %-28s %4d/%-4d  %3.0f%%" % (c, d, n, 100.0 * d / n))


def soundness(args):
    """Cross-check one relaxation against another for unsound contraction.

    A relaxation that cuts away part of the feasible set does not merely give a
    worse answer -- it gives a *wrong* one, and it looks like a win: the search
    ends early, with few nodes, reporting the instance refuted. Nothing inside a
    single run detects that, so the check is a comparison: whatever one
    relaxation proves about an instance, the other must not contradict.

    Two contradictions are reported:
      lost      one side encloses the optimum, the other calls the instance
                infeasible or never finds a feasible point;
      disjoint  both enclose it, but the enclosures do not intersect.
    """
    by = load_done(args.csv)
    rows = list(by.values())
    for r in rows:
        r["loup"] = float(r["loup"])
        r["uplo"] = float(r["uplo"])

    def get(i, u, rel):
        return by.get((i, u, rel, args.ub))

    instances = sorted({r["instance"] for r in rows})
    rules = sorted({r["rule"] for r in rows})

    lost, disjoint, checked = [], [], 0
    for i in instances:
        for u in rules:
            a, b = get(i, u, args.relax), get(i, u, args.against)
            if a is None or b is None:
                continue
            for r in (a, b):
                r["loup"] = float(r["loup"]); r["uplo"] = float(r["uplo"])
            checked += 1
            sa, sb = solved(a), solved(b)
            if sb and not sa and a["status"] in ("infeasible", "no_feasible_found",
                                                 "complete"):
                lost.append((i, u, a["status"], b["uplo"], b["loup"]))
            elif sa and sb:
                lo, hi = max(a["uplo"], b["uplo"]), min(a["loup"], b["loup"])
                if lo > hi + args.tol * max(1.0, abs(hi)):
                    disjoint.append((i, u, a["uplo"], a["loup"], b["uplo"], b["loup"]))

    print("cross-checked %d runs: %s against %s\n" % (checked, args.relax, args.against))
    print("%s refutes an instance %s solves: %d" % (args.relax, args.against, len(lost)))
    for i, u, st, lo, hi in lost[:20]:
        print("   %-26s %-14s %-18s but %s finds [%.8g, %.8g]"
              % (i, u, st, args.against, lo, hi))
    print("\nenclosures disjoint: %d" % len(disjoint))
    for i, u, au, al, bu, bl in disjoint[:20]:
        print("   %-26s %-14s %s [%.8g, %.8g] vs %s [%.8g, %.8g]"
              % (i, u, args.relax, au, al, args.against, bu, bl))

    bad = sorted({i for i, *_ in lost} | {i for i, *_ in disjoint})
    if bad:
        print("\n%d instance(s) to quarantine from any comparison:" % len(bad))
        print("   " + " ".join(bad))
        if args.write:
            open(args.write, "w").write("\n".join(bad) + "\n")
            print("   -> %s" % args.write)
    else:
        print("\nno contradiction found")


def oracle(args):
    """The virtual best: what a perfect per-instance choice of rule would give.

    This is the ceiling for any selection policy, learned or otherwise, and the
    number worth quoting when arguing that the choice is worth learning at all.
    It is a *per-instance* oracle: it commits to one rule for a whole search.
    A policy that chooses per node is not bounded by it and could do better --
    but it also has to earn its per-node cost, which this does not.
    """
    by = load_done(args.csv)
    rules = args.rules
    inst = sorted({i for (i, u, rel, ub) in by
                   if rel == args.relax and ub == args.ub})
    if args.only and os.path.exists(args.only):
        keep = {l.strip() for l in open(args.only) if l.strip()}
        inst = [i for i in inst if i in keep]

    def nodes(i, u):
        r = by.get((i, u, args.relax, args.ub))
        return int(float(r["nodes"])) if r and solved(r) else None

    solv = {u: sum(1 for i in inst if nodes(i, u) is not None) for u in rules}
    vb = [i for i in inst if any(nodes(i, u) is not None for u in rules)]
    base = [i for i in inst if nodes(i, args.baseline) is not None]

    ratios, best, uniq = [], {}, {}
    for i in base:
        vals = {u: nodes(i, u) for u in rules if nodes(i, u) is not None}
        mn = min(vals.values())
        ratios.append(max(mn, 1) / max(nodes(i, args.baseline), 1))
        winners = [u for u, v in vals.items() if v == mn]
        for u in winners:
            best[u] = best.get(u, 0) + 1
        if len(winners) == 1:
            uniq[winners[0]] = uniq.get(winners[0], 0) + 1

    print("relaxation %s, upper bounding %s, %d instances, baseline %s\n"
          % (args.relax, args.ub, len(inst), args.baseline))
    print("%-16s %8s" % ("rule", "solved"))
    print("-" * 25)
    for u in sorted(rules, key=lambda u: -solv[u]):
        print("%-16s %8d" % (u, solv[u]))
    print("%-16s %8d   (+%d over %s)"
          % ("ORACLE", len(vb), len(vb) - solv[args.baseline], args.baseline))

    print("\nnodes, over the %d instances %s closes" % (len(base), args.baseline))
    print("  oracle / %s : %.3f   (%.0f%% of its nodes)"
          % (args.baseline, geomean(ratios), 100 * geomean(ratios)))

    print("\nwho the oracle picks")
    print("%-16s %10s %12s" % ("rule", "is best", "uniquely best"))
    print("-" * 40)
    for u in sorted(rules, key=lambda u: -uniq.get(u, 0)):
        print("%-16s %10d %12d" % (u, best.get(u, 0), uniq.get(u, 0)))
    print("\n  'uniquely best' is the one that matters: it counts the instances that")
    print("  would be lost if that rule were dropped from the portfolio.")


def main():
    p = argparse.ArgumentParser(description=__doc__,
                                formatter_class=argparse.RawDescriptionHelpFormatter)
    sub = p.add_subparsers(dest="cmd", required=True)

    r = sub.add_parser("run", help="run the sweep")
    r.add_argument("-o", "--output", required=True, help="results csv (appended)")
    r.add_argument("--dir", default="benchs/optim/all", help="directory of .bch files")
    r.add_argument("--rules", nargs="+",
                   default=["lsmear", "smearsumrel", "smearsum", "smearmax",
                            "largestfirst", "roundrobin"],
                   help="bisector names, or label=model.file for a learned rule")
    r.add_argument("--timeout", type=float, default=600.0,
                   help="per run, in CPU seconds (default: 600). A run already "
                        "finished, or already given at least this much, is kept; "
                        "one that ran out under a smaller limit is redone")
    r.add_argument("--relax", default="xtaylor", choices=RELAXATIONS,
                   help="linear relaxation the contractor uses (default: xtaylor, "
                        "what ibexopt uses). It is an independent axis from the "
                        "bisector and is recorded per run, so one results file can "
                        "hold both without the rows becoming ambiguous")
    r.add_argument("--ub", default="default", choices=LOUP_FINDERS,
                   help="upper bounding (default: default, what ibexopt uses). "
                        "The ipopt* ones need a build configured with "
                        "-DIBEX_WITH_IPOPT=ON. A better incumbent found earlier "
                        "prunes more, so this changes the node counts of every "
                        "rule: it is a third axis, recorded per run like --relax")
    r.add_argument("--max-retries", type=int, default=2,
                   help="how many times a run that produces no answer is retried "
                        "before the sweep gives up on it (default: 2). Without a "
                        "cap, a reproducible failure is retried on every resume")
    r.add_argument("--wall-factor", type=float, default=1.5,
                   help="hard wall-clock kill, as a multiple of --timeout plus "
                        "60s (default: 1.5). The solver's own limit is CPU time "
                        "checked once per node, so a node whose contraction does "
                        "not return escapes it and stalls the whole sweep")
    r.add_argument("--jobs", type=int, default=DEFAULT_JOBS,
                   help="runs in parallel (default: %d). Ibex measures CPU time, "
                        "not wall clock, so parallelism does not inflate the "
                        "reported times directly -- but it does through cache and "
                        "memory-bandwidth contention, so leave the machine some "
                        "headroom" % DEFAULT_JOBS)
    r.add_argument("--random-seed", type=float, default=1.0)
    r.add_argument("--limit", type=int, default=0, help="only the first N instances")
    r.add_argument("--binary", default=DEFAULT_BINARY)
    r.set_defaults(func=run)

    t = sub.add_parser("status", help="what is left to do, on a running sweep")
    t.add_argument("csv", nargs="+")
    t.add_argument("--dir", default="benchs/optim/all")
    t.add_argument("--rules", nargs="+",
                   default=["lsmear", "lsmear-box", "smearsumrel", "smearsum",
                            "smearmax", "smearmaxrel", "largestfirst", "roundrobin"])
    t.add_argument("--timeout", type=float, default=600.0)
    t.add_argument("--relax", default="xtaylor", choices=RELAXATIONS)
    t.add_argument("--ub", default="default", choices=LOUP_FINDERS)
    t.add_argument("--jobs", type=int, default=DEFAULT_JOBS)
    t.add_argument("--by-set", action="store_true")
    t.set_defaults(func=status)

    o = sub.add_parser("oracle", help="the virtual best over a set of rules")
    o.add_argument("csv", nargs="+")
    o.add_argument("--relax", default="xtaylor", choices=RELAXATIONS)
    o.add_argument("--ub", default="default", choices=LOUP_FINDERS)
    o.add_argument("--baseline", default="lsmear")
    o.add_argument("--only", help="file of instance names to restrict to")
    o.add_argument("--rules", nargs="+",
                   default=["lsmear", "lsmear-box", "smearsumrel", "smearsum",
                            "smearmax", "smearmaxrel", "largestfirst", "roundrobin"])
    o.set_defaults(func=oracle)

    v = sub.add_parser("soundness", help="cross-check one relaxation against another")
    v.add_argument("csv", nargs="+", help="results csv(s); several are merged")
    v.add_argument("--relax", default="affine", choices=RELAXATIONS,
                   help="the relaxation under test")
    v.add_argument("--against", default="xtaylor", choices=RELAXATIONS,
                   help="the relaxation trusted as the reference")
    v.add_argument("--ub", default="default", choices=LOUP_FINDERS,
                   help="both sides of the cross-check are runs with this upper "
                        "bounding; it is held fixed, not compared")
    v.add_argument("--tol", type=float, default=1e-6)
    v.add_argument("--write", help="write the quarantine list to this file")
    v.set_defaults(func=soundness)

    q = sub.add_parser("report", help="summarize a results csv")
    q.add_argument("csv", nargs="+", help="results csv(s); several are merged, which "
                                          "is how runs under different relaxations "
                                          "written to different files are compared")
    q.add_argument("--baseline", default="lsmear")
    q.add_argument("--relax", default="xtaylor", choices=RELAXATIONS,
                   help="which relaxation's runs to report on (default: xtaylor)")
    q.add_argument("--ub", default="default", choices=LOUP_FINDERS,
                   help="which upper bounding's runs to report on (default: "
                        "default). Runs with Ipopt are a separate population: "
                        "they are not comparable with runs without it")
    q.add_argument("--exclude", help="file of instance names to leave out, one per "
                                     "line (e.g. the quarantine list `soundness` writes)")
    q.add_argument("--only", help="file of instance names to restrict to, one per line")
    q.add_argument("--timeout", type=float, default=0.0,
                   help="override the limit used for PAR2 (default: each run's own)")
    q.add_argument("--margin", type=float, default=0.05,
                   help="relative deadband for the win/loss count: a rule wins an "
                        "instance only if it uses at least this much fewer nodes "
                        "(default: 0.05). Inside the band it is a tie")
    q.add_argument("--tol", type=float, default=1e-6,
                   help="relative slack when checking that the enclosures agree")
    q.add_argument("--per-instance", action="store_true")
    q.set_defaults(func=report)

    args = p.parse_args()
    args.func(args)


if __name__ == "__main__":
    main()
