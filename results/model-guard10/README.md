# Paso 4: aprender la regla por nodo

Dataset: `results/dataset-guard10` (11 862 muestras, 117 162 etiquetas, 221
instancias, 131 familias), dives que continúan con `lsmear-guard:10`.

## GBDT sobre las 30 features del solver: no le gana a LSmear

`python/train_rule.py`: regresión de t_j = log2(y_j / y_mejor) con
`GradientBoostingRegressor` (300 árboles, profundidad 4), validación cruzada de
5 folds agrupada por familia. Fuera de fold (regret geo por instancia):

| regla | regret | elige el mejor | ≥ 2× |
|---|---|---|---|
| modelo | 1.268 | 56% | 19% |
| `lsmear-guard:10` | 1.266 | 52% | 21% |
| `lsmear` | 1.243 | 56% | 18% |
| oráculo | 1.000 | 100% | 0% |

Las 30 features (smear, duales, diámetros, rankings entre candidatos) son lo
que LSmear ya usa: no ven lo que distingue al mejor candidato. `fold<k>.model`
y `folds.json` quedan para evaluarlo de punta a punta, pero no hay razón para
esperar nada.

## Mirar un paso adelante (strong branching) sí lo ve

`python/strong_branching_offline.py`: con las cajas de los dos hijos ya
contraídos que trae cada etiqueta, sin aprender nada:

| regla | regret | elige el mejor | ≥ 2× |
|---|---|---|---|
| más hijos podados, luego menor volumen total | **1.108** | **71%** | **6%** |
| menor volumen total de los hijos | 1.112 | 71% | 7% |
| menor volumen del hijo más grande | 1.126 | 68% | 8% |
| mayor producto de mejoras de cota (SB de MILP) | 1.138 | 65% | 9% |
| mayor mejora de cota del peor hijo | 1.134 | 66% | 8% |

Cierra ~60% de la distancia `lsmear-guard:10` → oráculo, y las elecciones
malas (≥ 2×) bajan de 21% a 6%. Cuesta dos contracciones por candidato por
nodo: `ibexopt-ml --oracle --oracle-score sb`.

## Imitating strong branching, and strong branching near the root only

`python/imitate_sb_offline.py` regresses each candidate's strong-branching key
(children pruned, then log volume of the open ones) and picks the argmax.
Grouped 5-fold CV by family, regret vs dive labels (geo per instance; picks
the best; >=2x):

    strong branching (label)              1.108  71%   6%
    lsmear-guard:10                       1.266  52%  21%
    model, features A                     1.300  54%  21%   (agrees with SB 47%)
    model, A + constraint aggregates      1.295  54%  20%   (agrees with SB 47%)

The cheap features do not predict the contraction of the children.

`--oracle-max-depth D` (SB decides only at depth <= D, lsmear-guard:10 below),
nodes:

    instance    SB<=5        SB<=10       SB everywhere  guard:10
    avgasb      42           36           36             54
    dipigri     238          212          126            212
    avgasa      160          156          156            188
    dnieper     timeout      28           28             434
    dualc2      144          148          152            146
    ex6_2_8     13740        68404        15866          13672
    ship-1      55192        timeout      24             486
    schwefel5   2558         3292         36910          956
    ex8_5_6     2224         1734         1268           2144
    mconcon     88           84           678            116

ship-1 breaks because the guard never sees the hijack: its repeat signal is
"the primary wants the parent's variable", and above D the parent's variable
was SB's. Letting the guard watch the SB-decided nodes too (now done when
--oracle-max-depth is set) does not fix it (ship-1, D=5: 29358 nodes, no
switch).
With the guard watching (same instances): ship-1 29358 / timeout, dnieper
timeout / 28, ex6_2_8 13740 / 13754 (was 68404 at D=10) -- the guard switches
on none of them.

### The guard under a depth-limited oracle

The guard's repeat signal had to change for mixed policies. It now observes
only nodes whose parent the primary bisected (BxpPrimaryChoice +
not_primary(); guard alone unchanged). Comparing with the primary's own choice
at the parent instead switched on nearly everything (ex6_2_8 D=5: 71634).
Under SB near the root, the hijack shows up later than decision 10. With
horizon 10 ship-1 still fails (D=5 147642, D=10 timeout); without a horizon:

    instance    SB<=5 H=0          SB<=10 H=0         guard:10
    ship-1      372   (sw 89)      170   (sw 31)      486
    dnieper     timeout            28                 434
    ex6_2_8     71488 (sw 22)      68390 (sw 10)      13672
    schwefel5   2920  (sw 10)      3868  (sw 10)      956
    mconcon     90                 84                 116
    avgasb      42                 36                 54
    dipigri     238                212                212
    avgasa      160                156                188
    dualc2      144                148                146
    ex8_5_6     1894               1748               2144

Geometric mean of nodes relative to guard:10: D=10 0.83, D=5 1.22 (without
dnieper). This counts nodes only; one SB decision costs two dives. ex6_2_8 and
schwefel5 get worse when the guard switches (the guard alone never switches
on ex6_2_8).
