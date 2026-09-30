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
