"""Python client for the `ibexopt-ml` node server.

`ibexopt-ml` keeps one long-lived process per problem: the system, the
contractors (HC4 + ACID + X-Newton), the LP solver and the LSmear bisector are
built once, and every command reuses them.  Rebuilding them per call would cost
far more than the contractions themselves, so always keep the process open.

All the boxes exchanged here are *extended* boxes: they have ``nb_ext_var`` ==
n+1 components, and component ``goal_var`` is the objective variable y.  A box
is a list of ``[lb, ub]`` pairs; an empty box is ``None``.

Infinities travel as the JSON literals ``Infinity`` / ``-Infinity`` / ``NaN``,
which Python's ``json`` module reads and writes natively.

Typical use::

    with IbexOptML("benchs/optim/easy/ex2_1_1.bch") as srv:
        root = srv.reset()["root"]
        s = srv.sample(root, budget=200)
        print(s["node"]["bisect_var"], [d["nodes"] for d in s["labels"]])
"""

import heapq
import itertools
import json
import math
import os
import shutil
import subprocess
import warnings

INF = float("inf")

_HERE = os.path.dirname(os.path.abspath(__file__))
_ROOT = os.path.dirname(_HERE)


def find_binary():
    """Locate `ibexopt-ml`.

    Tried in order: $IBEXOPT_ML, `../bin/` (the standalone package layout),
    `../build/bin/` (an Ibex source tree), then $PATH. Falls back to the source
    tree path so that the error message names something meaningful.
    """
    env = os.environ.get("IBEXOPT_ML")
    if env:
        return env
    for cand in (os.path.join(_ROOT, "bin", "ibexopt-ml"),
                 os.path.join(_ROOT, "build", "bin", "ibexopt-ml")):
        if os.path.isfile(cand) and os.access(cand, os.X_OK):
            return cand
    found = shutil.which("ibexopt-ml")
    return found or os.path.join(_ROOT, "build", "bin", "ibexopt-ml")


DEFAULT_BINARY = find_binary()


#: Version of the JSON sample layout this module expects. Samples written by an
#: older build carry no "format" key and are treated as version 1: their nodes
#: have no "phi" and no "admissible", so the comparative features can only be
#: rebuilt over *every* variable instead of over the admissible candidates --
#: same shape, different meaning. Mixing the two vintages in one dataset trains
#: on inconsistent semantics, which is why the mismatch is reported rather than
#: absorbed.
SAMPLE_FORMAT = 2


class FormatWarning(UserWarning):
    """A sample was written by an older build than this module expects."""


def sample_format(sample):
    """The layout version of a sample (1 for anything predating the field)."""
    return int(sample.get("format", 1))


def load_samples(path, strict=True):
    """Iterate the samples of a .jsonl, checking their layout version.

    `strict` raises on a version this module does not understand; with False it
    warns and yields anyway.

    Use this rather than a bare ``json.loads`` loop: it is the one place that
    notices a dataset stitched together from two different builds.
    """
    seen = set()
    with open(path) as f:
        for n, line in enumerate(f, 1):
            line = line.strip()
            if not line:
                continue
            s = json.loads(line)
            v = sample_format(s)
            if v != SAMPLE_FORMAT and v not in seen:
                seen.add(v)
                msg = ("%s line %d: sample format %d, this module expects %d. "
                       "Version 1 samples carry no per-variable model vector and "
                       "no admissible set, so their comparative features cannot "
                       "be reproduced -- regenerate the dataset."
                       % (path, n, v, SAMPLE_FORMAT))
                if strict:
                    raise IbexError(msg)
                warnings.warn(msg, FormatWarning, stacklevel=2)
            yield s


# Arguments whose explicit null is meaningful to the server.
_NULLABLE = frozenset(("box", "var", "pos"))


class IbexError(RuntimeError):
    """The server answered {"ok": false, ...}."""


