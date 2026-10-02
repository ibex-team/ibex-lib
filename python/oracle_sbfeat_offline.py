#!/usr/bin/env python3
"""Offline: predict the one-step oracle's label (dive nodes, lsmear-guard:10
continuation) from features that include what a strong-branching probe
sees -- the two children after one contraction -- and pick the argmin.

Per candidate, the probe features (2 contractions each, vs a whole dive):
  pruned children; log volume of each open child relative to the node (goal
  variable excluded; -100 if pruned), min/max/log-sum; the rise of the goal
  lower bound in each child, as a fraction of the gap [node lb, loup]
  (1 if pruned), min/max.
Each also relative to the sample (value minus the sample's best), since only
the order within a node matters.

Feature sets: A (the 30 solver features), P (probe), S (how the probe's
contraction propagated over the other variables, see spread()), and unions. Grouped 5-fold CV
by family; out-of-fold regret against the dive labels (geo per instance;
picks the best; >=2x).

    python3 python/oracle_sbfeat_offline.py
"""
import glob, json, math, os, sys
import numpy as np, pandas as pd
from sklearn.ensemble import HistGradientBoostingRegressor
from sklearn.model_selection import GroupKFold
HERE = os.path.dirname(os.path.abspath(__file__)); sys.path.insert(0, HERE)
from ibexml import encode  # noqa
DATA = os.path.join(HERE, "..", "results", os.environ.get("DATASET", "dataset-guard10"))
geo = lambda x: float(np.exp(np.mean(np.log(x))))


def box(b):
    return json.loads(b) if isinstance(b, str) else b


def logvol(bx, parent, skip):
    s = 0.0
    for j, (a, b) in enumerate(bx):
        if j == skip:
            continue
        pa, pb = parent[j]; dp = pb - pa; d = b - a
        if not (math.isfinite(dp) and dp > 0 and math.isfinite(d)):
            continue
        s += math.log(max(d, 1e-300) / dp)
    return max(s, -100.0)


def probe(l, parent, gv, lb0, top):
    st = [l["left_status"], l["right_status"]]
    ch = [box(l["left"]), box(l["right"])]
    lv, rise = [], []
    for k in range(2):
        if st[k] != "open" or not ch[k]:
            lv.append(-100.0); rise.append(1.0); continue
        lv.append(logvol(ch[k], parent, gv))
        if gv >= 0 and math.isfinite(lb0) and math.isfinite(top) and top > lb0:
            rise.append(min(1.0, max(0.0, (ch[k][gv][0] - lb0) / (top - lb0))))
        else:
            rise.append(0.0)
    npr = sum(x != "open" for x in st)
    return [npr, min(lv), max(lv), float(np.logaddexp(lv[0], lv[1])), min(rise), max(rise)]


def spread(l, parent, gv, var):
    """How a probe's contraction propagated: per child, over the variables
    other than the bisected one and the goal, the fraction contracted by more
    than 1%, 10%, 50%, the strongest contraction (min log ratio) and the mean
    log ratio; and the bisected variable beyond the half (log of its width over
    half the parent's). A pruned child counts as total contraction. Then
    min/max over the two children."""
    st = [l["left_status"], l["right_status"]]
    ch = [box(l["left"]), box(l["right"])]
    per = []
    for k in range(2):
        if st[k] != "open" or not ch[k]:
            per.append([1.0, 1.0, 1.0, -30.0, -30.0, -30.0]); continue
        r = []
        for j, (a, b) in enumerate(ch[k]):
            if j == gv or j == var:
                continue
            pa, pb = parent[j]; dp = pb - pa; d = b - a
            if not (math.isfinite(dp) and dp > 0 and math.isfinite(d)):
                continue
            r.append(max(-30.0, math.log(max(d, 1e-300) / dp)))
        r = np.array(r) if r else np.zeros(1)
        pa, pb = parent[var]; a, b = ch[k][var]
        own = max(-30.0, math.log(max(b - a, 1e-300) / ((pb - pa) / 2))) if math.isfinite(pb - pa) and pb > pa else 0.0
        per.append([(r < math.log(0.99)).mean(), (r < math.log(0.9)).mean(), (r < math.log(0.5)).mean(),
                    r.min(), r.mean(), own])
    per = np.array(per)
    return list(per.min(axis=0)) + list(per.max(axis=0))


def probe2(c, parent, gv, lb0, top):
    """Two-level probe (python/probe2_offline.py): pruned grandchildren (of 4),
    log total volume of the open ones (-100 if none), and the rise of the goal
    lower bound over the open ones (min, max; 1 if all pruned)."""
    lv, rise, npr = [], [], 0
    for g in c["grand"]:
        if g["status"] != "open" or not g["box"]:
            npr += 1; continue
        lv.append(logvol(g["box"], parent, gv))
        if gv >= 0 and math.isfinite(lb0) and math.isfinite(top) and top > lb0:
            rise.append(min(1.0, max(0.0, (g["box"][gv][0] - lb0) / (top - lb0))))
        else:
            rise.append(0.0)
    vol = float(np.logaddexp.reduce(lv)) if lv else -100.0
    return [npr, vol, min(rise) if rise else 1.0, max(rise) if rise else 1.0]


