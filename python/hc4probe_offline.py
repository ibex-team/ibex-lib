#!/usr/bin/env python3
"""Offline: the drastic HC4 probe (server command "probe") on the samples of a
dataset, for several (dims, parts), scored against the dive labels with the
metric that matters for a rule replacing the base: per instance, sum of the
chosen candidates' dive nodes over the base's own choice (geo over
instances), instances above 1, deviation rate. Rules: always the probe's
best; conservative (deviate only if more empties, or as many and at most r
of the base's volume) for several r.

    DATASET=dataset-lffix JOBS=4 python3 python/hc4probe_offline.py
Probe outputs are cached in <dataset>/hc4probe/<dims>x<parts>/<instance>.jsonl.
"""
import glob, json, math, os, sys
from multiprocessing import Pool
import numpy as np, pandas as pd
HERE = os.path.dirname(os.path.abspath(__file__)); sys.path.insert(0, HERE)
import ibexml  # noqa
from oracle_sbfeat_offline import geo  # noqa
ROOT = os.path.join(HERE, "..")
DATA = os.path.join(ROOT, "results", os.environ.get("DATASET", "dataset-lffix"))
BIN = os.environ.get("BIN", os.path.join(ROOT, "build-fix", "bin", "ibexopt-ml"))
CONFIGS = [c for c in (("hc4", 1, 2), ("hc4", 1, 4), ("hc4", 1, 8), ("hc4", 2, 2), ("hc4", 2, 4),
                       ("lp", 1, 2), ("lp", 1, 4), ("full", 1, 2), ("proc", 1, 2))
           if not os.environ.get("CONFIGS") or "%s-%dx%d" % c in os.environ["CONFIGS"].split(",")]


def run(path):
    inst = os.path.basename(path)[:-6]
    outs = {c: os.path.join(DATA, "hc4probe", "%s-%dx%d" % c, inst + ".jsonl") for c in CONFIGS}
    todo = {c: o for c, o in outs.items() if not os.path.exists(o)}
    if not todo:
        return inst, 0
    for o in todo.values():
        os.makedirs(os.path.dirname(o), exist_ok=True)
    bch = os.path.join(ROOT, "benchs", "optim", "all", inst + ".bch")
    fs = {c: open(o + ".tmp", "w") for c, o in todo.items()}
    n = 0
    with ibexml.IbexOptML(bch, binary=BIN, loup="ipoptxn", random_seed=1,
                          extra_args=["--relax", "both", "--bisector", "lsmear-lffix"]) as srv:
        for sid, line in enumerate(open(path)):
            s = json.loads(line); node = s["node"]
            srv.set_loup(node["loup"] if node["loup"] is not None else ibexml.INF)
            for (cc, d, p), f in fs.items():
                try:
                    pr = srv._call(cmd="probe", box=node["box"], dims=d, parts=p, ctc=cc)["probes"]
                except ibexml.IbexError:
                    pr = []
                f.write(json.dumps({"sample": sid, "probes": pr}) + "\n")
            n += 1
    for c, f in fs.items():
        f.close(); os.rename(outs[c] + ".tmp", outs[c])
    return inst, n


def score(rows, name):
    R = pd.DataFrame(rows, columns=["instance", "y", "yb", "dev"])
    ratio = R.groupby("instance").apply(lambda g: g.y.sum() / g.yb.sum())
    return "  %-28s suma/base %.3f   instancias >1: %3.0f%%   >1.5x: %3.0f%%   desvía %3.0f%%" % (
        name, geo(ratio), 100 * (ratio > 1.0001).mean(), 100 * (ratio > 1.5).mean(), 100 * R.dev.mean())


def evaluate():
    lines = []
    for cc, d, p in CONFIGS:
        rules = {"siempre": []}
        for r in (0.5, 0.25, 0.1):
            rules["conservador r=%.2f" % r] = []
        tsum = 0.0; tn = 0
        for f in sorted(glob.glob(os.path.join(DATA, "hc4probe", "%s-%dx%d" % (cc, d, p), "*.jsonl"))):
            inst = os.path.basename(f)[:-6]
            samples = [json.loads(l) for l in open(os.path.join(DATA, "samples", inst + ".jsonl"))]
            for l in open(f):
                q = json.loads(l); s = samples[q["sample"]]
                L = {l["var"]: l for l in s["labels"] if l["valid"]}
                done = [l["nodes"] for l in L.values() if not l["censored"]]
                if len(L) < 2 or not done:
                    continue
                bv = s["node"].get("bisect_var")
                if bv not in L:
                    continue
                y = lambda l: l["nodes"] if not l["censored"] else 2 * l["budget_used"]
                pr = [x for x in q["probes"] if x["var"] in L]
                if not pr:
                    continue
                for x in pr: tsum += x["time"]; tn += 1
                best = None
                for x in pr:   # ranked by LSmear: ties keep the earlier
                    if best is None or x["empties"] > best["empties"] or \
                            (x["empties"] == best["empties"] and x["logvol"] < best["logvol"] - 1e-6):
                        best = x
                base = next((x for x in pr if x["var"] == bv), None)
                yb = y(L[bv])
                rules["siempre"].append((inst, y(L[best["var"]]), yb, best["var"] != bv))
                for r in (0.5, 0.25, 0.1):
                    clear = base is not None and (best["empties"] > base["empties"] or
                            (best["empties"] == base["empties"] and best["logvol"] <= base["logvol"] + math.log(r)))
                    ch = best["var"] if (best["var"] != bv and clear) else bv
                    rules["conservador r=%.2f" % r].append((inst, y(L[ch]), yb, ch != bv))
        lines.append("ctc=%s dims=%d parts=%d   (%.1f ms por candidato)" % (cc, d, p, 1000 * tsum / max(tn, 1)))
        for name, rows in rules.items():
            if rows: lines.append(score(rows, name))
    print("\n".join(lines))


if __name__ == "__main__":
    files = sorted(glob.glob(os.path.join(DATA, "samples", "*.jsonl")))
    with Pool(int(os.environ.get("JOBS", "4"))) as pool:
        for inst, n in pool.imap_unordered(run, files):
            if n: print(inst, n, flush=True)
    evaluate()
