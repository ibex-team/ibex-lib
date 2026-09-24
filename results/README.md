# Comparación de bisectores en IbexOpt — resultados

Una corrida es **una instancia × un bisector × una relajación**, resuelta con
`ibexopt-ml --solve`. Todo lo demás se mantiene fijo: mismo contractor salvo
donde se indica, mismo loup finder, misma selección de nodos, misma semilla
(`--random-seed 1`), misma precisión, mismo límite de 600 s de CPU.

## Archivos

| archivo | qué es |
|---|---|
| **`all-results.csv`** | **el consolidado: una fila por (instancia, regla, relajación)**, con los cuatro archivos crudos ya fusionados |
| `bisectors.csv` | crudo, relajación `xtaylor` |
| `bisectors-both.csv` | crudo, relajación `both` |
| `bisectors-affine.csv` | crudo, relajación `affine` (16 corridas, barrido abandonado) |
| `pilot.csv` | piloto de 25 instancias bajo `affine` |
| `paper-instances.txt` | las 55 instancias nombradas en las tablas del paper de 2018 |

Los archivos crudos son *append-only*: una corrida relanzada con más tiempo deja
las dos filas. `all-results.csv` ya resolvió esos duplicados quedándose con la
fila más informativa, así que **para leer resultados usá ese**; los crudos son
para que el barrido pueda retomarse.

## Columnas de `all-results.csv`

| columna | significado |
|---|---|
| `instance` | archivo `.bch`, en `benchs/optim/all/` |
| `set` | directorio de origen: `easy`, `medium`, `hard`, `blowup`, `others`, `unsolved`, `coconutbenchmark-library2`. Es el único registro de qué tan difícil es |
| `rule` | el bisector: `lsmear`, `lsmear-box`, `smearsumrel`, `smearsum`, `smearmax`, `smearmaxrel`, `largestfirst`, `roundrobin` |
| `relax` | la relajación lineal del contractor: `xtaylor`, `affine`, `both` |
| `ub` | el cálculo de la cota superior: `default` (el de `ibexopt`) o `ipoptprob`, `ipoptxn`, `ipoptxninhc4`. Las corridas de este archivo son todas anteriores al soporte de Ipopt, así que no traen la columna; el reporte las lee como `default`, que es lo que usaron |
| `status` | cómo terminó la búsqueda (ver abajo) |
| **`solved`** | **1 si encerró el óptimo con la precisión pedida, 0 si no** |
| `nodes` | celdas tratadas |
| `time` | segundos de **CPU** (`getrusage`), no de reloj |
| `uplo`, `loup` | cota inferior y superior del objetivo al terminar |
| `timeout` | el límite con el que corrió esa fila, en segundos de CPU |

### Valores de `status`

| valor | qué pasó | ¿cuenta como resuelta? |
|---|---|---|
| `complete` | encerró el óptimo | **sí** |
| `timeout` | agotó el límite de CPU | no |
| `infeasible` | probó que no hay solución | no |
| `no_feasible_found` | vació el buffer sin encontrar punto factible | no |
| `unbounded`, `unbounded_obj` | el objetivo no está acotado | no |
| `unreached_prec` | terminó sin alcanzar la precisión pedida | no |
| `killed` | la guarda de reloj de pared lo mató (ver más abajo) | no |
| `error: …` | no produjo salida; el texto dice por qué | no |

> **Usá la columna `solved`, no `status == "complete"`.** Los resultados
> anteriores a cierta corrección reportaban `complete` cuando la búsqueda
> vaciaba el buffer aunque nunca hubiera encontrado un punto factible. `solved`
> aplica el criterio correcto —`complete` **y** `loup` finito— y por eso vale
> igual sobre las filas viejas y las nuevas.

## Cómo leer la comparación

Tres números, porque ninguno solo es honesto:

**Cuántas resuelve.** El primario. Una regla que resuelve más es mejor,
digan lo que digan sus conteos de nodos.

**Nodos y tiempo sobre el conjunto común.** Totales sobre las instancias que
*todas* las reglas cerraron. Promediar sobre un conjunto que incluye timeouts
premia a la regla que se rinde, porque el timeout le trunca el costo.

**Media geométrica del cociente contra la línea base**, sobre ese mismo conjunto.
Los conteos de nodos abarcan órdenes de magnitud, así que una media aritmética
de cocientes la deciden dos o tres instancias.

**Gana / pierde / empata con banda del 5%.** Una regla gana una instancia solo si
usa al menos 5% menos nodos; dentro de esa banda es empate. Sin la banda, una
diferencia de un nodo cuenta como victoria.

**PAR2** junta "resuelve" y "tiempo" en un número cobrando a cada timeout el
doble de su límite. A diferencia de los totales del conjunto común, usa todas
las instancias.

## Comandos

```bash
# tabla comparativa
python3 python/experiment_bisectors.py report results/all-results.csv --relax xtaylor
python3 python/experiment_bisectors.py report results/all-results.csv --relax both

# solo las instancias del paper de 2018
python3 python/experiment_bisectors.py report results/all-results.csv \
    --relax both --only results/paper-instances.txt

# el techo: qué daría elegir perfectamente la regla en cada instancia
python3 python/experiment_bisectors.py oracle results/all-results.csv --relax xtaylor

# ¿alguna relajación contradice a otra? (contracción no sólida)
python3 python/experiment_bisectors.py soundness results/all-results.csv \
    --relax affine --against xtaylor --write results/quarantine.txt
```

`report` acepta varios CSV y tiene `--exclude` y `--only` con listas de nombres.

## Salvedades que hay que decir si esto se publica

**La relajación afín no es sólida en algunas instancias.** `wall.bch` se declara
`infeasible` bajo `affine` aunque tiene óptimo en −1, y `polak1.bch` con
`roundrobin` converge a un incumbente basura. No da una respuesta peor: da una
**equivocada**, y encima parece una victoria porque termina rapidísimo.
El subcomando `soundness` cruza una relajación contra otra y escribe una lista de
cuarentena para `report --exclude`. **Correlo siempre antes de comparar
relajaciones.**

**Cuatro corridas abortan.** `ex8_4_2`, `ex8_4_2bis`, `hs090` y `orthregb`, todas
con `largestfirst`, mueren con `double free or corruption` a los ~328 s de CPU.
No es falta de memoria (140 MB de pico) y no está atribuido todavía a Ibex o a
este código. Abortan en vez de dar un resultado equivocado, así que no contaminan
nada, pero dejan a `largestfirst` con cuatro instancias sin dato que no son
timeout, lo cual lo favorece levemente en PAR2.

**Los tiempos no son comparables entre relajaciones.** `xtaylor` corrió con 8
trabajos en paralelo y `both` con 15, sobre 16 núcleos. Ibex mide CPU, no reloj,
así que el efecto es chico, pero la contención de caché y memoria a 15/16 no es
cero. **Los nodos sí son comparables** — son deterministas dada la semilla.

**`robot.bch` quedó fuera.** Es la instancia donde el `ibexopt` de fábrica ignora
su propio `-t`: un nodo cuya contracción no retorna, y el límite de Ibex se
chequea una vez por nodo. Aparece como `killed` para las ocho reglas.

**El subconjunto importa más que el contractor.** Sobre las 298, LSmear le gana a
SmearSumRelative por 1.04 en nodos; sobre las 55 nombradas en el paper de 2018,
por 1.45. Esas 55 están seleccionadas por el paper con un filtro de razón ≥ 5
entre la mejor y la peor estrategia, o sea elegidas para mostrar diferencias.
Cualquier número que se cite tiene que decir sobre qué conjunto se midió.
