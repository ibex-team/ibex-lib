#!/usr/bin/env python3
"""Paso 1: cuánto se gana eligiendo mejor la variable en cada nodo.

Para cada nodo muestreado (trayectoria de lsmear-guard:10, compo + ipoptxn),
todos los candidatos corrieron un dive con el mismo presupuesto. El mejor es el
de menos nodos entre los dives que terminaron. El regret de una regla es
y(su elección) / y(mejor); si su dive no terminó cuenta el presupuesto, así que
es una cota inferior. Nodos donde ningún dive terminó no dicen nada y se
cuentan aparte. Se agrega por instancia (media geométrica) y luego entre
instancias, para que no pesen más las que tienen más muestras.

Reglas: la que decidió (lsmear-guard:10), lsmear, smearsumrel, round-robin
(la siguiente variable admisible después de la última bisectada) y un
"oráculo de un paso" (regret 1 por definición).

    python3 results/headroom/analyze.py
"""
import glob
import json
import math
import os

import numpy as np
import pandas as pd

HERE = os.path.dirname(os.path.abspath(__file__))


def rr_choice(last, valid):
    after = sorted(v for v in valid if v > last)
    return after[0] if after else (min(valid) if valid else None)


rows, skipped = [], {}
for f in sorted(glob.glob(os.path.join(HERE, "samples", "*.jsonl"))):
    inst = os.path.basename(f)[:-6]
    for line in open(f):
        s = json.loads(line)
        L = [l for l in s["labels"] if l["valid"]]
        done = [l["nodes"] for l in L if not l["censored"]]
        if len(L) < 2:
            continue
        if not done:
            skipped[inst] = skipped.get(inst, 0) + 1
            continue
        best = min(done)
        y = {l["var"]: l["nodes"] for l in L}
        cen = {l["var"]: l["censored"] for l in L}
        n = s["node"]
        ch = {"lsmear-guard:10": n.get("bisect_var"), "lsmear": n.get("lsmear_var"),
              "smearsumrel": n.get("smear_sum_rel_var"),
              "roundrobin": rr_choice(n.get("last_bisected_var", -1), list(y))}
        r = {"instance": inst, "ncand": len(L), "best": best, "depth": n.get("depth")}
        for k, v in ch.items():
            if v in y:
                r[k] = y[v] / best
                r[k + "_cens"] = cen[v]
        # how much better than the rule that decided is the best, and how spread are the candidates
        r["worst"] = max(y.values()) / best
        rows.append(r)

T = pd.DataFrame(rows)
rules = ["lsmear-guard:10", "lsmear", "smearsumrel", "roundrobin"]
print("%d nodos informativos en %d instancias; %d nodos sin ningún dive terminado (en %d instancias)"
      % (len(T), T.instance.nunique(), sum(skipped.values()), len(skipped)))
print("candidatos por nodo: mediana %d; peor candidato / mejor: mediana %.2f\n"
      % (T.ncand.median(), T.worst.median()))
g = lambda x: float(np.exp(np.mean(np.log(x))))
print("%-16s %9s %9s %8s %8s %8s" % ("regla", "geo inst", "geo nodo", "=mejor", ">=2x", ">=10x"))
for r in rules:
    x = T[r].dropna()
    per = T.groupby("instance")[r].apply(lambda v: g(v.dropna()) if v.notna().any() else np.nan).dropna()
    print("%-16s %9.2f %9.2f %7.0f%% %7.0f%% %7.0f%%" % (r, g(per), g(x), 100 * (x <= 1.0001).mean(),
                                                     100 * (x >= 2).mean(), 100 * (x >= 10).mean()))
print("\nmayor margen por instancia (geo del regret de lsmear-guard:10):")
per = T.groupby("instance").agg(n=("best", "size"), guard=("lsmear-guard:10", g), lsmear=("lsmear", g))
print(per.sort_values("guard", ascending=False).head(12).round(2).to_string())
print("\nregret de lsmear-guard:10 por profundidad:")
T["dbin"] = pd.cut(T.depth, [-1, 5, 15, 30, 60, 1e9], labels=["0-5", "6-15", "16-30", "31-60", ">60"])
print(T.groupby("dbin", observed=True)["lsmear-guard:10"].agg(["size", g]).round(2).to_string())
