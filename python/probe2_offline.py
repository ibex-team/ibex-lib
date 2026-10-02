#!/usr/bin/env python3
"""Two-level probe on the samples of a dataset, offline.

For each sample and each candidate j: the two children after bisecting j and
contracting are in the label (left/right). Each open child is bisected on
k_j, the best variable of the *node's* LSmear ranking other than j (no LSmear
call at the child), and both grandchildren are contracted (the full IbexOpt
contraction, loup as at the node, not kept). Writes, per sample, per
candidate: k_j and the status/box of the four grandchildren (a pruned child
counts as two pruned grandchildren).

    DATASET=dataset-lffix JOBS=4 python3 python/probe2_offline.py
Resumable: skips instances already in the output directory.
"""
import glob, json, math, os, sys
from multiprocessing import Pool
HERE = os.path.dirname(os.path.abspath(__file__)); sys.path.insert(0, HERE)
import ibexml  # noqa
ROOT = os.path.join(HERE, "..")
DATA = os.path.join(ROOT, "results", os.environ.get("DATASET", "dataset-lffix"))
OUT = os.path.join(DATA, "probe2")
BIN = os.environ.get("BIN", os.path.join(ROOT, "build-fix", "bin", "ibexopt-ml"))


def second_var(node, j):
    sc = node["scores"].get("lsmear") or node["scores"].get("smear_sum_rel")
    best, k = -1.0, -1
    for v, x in enumerate(sc):
        if v == j or x is None or not (isinstance(x, (int, float)) and math.isfinite(x)):
            continue
        if not node["vars"][v]["bisectable"] or node["vars"][v]["too_small"]:
            continue
        if x > best:
            best, k = x, v
    return k


def box(b):
    return json.loads(b) if isinstance(b, str) else b


def run(path):
    inst = os.path.basename(path)[:-6]
    out = os.path.join(OUT, inst + ".jsonl")
    if os.path.exists(out):
        return inst, 0
    bch = os.path.join(ROOT, "benchs", "optim", "all", inst + ".bch")
    n = 0
    with ibexml.IbexOptML(bch, binary=BIN, loup="ipoptxn", random_seed=1,
                          extra_args=["--relax", "both", "--bisector", "lsmear-lffix"]) as srv, \
            open(out + ".tmp", "w") as f:
        for sid, line in enumerate(open(path)):
            s = json.loads(line)
            node = s["node"]
            srv.set_loup(node["loup"] if node["loup"] is not None else ibexml.INF)
            res = []
            for l in s["labels"]:
                if not l["valid"]:
                    continue
                j = l["var"]; k = second_var(node, j)
                gk = []
                for side in ("left", "right"):
                    st = l[side + "_status"]; b = box(l[side])
                    if st != "open" or not b or k < 0:
                        gk += [{"status": "pruned" if st != "open" else st, "box": None}] * 2
                        continue
                    try:
                        _, lb, rb = srv.bisect(b, var=k)
                        for g in (lb, rb):
                            r = srv.contract(g, keep_loup=False)
                            gk.append({"status": r["status"], "box": r["box"]})
                    except ibexml.IbexError:
                        gk += [{"status": "open", "box": b}] * 2
                res.append({"var": j, "k": k, "grand": gk})
            f.write(json.dumps({"sample": sid, "cands": res}) + "\n")
            n += 1
    os.rename(out + ".tmp", out)
    return inst, n


if __name__ == "__main__":
    os.makedirs(OUT, exist_ok=True)
    files = sorted(glob.glob(os.path.join(DATA, "samples", "*.jsonl")))
    if len(sys.argv) > 1:
        files = [f for f in files if os.path.basename(f)[:-6] in sys.argv[1:]]
    with Pool(int(os.environ.get("JOBS", "4"))) as p:
        for inst, n in p.imap_unordered(run, files):
            print(inst, n, flush=True)
