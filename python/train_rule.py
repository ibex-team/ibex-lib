#!/usr/bin/env python3
"""Paso 4: aprender la regla por nodo del dataset de results/dataset-guard10.

Objetivo por candidato j de una muestra: t_j = log2(y_j / y_best), cuántas veces
más nodos que el mejor candidato necesita el subárbol si se bisecta en j y
después decide lsmear-guard:10 (y_best = el menor y de los dives que cerraron;
un dive que no cerró cuenta 2x su presupuesto, cota conservadora). El modelo
regresa t_j sobre las 30 features que el solver calcula (ibexml.MODEL_FEATURES)
y la regla bisecta en el argmin, así que se exporta el negativo (el solver toma
el argmax).

Validación cruzada agrupada por familia (GroupKFold, 5 folds): cada instancia se
evalúa con un modelo que no vio su familia. Se reporta, sobre las muestras
retenidas, el regret de la elección del modelo frente a la de lsmear-guard:10
(bisect_var, la regla que se muestreó) y la de LSmear, y se exporta un modelo
por fold (results/model-guard10/fold<k>.model) más el mapa instancia -> fold,
para evaluar de punta a punta con el modelo que no la vio.

    python3 python/train_rule.py [--trees 300 --depth 4]
"""
import argparse
import glob
import json
import math
import os
import sys

import numpy as np
import pandas as pd
from sklearn.ensemble import GradientBoostingRegressor
from sklearn.model_selection import GroupKFold

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HERE)
from ibexml import encode, export_sklearn, MODEL_FEATURES  # noqa: E402

ROOT = os.path.join(HERE, "..")
DATA = os.path.join(ROOT, "results", "dataset-guard10")
OUT = os.path.join(ROOT, "results", "model-guard10")


def load():
    fam = {l.split()[0][:-4]: l.split()[2] for l in open(os.path.join(DATA, "instances.txt"))}
    X, T, rows = [], [], []
    sid = 0
    for f in sorted(glob.glob(os.path.join(DATA, "samples", "*.jsonl"))):
        inst = os.path.basename(f)[:-6]
        for line in open(f):
            s = json.loads(line)
            L = [l for l in s["labels"] if l["valid"]]
            done = [l["nodes"] for l in L if not l["censored"]]
            if len(L) < 2 or not done:
                continue
            best = min(done)
            Xv = encode(s["node"])[0]
            n = s["node"]
            for l in L:
                y = l["nodes"] if not l["censored"] else 2 * l["budget_used"]
                X.append(Xv[l["var"]])
                T.append(math.log2(max(y, 1) / max(best, 1)))
                rows.append((sid, inst, fam.get(inst, inst), l["var"],
                             l["var"] == n.get("bisect_var"), l["var"] == n.get("lsmear_var"),
                             l["censored"]))
            sid += 1
    R = pd.DataFrame(rows, columns=["sample", "instance", "family", "var", "guard", "lsmear", "censored"])
    return np.asarray(X, float), np.asarray(T, float), R


def regret_table(R, t, pred):
    """Per sample: the regret 2^t of the choice of each rule, then geo per instance."""
    D = R.assign(t=t, pred=pred)
    out = {}
    for name, pick in (("modelo", lambda g: g.loc[g.pred.idxmin()]),
                       ("lsmear-guard:10", lambda g: g[g.guard].iloc[0] if g.guard.any() else None),
                       ("lsmear", lambda g: g[g.lsmear].iloc[0] if g.lsmear.any() else None),
                       ("oráculo", lambda g: g.loc[g.t.idxmin()])):
        v = []
        for sid, g in D.groupby("sample"):
            c = pick(g)
            if c is not None:
                v.append((g.instance.iloc[0], 2 ** c.t))
        V = pd.DataFrame(v, columns=["instance", "r"])
        per = V.groupby("instance").r.apply(lambda x: float(np.exp(np.mean(np.log(x)))))
        out[name] = (float(np.exp(np.mean(np.log(per)))), float((V.r <= 1.0001).mean()),
                     float((V.r >= 2).mean()), len(V))
    return out


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--trees", type=int, default=300)
    ap.add_argument("--depth", type=int, default=4)
    ap.add_argument("--lr", type=float, default=0.05)
    ap.add_argument("--folds", type=int, default=5)
    a = ap.parse_args()

    X, t, R = load()
    X = np.nan_to_num(X, nan=0.0, posinf=1e12, neginf=-1e12)
    ns = R.groupby("instance")["sample"].nunique()
    # each sample weighs 1/|C| (a sample is one decision, not |C| of them), and an
    # instance with many samples does not outweigh the rest: cap at 50 samples' worth
    w = (1.0 / R.groupby("sample")["var"].transform("size")) * np.minimum(1.0, 50.0 / R.instance.map(ns))
    print("%d filas, %d muestras, %d instancias, %d familias; %d features"
          % (len(R), R["sample"].nunique(), R.instance.nunique(), R.family.nunique(), X.shape[1]))
    os.makedirs(OUT, exist_ok=True)
    pred = np.zeros(len(t))
    fold_of = {}
    gkf = GroupKFold(n_splits=a.folds)
    for k, (tr, te) in enumerate(gkf.split(X, t, R.family)):
        m = GradientBoostingRegressor(n_estimators=a.trees, max_depth=a.depth, learning_rate=a.lr,
                                      subsample=0.8, random_state=k)
        m.fit(X[tr], t[tr], sample_weight=w.values[tr])
        pred[te] = m.predict(X[te])
        # the solver takes the argmax: export -t
        neg = GradientBoostingRegressor(n_estimators=a.trees, max_depth=a.depth, learning_rate=a.lr)
        path = os.path.join(OUT, "fold%d.model" % k)
        export_sklearn(path, m)
        # flip the sign of every leaf so that the argmax is the argmin of t
        lines = open(path).read().splitlines()
        with open(path, "w") as fh:
            for ln in lines:
                if ln.startswith("L "):
                    ln = "L %.17g" % (-float(ln[2:]))
                fh.write(ln + "\n")
        for i in R.instance.iloc[te].unique():
            fold_of[i] = k
        print("  fold %d: %d filas de entrenamiento, %d retenidas -> %s" % (k, len(tr), len(te), path))
    # instances without samples (solved at the root) still get a fold, for end-to-end runs
    fam = {l.split()[0][:-4]: l.split()[2] for l in open(os.path.join(DATA, "instances.txt"))}
    fam_fold = {f: fold_of[i] for i, f in ((i, fam[i]) for i in fold_of)}
    for i, f in fam.items():
        if i not in fold_of:
            fold_of[i] = fam_fold.get(f, hash(f) % a.folds)
    json.dump(fold_of, open(os.path.join(OUT, "folds.json"), "w"), indent=0, sort_keys=True)

    print("\nregret por muestra, fuera de fold (geo por instancia; = el mejor; >= 2x):")
    for name, (gi, eq, ge2, n) in regret_table(R, t, pred).items():
        print("  %-16s %.3f   %3.0f%%   %3.0f%%   (%d muestras)" % (name, gi, 100 * eq, 100 * ge2, n))


if __name__ == "__main__":
    main()
