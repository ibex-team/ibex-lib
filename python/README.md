# ibexopt-ml — learning a branching rule for IbexOpt

`ibexopt-ml` opens up IbexOpt one node at a time so that its variable-selection
heuristic can be replaced by a learned model, with the search itself driven from
Python.

It is not a fork of the optimizer: it reuses `DefaultOptimizerConfig`, so the
contractor (HC4 → ACID(HC4) → fix-point of X-Newton + HC4), the loup finder
(`LoupFinderDefault` with inHC4), the cell buffer (`CellBeamSearch`) and the
bisector (`LSmear`, variant `LSMEAR_MG`) are byte-for-byte the ones `ibexopt`
runs. Whatever you measure here is what the real solver would have done.

---

## Table of contents

1. [What it gives you](#1-what-it-gives-you)
2. [Building](#2-building)
3. [Concepts](#3-concepts)
4. [Quick start](#4-quick-start)
5. [The four modes](#5-the-four-modes)
6. [Command-line reference](#6-command-line-reference)
7. [Data format](#7-data-format)
8. [Wire protocol reference](#8-wire-protocol-reference)
9. [Python API](#9-python-api)
10. [Training: a worked path](#10-training-a-worked-path)
11. [Closing the loop (DAgger)](#11-closing-the-loop-dagger)
12. [Performance and tuning](#12-performance-and-tuning)
13. [How it works inside](#13-how-it-works-inside)
14. [Known limitations](#14-known-limitations)
15. [Comparing branching rules](#15-comparing-branching-rules)
16. [File map](#16-file-map)

---

## 1. What it gives you

**A label for every (node, variable) pair.** Take a node, force the first
bisection on variable `x_j`, and run a truncated branch & bound underneath —
**both** children, depth-first, best sibling first, with LSmear choosing every
variable after the first one. The number of nodes contracted in that subtree is
the target `y_j`. Small `y_j` means bisecting on `x_j` was a good idea.

The candidates of a node are not evaluated independently: each one is measured
against the best result so far, so a dive is cut as soon as it is known not to
win. See [§3.4](#34-budgeting-pruning-and-censoring).

**A node oracle.** Contract a box exactly as IbexOpt would, ask what LSmear
would bisect, split a box on a chosen variable — enough to run the whole
branch & bound from Python with your model in place of LSmear.

**Two ways to put a rule back in.** From Python, with the solver running the
search and asking you for each decision (`--run`), so node counts stay
comparable with `ibexopt`; or entirely inside C++, by exporting the model to a
text file the solver evaluates itself (`--model`), which is the only way to
measure wall-clock time honestly.

## 2. Building

`ibexopt-ml` is built by the normal Ibex build. It needs an LP solver — the
same requirement as `ibexopt` — and `LP_LIB` defaults to `none`, so it has to
be requested explicitly. SoPlex is bundled, nothing is downloaded:

```bash
mkdir -p build && cd build
cmake .. -DLP_LIB=soplex     # or: cmake .   if the build dir is already configured
make -j8
ls bin/ibexopt-ml
```

Without `-DLP_LIB` the build succeeds but the binary refuses to run:
`error: ibexopt-ml requires a LP Solver (use -DLP_LIB with cmake)`. Check an
existing build directory with `grep ^LP_LIB: build/CMakeCache.txt`.

The Python side needs nothing beyond the standard library, except `numpy` for
the optional `encode()` / `labels()` helpers.

```bash
python3 -c "import sys; sys.path.insert(0,'python'); import ibexml; print('ok')"
```

---

## 3. Concepts

### 3.1 The extended space

IbexOpt does not work on your `n` variables. It works on the **extended
system**: `n+1` variables `x_1..x_n, y`, where `y` is the objective, plus the
constraint `y = f(x)` on top of the normalized constraints (all inequalities
turned into `≤`, equalities relaxed by `eps_h`).

Everything crossing the wire is in that space:

* a box has `nb_ext_var == n+1` components;
* component `goal_var` is `y` — it is **not** necessarily the last one, read it
  from `info`;
* `nb_ext_ctr` is usually larger than your `nb_ctr` (normalization splits and
  adds constraints). For `ex6_1_2.bch`: `nb_var=4`, `nb_ext_var=5`,
  `nb_ctr=3`, `nb_ext_ctr=7`, `goal_var=4`, `goal_ctr=0`.

A box is a list of `[lb, ub]` pairs. An empty box is `null`. Infinities travel
as the bare `Infinity` / `-Infinity` / `NaN` literals — not standard JSON, but
exactly what Python's `json` module reads and writes by default, which is the
whole point. If you consume the data from another language, allow those tokens.

### 3.2 A node

A node is a box plus the search context it lives in: the current incumbent
(`loup`), the current lower bound (`uplo`), its depth, and which variable its
parent bisected. The context matters — the same box behaves differently under a
tight incumbent than under `+∞`, because the bound `f(x) ≤ ymax` is what prunes.

### 3.3 The dive

```
                    node (already contracted)
                          |
              forced bisection on x_j          <-- the decision being scored
                   /              \
              left child        right child
                 |                   |
            LSmear decides      LSmear decides
                /   \               /   \
               ...   ...           ...   ...
```

Each box that comes out of a bisection is contracted (`contract_and_bound`:
the `y ≤ ymax` bound, then HC4/ACID/X-Newton, then upper bounding) and then
classified:

| | |
|---|---|
| **pruned** | the box became empty — refuted or dominated by the incumbent |
| **eps** | it survived but is below the precision threshold, so it cannot be split; it certifies a lower bound on the objective |
| **open** | it survived and gets bisected in turn |

`nodes` counts every box that went through that contraction, which is the
natural notion of "work". Both children are explored, because following a
single branch is noisy: one side may collapse by luck while the other is a
nightmare, and the decision you are scoring is the *pair*.

Siblings are explored best-first (smallest lower bound on `y` first), which is
the spirit of the diving strategies in IbexOpt's `CellBeamSearch`.

### 3.4 Budgeting, pruning and censoring

A dive stops when the subtree is exhausted **or** when its budget runs out. In
the second case `censored` is true and the label is a *lower bound*,
`y >= budget_used`, not a value.

Giving every candidate the same fixed budget wastes most of it and produces
nothing useful at the ceiling:

```
ex3_1_1.bch, root, fixed budget 40:
  y     = [40, 40, 40, 40, 40, 40, 40, 40, 40]     <- nine ties, no information
  cens  = [ 1,  1,  1,  1,  1,  1,  1,  1,  1]
```

Two mechanisms fix that, both on by default.

**Mutual pruning** (`prune`, `--no-prune` to disable). The candidates are
evaluated in order of promise, and once one has closed its dive in `k` nodes
the rest run with a budget of `k`. A dive that reaches `k` is already known not
to be better, and a *ranking* does not care by how much. This is branch and
bound applied to the data collection itself: cost falls from
`|C| x budget` to roughly `|C| x min_j y_j`, and the censoring lands exactly
where it costs nothing — on the losers.

**Adaptive budget** (`budget_start`, `--budget-start 0` to disable). Choosing
`budget` blind is the worst of both worlds: too small and everything ties at the
ceiling, too large and easy nodes are billed for work nobody reads. Instead the
most promising candidate is probed alone with a budget that doubles until its
dive closes, and that result then caps everyone else. Only that one candidate is
replayed, so not knowing the right budget in advance is paid once rather than
`|C|` times.

Together, on `ex2_1_1.bch` at the root with a ceiling of 200:

```
 no pruning      y = [170, 140, 182, 180, 164, 152]   988 dive nodes
 pruning         y = [170, 140, 140, 140, 140, 140]   866 dive nodes
                 cens= [ 0,   0,   1,   1,   1,   1]
```

Both rank variable 0 first. The pruned run simply stops paying to learn that
182 and 180 are worse than 140.

**What this means for training.** Most labels are censored by construction, and
that is the intended outcome, not a defect. Do not regress `y`. Use the ordered
pairs the sample actually licenses — `pairs()` in `ibexml.py` derives them,
including the "finished vs censored" ones — and feed a pairwise or listwise
ranking loss. Even when every dive is censored, `open` and `lower_bound` still
separate the candidates.

### 3.5 The admissible set

LSmear does not choose from "the variables of the box". It chooses from that
list *after* its own guards: domains below the precision threshold or no longer
bisectable, domains too narrow relative to their magnitude, and — the one that
matters most — the objective variable `y`, which it keeps only when the LP
optimum is strictly above the variable's lower bound, and never on its own.
When an entry of the Jacobian is infinite it does not choose at all and hands
over to `OptimLargestFirst`.

Those guards are not tuning. On `ex8_1_2.bch`, a rule allowed to pick `y` at the
root splits the objective in half forever: **250 000 nodes and counting where
LSmear finishes in 14**. The gap between a competitive learned rule and one that
never terminates is there, not in the quality of the ranking.

So a learned rule scores **the same admissible set**, and when the guards leave
nothing choosable the decision goes back to the bisector. The set is what
`candidates` returns, what the dives are run over, and what the node features
report as `admissible`.

Two consequences worth stating:

* `candidates` comes back in the **default heuristic's order of preference**,
  not in index order. A model whose scores tie therefore falls back on what the
  hand-written rule would have taken. That is what makes
  `python/test_admissible.py` pass: a model with a constant score terminates on
  all 96 `benchs/optim/easy` instances the default rule closes in under 1000
  nodes. Ordering by index instead — "the first candidate" read literally —
  fails 13 of them, because always splitting the lowest-indexed variable is not
  a bad ranking, it is a non-terminating search.
* `--no-goal` and `--topk` are applied *on top of* the guards, as a further
  restriction, and they apply to every path: dives, sampling, `model_var`,
  `--collect --model`, `--solve --model` and `--run`.

---

## 4. Quick start

Collect a small dataset:

```bash
python3 python/collect_dataset.py benchs/optim/easy/ex2_1_1.bch \
    -o data/train.jsonl --budget 200 --sample-prob 0.2 --max-samples 50
```

Look at one sample:

```python
import json, sys
sys.path.insert(0, "python")
from ibexml import encode, labels

s = json.loads(open("data/train.jsonl").readline())
Xv, Xc, E = encode(s["node"])        # (n+1, f_v), (m, f_c), (m, n+1, 2)
y, mask   = labels(s, Xv.shape[0])   # per-variable dive size + candidate mask
print(s["node"]["bisect_var"], y[mask])
```

Talk to the server by hand:

```bash
printf '{"cmd":"info"}\n{"cmd":"quit"}\n' | ./build/bin/ibexopt-ml problem.bch
```

---

## 5. The four modes

### 5.1 Batch: `--collect`

The search runs inside C++ and writes one JSON sample per line. Fastest way to
get a dataset, and the trajectory is exactly the one `ibexopt` follows.

```bash
./build/bin/ibexopt-ml problem.bch --collect -o data.jsonl \
    --budget 200 --sample-prob 0.2 --max-samples 200 --progress
```

At every node about to be bisected a coin is flipped; on heads the node becomes
a sample. Sampling is a no-op on the search — measured on `ex6_1_2.bch`:

| run | objective enclosure | nodes |
|---|---|---|
| `ibexopt` | `[-0.032487633, -0.032455145]` | 30 |
| `--collect --sample-prob 0` | same | 30 |
| `--collect --sample-prob 0.3` | same | 30 |
| `--collect --sample-prob 0.6` | same | 30 |

Reaching `--max-samples` stops the *sampling*, not the search: cutting the run
short would bias the dataset towards the top of the tree.

With `--model`, the search branches with the model instead of LSmear, which is
how you collect on-policy data at full speed.

### 5.2 Interactive: `--run`

The solver runs the real search — its own node selection (`CellBeamSearch`),
contraction and bounding — and stops at each bisection to ask who to branch on.
One message per decision instead of contract + features + bisect, and the node
counts are directly comparable with `ibexopt`.

Server to client, one line:

```json
{"event":"bisect","nodes":12,"depth":3,"last_bisected_var":1,
 "loup":-16.5,"uplo":-76.06,"open":4,"candidates":[0,1,2,3,4,5],
 "node":{ ...features... }}
```

Client to server, one line:

```json
{"var": 2}                  // branch on x2
{"var": null}               // let LSmear decide
{"var": 2, "sample": true}  // emit a full sample here first, then branch on x2
{"cmd": "stop"}             // end the search
```

A requested sample arrives as its own `{"event":"sample","sample":{...}}` line
before the next `bisect`, so the client loop must read events until it sees the
one it has to answer. The run ends with
`{"event":"done","status":...,"loup":...,"uplo":...,"nodes":...,"time":...}`.

`--no-features` drops the `node` field when the rule does not need it.
`run()` in `ibexml.py` wraps all of this.

### 5.3 Timing: `--solve`

Runs to completion under the current rule and prints one JSON line. This is the
only measurement that settles whether a learned rule is an improvement: fewer
nodes at a higher price per node is not.

```bash
./build/bin/ibexopt-ml problem.bch --solve                       # LSmear
./build/bin/ibexopt-ml problem.bch --solve --model my.model      # the learned rule
```

```json
{"status":"complete","loup":-17,"uplo":-17.017,"nodes":166,"time":0.136,"rule":"LSmear"}
```

### 5.4 Server: the default

Reads one JSON command per line on stdin, answers one per line on stdout.
Everything is built once at startup — parsing the Minibex file, normalizing the
system, constructing HC4/ACID/X-Newton and the LP solver — and every command
reuses it, so **keep the process alive**.

Use it when Python owns the search and you want per-node control. Note that its
`search()` helper selects nodes best-first, unlike the solver's own buffer: for
comparable node counts use `--run` instead.

## 6. Command-line reference

```
ibexopt-ml [options] problem.bch
```

### Mode

| flag | meaning |
|---|---|
| *(none)* | JSON server on stdin/stdout |
| `--collect` | run the search and write samples |
| `--run` | run the search, ask the client for every branching variable |
| `--solve` | run to completion and report nodes and time |
| `-o, --output FILE` | where samples go in `--collect` mode (default: stdout) |

### The branching rule

| flag | default | meaning |
|---|---|---|
| `--model FILE` | LSmear | branch with this model, evaluated inside the solver |
| `--bisector NAME` | `lsmear` | a hand-written rule instead (see §15) |

### The rest of the strategy

Two axes that are not the branching rule but change every node count, so a run
under one is not comparable with a run under another:

| flag | default | meaning |
|---|---|---|
| `--relax NAME` | `xtaylor` | the linear relaxation the X-Newton contraction is built on: `xtaylor` (what `ibexopt` uses), `affine` (affine arithmetic), `both` |
| `--loup NAME` | `default` | upper bounding: `default` (what `ibexopt` uses), or `ipoptprob`, `ipoptxn`, `ipoptxninhc4` |
| `--ipopt-freq INT` | `100` | Ipopt runs every N calls to the loup finder, plus the 10th, 20th and 50th, plus whenever another finder improves the incumbent |
| `--ipopt-qp` | off | quadratic objective and linear constraints: the Lagrangian hessian is then computed once instead of per call |

The `ipopt*` names exist only in a build configured with
`-DIBEX_WITH_IPOPT=ON`; otherwise the solver rejects them and says so. They are
worth the dependency: Ipopt returns a local minimum, so the incumbent arrives
early -- on `hs071` the root node already holds a point within 1e-5 of the
optimum, and the dive under the same variable falls from 200 nodes to 14.

### Diving and sampling

| flag | default | meaning |
|---|---|---|
| `--budget INT` | `200` | ceiling on the dive node budget (`0`: no limit) |
| `--budget-start INT` | `25` | first budget probed, doubling until the dive closes; `0` goes straight to `--budget` |
| `--no-prune` | off | give every candidate the full budget instead of the best result so far |
| `--max-depth INT` | `0` | dive depth limit (`0`: none) |
| `--topk INT` | `0` | evaluate only the k most promising candidates (`0`: all) |
| `--no-goal` | off | do not treat the objective variable as a candidate |
| `--sample-prob FLOAT` | `0.1` | probability a visited node becomes a sample |
| `--max-samples INT` | `1000` | stop *sampling* after that many; the search continues (`0`: no limit) |

### Search control

| flag | default | meaning |
|---|---|---|
| `--max-nodes INT` | `0` | stop the search after that many nodes |
| `-t, --timeout FLOAT` | none | time limit, in seconds |
| `--no-features` | off | in `--run` mode, send decisions without the node features |
| `--progress` | off | progress on stderr |

### Optimizer settings (same meaning as in `ibexopt`)

| flag | default | meaning |
|---|---|---|
| `-r, --rel-eps-f FLOAT` | `1e-3` | relative precision on the objective |
| `-a, --abs-eps-f FLOAT` | `1e-7` | absolute precision on the objective |
| `--eps-h FLOAT` | `1e-8` | equality relaxation |
| `--eps-x FLOAT` | `0` | precision on the variables |
| `--random-seed FLOAT` | `1` | seed; also the seed every dive restarts from |
| `--simpl INT` | `1` | expression simplification level (0–3) |
| `--initial-loup FLOAT` | `+∞` | a priori upper bound of the minimum |
| `--rigor` / `--kkt` | off | rigor mode / Kuhn-Tucker contractor |

## 7. Data format

### 7.1 A sample

One JSON object per line:

```json
{"format":  2,
 "node":    { ... features ... },
 "budget":  200,
 "max_depth": 0,
 "labels":  [ { "var": 0, "nodes": 12, "censored": false, ... }, ... ],
 "instance": "ex2_1_1.bch"}
```

`instance` is added by `collect_dataset.py`, not by the binary.

`format` is the layout version, and it exists because two vintages of this
dataset have the same *shape* and different *meaning*. A sample written before
version 2 carries no `phi` and no `admissible`, so its comparative features can
only be rebuilt over every variable instead of over the admissible candidates —
same 30 columns, same names, a different quantity in fifteen of them. Stitch the
two together and you train on a mix without noticing. Read datasets with
`load_samples()`, which checks the version and raises; `encode()` warns
separately if it is ever handed a node with no `phi`.

`labels` has one entry per *candidate* variable, i.e. per variable that is
bisectable and above the precision threshold in this box. Variables that are
not candidates simply do not appear — use the `mask` returned by `labels()`
rather than assuming a fixed length.

### 7.2 `node` — the features

**Context**

| field | type | meaning |
|---|---|---|
| `box` | `[[lb,ub]] × (n+1)` | the node itself |
| `depth` | int | depth in the search tree, `-1` if not supplied |
| `last_bisected_var` | int | variable the parent bisected, `-1` at the root |
| `loup` | float | current incumbent (`Infinity` if none yet) |
| `uplo` | float | current lower bound on the minimum |
| `ymax` | float | the bound actually enforced on `y`, i.e. the incumbent reduced by the required precision (anticipated upper bounding) |

**`vars[j]`** — one entry per extended variable, in index order

| field | meaning |
|---|---|
| `index` | `j` |
| `lb`, `ub`, `diam`, `mid`, `mag` | the domain |
| `rel_diam` | `diam/mag`, or `diam` when `mag == 0` |
| `is_goal` | `j == goal_var` |
| `bisectable` | the domain can be split at all |
| `too_small` | below the precision threshold, or not bisectable — i.e. not a candidate |
| `eps` | the precision threshold for this variable |
| `dual` | LP dual of this variable's bound (`NaN` when `dual_ok` is false) |

**`ctrs[i]`** — one entry per extended constraint

| field | meaning |
|---|---|
| `index` | `i` |
| `op` | `"LT"`, `"LEQ"`, `"EQ"`, `"GEQ"`, `"GT"` |
| `is_goal_ctr` | this is the constraint `y = f(x)` |
| `f_lb`, `f_ub` | interval evaluation of `f_i` on the box (the activity); `null` if the evaluation failed |
| `entailed` | an inequality already satisfied over the whole box (`f_ub ≤ 0`) |
| `dual` | LP dual of the constraint (`NaN` when `dual_ok` is false) |

**Jacobians and duals**

| field | meaning |
|---|---|
| `J` | `m × (n+1)` array of `[lb,ub]`: the Jacobian over the box. `null` if the evaluation failed |
| `J_mid` | same shape, Jacobian at the midpoint inflated by `1e-8` — this is the matrix `LSMEAR_MG` linearizes to get the duals |
| `dual_ok` | the LP relaxation solved to optimality. When false, every `dual` is `NaN` and `scores.lsmear` is `[]` |
| `admissible` | the variables the bisector would actually consider ([§3.5](#35-the-admissible-set)), in index order — also the set the comparative features are computed over |
| `model_fallback` | the guards left nothing choosable, so the decision belongs to the bisector |
| `phi` | `(n+1) x 30`: the model input vector, exactly as a C++-scored model receives it |

**Scores of the hand-written heuristics** (`scores`, or `null` if `J` failed)

| field | formula |
|---|---|
| `smear_sum[j]` | `Σ_i \|J[i][j]\| · diam(x_j)` |
| `smear_max[j]` | `max_i \|J[i][j]\| · diam(x_j)` |
| `smear_sum_rel[j]` | `Σ_i \|J[i][j]\|·diam(x_j) / NC_i`, with `NC_i = Σ_k \|J[i][k]\|·diam(x_k)` |
| `lsmear[j]` | `\|λ_j + Σ_i λ_i · J[i][j]\| · diam(x_j)` — **the quantity LSmear maximizes** |

**Decisions** (the imitation targets)

| field | meaning |
|---|---|
| `bisect_var` | **what the bisector actually does**, fallbacks to `OptimLargestFirst` included. `null` if the box cannot be bisected. This is the target to imitate |
| `bisect_pos`, `bisect_rel_pos` | where it splits (`0.5`, relative, with this configuration) |
| `lsmear_var` | raw `LSmear::var_to_bisect`; `-1` when LSmear declines and hands over to the fallback |
| `smear_sum_rel_var` | what plain `SmearSumRelative` would pick |

> **A warning about `scores.lsmear`.** It is, up to a `max`, the decision
> itself. Feed it to a model and imitating LSmear becomes trivial — and you
> learn nothing about whether the model found anything better.
> `encode(node, lsmear_score=False)` drops it. Train both ways: the gap between
> them tells you what the model actually contributed.

`phi` deserves a word. It is the vector a model scored *inside* the solver is
fed, and it is shipped rather than left to be rebuilt by the client — the
comparative columns involve divisions, and the same formula evaluated under a
different floating-point environment comes out an ulp apart. `encode()` reads it
back, so the two sides cannot drift: verified bit for bit against
`features_vector()` on every instance tried.

`vars`, `ctrs` and `J` are deliberately **not** pre-aggregated into a flat
per-variable feature vector. They are the bipartite variable ↔ constraint
structure, with the Jacobian as the edges — the same shape learning-to-branch
models for MILP use, and the one an attention model wants: attend over
variables, cross-attend to constraints through `J`. Any aggregate you want
(smear and friends included) is derivable from them, and the aggregates are
provided anyway so you do not have to re-derive them correctly.

### 7.3 `labels[k]` — the dives

| field | type | meaning |
|---|---|---|
| `var` | int | the variable whose bisection is being scored |
| `valid` | bool | `false` if the variable turned out not to be bisectable; every counter is then 0 |
| **`nodes`** | int | **the target**: boxes contracted in the dive |
| `budget_used` | int | the budget this particular dive ran under — under pruning it differs from candidate to candidate |
| `censored` | bool | the dive was cut; `nodes == budget_used` and the truth is `y >= budget_used` |
| `depth_limited` | bool | the depth limit specifically was hit |
| `pruned`, `eps`, `open` | int | leaves refuted / too small / left unexplored (`censored == (open > 0)`) |
| `max_depth` | int | deepest node reached; the forced bisection's children are at depth 1 |
| `lower_bound` | float | objective lower bound the dive proves over the subtree; `Infinity` iff it was entirely refuted |
| `loup_before`, `loup_after`, `found_loup` | | whether the dive improved the incumbent |
| `left`, `right`, `left_status`, `right_status` | box, string | the depth-1 children after contraction — a one-step label, `budget/2` times cheaper |
| `time` | float | CPU seconds |
| `lp_calls` | int | LP solves by the *bisector* during the dive (see the caveat below) |

The sample also carries `budget` (the budget of the probe round), `budget_cap`
(what `--budget` allowed), `rounds` (doubling rounds), `pruned_budget`, `topk`,
`dive_nodes` and `dive_time` — the total cost of producing it.

**Work units.** `nodes` is a biased proxy for effort: a node killed by HC4 costs
far less than one that goes through the X-Newton fix-point. `time` is the
unbiased measure and is recorded per dive. `lp_calls` counts only the
*bisector's* LP solves; X-Newton's are not counted, because `CtcLinearRelax`
does not implement Ibex's statistics interface. If your objective is wall-clock
time, rank on `time`; if it is tree size, rank on `nodes`.

**Comparability.** All the labels of one node are exactly comparable: same
incumbent, same random stream, same contractor tuning (see
[§13](#13-how-it-works-inside)). They are *not* comparable across nodes
— `nodes` spans orders of magnitude with depth.

## 8. Wire protocol reference

One JSON object per line in, one per line out. Every answer has `"ok"`. On
failure it is `false` and `"error"` says why:

```json
{"ok":false,"error":"unknown command 'nope'"}
```

The examples below are real output from
`./build/bin/ibexopt-ml benchs/optim/easy/ex6_1_2.bch`, with long boxes elided
and floats shortened for readability — the binary always prints 17 significant
digits so that every value round-trips through an IEEE-754 double.

### `info`

Static description of the problem. No arguments.

```json
{"ok":true,"info":{"nb_var":4,"nb_ext_var":5,"nb_ext_ctr":7,"goal_var":4,
 "goal_ctr":0,"nb_ctr":3,"var_names":["x2","x3","x4","x5","__goal__"],
 "ops":["EQ","LEQ","LEQ","LEQ","LEQ","LEQ","LEQ"],"eps_x":[0,0,0,0],
 "rel_eps_f":0.001,"abs_eps_f":1e-07,"eps_h":1e-08,"rigor":false,"inHC4":true,
 "kkt":false,"random_seed":1,"bisect_ratio":0.5,"lsmear_mode":"LSMEAR_MG",
 "root_box":[[1e-06,0.99999901],[1e-06,0.99999901],[4.436e-06,1.15604],
             [2.754e-06,1.020853],[-4.0979784,1.8709829]],
 "loup":Infinity,"uplo":-Infinity}}
```

`var_names` is in the **extended** order, with `__goal__` inserted at
`goal_var`. `root_box` is the root **already contracted** — the server does a
`reset` on the problem's declared domain at startup.

### `reset`

| argument | default | meaning |
|---|---|---|
| `box` | the problem's declared domain | restart from this box (size `n` or `n+1`) |
| `loup` | `Infinity` | initial incumbent |

Resets the incumbent, the bounds and the cell buffer, then contracts the root.

```json
{"ok":true,"root":[[1e-06,0.99999901], ... ],"loup":Infinity}
```

> Note `root` is the *contracted* root, so passing it back to `reset` would
> contract it a second time rather than start over. `reset` with no `box` is
> the idempotent thing to do.

### `contract`

| argument | default | |
|---|---|---|
| `box` | — | required, size `n+1` |
| `keep_loup` | `true` | `false` discards an incumbent found while upper bounding |

```json
{"ok":true,"status":"open",
 "box":[[1e-06,0.99999901], ... ,[-4.0979784,1.2740868]],
 "eps_lb":Infinity,"loup_before":Infinity,"loup_after":Infinity,
 "time":0.001038}
```

`status` is `open`, `pruned` or `eps`. `box` is `null` unless `open`. When the
status is `eps`, `eps_lb` is the objective lower bound that tiny box certifies —
the server hands it back instead of recording it, because in server mode the
client owns the cell buffer and therefore the `uplo` bookkeeping.

### `features`

| argument | default |
|---|---|
| `box` | required |
| `depth`, `last_bisected_var` | `-1` |

Returns `{"ok":true,"node":{...}}` — see [§7.2](#72-node--the-features).

### `choose_var`

What LSmear would do. `null` everywhere if the box cannot be bisected.

```json
{"ok":true,"var":0,"pos":0.5,"rel_pos":true}
```

### `bisect`

| argument | default | |
|---|---|---|
| `box` | — | required |
| `var` | `-1` | `-1` lets LSmear decide |
| `pos`, `rel_pos` | the default point | where to split |

Children are returned **uncontracted** — call `contract` on each.

```json
{"ok":true,"var":0,
 "left":[[1e-06,0.500000005], ... ],
 "right":[[0.500000005,0.99999901], ... ]}
```

### `candidates`

| argument | default |
|---|---|
| `box` | required |
| `include_goal` | `true` |
| `topk` | `0` (all) |

```json
{"ok":true,"vars":[0,1,2,3,4]}
```

### `model_var`

The variable the model loaded with `--model` picks. `null` when there is no
model or no candidate.

| argument | default |
|---|---|
| `box` | required |
| `include_goal`, `topk` | the command-line values |

### `features_vector`

The per-variable matrix a C++ model is scored on, `(n+1) x nb_features`,
already sanitized. Use it to check that a model scored in Python and the same
model scored in C++ agree.

```json
{"ok":true,"nb_features":15,"phi":[[...],[...]]}
```

### `dive`

| argument | default | |
|---|---|---|
| `box` | — | required; assumed already contracted |
| `var` | — | required |
| `budget` | `100` | max nodes; `0` = no limit |
| `max_depth` | `0` | `0` = no limit |

```json
{"ok":true,"dive":{"var":0,"valid":true,"nodes":30,"pruned":9,"eps":0,"open":7,
 "max_depth":12,"censored":true,"depth_limited":false,
 "lower_bound":-1.3291033,"loup_before":Infinity,"loup_after":-0.0324594,
 "found_loup":true,"left_status":"open","right_status":"open",
 "left":[...],"right":[...],"time":0.039989}}
```

The search state is restored afterwards, so repeated calls on the same box
return the same thing.

### `sample`

`features` plus a `dive` for every candidate, in one round trip.

| argument | default |
|---|---|
| `box` | required |
| `budget`, `budget_start`, `prune`, `max_depth`, `topk`, `include_goal` | the command-line values |
| `depth`, `last_bisected_var` | `-1` |

Returns `{"ok":true,"sample":{...}}` — see [§7.1](#71-a-sample).

### `set_loup` / `state`

```json
{"ok":true,"loup":Infinity,"uplo":-Infinity,"nb_cells":0}
```

### `quit`

Terminates the process. **No answer is sent** — do not wait for one.

---

## 9. Python API

`python/ibexml.py`, standard library only (plus `numpy` for the encoders).

### `IbexOptML(bch, binary=None, **opts)`

A running server; use it as a context manager. Constructor options map to the
CLI flags (`initial_loup`, `random_seed`, `eps_x`, `rel_eps_f`, `abs_eps_f`,
`eps_h`, `rigor`, `kkt`, `simpl`, `extra_args`). Attributes: `info`,
`nb_ext_var`, `nb_ext_ctr`, `goal_var`, `goal_ctr`.

| method | returns |
|---|---|
| `reset(box=None, loup=INF)` | `{"root", "loup"}` |
| `contract(box, keep_loup=True)` | status, contracted box, `eps_lb`, incumbent, time |
| `features(box, depth=-1, last_bisected_var=-1)` | the `node` dict |
| `features_vector(box)` | the `(n+1, nb_features)` matrix a C++ model sees |
| `choose_var(box)` | `(var, pos, rel_pos)` — LSmear's decision |
| `model_var(box, include_goal=True, topk=0)` | the loaded model's decision, or None |
| `bisect(box, var=None, pos=None)` | `(var, left, right)`, children not contracted |
| `candidates(box, include_goal=True, topk=0)` | list of ints |
| `dive(box, var, budget=100, max_depth=0)` | the `dive` dict |
| `sample(box, budget=200, budget_start=25, prune=True, max_depth=0, include_goal=True, topk=0, ...)` | the `sample` dict |
| `set_loup(loup)` / `state()` | the incumbent / the full state |

Any `{"ok": false}` answer is raised as `IbexError`.

### `run(bch, select_var, ...)` — your rule, the solver's search

```python
def my_rule(node, candidates, state):
    # node: the feature dict (None with features=False)
    # state: nodes, depth, last_bisected_var, loup, uplo, open
    return int(model(node).argmax())     # or None to let LSmear decide

done = run("problem.bch", my_rule, sample_prob=0.1, on_sample=collect)
# {'event': 'done', 'status': 'complete', 'loup': ..., 'nodes': 30, 'time': ...}
```

The node selection, contraction and bounding stay inside the solver, so `nodes`
is comparable with `ibexopt`. `sample_prob` also asks for a full sample at a
fraction of the nodes — that is on-policy data, collected while *your* rule
drives. `sample_when(state) -> bool` replaces the coin when you need an exact
quota or a depth-based rule; returning False simply stops sampling, it does not
stop the search. Other arguments: `max_nodes`, `timeout`, `features`,
`include_goal`, `topk`, `budget`, `budget_start`, `prune`, `max_depth`,
`random_seed`, `initial_loup`, `progress`, `rng`.

### `solve(bch, model=None, ...)` — wall-clock time

```python
solve("problem.bch")                      # LSmear
solve("problem.bch", model="my.model")    # the learned rule, scored in C++
# {'status': 'complete', 'loup': -17, 'uplo': -17, 'nodes': 166,
#  'time': 0.136, 'rule': 'LSmear'}
```

### `search(srv, select_var=None, ...)` — a branch & bound in Python

Best-first on the objective lower bound. Useful when you want to own the cell
buffer and experiment with node selection; **not** comparable with `ibexopt` on
node counts — use `run()` for that.

### `encode(node, scores=True, lsmear_score=True, node_features=True, derived=True, clip=1e12)`

Returns `(X_var, X_ctr, E)` as float64 arrays: `X_var` is `(n+1, 30)`,
`X_ctr` is `(m, 10)` (5 fields plus a one-hot operator), `E` is `(m, n+1, 2)` —
the Jacobian as `[lb, ub]` per edge. Infinities are clipped to `±clip` and
`NaN`s sent to 0, exactly as the solver does, so a model trained on this output
can be scored inside C++ with no glue. With the default flags it returns the
solver's own `phi`, so it is identical to `features_vector()` bit for bit.

`lsmear_score=False` drops the one feature that makes imitating LSmear trivial,
and its three comparative columns with it. That breaks the correspondence with
the C++ scorer on purpose: use it to probe what the model learned, not to train
something you will export.

### `load_samples(path, strict=True)`

Iterates the samples of a `.jsonl` and checks their layout version against
`SAMPLE_FORMAT`. Use it instead of a bare `json.loads` loop: it is the one place
that notices a dataset stitched together from two builds. `strict=False` warns
and yields anyway. `sample_format(sample)` returns the version of one sample
(1 for anything predating the field).

### `labels(sample, n_ext, censored_as=None)` and `pairs(sample)`

`labels` returns `(y, mask, censored)`. `pairs` returns the ordered
`(better, worse)` pairs the sample actually licenses, censoring included: a
finished dive beats a larger finished dive, and beats a dive censored at or
above its own size; two censored dives say nothing. That is the supervision
signal to train on.

### `export_linear`, `export_trees`, `export_sklearn`

Write a model the solver evaluates itself:

```python
export_linear("my.model", w, bias=0.0)     # len(w) == len(MODEL_FEATURES)
export_sklearn("my.model", gbr)            # sklearn tree / forest / gradient boosting
```

`MODEL_FEATURES` is the exact feature order, which is also `encode()`'s column
order — 30 columns, in three blocks:

```
 0-9   lb ub diam mid mag rel_diam is_goal bisectable too_small dual
10-14  smear_sum smear_max smear_sum_rel lsmear dual_ok
15-29  <base>_rank <base>_relmax <base>_relsum
       for base in lsmear, smear_sum, smear_sum_rel, dual, rel_diam
```

`dual_ok` matters: `dual` and `lsmear` are set to 0 when the LP relaxation did
not solve, and without the flag a model cannot tell that from a genuine zero —
which is precisely where LSmear hands over to its fallback.

**The comparative block (15–29) is the point.** Choosing a branching variable is
a comparison between the candidates of the node, and a strictly per-variable
vector cannot express that — which would confine any comparative model to
Python, behind a JSON round trip per node, losing on wall-clock time whatever it
gained in nodes. So the solver derives, over the admissible candidates `C`:

| | |
|---|---|
| `rank` | `\|{i in C : v_i < v_j}\| / (\|C\|-1)`, or `0.5` when `\|C\| == 1` |
| `relmax` | `v_j / max_{i in C} \|v_i\|` (0 if that max is 0) |
| `relsum` | `v_j / sum_{i in C} \|v_i\|` (0 if that sum is 0) |

Variables outside `C` keep 0; they are never scored. One linear pass over a list
that already exists, negligible next to a contraction — and a plain exportable
GBDT suddenly sees the context it needs.

Monotone per-feature transforms (log, affine rescaling) are deliberately *not*
provided: trees are invariant to them and a linear model folds them into its
weights. The comparative features are the only thing that cannot be recovered
from a single variable's vector.

## 10. Training: a worked path

### Step 1 — collect

Spread over many instances, not many nodes of one instance; the variation you
want is across problems.

```bash
python3 python/collect_dataset.py benchs/optim/easy/*.bch \
    -o data/train.jsonl --budget 200 --sample-prob 0.15 --max-samples 100
python3 python/collect_dataset.py benchs/optim/medium/*.bch \
    -o data/dev.jsonl   --budget 200 --sample-prob 0.15 --max-samples 50
```

### Step 2 — load

```python
import sys
sys.path.insert(0, "python")
from ibexml import load_samples, encode, labels, pairs

def load(path):
    for s in load_samples(path):          # checks the layout version
        Xv, Xc, E = encode(s["node"], lsmear_score=False)
        y, mask, cens = labels(s, Xv.shape[0])
        pr = pairs(s)                       # ordered pairs, censoring aware
        if not pr:                          # nothing to rank at this node
            continue
        yield Xv, Xc, E, y, mask, cens, pr, s["node"]["bisect_var"]

rows = list(load("data/train.jsonl"))
```

Shapes differ from instance to instance — on a three-problem sample `(n+1, m)`
was `(6, 2)`, `(9, 7)` and `(5, 7)`. **Your model must handle variable-length
sequences on both sides.** That is what attention buys you, and why a
fixed-width MLP over a flattened box is a dead end here.

### Step 3 — model

Variables are tokens, constraints are a second set of tokens, and the Jacobian
is the valued adjacency between them, so it belongs in the attention bias
rather than as a plain mask:

```
h_v  = Linear(X_var)                     # (n+1, d)
h_c  = Linear(X_ctr)                     # (m, d)
h_c  = h_c + CrossAttention(h_c, h_v, bias=g(E))      # constraints read variables
h_v  = h_v + CrossAttention(h_v, h_c, bias=g(E^T))    # variables read constraints
h_v  = h_v + SelfAttention(h_v)          # variables compare to each other
score = Linear(h_v).squeeze(-1)          # (n+1,)
```

The self-attention over variables is the part that matters: choosing a
branching variable is a *comparison*, not an independent per-variable
judgement. Mask out `~mask` before the loss and before the argmax.

*(Sketch, not runnable code — the architecture is yours to pick.)*

### Step 4 — two objectives, in this order

1. **Imitate LSmear.** Cross-entropy on `bisect_var` over the masked
   candidates. Cheap, needs no dives at all, and it tells you the features and
   the architecture are adequate before you spend anything on labels.

2. **Beat LSmear.** A pairwise ranking loss over `pairs(sample)`, not a
   regression on `y`: under pruning most labels are lower bounds, and the scale
   of `y` changes by orders of magnitude between nodes. Only the order matters.

There is room between the two. On 15 sampled nodes of `ex2_1_1.bch`, LSmear
picked the smallest *measured* dive in 11 of them.

### Step 5 — evaluate on wall-clock time, not accuracy

Accuracy against LSmear is a proxy; node count is a better one; time is the
thing. Export the model and let the solver score it:

```python
from ibexml import export_sklearn, solve

export_sklearn("my.model", gbr)
for bch in held_out:
    base    = solve(bch)
    learned = solve(bch, model="my.model")
    print(bch, base["nodes"], learned["nodes"], base["time"], learned["time"])
```

Why this matters, measured: a hand-made linear model that simply takes the
argmax of the `lsmear` feature *looks* like LSmear, but on `ex2_1_1.bch` it does
not terminate in 60 s (107k nodes) where LSmear finishes in 0.14 s (166 nodes) —
because LSmear is not an argmax. It has guards on tiny domains, a special rule
for the objective variable, and a fallback when the LP fails. Counting nodes on
paper would never have shown that; `--solve` shows it in one command.

## 11. Closing the loop (DAgger)

A dataset collected while LSmear branches only covers the nodes **LSmear**
visits. The moment your model branches, it lands in nodes it has never seen, and
its accuracy there is unrelated to its accuracy on the training set. This is the
standard imitation-learning distribution shift, and it is the reason the server
exists.

The fix is to iterate:

Round 2 is one command. Put your rule in a module:

```python
# mymodel.py
def choose(node, candidates, state):
    Xv, Xc, E = encode(node)
    scores = model(Xv, Xc, E)
    return max(candidates, key=lambda j: scores[j])
```

and collect under it:

```bash
python3 python/collect_dataset.py benchs/optim/easy/*.bch \\
    -o data/round2.jsonl --rule mymodel:choose --sample-prob 0.15
```

`--rule` implies `--driver run`, so the node selection, the contraction and the
bounding still happen inside the solver — only the branching comes to Python.
That matters: a driver that reimplements node selection visits a *different*
tree, so its "on-policy" data is on-policy for a search nobody will run. If the
model is exportable, `--model my.model` does the same thing without the round
trip.

The branching is done by the model; the *labels* still come from dives, which
are ground truth regardless of who is branching. Retrain on the union of all
rounds.

---

## 12. Performance and tuning

With pruning and adaptive budgeting on, a sample costs roughly
`|candidates| x min_j y_j` contractions instead of `|candidates| x budget`.

Measured on `ex2_1_1.bch` (5 variables, 6 extended candidates), `--budget 200`:

| | |
|---|---|
| one dive of 30 nodes | 0.021 s |
| one sample at the root (1 926 dive nodes, 4 doubling rounds) | 0.80 s |
| `--collect`, 15 samples **plus the complete search** | 0.71 s |

Knobs, roughly in order of effectiveness:

* **`--topk k`** — the cost is linear in the number of candidates, and that is
  what puts problems like `ackley100` or `alpine100` out of reach. Ranking by
  the LSmear score and keeping the best few is usually enough.
* **`--budget`** — a ceiling now, not a per-dive cost, so it can be set
  generously; `--budget-start` finds the working value on its own.
* **`--no-goal`** — one candidate fewer, and the objective variable is rarely
  the interesting decision.
* **`--sample-prob`** — sample fewer nodes of many instances rather than many
  nodes of one; nodes from the same subtree are highly correlated.
* **`--collect` over a Python driver** — the Python side pays a JSON round trip
  per node. Fine for DAgger rounds, wasteful for bulk collection.

Collection over an instance set is embarrassingly parallel: one
`ibexopt-ml --collect` per instance, then concatenate.

## 13. How it works inside

### The class

```cpp
class MLNodeServer : protected DefaultOptimizerConfig,   // owns the operators
                     public Optimizer                    // owns the search state
```

Same pattern as `DefaultOptimizer`: the config is constructed first, so
`Optimizer`'s constructor pulls the contractor, bisector, loup finder and cell
buffer out of it — the same objects `ibexopt` gets. `new_cell()` builds a
`Cell` from an arbitrary extended box and attaches the four sets of box
properties, mirroring `Optimizer::start()`; that is what makes a box from the
client a legitimate node.

### Telling `pruned` from `eps`

`Optimizer::contract_and_bound()` empties the box in both cases. `process()`
recovers the distinction by setting `uplo_of_epsboxes` to `+∞` before the call:
`update_uplo_of_epsboxes()` only ever lowers that field, so a finite value
afterwards means "this box hit the precision criterion", and the value itself is
the objective lower bound it certifies. `uplo` is set to `-∞` for the duration
to neutralize two assertions that assume the box belongs to the current subtree.

### Making a dive a true no-op

Three pieces of state leak between calls, and all three have to be put back or
sampling changes the search it is supposed to observe.

**The incumbent.** `loup`, `uplo`, `uplo_of_epsboxes`, `loup_point`,
`loup_changed` and `nb_cells` are saved and restored.

**The random generator.** Every candidate of a node must run against the same
stream or their dive sizes are not comparable, so a dive starts with
`RNG::srand(random_seed)`. Restoring the caller's stream afterwards cannot use
`srand`: that function repositions the stream by *drawing* its argument, so
resuming at an arbitrary position costs O(seed) — with a 32-bit position that
is billions of iterations, and it dominated everything else. `RNG::get_state()`
and `RNG::set_state()` (added for this) save and restore the three-word state in
constant time. Removing that one call took a dive on `ex2_1_1.bch` from 1.44 s
to 0.021 s.

**The contractor's tuning.** `CtcAcid` adapts how many variables it shaves from
one call to the next, and that state is *not* part of the box. Left alone, the
same dive repeated five times gave `178, 170, 170, 170, 170`. With
`CtcAcid::get_tuning()` / `set_tuning()` (also added for this) saved and
restored around every dive, it gives `178` five times.

With all three in place, sampling is exactly a no-op: `--collect` with
`--sample-prob` 0, 0.3 and 0.6 all visit 30 nodes on `ex6_1_2.bch`, the same 30
`ibexopt` visits, with the same enclosure.

### Changes to existing Ibex files

All three are additive and documented in place; no public API changed.

| file | change |
|---|---|
| `src/optim/ibex_Optimizer.h` | the search state moved from `private` to `protected`, so a subclass can save and restore it |
| `src/tools/ibex_Random.h/.cpp` | `RNG::get_state()` / `set_state()` — constant-time save/restore of the generator |
| `src/contractor/ibex_CtcAcid.h` | `get_tuning()` / `set_tuning()` — save/restore of the adaptive shaving state |
| `src/strategy/ibex_Sts.h` | `calls()`, to read a call counter without parsing a report |

`MLNodeServer::collect()` duplicates the body of `Optimizer::optimize()` — minus
the reporting and the COV output, plus the sampling hook and the learned rule —
because there is no hook to attach to. If `Optimizer::optimize()` changes
upstream, that loop needs the same change; it is marked with a comment.

---

## 14. Known limitations

* **`search()` is not `ibexopt`.** It selects nodes best-first; `ibexopt` uses
  `CellBeamSearch` (feasible diving). Use `--run` / `run()` when node counts
  have to be comparable.
* **`lp_calls` undercounts.** It records the bisector's LP solves only;
  X-Newton's are invisible because `CtcLinearRelax` does not implement
  `enable_statistics`. Use `time` for an unbiased measure of effort.
* **Censoring is the norm, not an error.** Under mutual pruning most labels are
  lower bounds. Train on `pairs()`, not on `y`.
* **`dive` assumes its input box is already contracted.** It is, if the box came
  from `contract`, `reset`, a `sample` or the search itself.
* **A C++-scored model cannot attend over constraints.** It sees the 30
  features of `MODEL_FEATURES`, which include the comparative view over the
  node's candidates but nothing per-constraint. A model that needs the bipartite
  structure has to run in Python, through `--run`, and pay the round trip.
* **`--solve` measures one instance, one seed.** Compare over a set.
* **No batching in server mode.** One command, one round trip.
* **`Infinity` / `NaN` are not standard JSON.** Fine from Python, a nuisance
  from stricter parsers.

---

## 15. Comparing branching rules

### The instance set

`benchs/optim` holds seven overlapping directories. `benchs/optim/make_all.py`
merges the ones worth experimenting on — `easy`, `blowup`, `medium`, `hard`,
`others`, `unsolved`, `coconutbenchmark-library2` — into a single flat
`benchs/optim/all`, **298 instances**, and writes a `manifest.csv` mapping each
one back to the directory it came from, because that directory is the only
record of how hard it is.

The overlap is not trivial and a plain `cp` gets it wrong:

| case | example | what happens |
|---|---|---|
| same name, same bytes | `ex2_1_7.bch` in `blowup` and `medium` | one copy |
| same name, **different** formulation | `dualc1`, `ex2_1_7`, `ex2_1_8`, `launch` | both kept; the `coconut` one is suffixed `__coconut` |
| different name, same bytes | `easy/ex8_4inf-1.bch` = `easy/test_infinity1.bch` | one copy |

298 is exactly the number of distinct file contents across the seven
directories, and all 298 parse as optimization problems.

```bash
python3 benchs/optim/make_all.py            # --force to rebuild
```

The originals are left in place; only a copy is made.

### The rules

`--bisector NAME` selects the branching rule, everything else held fixed —
same contractor, same loup finder, same node selection, same seed, same
precision.

| name | |
|---|---|
| `lsmear` | LSmear, variant `LSMEAR_MG`. **What `ibexopt` uses**, and the baseline |
| `lsmear-box` | LSmear with the Jacobian taken over the box instead of at the midpoint |
| `smearsumrel` | `SmearSumRelative` — LSmear's own fallback, on its own |
| `smearsum` | `SmearSum` (Hansen) |
| `smearmax` | `SmearMax` (Kearfott) |
| `smearmaxrel` | `SmearMaxRelative` |
| `largestfirst` | `OptimLargestFirst`: widest domain, objective-variable aware |
| `roundrobin` | the naive baseline |
| `lsmear-guard` | `lsmear` until it is hijacked, then `roundrobin` for the rest of the search (see below) |

`lsmear-guard` watches LSmear's last 20 decisions; once at least 10 are
recorded and half of them bisect the variable the cell came from, it hands over
to RoundRobin and never goes back. Until then its trajectory is `lsmear`'s,
node for node. The switch is one-way because a reversible one does not work:
once RoundRobin decides, the parent's variable is RoundRobin's, "LSmear wants
to repeat it" stops measuring the hijack, and the two rules end up alternating,
which is worse than either. `--solve` reports `guard_switched_at`, the decision
at which the switch happened (-1: never).

A learned rule joins the comparison as `label=path/to.model`.

### The other two axes

The bisector is not the only thing that decides how many nodes a search visits.
`--relax` changes the contraction and `--ub` changes the upper bounding, and
both are recorded per run, so one results file can hold every combination
without the rows becoming ambiguous:

```bash
python3 python/experiment_bisectors.py run -o results/bisectors.csv \
    --dir benchs/optim/all --rules lsmear roundrobin --ub ipoptxn
python3 python/experiment_bisectors.py report results/bisectors.csv --ub ipoptxn
```

`report`, `status`, `oracle` and `soundness` all take `--relax` and `--ub` and
look only at the rows matching them; the defaults (`xtaylor`, `default`) are
what every run recorded before these axes existed used, so old files keep
reading the same way. Comparing *across* an axis is a different question from
comparing bisectors and the report deliberately does not mix them: Ipopt finds
better incumbents earlier, which prunes more for every rule at once.

### Running it

```bash
python3 python/experiment_bisectors.py run -o results/bisectors.csv \
    --dir benchs/optim/all --timeout 20 --jobs 8 \
    --rules lsmear smearsumrel smearsum smearmax largestfirst roundrobin

python3 python/experiment_bisectors.py report results/bisectors.csv --timeout 20
```

Results are appended as they land and an existing csv is read back, so an
interrupted sweep resumes. Adding a rule later only runs the missing cells.

`--jobs` defaults to 8. Ibex measures **CPU** seconds (`getrusage`), not wall
clock, and enforces `--timeout` in those same units, so running several
instances at once does not inflate the reported times the way wall-clock timing
would — and node counts are deterministic regardless. What parallelism does cost
is a few percent through cache and memory-bandwidth contention, which is why the
default leaves half of a 16-core machine free rather than saturating it. A run
can therefore take longer in wall time than `--timeout` suggests.

### Reading the report

Three numbers, because none of them is honest alone:

* **solved** — how many instances the rule closed inside the limit. The primary
  number: a rule that solves more is better whatever its node counts say.
* **nodes(com) / time(com)** — totals over the instances **every** rule solved.
  Averaging over a set that includes timeouts rewards a rule for giving up,
  since a timeout truncates its own cost at the limit.
* **geo.nodes / geo.time** — geometric mean of the per-instance ratio to the
  baseline, over that same common set. Node counts span orders of magnitude, so
  an arithmetic mean of ratios is decided by two or three instances.

**PAR2** folds "solved" and "time" into one number by charging a timeout twice
the limit; unlike the common-set totals it uses every instance.

The report also checks that the rules **agree on the answer**: it intersects the
`[uplo, loup]` enclosures over the commonly solved instances and lists any where
the intersection is empty. A rule that returns a different optimum has a bug, it
does not have a win. And it breaks the solved counts down by source directory,
so "better on `easy`, worse on `coconut`" does not hide inside a single average.

`--per-instance` prints the full table when an aggregate looks suspicious.

---

## 16. File map

| file | role |
|---|---|
| [`src/ml/ibex_MLNodeServer.h`](../src/ml/ibex_MLNodeServer.h) / [`.cpp`](../src/ml/ibex_MLNodeServer.cpp) | the instrumentation: contraction, features, dives, batch collection |
| [`src/ml/ibex_Json.h`](../src/ml/ibex_Json.h) / [`.cpp`](../src/ml/ibex_Json.cpp) | dependency-free JSON reader and streaming writer |
| [`src/bin/ibexopt-ml.cpp`](../src/bin/ibexopt-ml.cpp) | CLI, batch mode, command dispatch |
| [`python/ibexml.py`](ibexml.py) | client, Python branch & bound, tensor encoding |
| [`python/collect_dataset.py`](collect_dataset.py) | dataset collection over a set of instances |
| [`src/optim/ibex_Optimizer.h`](../src/optim/ibex_Optimizer.h) | the only pre-existing file touched (`private` → `protected`) |

Reference for LSmear itself: I. Araya, B. Neveu, *lsmear: a variable selection
strategy for interval branch and bound solvers*, Journal of Global Optimization,
2018 — implemented in
[`src/bisector/ibex_LSmear.cpp`](../src/bisector/ibex_LSmear.cpp).