class IbexOptML(object):
    """A running `ibexopt-ml` process, driven one command at a time."""

    def __init__(self, bch, binary=None, initial_loup=None, random_seed=None,
                 eps_x=None, rel_eps_f=None, abs_eps_f=None, eps_h=None,
                 rigor=False, kkt=False, simpl=None, loup=None,
                 ipopt_freq=None, ipopt_qp=False, extra_args=()):
        argv = [binary or DEFAULT_BINARY]
        if loup:                     argv += ["--loup", loup]
        if ipopt_freq is not None:   argv += ["--ipopt-freq", str(ipopt_freq)]
        if ipopt_qp:                 argv += ["--ipopt-qp"]
        if initial_loup is not None: argv += ["--initial-loup", repr(initial_loup)]
        if random_seed is not None:  argv += ["--random-seed", repr(random_seed)]
        if eps_x is not None:        argv += ["--eps-x", repr(eps_x)]
        if rel_eps_f is not None:    argv += ["--rel-eps-f", repr(rel_eps_f)]
        if abs_eps_f is not None:    argv += ["--abs-eps-f", repr(abs_eps_f)]
        if eps_h is not None:        argv += ["--eps-h", repr(eps_h)]
        if rigor:                    argv += ["--rigor"]
        if kkt:                      argv += ["--kkt"]
        if simpl is not None:        argv += ["--simpl", str(simpl)]
        argv += list(extra_args)
        argv += [bch]

        self.argv = argv
        if not os.path.isfile(argv[0]):
            raise IbexError(
                "ibexopt-ml not found at %r. Build it, put it in bin/ next to "
                "this package, or set $IBEXOPT_ML." % argv[0])
        self.proc = subprocess.Popen(argv, stdin=subprocess.PIPE,
                                     stdout=subprocess.PIPE, text=True, bufsize=1)
        self.bch = bch
        self.info = self._call(cmd="info")["info"]
        self.nb_ext_var = self.info["nb_ext_var"]
        self.nb_ext_ctr = self.info["nb_ext_ctr"]
        self.goal_var = self.info["goal_var"]
        self.goal_ctr = self.info["goal_ctr"]

    # ------------------------------------------------------------------ plumbing

    def _call(self, **kw):
        if self.proc.poll() is not None:
            raise IbexError("ibexopt-ml exited with code %s" % self.proc.returncode)
        # Drop the arguments the caller left unset, so that the values the
        # server was started with (--topk, --no-goal, --budget, ...) apply
        # instead of being silently overridden by a Python-side default.
        kw = {k: v for k, v in kw.items() if v is not None or k in _NULLABLE}
        self.proc.stdin.write(json.dumps(kw) + "\n")
        self.proc.stdin.flush()
        line = self.proc.stdout.readline()
        if not line:
            raise IbexError("ibexopt-ml closed its output (command: %r)" % (kw,))
        ans = json.loads(line)
        if not ans.get("ok"):
            raise IbexError(ans.get("error", "unknown error") + " (command: %r)" % (kw,))
        return ans

    def close(self):
        if self.proc.poll() is None:
            try:
                self.proc.stdin.write('{"cmd":"quit"}\n')
                self.proc.stdin.flush()
            except (BrokenPipeError, ValueError):
                pass
            try:
                self.proc.wait(timeout=5)
            except subprocess.TimeoutExpired:
                self.proc.kill()

    def __enter__(self):
        return self

    def __exit__(self, *exc):
        self.close()
        return False

    # ------------------------------------------------------------------ commands

    def reset(self, box=None, loup=INF):
        """Restart from `box` (default: the problem's declared domain).

        Returns ``{"root": <contracted root box>, "loup": ...}``.
        """
        return self._call(cmd="reset", box=box, loup=loup)

    def contract(self, box, keep_loup=True):
        """Run the full IbexOpt contraction + upper bounding on `box`.

        Returns ``{"status": "open"|"pruned"|"eps", "box", "eps_lb",
        "loup_before", "loup_after", "time"}``.  ``box`` is None unless the
        status is "open"; when it is "eps" the node is too small to be split and
        ``eps_lb`` is the objective lower bound it certifies.
        """
        return self._call(cmd="contract", box=box, keep_loup=keep_loup)

    def features(self, box, depth=-1, last_bisected_var=-1):
        """Everything a model can be fed with for this node (see README)."""
        return self._call(cmd="features", box=box, depth=depth,
                          last_bisected_var=last_bisected_var)["node"]

    def choose_var(self, box):
        """The variable LSmear picks, or None if the box cannot be bisected.

        Returns ``(var, pos, rel_pos)`` or ``(None, None, None)``.
        """
        a = self._call(cmd="choose_var", box=box)
        return a["var"], a["pos"], a["rel_pos"]

    def bisect(self, box, var=None, pos=None):
        """Split `box`; `var=None` lets LSmear decide.

        Returns ``(var, left, right)``.  The children are *not* contracted.
        """
        a = self._call(cmd="bisect", box=box, var=(-1 if var is None else var), pos=pos,
                       rel_pos=(pos is None))
        return a["var"], a["left"], a["right"]

    def candidates(self, box, include_goal=None, topk=None):
        """The variables of `box` worth evaluating.

        The variables the default bisector would actually consider: its own
        guards are applied first (tiny domains, the objective variable when its
        bound is already tight, ...), then `include_goal`, then `topk`.

        Arguments left as None keep whatever the server was started with
        (`--no-goal`, `--topk`), which is almost always what you want.
        """
        return self._call(cmd="candidates", box=box, include_goal=include_goal,
                          topk=topk)["vars"]

    def model_var(self, box, include_goal=None, topk=None):
        """The variable the model loaded in the server picks, or None."""
        return self._call(cmd="model_var", box=box, include_goal=include_goal,
                          topk=topk)["var"]

    def features_vector(self, box):
        """The per-variable feature matrix exactly as a C++ model sees it.

        Shape (n+1, NB_FEATURES), already sanitized. Use this to check that a
        model scored in Python and the same model scored in C++ agree.
        """
        a = self._call(cmd="features_vector", box=box)
        return a["phi"]

    def dive(self, box, var, budget=None, max_depth=None):
        """Force the first bisection on `var`, then dive with LSmear.

        Both children are explored, depth-first, best-first among siblings.
        ``nodes`` is the label; it is only a lower bound when ``censored``.
        The search state is restored afterwards, so successive calls on the same
        box are independent.
        """
        return self._call(cmd="dive", box=box, var=var, budget=budget,
                          max_depth=max_depth)["dive"]

    def sample(self, box, budget=None, budget_start=None, prune=None, max_depth=None,
               include_goal=None, topk=None, depth=-1, last_bisected_var=-1):
        """One training sample: ``{"node": features, "labels": [dive, ...]}``.

        `budget` is a *ceiling*, not the budget every dive gets:

        * with `prune` (the default), once a candidate has closed its dive in k
          nodes the remaining ones run with a budget of k. A dive that reaches
          k is already known not to be better, and a ranking does not care by
          how much. Cost drops from ``|candidates| x budget`` to roughly
          ``|candidates| x min_j y_j``, and the artificial ties a fixed budget
          creates at its ceiling disappear.
        * `budget_start` turns on adaptive budgeting: the most promising
          candidate is probed alone, its budget doubling until its dive closes,
          and that result then caps everyone else. Set it to 0 to go straight
          to `budget`.

        The consequence for training: a censored label is a *lower bound*
        (``y >= budget_used``), not a value. See `pairs()`.
        """
        return self._call(cmd="sample", box=box, budget=budget,
                          budget_start=budget_start, prune=prune,
                          max_depth=max_depth, include_goal=include_goal, topk=topk,
                          depth=depth, last_bisected_var=last_bisected_var)["sample"]

    def set_loup(self, loup):
        return self._call(cmd="set_loup", loup=loup)["loup"]

    def state(self):
        return self._call(cmd="state")


