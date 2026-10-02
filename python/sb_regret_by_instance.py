#!/usr/bin/env python3
"""Per instance: regret of strong branching's choice (pruned children, then
least open volume) against the dive labels, and of the trajectory's own
choice, on the samples of a directory.

    python3 python/sb_regret_by_instance.py DIR [instance ...]
"""
import glob, json, math, os, sys
import numpy as np
HERE = os.path.dirname(os.path.abspath(__file__)); sys.path.insert(0, HERE)
from oracle_sbfeat_offline import box, logvol  # noqa


def main():
    d = sys.argv[1]; want = set(sys.argv[2:])
    print("%-12s %5s  %-22s %-22s" % ("instancia", "n", "SB: geo  mejor  >=2x", "trayect.: geo mejor >=2x"))
    for f in sorted(glob.glob(os.path.join(d, "*.jsonl"))):
        inst = os.path.basename(f)[:-6]
        if want and inst not in want:
            continue
        rs, rt = [], []
        for line in open(f):
            s = json.loads(line)
            L = [l for l in s["labels"] if l["valid"]]
            done = [l["nodes"] for l in L if not l["censored"]]
            if len(L) < 2 or not done:
                continue
            best = min(done); n = s["node"]
            parent = [(v["lb"], v["ub"]) for v in n["vars"]]
            gv = next((j for j, v in enumerate(n["vars"]) if v.get("is_goal")), -1)
            def key(l):
                st = [l["left_status"], l["right_status"]]; ch = [box(l["left"]), box(l["right"])]
                lv = [logvol(ch[k], parent, gv) for k in range(2) if st[k] == "open" and ch[k]]
                return (sum(x != "open" for x in st), -(float(np.logaddexp.reduce(lv)) if lv else -100.0))
            y = lambda l: (l["nodes"] if not l["censored"] else 2 * l["budget_used"]) / best
            rs.append(y(max(L, key=key)))
            t = [l for l in L if l["var"] == n.get("bisect_var")]
            if t: rt.append(y(t[0]))
        if not rs:
            continue
        g = lambda r: (math.exp(np.mean(np.log(r))), np.mean(np.array(r) <= 1.0001), np.mean(np.array(r) >= 2))
        a = g(rs); b = g(rt) if rt else (float("nan"),) * 3
        print("%-12s %5d  %.3f %4.0f%% %4.0f%%      %.3f %4.0f%% %4.0f%%" % (inst, len(rs), a[0], 100 * a[1], 100 * a[2], b[0], 100 * b[1], 100 * b[2]))


if __name__ == "__main__":
    main()
