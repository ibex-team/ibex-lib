# Step 2 dataset, lsmear-lffix

Collected with `collect.sh` (trajectory and dive continuation: lsmear-lffix,
compo + ipoptxn; otherwise as dataset-guard10). 221 instances, 11017 samples;
10278 usable (>= 2 valid candidates and at least one uncensored dive), from 194
instances in 120 families. `instances.txt` is dataset-guard10's.

## Offline: predicting the oracle

`DATASET=dataset-lffix python3 python/oracle_sbfeat_offline.py`. Grouped
5-fold CV by family, out of fold; regret against the dive labels (geo per
instance; picks the best; >=2x):

                                   lffix data           (guard10 data)
    oracle (label)                 1.000  100%   0%
    strong branching rule          1.085   71%   5%     1.108  71%   6%
    trajectory bisector            1.239   54%  18%     1.266  52%  21%
    model A (30 features)          1.271   56%  18%     1.276  58%  17%
    model P (probe)                1.090   72%   5%     1.109  72%   6%
    model A+P                      1.087   72%   5%     1.102  73%   5%

Same picture as with the old labels: the 30 features do not beat the
bisector; the probe (the two children after one contraction) brings a model
to strong branching and no further.

## Propagation features

S: how the probe's contraction spread over the other variables (per child:
fraction contracted by >1%, >10%, >50%, strongest and mean log ratio, the
bisected variable beyond the half; min/max over the children; raw and
relative to the sample).

    model S         1.084  71%  5%
    model P+S       1.087  71%  5%
    model A+P+S     1.096  71%  5%

The breakdown adds nothing over the aggregate probe: every one-step view of
the contraction lands on strong branching's level.

## Two-level probe

`python/probe2_offline.py` (outputs in `probe2/`): for each candidate j, each
open child of the probe is bisected on k_j, the best variable of the node's
LSmear ranking other than j, and both grandchildren contracted. P2: pruned
grandchildren (of 4), log total volume of the open ones, rise of the goal
lower bound (min/max), raw and relative to the sample. SB2 rule: most pruned
grandchildren, then least volume.

    strong branching rule   1.085  71%   5%
    SB 2 levels rule        1.170  58%  15%
    model P2                1.181  60%  16%
    model P+P2              1.080  73%   5%
    model A+P+P2            1.080  74%   5%

The second level alone is worse than the first: with k_j fixed from the
node's ranking, the grandchildren mostly measure k_j, the same for most
candidates, and blur j's own effect. On top of the first level it adds
little (1.085 -> 1.080).