# ---------------------------------------------------------------------------
# A branch & bound loop living in Python.
#
# This is what you will replace LSmear in: pass your model as `select_var`.
# It is also how you produce *on-policy* data once the model starts driving the
# search (DAgger), which matters because a dataset collected under LSmear only
# covers the nodes LSmear itself visits.
#
# Node selection here is plain best-first on the objective lower bound; real
# IbexOpt uses CellBeamSearch (feasible diving), so absolute node counts will
# differ from `ibexopt`.
# ---------------------------------------------------------------------------

def search(srv, select_var=None, box=None, loup=INF, max_nodes=10000,
           on_node=None, rel_eps_f=None, abs_eps_f=None):
    """Run a best-first branch & bound driven from Python.

    Parameters
    ----------
    select_var : callable(srv, box, depth, last_var) -> int or None
        Returns the variable to bisect.  Defaults to LSmear.  Return None to
        treat the node as a leaf.
    on_node : callable(srv, box, depth, last_var, node_index)
        Called on every node just before it is bisected -- this is where you
        collect a sample.

    Returns a dict with ``loup``, ``uplo``, ``nodes`` and ``status``.
    """
    if select_var is None:
        select_var = lambda s, b, d, lv: s.choose_var(b)[0]

    gv = srv.goal_var
    rel_eps_f = srv.info["rel_eps_f"] if rel_eps_f is None else rel_eps_f
    abs_eps_f = srv.info["abs_eps_f"] if abs_eps_f is None else abs_eps_f

    r = srv.reset(box, loup)
    root = r["root"]
    loup = r["loup"]

    counter = itertools.count()
    heap = []
    if root is not None:
        heapq.heappush(heap, (root[gv][0], next(counter), root, 0, -1))

    uplo_of_eps = INF
    nodes = 0
    status = "complete"

    while heap:
        lb, _, cur, depth, last_var = heap[0]
        uplo = min(lb, uplo_of_eps)

        if loup < INF and (loup - uplo <= abs_eps_f or
                           (uplo != 0 and (loup - uplo) / abs(uplo) <= rel_eps_f)):
            status = "precision_reached"
            break
        if nodes >= max_nodes:
            status = "node_limit"
            break

        heapq.heappop(heap)

        if on_node is not None:
            on_node(srv, cur, depth, last_var, nodes)

        var = select_var(srv, cur, depth, last_var)
        if var is None:
            uplo_of_eps = min(uplo_of_eps, cur[gv][0])
            continue

        _, left, right = srv.bisect(cur, var=var)
        for child in (left, right):
            res = srv.contract(child)
            nodes += 1
            if res["status"] == "open":
                b = res["box"]
                heapq.heappush(heap, (b[gv][0], next(counter), b, depth + 1, var))
            elif res["status"] == "eps":
                uplo_of_eps = min(uplo_of_eps, res["eps_lb"])
        loup = srv.state()["loup"]

    if heap:
        uplo = min(heap[0][0], uplo_of_eps)
    else:
        uplo = uplo_of_eps if uplo_of_eps < INF else loup

    return {"loup": loup, "uplo": uplo, "nodes": nodes,
            "open": len(heap), "status": status}


