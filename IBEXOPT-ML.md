# ibexopt-ml

This tree is [ibex-lib](README.md) plus `ibexopt-ml`, a node-level
instrumentation of IbexOpt used to learn a branching (bisection) rule from
Python.

It adds, for every node of the search, the features a model can score variables
with, and — for each candidate variable — the size of the truncated branch &
bound ("dive") that bisecting on it would trigger. It also exposes contraction
and bisection as a line protocol, so the search itself can be driven from
Python with a learned rule in place of LSmear.

## Build

Same as plain Ibex. **An LP solver is mandatory** — `ibexopt-ml` needs it for
the same reason `ibexopt` does, and `LP_LIB` defaults to `none`, so it has to
be asked for explicitly. SoPlex is bundled and needs no download:

```bash
mkdir -p build && cd build
cmake .. -DLP_LIB=soplex          # -DLP_LIB=clp also works (bundled too)
make -j8
```

Leaving `-DLP_LIB` out builds cleanly but produces a binary that refuses to run
with `requires a LP Solver (use -DLP_LIB with cmake)`.

This produces `build/bin/ibexopt-ml` next to the usual `ibexopt` and
`ibexsolve`. The interval library defaults to Gaol, also bundled; nothing is
downloaded.

### Optional: Ipopt

Upper bounding can call [Ipopt](https://github.com/coin-or/Ipopt) instead of
relying only on Ibex's own probing and X-Taylor restriction. This is off by
default and is the one part of the tree that needs something installed:

```bash
cmake .. -DLP_LIB=soplex -DIBEX_WITH_IPOPT=ON     # needs ipopt.pc on PKG_CONFIG_PATH
```

It adds `--loup ipoptprob|ipoptxn|ipoptxninhc4` to `ibexopt-ml` (with
`--ipopt-freq` and `--ipopt-qp`) and builds a second binary, `ibexopt-ipopt`,
which is the reference strategy the finder was written for, kept with its
original positional command line. Without the flag everything still builds and
`--loup` accepts only `default`.

A local minimum found early prunes far more than a late one: on `hs071` the root
node alone goes from no incumbent to one within 1e-5 of the optimum, and the
dive under the same variable drops from 200 nodes to 14. It changes the node
counts of *every* bisector, so runs with and without it are separate
populations, not a before/after.

## Try it

```bash
# one JSON training sample per line
./build/bin/ibexopt-ml benchs/optim/easy/ex2_1_1.bch --collect -o data.jsonl \
    --budget 200 --sample-prob 0.2 --max-samples 20 --progress

# or talk to it as a server
printf '{"cmd":"info"}\n{"cmd":"quit"}\n' \
    | ./build/bin/ibexopt-ml benchs/optim/easy/ex2_1_1.bch

# from Python
python3 python/collect_dataset.py benchs/optim/easy/*.bch \
    -o data/train.jsonl --budget 200 --sample-prob 0.2 --max-samples 100
```

## Documentation

**[python/README.md](python/README.md)** — the full document: concepts, data
dictionary, protocol reference, Python API, a training path, DAgger, measured
costs, internals and known limitations.

## What was added

| | |
|---|---|
| `src/ml/ibex_MLNodeServer.{h,cpp}` | the instrumentation, on top of `Optimizer` |
| `src/ml/ibex_BscHijackGuard.{h,cpp}` | `--bisector lsmear-guard`: LSmear until one variable hijacks it, then RoundRobin |
| `src/ml/ibex_MLModel.{h,cpp}` | plain-text linear / GBDT models, scored inside the solver |
| `src/ml/ibex_Json.{h,cpp}` | dependency-free JSON reader / streaming writer |
| `src/bin/ibexopt-ml.cpp` | CLI, the four modes, command dispatch |
| `python/ibexml.py` | client, search drivers, tensor encoding, model export |
| `python/collect_dataset.py` | dataset collection over a set of instances |
| `python/make_package.py` | builds the standalone zip (binary + Python + benchmarks) |
| `src/affine/` | the affine-arithmetic plugin (`ibex-affine`), vendored so that `--relax affine\|both` needs no separate install |
| `src/ipopt/` | the Ipopt loup finder, ported from Bertrand Neveu's fork (see `src/ipopt/UPSTREAM`); built only with `-DIBEX_WITH_IPOPT=ON` |
| `src/bin/ibexopt-ipopt.cpp` | the reference Ipopt strategy, kept runnable as its own binary |

Six pre-existing files were modified, all additively and all documented in
place; no public API changed:

| file | change | why |
|---|---|---|
| `src/optim/ibex_Optimizer.h` | search state `private` → `protected` | a subclass must be able to save and restore it around a dive |
| `src/tools/ibex_Random.h/.cpp` | `RNG::get_state()` / `set_state()` | `srand(s)` repositions the stream by *drawing* s numbers, so resuming an arbitrary position costs O(s); this saves and restores it in constant time |
| `src/contractor/ibex_CtcAcid.h` | `get_tuning()` / `set_tuning()` | ACID adapts across calls, so a speculative dive changes what the enclosing search does next unless its tuning is put back |
| `src/strategy/ibex_Sts.h` | `calls()` | read a call counter without parsing a report |
| `src/loup/ibex_LoupFinder.h/.cpp` | `bound_check*()`, `is_inner0()`, `goal_ub0()`, `ipopttime` / `ampltime` | helpers the ported Ipopt finder calls; they were free functions in the fork it comes from |
| `src/optim/ibex_Optimizer.h` | `set_loup()`, `set_uplo()`, `set_loup_point()`, public `compute_ymax()` | the Ipopt finder certifies its point with a nested optimizer, which has to start from the enclosing search's bounds |

`src/CMakeLists.txt` and `src/bin/CMakeLists.txt` were extended to build the new
files, and the top-level `CMakeLists.txt` grew the `IBEX_WITH_IPOPT` option.

Benchmarks are in `benchs/optim/` (`easy/`, `medium/`, `hard/`, …), in Minibex
format.
