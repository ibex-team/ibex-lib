#!/usr/bin/env python3
"""Offline: a conservative rule that leaves the base bisector only when a
model predicts a clear gain.

Per candidate j of a sample: t_j = log(y_j / y_base), the dive nodes relative
to the base bisector's own choice. A model predicts t_j from the candidate's
features and their difference with the base's; the rule takes the argmin and
deviates only if its prediction is below log(thr). Evaluated, out of fold
(grouped by family), by the per-instance sum of the chosen candidates' nodes
over the base's (geo over instances), the share of instances above 1, and how
often it deviates.

    DATASET=dataset-lffix python3 python/deviate_offline.py
"""
import math, os, sys
import numpy as np, pandas as pd
from sklearn.ensemble import HistGradientBoostingRegressor
from sklearn.model_selection import GroupKFold
HERE = os.path.dirname(os.path.abspath(__file__)); sys.path.insert(0, HERE)
from oracle_sbfeat_offline import load, geo  # noqa


def score(R, choice, name):
    """choice: row index chosen per sample (base when not deviating)."""
    V = R.loc[choice]
    B = R[R.guard].set_index("sample")
    V = V.set_index("sample").join(B[["y"]].rename(columns={"y": "yb"}), how="inner")
    ratio = V.groupby("instance").apply(lambda g: g.y.sum() / g.yb.sum())
    dev = (V.index.map(lambda s: True) & (V.y != V.yb)).mean()
    return "  %-34s suma/base %.3f   instancias >1: %3.0f%%   peor en >=1.5x: %3.0f%%   desvía %3.0f%%" % (
        name, geo(ratio), 100 * (ratio > 1.0001).mean(), 100 * (ratio > 1.5).mean(), 100 * dev)


def main():
    XA, XP, XS, X2, Y, R = load()
    R = R.reset_index(drop=True)
    R["y"] = R.r  # nodes / best; ratios to the base are what matter
    has = R.groupby("sample").guard.transform("any")
    keep = has.values
    R, XA, XP, Y = R[keep].reset_index(drop=True), XA[keep], XP[keep], Y[keep]
    base_idx = R[R.guard].groupby("sample").apply(lambda g: g.index[0])
    bi = R["sample"].map(base_idx).values
    T = Y - Y[bi]
    print("%d muestras con la elección base entre los candidatos" % R["sample"].nunique())
    out = [score(R, base_idx.values, "base (lsmear-lffix)"),
           score(R, R.groupby("sample").r.idxmin().values, "oráculo")]
    for thr in (0.5, 0.7):
        tt = R.assign(t=T)
        best = tt.groupby("sample").t.idxmin()
        ch = [b if tt.t[b] < math.log(thr) else base_idx[s] for s, b in best.items()]
        out.append(score(R, ch, "oráculo conservador thr=%.1f" % thr))
    for fname, X in (("A", XA), ("A+P (con sondeo)", np.hstack([XA, XP]))):
        Xd = np.hstack([X, X - X[bi]])
        pred = np.zeros(len(T))
        for tr, te in GroupKFold(5).split(Xd, T, R.family):
            m = HistGradientBoostingRegressor(max_iter=300, learning_rate=0.05, max_leaf_nodes=31, random_state=0)
            m.fit(Xd[tr], T[tr]); pred[te] = m.predict(Xd[te])
        pp = R.assign(p=pred)
        pp.loc[pp.guard, "p"] = 0.0
        best = pp.groupby("sample").p.idxmin()
        for thr in (0.9, 0.7, 0.5, 0.3):
            ch = [b if pp.p[b] < math.log(thr) else base_idx[s] for s, b in best.items()]
            out.append(score(R, ch, "modelo %s thr=%.1f" % (fname, thr)))
    print("\n".join(out))


if __name__ == "__main__":
    main()