# ---------------------------------------------------------------------------
# Turning a sample into tensors.
#
# The natural structure here is bipartite, variables on one side and
# constraints on the other, with the Jacobian as the edges -- the same shape
# learning-to-branch models for MILP use.  Attend over variables, cross-attend
# to constraints through J.
# ---------------------------------------------------------------------------

VAR_FEATURES = ["lb", "ub", "diam", "mid", "mag", "rel_diam",
                "is_goal", "bisectable", "too_small", "dual"]
SCORE_FEATURES = ["smear_sum", "smear_max", "smear_sum_rel", "lsmear"]
NODE_FEATURES = ["dual_ok"]   # node-level, broadcast to every variable
CTR_FEATURES = ["is_goal_ctr", "entailed", "f_lb", "f_ub", "dual"]
OPS = ["LT", "LEQ", "EQ", "GEQ", "GT"]

# Comparative features: choosing a variable is a comparison between the
# candidates of a node, which a per-variable vector cannot express. For each
# base quantity, three values over the admissible candidates C.
DERIVED_BASES = ["lsmear", "smear_sum", "smear_sum_rel", "dual", "rel_diam"]
DERIVED_KINDS = ["rank", "relmax", "relsum"]
DERIVED_FEATURES = ["%s_%s" % (b, k) for b in DERIVED_BASES for k in DERIVED_KINDS]

# Column index, inside the 15 base columns, each comparative block derives from.
# Must match MLModel::DERIVED_FROM in C++.
_DERIVED_FROM = [13, 10, 12, 9, 5]

_BASE_FEATURES = VAR_FEATURES + SCORE_FEATURES + NODE_FEATURES


