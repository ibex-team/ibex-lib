#!/usr/bin/env python3
"""lsmear-guard against the rules of results/all-results.csv (xtaylor), and
the exact evaluation of a detection horizon.

The sweep behind bisectors-guard.csv ran on a different machine from
all-results.csv. Where the guard never switched its nodes are lsmear's, so the
median time ratio over those runs is the machine factor; guard times are
rescaled by it and the 600 s limit is applied on the original machine's scale.

Horizon H: "switch only if the hijack shows within the first H decisions".
Before its switch the guard *is* lsmear, node for node, so that variant is
known exactly without running it: the guard's row when it switched at a
decision <= H, lsmear's row otherwise. The switch points come from
`ibexopt-ml --solve --bisector lsmear-guard --max-nodes N` (guard_switched_at).

    python3 python/compare_guard.py results/bisectors-guard.csv switch.txt
"""
import json
import re
import sys

import numpy as np
import pandas as pd

LIMIT = 600.0
QUARANTINE = {"polak1.bch", "wall.bch"}


def load(all_results, guard_csv):
    a = pd.read_csv(all_results)
    a = a[(a.relax == "xtaylor") & ~a.instance.isin(QUARANTINE)]
    amb = set(a[(a.solved == 0) & (a.timeout < LIMIT)].instance)
    a = a[~a.instance.isin(amb)]
    g = pd.read_csv(guard_csv).drop_duplicates("instance", keep="last").set_index("instance")
    g["solved"] = (g.status == "complete").astype(int)
    l = a[a.rule == "lsmear"].set_index("instance")
    i = g.index.intersection(l.index)
    same = (g.loc[i].nodes == l.loc[i].nodes) & (g.loc[i].solved == 1) & (l.loc[i].solved == 1) & (l.loc[i].time > 1)
    k = float((g.loc[i].time / l.loc[i].time)[same].median())
    g["time"] = g.time / k
    g.loc[g.time >= LIMIT, "solved"] = 0
    a["par2"] = np.where(a.solved == 1, a.time.clip(lower=1e-3), 2 * LIMIT)
    C = a.pivot_table(index="instance", columns="rule", values="par2", aggfunc="first")
    sets = a.drop_duplicates("instance").set_index("instance")["set"]
    idx = C.index.intersection(g.index)
    C = C.loc[idx]
    C["lsmear-guard"] = np.where(g.loc[idx].solved == 1, g.loc[idx].time.clip(lower=1e-3), 2 * LIMIT)
    return C, (sets.loc[idx] == "coconutbenchmark-library2").values, k, int(same.sum())


def switches(path):
    """instance -> decision of the switch; inf if none within what was run."""
    s = {}
    for line in open(path):
        name, _, js = line.partition(" ")
        m = re.search(r'"guard_switched_at":(-?\d+)', js)
        if m:
            v = int(m.group(1))
            s[name] = v if v >= 0 else np.inf
    return pd.Series(s)


def row(C, coc, c):
    ls, orc = C.lsmear, C.drop(columns=[x for x in C.columns if x.startswith("lsmear-guard")]).min(axis=1)
    gap = lambda m: 100 * (ls[m].sum() - c[m].sum()) / (ls[m].sum() - orc[m].sum())
    al = np.ones(len(c), bool)
    win = ((c < ls / 1.5) & ((ls - c) > 5)).sum()
    lose = ((c > ls * 1.5) & ((c - ls) > 5)).sum()
    return "PAR2 %7.0f  res %3d  brecha %5.1f%% (coconut %5.1f%%, resto %5.1f%%)  vs lsmear +%d -%d" % (
        c.sum(), (c < 2 * LIMIT).sum(), gap(al), gap(coc), gap(~coc), win, lose)


def main():
    guard_csv = sys.argv[1] if len(sys.argv) > 1 else "results/bisectors-guard.csv"
    C, coc, k, n = load("results/all-results.csv", guard_csv)
    print("%d instancias (%d coconut); factor de máquina %.3f (mediana de %d corridas idénticas)\n"
          % (len(C), coc.sum(), k, n))
    for r in ("lsmear", "roundrobin"):
        print("%-18s %s" % (r, row(C, coc, C[r])))
    print("%-18s %s" % ("lsmear-guard", row(C, coc, C["lsmear-guard"])))
    if len(sys.argv) > 2:
        sw = switches(sys.argv[2]).reindex(C.index).fillna(np.inf)
        print("\nhorizonte H: cambia solo si el secuestro aparece en las primeras H decisiones")
        for H in (10, 15, 20, 30, 50, 100, 200, 500, 1000, 5000):
            c = C["lsmear-guard"].where(sw <= H, C.lsmear)
            print("  H=%-5d %s   (cambian %d)" % (H, row(C, coc, c), (sw <= H).sum()))


if __name__ == "__main__":
    main()
