#!/usr/bin/env python3
"""lsmear-guard:10 against lsmear under compo + ipoptxn, run on one machine.

results/ipopt-compo-guard10.csv: both arms on the same machine and binary (the
LP fixes included), 310 CPU seconds -- 600 on the machine ipopt-compo.csv ran
on, at the measured factor of 0.52. Reports instances solved, PAR2, the
pairwise score t(A)/max(t(A),t(B)) (2 for a timeout, 1 s floor, the instances
neither closes left out) with a bootstrap interval, and the instances that
change between solved and not. Then puts it next to the exact evaluation of
H=10 on ipopt-compo.csv (guard_horizon_ipopt.py), which it was meant to check.

    python3 python/compare_guard10_ipopt.py
"""
import os
import sys

import numpy as np
import pandas as pd

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HERE)
from experiment_bisectors import solved   # noqa: E402

L = 310.0
A, B = "lsmear-guard:10", "lsmear"


def main():
    path = sys.argv[1] if len(sys.argv) > 1 else os.path.join(HERE, "..", "results", "ipopt-compo-guard10.csv")
    d = pd.read_csv(path).drop_duplicates(["instance", "rule"], keep="last")
    d["ok"] = [solved(r) for r in d.to_dict("records")]
    T = d.pivot(index="instance", columns="rule", values="time").dropna()
    S = d.pivot(index="instance", columns="rule", values="ok").loc[T.index].astype(bool)
    N = d.pivot(index="instance", columns="rule", values="nodes").loc[T.index]
    sets = d.drop_duplicates("instance").set_index("instance")["set"].loc[T.index]
    coc = (sets == "coconutbenchmark-library2").values
    P = pd.DataFrame({r: np.where(S[r], T[r], 2 * L) for r in (A, B)}, index=T.index)

    def score(a, b):
        ta = T[a].clip(lower=1.0)
        tb = np.where(S[b], T[b].clip(lower=1.0), L)
        return pd.Series(np.where(S[a], ta / np.maximum(ta, tb), 2.0), index=T.index)

    print("%d instancias (%d coconut), %s vs %s, %d s\n" % (len(T), coc.sum(), A, B, L))
    for r in (A, B):
        print("%-16s resueltas %3d (coconut %2d, resto %3d)  PAR2 medio %.1f"
              % (r, S[r].sum(), S[r][coc].sum(), S[r][~coc].sum(), P[r].mean()))
    g, l = P[A], P[B]
    print("\ncara a cara (>1,5x y >5 s): %s gana %d, pierde %d; mismos nodos en %d"
          % (A, ((g < l / 1.5) & (l - g > 5)).sum(), ((g > l * 1.5) & (g - l > 5)).sum(),
             (N[A] == N[B]).sum()))
    x, y = score(A, B), score(B, A)
    m = ~((x == 2) & (y == 2))
    x, y = x[m].values, y[m].values
    rs = np.random.RandomState(0)
    D = [y[i].mean() - x[i].mean() for i in (rs.randint(0, len(x), len(x)) for _ in range(4000))]
    print("t(A)/max(t(A),t(B)) sin las que ninguna resuelve (%d): %s %.3f, %s %.3f, diferencia %.3f IC95 [%.3f, %.3f]"
          % (m.sum(), A, x.mean(), B, y.mean(), y.mean() - x.mean(), *np.percentile(D, [2.5, 97.5])))
    ch = S[A] != S[B]
    print("\ncambian de resuelta a no resuelta (%d):" % ch.sum())
    print(pd.DataFrame({B: P[B], A: P[A], "nodos " + B: N[B], "nodos " + A: N[A]})[ch].round(1).to_string())


if __name__ == "__main__":
    main()