def encode(node, scores=True, lsmear_score=True, node_features=True,
           derived=True, clip=1e12):
    """Turn a node's features into numpy arrays.

    Returns ``(X_var, X_ctr, E)`` where X_var is ``(n_ext, 30)``, X_ctr is
    ``(m, 10)`` and E is ``(m, n_ext, 2)``, the Jacobian as [lb, ub] per edge.

    With the default flags the X_var columns are exactly `MODEL_FEATURES`, in
    that order, which is exactly what the solver computes for a model it scores
    itself -- verified bit for bit against `features_vector()`. Train on this
    and `export_sklearn()` gives you a model the solver can run.

    Infinities are clipped to +-`clip` and NaNs sent to 0, as the solver does.

    Set ``lsmear_score=False`` to drop the one feature that makes imitating
    LSmear trivial -- and its three comparative columns with it. That breaks
    the column correspondence with the C++ scorer on purpose: use it to probe
    what the model learned, not to train something you will export.
    """
    import numpy as np

    def clean(a):
        a = np.asarray(a, dtype=np.float64)
        return np.nan_to_num(a, nan=0.0, posinf=clip, neginf=-clip)

    n = len(node["vars"])

    # The solver ships the vector it would feed a C++-scored model. Reading it
    # back beats rebuilding it: the comparative block involves divisions, and
    # the same formula evaluated under a different floating point environment
    # comes out an ulp apart. This way the two sides cannot drift at all.
    if derived and scores and node_features and node.get("phi") is None:
        warnings.warn(
            "this node has no 'phi': it was written by a build predating sample "
            "format %d. The comparative features (columns %d..%d) are being "
            "rebuilt over every variable instead of over the admissible "
            "candidates, which is a different quantity under the same column "
            "names. Regenerate the dataset, or pass derived=False."
            % (SAMPLE_FORMAT, len(_BASE_FEATURES), len(MODEL_FEATURES) - 1),
            FormatWarning, stacklevel=2)

    if derived and scores and node_features and node.get("phi") is not None:
        X = clean(node["phi"])
        if not lsmear_score:
            base = len(_BASE_FEATURES) + 3 * DERIVED_BASES.index("lsmear")
            X = np.delete(X, [_BASE_FEATURES.index("lsmear"), base, base + 1, base + 2],
                          axis=1)
        return X, _encode_ctrs(node, clean), _encode_edges(node, clean, n)

    X = clean([[float(v[k]) for k in VAR_FEATURES] for v in node["vars"]])

    if scores:
        cols = []
        for k in SCORE_FEATURES:
            col = (node.get("scores") or {}).get(k) or [0.0] * n
            cols.append(col)
        X = np.concatenate([X, clean(cols).T], axis=1)

    if node_features:
        cols = [[1.0 if node.get(k) else 0.0] * n for k in NODE_FEATURES]
        X = np.concatenate([X, clean(cols).T], axis=1)

    if derived and scores and node_features:
        cand = node.get("admissible")
        if cand is None:           # older dumps: fall back on every variable
            cand = list(range(n))
        K = len(cand)
        D = np.zeros((n, 3 * len(_DERIVED_FROM)), dtype=np.float64)

        # Summed in candidate order, one term at a time, so that the result is
        # bit-identical to the C++ loop rather than to a pairwise reduction.
        for b, src in enumerate(_DERIVED_FROM):
            mx = 0.0
            sm = 0.0
            for j in cand:
                a = abs(float(X[j, src]))
                if a > mx:
                    mx = a
                sm += a
            for j in cand:
                vj = float(X[j, src])
                lt = 0
                for q in cand:
                    if float(X[q, src]) < vj:
                        lt += 1
                D[j, 3 * b + 0] = (lt / (K - 1)) if K > 1 else 0.5
                D[j, 3 * b + 1] = (vj / mx) if mx > 0 else 0.0
                D[j, 3 * b + 2] = (vj / sm) if sm > 0 else 0.0

        X = np.concatenate([X, D], axis=1)

    if not lsmear_score and scores:
        drop = [_BASE_FEATURES.index("lsmear")]
        if derived and node_features and X.shape[1] == len(MODEL_FEATURES):
            base = len(_BASE_FEATURES) + 3 * DERIVED_BASES.index("lsmear")
            drop += [base, base + 1, base + 2]
        X = np.delete(X, drop, axis=1)

    return X, _encode_ctrs(node, clean), _encode_edges(node, clean, n)