def load():
    fam = {l.split()[0][:-4]: l.split()[2] for l in open(os.path.join(DATA, "instances.txt"))}
    XA, XP, XS, X2, Y, rows = [], [], [], [], [], []
    P2DIR = os.path.join(DATA, "probe2")
    sid = 0
    for f in sorted(glob.glob(os.path.join(DATA, "samples", "*.jsonl"))):
        inst = os.path.basename(f)[:-6]
        p2f = os.path.join(P2DIR, inst + ".jsonl")
        p2 = {json.loads(l)["sample"]: json.loads(l)["cands"] for l in open(p2f)} if os.path.exists(p2f) else None
        if os.path.isdir(P2DIR) and p2 is None:
            continue   # compare on the same samples
        for ls, line in enumerate(open(f)):
            s = json.loads(line)
            L = [l for l in s["labels"] if l["valid"]]
            done = [l["nodes"] for l in L if not l["censored"]]
            if len(L) < 2 or not done:
                continue
            best = min(done); n = s["node"]
            Xv, _, _ = encode(n)
            parent = [(v["lb"], v["ub"]) for v in n["vars"]]
            gv = next((j for j, v in enumerate(n["vars"]) if v.get("is_goal")), -1)
            lb0 = parent[gv][0] if gv >= 0 else float("nan")
            top = n.get("ymax", n.get("loup"))
            top = top if top is not None and math.isfinite(top) else (parent[gv][1] if gv >= 0 else float("nan"))
            P = np.array([probe(l, parent, gv, lb0, top) for l in L])
            # relative to the sample's best (pruned, rise: max is best; volume: min is best)
            rel = np.stack([P[:, 0] - P[:, 0].max(), P[:, 1] - P[:, 1].min(), P[:, 2] - P[:, 2].min(),
                            P[:, 3] - P[:, 3].min(), P[:, 4] - P[:, 4].max(), P[:, 5] - P[:, 5].max()], axis=1)
            S = np.array([spread(l, parent, gv, l["var"]) for l in L])
            if p2 is not None:
                cm = {c["var"]: c for c in p2[ls]}
                Q = np.array([probe2(cm[l["var"]], parent, gv, lb0, top) for l in L])
            else:
                Q = np.zeros((len(L), 4))
            Qrel = np.stack([Q[:, 0] - Q[:, 0].max(), Q[:, 1] - Q[:, 1].min(),
                             Q[:, 2] - Q[:, 2].max(), Q[:, 3] - Q[:, 3].max()], axis=1)
            Srel = S - S.min(axis=0)
            for k, l in enumerate(L):
                XS.append(np.concatenate([S[k], Srel[k]]))
                X2.append(np.concatenate([Q[k], Qrel[k]]))
                y = l["nodes"] if not l["censored"] else 2 * l["budget_used"]
                XA.append(Xv[l["var"]]); XP.append(np.concatenate([P[k], rel[k], [len(L)]]))
                Y.append(math.log(max(y, 1) / best))
                rows.append((sid, inst, fam.get(inst, inst), y / best, l["var"] == n.get("bisect_var"),
                             100.0 * P[k, 0] - P[k, 3], 100.0 * Q[k, 0] - Q[k, 1]))
            sid += 1
    R = pd.DataFrame(rows, columns=["sample", "instance", "family", "r", "guard", "sb", "sb2"])
    XA = np.nan_to_num(np.asarray(XA, float), nan=0.0, posinf=1e12, neginf=-1e12)
    return XA, np.asarray(XP, float), np.asarray(XS, float), np.asarray(X2, float), np.asarray(Y), R


def evaluate(R, score, name):
    """Geometric regret against the best (per instance, then geo), % best, %
    >=2x; and the ratio that matters for a rule replacing the base bisector
    everywhere: per instance, sum of the chosen candidates' dive nodes over the
    sum for the base's own choice (geo over instances), with the share of
    instances where it is > 1."""
    V = R.loc[R.assign(s=score).groupby("sample").s.idxmax()]
    per = V.groupby("instance").r.apply(geo)
    base = R[R.guard].groupby("sample").r.first()
    V = V.set_index("sample").join(base.rename("rb"), how="inner")
    ratio = V.groupby("instance").apply(lambda g: g.r.sum() / g.rb.sum())
    return name, geo(per), (V.r <= 1.0001).mean(), (V.r >= 2).mean(), geo(ratio), (ratio > 1.0001).mean()


def main():
    XA, XP, XS, X2, Y, R = load()
    print("%d filas, %d muestras, %d instancias, %d familias" % (len(R), R["sample"].nunique(), R.instance.nunique(), R.family.nunique()))
    out = [evaluate(R, -R.r.values, "oráculo (la etiqueta)"),
           evaluate(R, R.sb.values, "strong branching (regla)"),
           evaluate(R, R.sb2.values, "SB 2 niveles (regla)"),
           evaluate(R, R.guard.astype(float).values, "bisector de la trayectoria")]
    for fname, X in (("modelo A", XA), ("modelo P (sondeo)", XP), ("modelo A+P", np.hstack([XA, XP])),
                     ("modelo S (propagación)", XS), ("modelo P+S", np.hstack([XP, XS])),
                     ("modelo A+P+S", np.hstack([XA, XP, XS])),
                     ("modelo P2 (2 niveles)", X2), ("modelo P+P2", np.hstack([XP, X2])),
                     ("modelo A+P+P2", np.hstack([XA, XP, X2]))):
        pred = np.zeros(len(Y))
        for tr, te in GroupKFold(5).split(X, Y, R.family):
            m = HistGradientBoostingRegressor(max_iter=300, learning_rate=0.05, max_leaf_nodes=31, random_state=0)
            m.fit(X[tr], Y[tr]); pred[te] = m.predict(X[te])
        out.append(evaluate(R, -pred, fname))
    print("\nfuera de fold: regret geo vs el mejor; elige el mejor; >=2x;  suma vs elección del bisector base (geo por instancia); instancias donde suma > base")
    for name, gi, eq, ge2, rb, worse in out:
        print("  %-30s %.3f  %3.0f%%  %3.0f%%    %.3f  %3.0f%%" % (name, gi, 100 * eq, 100 * ge2, rb, 100 * worse))


if __name__ == "__main__":
    main()
