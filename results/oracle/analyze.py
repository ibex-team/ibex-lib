#!/usr/bin/env python3
"""El oráculo de un paso de punta a punta frente a lsmear y lsmear-guard:10.

Nodos del árbol que arma el oráculo (results/oracle/oracle.jsonl) contra los de
la línea base con el binario arreglado (results/ipopt-compo-fixed.csv), misma
configuración: compo + ipoptxn. Los nodos son deterministas; los tiempos del
oráculo incluyen los dives y no se comparan.

    python3 results/oracle/analyze.py
"""
import json
import os

import numpy as np
import pandas as pd

HERE = os.path.dirname(os.path.abspath(__file__))
import glob
R = [json.loads(l) for f in sorted(glob.glob(os.path.join(HERE, "oracle*.jsonl")))
     for l in open(f) if l.strip()]
O = pd.DataFrame([{"instance": r["instance"], "arm": r["arm"],
                   "status": r["result"].get("status"), "nodes": r["result"].get("nodes"),
                   "calls": r["result"].get("oracle_calls"), "fallbacks": r["result"].get("oracle_fallbacks")}
                  for r in R])
B = pd.read_csv(os.path.join(HERE, "..", "ipopt-compo-fixed.csv")).drop_duplicates(["instance", "rule"], keep="last")
BN = B.pivot(index="instance", columns="rule", values="nodes")
BS = B.pivot(index="instance", columns="rule", values="status")
g = lambda x: float(np.exp(np.mean(np.log(x)))) if len(x) else float("nan")

for arm, base in (("lsmear", "lsmear"), ("guard10", "lsmear-guard:10"), ("lazy50", "lsmear")):
    o = O[O.arm == arm].set_index("instance")
    if o.empty:
        continue
    ok = (o.status == "complete")
    both = ok & (BS.reindex(o.index)[base] == "complete")
    r = (o.nodes[both] / BN.reindex(o.index)[base][both]).astype(float)
    fb = (o.fallbacks / o.calls.replace(0, np.nan)).astype(float)
    print("[%-7s] continuación %-16s %d corridas, cierra %d; decisiones sin ningún dive terminado: %.0f%% (mediana por instancia)"
          % (arm, base, len(o), ok.sum(), 100 * fb.median()))
    print("   nodos oráculo / %s sobre %d que ambos cierran: geo %.2f, mediana %.2f, p10 %.2f, p90 %.2f; peor en %d"
          % (base, both.sum(), g(r), r.median(), r.quantile(.1), r.quantile(.9), (r > 1.05).sum()))
    for other in ("lsmear", "lsmear-guard:10"):
        if other == base:
            continue
        b2 = ok & (BS.reindex(o.index)[other] == "complete")
        r2 = (o.nodes[b2] / BN.reindex(o.index)[other][b2]).astype(float)
        print("   nodos oráculo / %s sobre %d: geo %.2f" % (other, b2.sum(), g(r2)))
    no = o[~ok]
    if len(no):
        print("   no cierra (%d): %s" % (len(no), ", ".join("%s (%s, base %s)" % (i, s, BS.at[i, base] if i in BS.index else "?")
                                                         for i, s in no.status.items())))
    print()