def _encode_ctrs(node, clean):
    xc = []
    for c in node["ctrs"]:
        row = [float(c[k]) if c[k] is not None else 0.0 for k in CTR_FEATURES]
        row += [1.0 if c["op"] == o else 0.0 for o in OPS]   # one-hot operator
        xc.append(row)
    return clean(xc)


def _encode_edges(node, clean, n):
    import numpy as np
    J = node["J"] if node["J"] is not None else node["J_mid"]
    return clean(J) if J is not None else np.zeros((len(node["ctrs"]), n, 2))


def labels(sample, n_ext, censored_as=None):
    """Turn a sample's dive results into per-variable targets.

    Returns ``(y, mask, censored)``:

    * ``y[j]``        - the dive size of variable j;
    * ``mask[j]``     - whether j was evaluated at all;
    * ``censored[j]`` - whether ``y[j]`` is only a lower bound.

    With mutual pruning on (the default), most labels *are* censored by
    construction, and that is the point: they were cut as soon as they were
    known not to beat the best. Do not regress ``y`` -- rank within the node.
    `censored_as="drop"` clears the mask on censored entries if you want exact
    values only.
    """
    import numpy as np

    y = np.zeros(n_ext, dtype=np.float64)
    mask = np.zeros(n_ext, dtype=bool)
    cens = np.zeros(n_ext, dtype=bool)
    for d in sample["labels"]:
        if not d["valid"]:
            continue
        if censored_as == "drop" and d["censored"]:
            continue
        y[d["var"]] = float(d["nodes"])
        mask[d["var"]] = True
        cens[d["var"]] = bool(d["censored"])
    return y, mask, cens


def pairs(sample):
    """The ordered pairs a sample actually licenses, censoring included.

    Yields ``(better, worse)`` variable indices such that the dive of `better`
    is known to be no larger than the dive of `worse`. This is the honest
    supervision signal under mutual pruning:

    * a finished dive of size a vs a finished dive of size b>a  -> (a, b);
    * a finished dive of size a vs a dive censored at c>=a      -> (a, censored);
    * two censored dives                                        -> nothing.

    Feed these to a pairwise ranking loss instead of regressing the raw sizes.
    """
    out = []
    ds = [d for d in sample["labels"] if d["valid"]]
    for i in ds:
        if i["censored"]:
            continue
        for j in ds:
            if j["var"] == i["var"]:
                continue
            if j["censored"]:
                # j was cut at budget_used: all we know is y_j >= budget_used
                if i["nodes"] <= j["budget_used"]:
                    out.append((i["var"], j["var"]))
            elif i["nodes"] < j["nodes"]:
                out.append((i["var"], j["var"]))
    return out


# ---------------------------------------------------------------------------
# Running the real search with your own rule (inversion of control).
#
# Unlike search(), the node selection, the contraction and the bounding all
# stay inside the solver -- only the branching decision comes here. That is
# what makes node counts comparable with `ibexopt`.
# ---------------------------------------------------------------------------

