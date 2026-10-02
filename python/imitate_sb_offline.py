#!/usr/bin/env python3
"""Offline: can a model predict the strong-branching score from features that
are cheap at the node, and does its choice keep strong branching's quality?

Target per candidate j of a sample: its strong-branching key, pruned children
first, then the (log) total volume of the open ones -- as one number,
q_j = 100*pruned_j - clip(logvol_j, -100, 0). The model regresses q_j and picks
the argmax. Two feature sets:
  A     the 30 per-variable features the solver computes (ibexml.MODEL_FEATURES)
  A+C   A plus aggregates over the constraints a variable appears in: how much
        of each constraint's range its interval accounts for
        (mag(J_ij) * diam_j / width(f_i)), max and sum over the non-entailed
        constraints, how many it is in, and how many of those are equalities.
Grouped 5-fold CV by family; reports, out of fold, how often the model picks
strong branching's choice and the regret of its choice against the dive
labels, next to strong branching itself and lsmear-guard:10.

    python3 python/imitate_sb_offline.py
"""
import glob, json, math, os, sys
import numpy as np, pandas as pd
from sklearn.ensemble import HistGradientBoostingRegressor
from sklearn.model_selection import GroupKFold
HERE = os.path.dirname(os.path.abspath(__file__)); sys.path.insert(0, HERE)
from ibexml import encode  # noqa
DATA = os.path.join(HERE, "..", "results", os.environ.get("DATASET", "dataset-guard10"))
geo = lambda x: float(np.exp(np.mean(np.log(x))))


def logvol(box, parent, skip):
    s = 0.0
    for j, (a, b) in enumerate(box):
        if j == skip:
            continue
        pa, pb = parent[j]; dp = pb - pa; d = b - a
        if not (math.isfinite(dp) and dp > 0 and math.isfinite(d)):
            continue
        s += math.log(max(d, 1e-300) / dp)
    return s


def ctr_feats(Xv, Xc, E):
    n = Xv.shape[0]
    if Xc.shape[0] == 0:
        return np.zeros((n, 5))
    ent = Xc[:, 1] > 0.5
    w = np.abs(Xc[:, 3] - Xc[:, 2])
    w = np.where(np.isfinite(w) & (w > 0), w, np.nan)
    magJ = np.maximum(np.abs(E[:, :, 0]), np.abs(E[:, :, 1]))       # (m, n)
    diam = Xv[:, 2]
    imp = magJ * np.where(np.isfinite(diam), diam, np.nan)[None, :] / w[:, None]
    imp[ent, :] = np.nan
    has = magJ > 0
    eq = Xc[:, 5 + 2] > 0.5 if Xc.shape[1] >= 8 else np.zeros(len(Xc), bool)
    with np.errstate(all="ignore"):
        f = np.stack([np.nanmax(np.where(np.isfinite(imp), imp, np.nan), axis=0),
                      np.nansum(np.where(np.isfinite(imp), imp, 0), axis=0),
                      has.sum(axis=0), (has & eq[:, None]).sum(axis=0),
                      (has & ~ent[:, None]).sum(axis=0)], axis=1)
    f = np.nan_to_num(f, nan=0.0, posinf=1e12, neginf=-1e12)
    return np.log1p(np.clip(f, 0, 1e12))


def load():
    fam = {l.split()[0][:-4]: l.split()[2] for l in open(os.path.join(DATA, "instances.txt"))}
    XA, XC, Q, rows = [], [], [], []
    sid = 0
    for f in sorted(glob.glob(os.path.join(DATA, "samples", "*.jsonl"))):
        inst = os.path.basename(f)[:-6]
        for line in open(f):
            s = json.loads(line)
            L = [l for l in s["labels"] if l["valid"]]
            done = [l["nodes"] for l in L if not l["censored"]]
            if len(L) < 2 or not done:
                continue
            best = min(done); n = s["node"]
            Xv, Xc, E = encode(n)
            C = ctr_feats(Xv, Xc, E)
            parent = [(v["lb"], v["ub"]) for v in n["vars"]]
            gv = next((j for j, v in enumerate(n["vars"]) if v.get("is_goal")), -1)
            for l in L:
                st = [l["left_status"], l["right_status"]]; ch = [l["left"], l["right"]]
                npr = sum(x != "open" for x in st)
                lv = [logvol(b, parent, gv) for k, b in enumerate(ch) if st[k] == "open" and b]
                vol = float(np.logaddexp.reduce(lv)) if lv else -100.0
                y = l["nodes"] if not l["censored"] else 2 * l["budget_used"]
                XA.append(Xv[l["var"]]); XC.append(C[l["var"]])
                Q.append(100.0 * npr - max(-100.0, min(0.0, vol)))
                rows.append((sid, inst, fam.get(inst, inst), y / best, l["var"] == n.get("bisect_var")))
            sid += 1
    R = pd.DataFrame(rows, columns=["sample", "instance", "family", "r", "guard"])
    XA = np.nan_to_num(np.asarray(XA, float), nan=0.0, posinf=1e12, neginf=-1e12)
    return XA, np.asarray(XC, float), np.asarray(Q, float), R


def evaluate(R, score, name):
    D = R.assign(s=score)
    idx = D.groupby("sample").s.idxmax()
    V = D.loc[idx]
    per = V.groupby("instance").r.apply(geo)
    return name, geo(per), (V.r <= 1.0001).mean(), (V.r >= 2).mean()


def main():
    XA, XC, Q, R = load()
    print("%d filas, %d muestras, %d instancias, %d familias" % (len(R), R["sample"].nunique(), R.instance.nunique(), R.family.nunique()))
    out = [evaluate(R, Q, "strong branching (la etiqueta)"),
           evaluate(R, R.guard.astype(float), "lsmear-guard:10")]
    sbpick = R.assign(q=Q).groupby("sample").q.transform("max") == Q
    for fname, X in (("modelo, features A", XA), ("modelo, features A+C", np.hstack([XA, XC]))):
        pred = np.zeros(len(Q))
        for tr, te in GroupKFold(5).split(X, Q, R.family):
            m = HistGradientBoostingRegressor(max_iter=300, learning_rate=0.05, max_leaf_nodes=31, random_state=0)
            m.fit(X[tr], Q[tr]); pred[te] = m.predict(X[te])
        D = R.assign(p=pred, sb=sbpick)
        agree = D.loc[D.groupby("sample").p.idxmax()].sb.mean()
        name, gi, eq, ge2 = evaluate(R, pred, fname)
        out.append((name + " (elige lo de SB en %.0f%%)" % (100 * agree), gi, eq, ge2))
    print("\nfuera de fold, regret contra las etiquetas de dive (geo por instancia; elige el mejor; >=2x):")
    for name, gi, eq, ge2 in out:
        print("  %-52s %.3f  %3.0f%%  %3.0f%%" % (name, gi, 100 * eq, 100 * ge2))


if __name__ == "__main__":
    main()
