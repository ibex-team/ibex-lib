#!/usr/bin/env python3
"""lsmear-guard:10 under compo + ipoptxn, evaluated from results/ipopt-compo.csv.

H=10 was chosen on the xtaylor sweep without Ipopt, so this is the first look
at it on data it was not chosen on. Before its switch the guard is lsmear, node
for node, so the H=10 variant is the lsmear-guard row where the guard switched
within its first 10 decisions and the lsmear row otherwise. The switch points
come from --solve --bisector lsmear-guard --relax both --loup ipoptxn
--max-nodes 2000 (guard_switched_at), run on this machine.

The check that the switch points transfer: where the guard did not switch here,
the sweep's lsmear-guard row should have lsmear's node count.

    python3 python/guard_horizon_ipopt.py results/ipopt-compo.csv results/guard-switch-ipopt.txt
"""
import math
import os
import re
import sys

import numpy as np
import pandas as pd

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from experiment_bisectors import solved   # noqa: E402

LIMIT = 600.0


def main():
    csv = sys.argv[1] if len(sys.argv) > 1 else "results/ipopt-compo.csv"
    sw_path = sys.argv[2] if len(sys.argv) > 2 else "results/guard-switch-ipopt.txt"
    d = pd.read_csv(csv)
    d = d[d.rule.isin(["lsmear", "lsmear-guard", "roundrobin"])].drop_duplicates(["instance", "rule"], keep="last")
    d["ok"] = [solved(r) for r in d.to_dict("records")]
    d["par2"] = np.where(d.ok, d.time, 2 * LIMIT)
    P = d.pivot(index="instance", columns="rule", values="par2")
    S = d.pivot(index="instance", columns="rule", values="ok").astype(bool)
    N = d.pivot(index="instance", columns="rule", values="nodes")
    sets = d.drop_duplicates("instance").set_index("instance")["set"]

    sw = {}
    for line in open(sw_path):
        name, _, js = line.partition(" ")
        m = re.search(r'"guard_switched_at":(-?\d+)', js)
        if m:
            v = int(m.group(1))
            sw[name] = v if v >= 0 else math.inf
    sw = pd.Series(sw).reindex(P.index)
    known = sw.notna()
    print("%d instancias; punto de cambio medido en %d" % (len(P), known.sum()))

    # does the switch point transfer to the machine the sweep ran on?
    never = known & (sw == math.inf) & S.lsmear & S["lsmear-guard"]
    agree = (N.loc[never, "lsmear-guard"] == N.loc[never, "lsmear"])
    print("no cambia aquí y ambas cierran allá: %d; con los mismos nodos que lsmear allá: %d"
          % (never.sum(), agree.sum()))
    early = known & (sw <= 10) & S.lsmear & S["lsmear-guard"]
    diff = (N.loc[early, "lsmear-guard"] != N.loc[early, "lsmear"])
    print("cambia en <= 10 aquí y ambas cierran allá: %d; con nodos distintos de lsmear allá: %d\n"
          % (early.sum(), diff.sum()))

    h10 = (known & (sw <= 10))
    P["lsmear-guard:10"] = P["lsmear-guard"].where(h10, P.lsmear)
    S["lsmear-guard:10"] = S["lsmear-guard"].where(h10, S.lsmear)
    coc = (sets.reindex(P.index) == "coconutbenchmark-library2").values
    ls = P.lsmear
    print("%-16s %9s %9s %5s   vs lsmear (>1,5x y >5 s)" % ("", "PAR2 med", "PAR2 tot", "res"))
    for r in ("lsmear", "roundrobin", "lsmear-guard", "lsmear-guard:10"):
        c = P[r]
        win = ((c < ls / 1.5) & ((ls - c) > 5)).sum()
        lose = ((c > ls * 1.5) & ((c - ls) > 5)).sum()
        print("%-16s %9.1f %9.0f %5d   +%d -%d   (coconut %d res)"
              % (r, c.mean(), c.sum(), S[r].sum(), win, lose, S[r][coc].sum()))
    print("\ncambian con H=10: %d; cambian más tarde (sin horizonte): %d"
          % (h10.sum(), (known & (sw > 10) & (sw < math.inf)).sum()))
    g, h = P["lsmear-guard"], P["lsmear-guard:10"]
    delta = (h - g)
    print("\nH=10 frente a sin horizonte, donde difieren:")
    t = pd.DataFrame({"lsmear": ls, "guard": g, "guard:10": h, "switch": sw})[delta.abs() > 5]
    print(t.sort_values("guard:10").round(1).to_string())


if __name__ == "__main__":
    main()