def run(bch, select_var, binary=None, sample_prob=0.0, on_sample=None, sample_when=None,
        max_nodes=0, timeout=0.0, features=True, include_goal=True, topk=0,
        budget=200, budget_start=25, prune=True, max_depth=0,
        random_seed=None, initial_loup=None, progress=False, extra_args=(),
        rng=None):
    """Run IbexOpt with `select_var` choosing every bisection variable.

    Parameters
    ----------
    select_var : callable(node, candidates, state) -> int or None
        `node` is the feature dict (None when `features=False`), `candidates`
        the list of admissible variables, `state` a dict with nodes/loup/uplo/
        depth/last_bisected_var/open. Return None to let LSmear decide.
    sample_prob : float
        Probability of also asking the solver for a full sample at a node --
        this is how you get on-policy training data while your own rule drives.
    on_sample : callable(sample)
        Called with each sample produced that way.
    sample_when : callable(state) -> bool
        Overrides `sample_prob` when given -- use it to stop after a quota
        without stopping the search, or to sample by depth rather than at
        random.

    Returns the final ``{"event": "done", ...}`` record.
    """
    import random as _random

    argv = [binary or DEFAULT_BINARY, "--run"]
    if max_nodes:               argv += ["--max-nodes", str(max_nodes)]
    if timeout:                 argv += ["--timeout", repr(timeout)]
    if not features:            argv += ["--no-features"]
    if not include_goal:        argv += ["--no-goal"]
    if topk:                    argv += ["--topk", str(topk)]
    if not prune:               argv += ["--no-prune"]
    if max_depth:               argv += ["--max-depth", str(max_depth)]
    if progress:                argv += ["--progress"]
    argv += ["--budget", str(budget), "--budget-start", str(budget_start)]
    if random_seed is not None:  argv += ["--random-seed", repr(random_seed)]
    if initial_loup is not None: argv += ["--initial-loup", repr(initial_loup)]
    argv += list(extra_args)
    argv += [bch]

    if rng is None:
        rng = _random.Random(0)

    proc = subprocess.Popen(argv, stdin=subprocess.PIPE, stdout=subprocess.PIPE,
                            text=True, bufsize=1)
    done = None
    try:
        for line in proc.stdout:
            line = line.strip()
            if not line:
                continue
            ev = json.loads(line)
            kind = ev.get("event")

            if kind == "sample":
                if on_sample is not None:
                    on_sample(ev["sample"])
                continue

            if kind == "done":
                done = ev
                break

            if kind != "bisect":
                raise IbexError("unexpected event %r" % (ev,))

            state = {k: ev[k] for k in
                     ("nodes", "depth", "last_bisected_var", "loup", "uplo", "open")}
            var = select_var(ev.get("node"), ev["candidates"], state)
            if sample_when is not None:
                want = bool(sample_when(state))
            else:
                want = sample_prob > 0 and rng.random() < sample_prob
            proc.stdin.write(json.dumps({"var": var, "sample": want}) + "\n")
            proc.stdin.flush()
    finally:
        try:
            proc.stdin.close()
        except (BrokenPipeError, ValueError):
            pass
        proc.wait(timeout=10)

    return done


def solve(bch, model=None, binary=None, max_nodes=0, timeout=0.0,
          random_seed=None, bisector=None, relax=None, loup=None,
          ipopt_freq=None, ipopt_qp=False, wall_timeout=None,
          extra_args=()):
    """Run the search to completion under LSmear or under `model`, and time it.

    This is the only measurement that answers "is the learned rule actually
    better": fewer nodes at a higher per-node cost is not an improvement.
    `bisector` selects the hand-written rule to run (see `BISECTORS`); `model`
    overrides it with a learned one. `relax` selects the linear relaxation the
    contractor's X-Newton step is built on (see `RELAXATIONS`) -- it changes the
    contraction, not the branching, so it is an independent axis. `loup` selects
    the upper bounding (see `LOUP_FINDERS`); the ipopt* ones call Ipopt every
    `ipopt_freq` calls and need a build configured with -DIBEX_WITH_IPOPT=ON.
    They find better incumbents earlier, which prunes more, so a run with one is
    not comparable with a run without -- it is a third axis, not a tweak.

    `timeout` is a limit on *CPU* seconds, and the solver checks it once per
    node -- so a single node whose contraction does not return escapes it
    entirely. That is not hypothetical: stock `ibexopt` hangs on
    `robot.bch` (7 unbounded variables, trigonometric constraints) despite its
    own `-t`. Pass `wall_timeout` to put a hard wall-clock kill on top; the
    status then comes back as ``"killed"``.

    Returns ``{"status", "loup", "uplo", "nodes", "time", "bisector", "rule"}``.
    """
    argv = [binary or DEFAULT_BINARY, "--solve"]
    if model:                   argv += ["--model", model]
    if bisector:                argv += ["--bisector", bisector]
    if relax:                   argv += ["--relax", relax]
    if loup:                    argv += ["--loup", loup]
    if ipopt_freq is not None:  argv += ["--ipopt-freq", str(ipopt_freq)]
    if ipopt_qp:                argv += ["--ipopt-qp"]
    if max_nodes:               argv += ["--max-nodes", str(max_nodes)]
    if timeout:                 argv += ["--timeout", repr(timeout)]
    if random_seed is not None: argv += ["--random-seed", repr(random_seed)]
    argv += list(extra_args)
    argv += [bch]

    try:
        out = subprocess.run(argv, capture_output=True, text=True,
                             timeout=wall_timeout)
    except subprocess.TimeoutExpired:
        # the solver blew through its own CPU limit: it is stuck inside one node
        return {"status": "killed", "loup": float("inf"), "uplo": float("-inf"),
                "nodes": 0, "time": wall_timeout, "bisector": bisector or "",
                "relax": relax or "xtaylor", "loup_finder": loup or "default",
                "rule": model or bisector or ""}
    line = out.stdout.strip().splitlines()
    if not line:
        raise IbexError("no output from --solve: %s" % out.stderr)
    return json.loads(line[-1])


# ---------------------------------------------------------------------------
# Exporting a model the solver can evaluate itself.
#
# The features are the ones encode() produces with scores=True and
# lsmear_score=True, in that exact order, so a model trained on encode()'s
# output can be scored inside C++ without any glue.
# ---------------------------------------------------------------------------

#: The linear relaxations `solve()` and `--relax` accept.
RELAXATIONS = ["xtaylor", "affine", "both"]

#: The upper bounding methods `solve()` and `--loup` accept. Everything but
#: "default" runs Ipopt and is only available in a build configured with
#: -DIBEX_WITH_IPOPT=ON; the solver rejects the name otherwise.
LOUP_FINDERS = ["default", "ipoptprob", "ipoptxn", "ipoptxninhc4"]

#: The hand-written bisectors `solve()` and `--bisector` accept.
BISECTORS = ["lsmear", "lsmear-box", "smearsumrel", "smearsum", "smearmax",
             "smearmaxrel", "largestfirst", "roundrobin", "lsmear-guard"]

MODEL_FEATURES = _BASE_FEATURES + DERIVED_FEATURES


def export_linear(path, w, bias=0.0):
    """Write a linear model: score_j = bias + w . phi_j."""
    w = list(w)
    if len(w) != len(MODEL_FEATURES):
        raise ValueError("expected %d weights, got %d" % (len(MODEL_FEATURES), len(w)))
    with open(path, "w") as f:
        f.write("ibexml-model 1\ntype linear\nnfeat %d\n" % len(w))
        f.write("bias %.17g\n" % bias)
        f.write("w " + " ".join("%.17g" % v for v in w) + "\n")
    return path


def _write_tree(f, tree, scale):
    """One sklearn tree_ object, as the flat node list the solver reads."""
    n = tree.node_count
    f.write("tree %d\n" % n)
    for i in range(n):
        if tree.children_left[i] == -1:      # leaf
            f.write("L %.17g\n" % (scale * float(tree.value[i][0][0])))
        else:
            f.write("S %d %.17g %d %d\n" % (int(tree.feature[i]),
                                             float(tree.threshold[i]),
                                             int(tree.children_left[i]),
                                             int(tree.children_right[i])))


def export_trees(path, trees, scale=1.0):
    """Write a sum of regression trees. `trees` are sklearn ``tree_`` objects."""
    with open(path, "w") as f:
        f.write("ibexml-model 1\ntype gbdt\nnfeat %d\nbase 0\n" % len(MODEL_FEATURES))
        for t in trees:
            _write_tree(f, t, scale)
    return path


def export_sklearn(path, est):
    """Export a fitted sklearn regressor the solver can evaluate.

    Supports DecisionTreeRegressor, RandomForestRegressor and
    GradientBoostingRegressor. The additive constant of a GBDT is dropped: it
    is the same for every variable, so it cannot change an argmax.
    """
    name = type(est).__name__
    if name == "DecisionTreeRegressor":
        return export_trees(path, [est.tree_], 1.0)
    if name in ("RandomForestRegressor", "ExtraTreesRegressor"):
        n = len(est.estimators_)
        return export_trees(path, [e.tree_ for e in est.estimators_], 1.0 / n)
    if name == "GradientBoostingRegressor":
        trees = [e[0].tree_ for e in est.estimators_]
        return export_trees(path, trees, float(est.learning_rate))
    if hasattr(est, "coef_"):
        return export_linear(path, est.coef_.ravel(),
                             float(getattr(est, "intercept_", 0.0)))
    raise ValueError("don't know how to export %s" % name)
