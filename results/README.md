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
| `bisectors-guard.csv` | crudo, `lsmear-guard`, relajación `xtaylor`, **en otra máquina** (ver abajo) |
| `guard-switch.txt` | en qué decisión cambia `lsmear-guard` a round-robin, por instancia (`--max-nodes 12000`) |
| **`ipopt-compo-per-instance.csv`** | **la tabla completa detrás de los agregados: una fila por instancia, una columna por regla con los nodos que necesitó —o por qué no tiene ninguno— y otra con su tiempo**. La escribe `report --out`; se regenera cuando cambian los datos |
| `ipopt-compo.csv` | crudo, relajación `both` y cota superior `ipoptxn`: nueve bisectores y la estrategia copiada × 298 instancias, 600 s, 15 jobs. Es una población aparte: con Ipopt los conteos de nodos de *todas* las reglas bajan, así que no se compara fila a fila con los archivos de arriba |
| `guard-switch-ipopt.txt` | lo mismo bajo `--relax both --loup ipoptxn` (`--max-nodes 2000`), medido con Ipopt 3.11.9 |
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

## El barrido con Ipopt (`ipopt-compo.csv`)

Cinco brazos bajo la misma relajación `both` (compo) y la misma cota superior
`ipoptxn`. Cuatro corren con `ibexopt-ml`; `ref-ipopt` es la estrategia portada
del fork de Bertrand Neveu, corrida con su propio binario `ibexopt-ipopt`
—contractor armado a mano, selección de nodos *best-first*— con la misma
relajación, la misma cota, la misma semilla y el mismo criterio de parada.

### La tabla

Los nueve bisectores bajo esa estrategia. `geo.nodos`, `geo.tiempo` y el
recuento gana/pierde/empata son **cara a cara contra `lsmear`**, sobre las
instancias que ese par cierra —no sobre el conjunto común a los nueve, que
`largestfirst` reduce a 135 y deja de ser representativo—. Un triunfo exige al
menos 5% menos nodos; dentro de esa banda es empate.

| bisector | resueltas | PAR2 | ambas | geo.nodos | geo.tiempo | gana/pierde/empata | exclusivas |
|---|---|---|---|---|---|---|---|
| **`lsmear-guard`** | **219** | **333.3** | 203 | 1.08 | 1.08 | 11 / 26 / 166 | +16 −4 |
| `roundrobin` | 213 | 355.2 | 198 | 1.31 | 1.11 | 31 / 82 / 85 | +15 −9 |
| `smearsum` | 209 | 370.2 | 206 | 1.09 | 1.20 | 37 / 61 / 108 | +3 −1 |
| `smearmax` | 208 | 373.9 | 205 | 1.09 | 1.22 | 34 / 57 / 114 | +3 −2 |
| `lsmear` | 207 | 375.4 | — | 1.00 | 1.00 | — | — |
| `lsmear-box` | 206 | 379.6 | 206 | 1.01 | 1.00 | 8 / 10 / 188 | +0 −1 |
| `smearsumrel` | 204 | 391.0 | 203 | 1.04 | 1.18 | 34 / 39 / 130 | +1 −4 |
| `smearmaxrel` | 189 | 450.3 | 188 | 1.30 | 1.45 | 25 / 49 / 114 | +1 −19 |
| `largestfirst` | 142 | 636.4 | 141 | 1.49 | 1.23 | 15 / 33 / 93 | +1 −66 |

Fuera de concurso, porque no es un bisector sino otra estrategia entera:
`ref-ipopt` cierra **224** con PAR2 **311.8**, a costa de 1.9× el tiempo por
instancia. Y los brazos `@0.45` del ratio de bisección: `roundrobin@0.45` 214 /
351.1, `lsmear@0.45` 207 / 376.7.

**`lsmear` no domina.** Es quinto en instancias cerradas y el mejor de los
clásicos en nodos por instancia: todos los demás están en 1.01 o peor. Las dos
cosas a la vez son el resultado: gana el nodo, pierde la instancia. `smearsumrel`
—su rival en el paper de 2018— queda séptimo en cerradas pero empata en nodos
(1.04) con 34 triunfos contra 39, que es la misma foto que ya habíamos visto sin
Ipopt.

### El oráculo

Elegir perfectamente el bisector en cada instancia cerraría **226** (+19 sobre
`lsmear`) con el **79%** de sus nodos. Quién aporta al portafolio:

| bisector | mejor en | **único mejor en** |
|---|---|---|
| `smearsumrel` | 122 | **16** |
| `lsmear-box` | 119 | **12** |
| `smearsum` | 101 | **11** |
| `smearmaxrel` | 104 | **8** |
| `roundrobin` | 92 | **8** |
| `smearmax` | 96 | **6** |
| `largestfirst` | 95 | **6** |
| `lsmear` | 121 | **4** |
| `lsmear-guard` | 118 | **3** |

"Único mejor" es el que cuenta: las instancias que el portafolio perdería si se
sacara esa regla. `largestfirst` cierra 142 de 298 y aun así es el único mejor
en 6 — ninguna regla es descartable, que es el argumento para elegir por nodo en
vez de fijar una.

Los brazos `@0.45` bisecan por 0.45 del dominio en vez de por la mitad: es el
default de `Bsc` que heredan los bisectores armados a mano, frente al 0.5 de
`DefaultOptimizerConfig` que usa `ibexopt`. **El punto de corte no cambia
nada.** Cara a cara, sobre las instancias que ambos cierran:

| | resueltas | geo.nodos | geo.tiempo | gana | pierde | empata |
|---|---|---|---|---|---|---|
| `roundrobin@0.45` vs `roundrobin` | 214 vs 213 | 0.98 | 0.98 | 48 | 49 | 113 |
| `lsmear@0.45` vs `lsmear` | 207 vs 207 | 1.03 | 1.03 | 46 | 37 | 123 |

`roundrobin` es la prueba fuerte: no calcula su propio punto de corte, así que
el ratio le aplica en **todos** los nodos, mientras que LSmear casi siempre saca
el suyo del LP. Ni así se mueve. El 0.5 de `ibexopt` no está dejando nada.

Lo que aporta Ipopt, comparando cada regla contra sí misma bajo `both` sin él
(media geométrica del cociente por instancia, sobre las que ambas cierran):

| regla | resueltas sin → con | geo.nodos | geo.tiempo |
|---|---|---|---|
| `lsmear` | 203 → 207 | 0.30 | 0.36 |
| `roundrobin` | 206 → 213 | 0.29 | 0.36 |
| `smearsumrel` | 201 → 204 | 0.29 | 0.38 |

Tres cosas que hay que tener presentes al leer esa tabla:

* `ref-ipopt` gana en instancias cerradas y en nodos totales sobre el conjunto
  común, pero es ~1.9× más lento **por instancia** en media geométrica. No
  aísla ningún ingrediente: es una estrategia entera. **No es el buffer**:
  `ref-ipopt-dh` es la misma corrida con el `CellDoubleHeap` de `ibexopt` en
  lugar de su `CellHeap`, y da 223 en lugar de 224 con el mismo tiempo. Tampoco
  es el contractor KKT (solo 1 de las 21 que gana no tiene restricciones) ni el
  recorte de la caja inicial a ±1e20 (toca 7 de las 21, contra 61 de las 207
  que `lsmear` ya cierra: es la tasa base). Lo que queda es que las dos
  configuraciones bisectan en puntos distintos —`ibexopt` por 0.5, la estrategia
  copiada por el 0.45 que hereda sin decirlo— **pero eso tampoco es**: medido
  con `--bisect-ratio`, el 0.45 no reproduce a la estrategia copiada en ninguna
  instancia de prueba (en `hs071`, que no tiene bornes raros, `ibexopt-ml` hace
  118 nodos, con 0.45 hace 200, y la copiada 132), y sobre las 298 no mueve la
  aguja (tabla de arriba). Descartados también la granularidad de HC4 (mismos
  conteos construyéndolo desde el `System` o desde su array de restricciones) y
  el nivel de simplificación (los dos binarios usan el mismo default). Queda el
  recorte de la caja inicial a ±1e20, que sí cambia el problema en el 45% del
  conjunto pero no se concentra en las que gana. **Ninguna perilla sola explica
  las 17 instancias extra**: es diferencia acumulada, y separarla ya es
  depuración instancia por instancia.
* 29 de las 1490 corridas terminaron en `killed`: la guarda de reloj de pared
  mató un nodo cuya contracción no retorna. Se reparten parejo entre las cinco
  reglas (5 a 7 cada una) y se concentran en instancias ya conocidas por esto
  (`hs090`, `hs091`, `polak6`, `least`, `ex5_3_3`), así que no sesgan la
  comparación.
* `eigmaxc.bch` es la única de las 193 comunes donde los encierros se
  contradicen: `lsmear-guard` termina con `loup = uplo = 0` y las otras cuatro
  encierran −10.746. No es del guard ni de Ipopt: en el barrido anterior, sin
  Ipopt, le pasó lo mismo a `smearmax`. Es la instancia, o un error que el
  orden de recorrido destapa.

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

## `lsmear-guard`: LSmear hasta que lo secuestran, después round-robin

`lsmear-guard` corre LSmear y mira sus últimas 20 decisiones; si con al menos 10
registradas la mitad bisecta la variable del padre, pasa a round-robin por el
resto de la búsqueda. Mismo protocolo que el resto (xtaylor, 600 s, semilla 1).

**Corrió en otra máquina, ~1,9× más rápida.** Donde el guardián nunca cambió, sus
nodos son exactamente los de `lsmear`; la mediana de la razón de tiempos sobre
esas 37 corridas es 0,517. `python/compare_guard.py` divide los tiempos por ese
factor y aplica el límite de 600 s en la escala de `all-results.csv`. Los nodos
no necesitan corrección.

| 289 instancias, xtaylor | PAR2 | resueltas | brecha lsmear→oráculo | coconut | resto | vs lsmear (>1,5× y >5 s) |
|---|---|---|---|---|---|---|
| `lsmear` | 116.383 | 195 | — | — | — | — |
| `roundrobin` | 107.227 | 203 | 49,8 % | −9,4 % | 63,0 % | +16 −31 |
| `lsmear-guard` | 103.416 | 206 | 70,5 % | 47,6 % | 75,6 % | +14 −14 |
| `lsmear-guard`, horizonte H=10 | 102.790 | 207 | 73,9 % | 69,5 % | 74,9 % | +13 −4 |
| oráculo de 8 reglas | 97.987 | 211 | 100 % | | | |

**Los cambios tardíos son falsas alarmas.** Todas las ganancias salvo una cambian
en la decisión 10, la primera posible. Las pérdidas (`ex6_2_10`, `hs093`,
`ex6_2_6`, `ex6_2_8`, `ex6_1_3`, `ex14_1_7`, `bearing`…) cambian entre la 30 y la
608. La excepción es `ex7_2_3`: secuestro tardío genuino (cambia en la decisión
5287, pasa de timeout a ~11 s).

**El horizonte se evalúa exacto sin correrlo.** "Cambiar solo si el secuestro
aparece en las primeras H decisiones" es, antes del cambio, `lsmear` nodo por
nodo; así que es la fila del guardián si cambió en una decisión ≤ H y la de
`lsmear` si no. H entre 10 y 50 da lo mismo (73,5 %–73,9 %, +13 −4 a −6); desde
H=100 vuelve a lo de sin horizonte. **Ojo:** el horizonte se eligió mirando esta
misma tabla, así que no es una evaluación ciega. Con H=10 la regla se reduce a
"mirar las 10 primeras decisiones de LSmear; si la mitad repite la variable del
padre, round-robin", que es la regla con sonda del HANDOFF sin reinicio, y da lo
mismo que ella (PAR2 102.551, 207 resueltas, 75,2 %).

```bash
python3 python/compare_guard.py results/bisectors-guard.csv results/guard-switch.txt
```

El horizonte está en el binario: `--bisector lsmear-guard --guard-horizon 10`, o
`--rules lsmear-guard:10` en `experiment_bisectors.py`. Verificado en 7
instancias contra la evaluación exacta: las que cambian lo hacen en la decisión
10 con los mismos nodos que `lsmear-guard` (`ship-1` 256, `ex8_2_4` 808), y las
falsas alarmas tardías (`ex6_2_8`, `ex6_2_9`, `ex14_1_7`, `bearing`) dan
exactamente los nodos de `lsmear`.

### El horizonte bajo compo + ipoptxn (`guard-switch-ipopt.txt`)

H=10 se eligió sobre el barrido xtaylor sin Ipopt; `ipopt-compo.csv` es la
primera mirada con datos sobre los que no se eligió (mismas 298 instancias,
otra configuración). Se evalúa igual que antes, exacto sin correrlo: la fila
`lsmear-guard` donde el guardián cambió en sus primeras 10 decisiones, la de
`lsmear` si no. Los puntos de cambio se midieron en otra máquina, con Ipopt
3.11.9 + MUMPS de la distribución (`--max-nodes 2000`).

**¿Se transfieren?** Donde aquí no cambia y allá cierran las dos, la fila del
guardián tiene los nodos de `lsmear` en 160 de 165; donde aquí cambia en ≤ 10,
difiere de `lsmear` en 17 de 19. Transfieren en ~97% de los casos.

| 298 instancias | resueltas | PAR2 (media) | vs lsmear (>1,5× y >5 s) |
|---|---|---|---|
| `lsmear` | 207 | 375.4 | — |
| `roundrobin` | 213 | 355.2 | +17 −31 |
| `lsmear-guard` | 219 | 333.3 | +17 −15 |
| **`lsmear-guard:10`** | **221** | **326.1** | **+17 −5** |

Cambian 61 instancias con H=10 y 25 más tarde. Las 12 donde el horizonte cambia
el resultado son todas a favor: las falsas alarmas tardías de siempre (la
familia `ex6_2_*`, `ex6_1_3`, `ex14_1_7`, `hs108`), con `ex6_2_10` (1200 → 115)
y `ex6_2_13` (1200 → 579) pasando de timeout a resueltas. Ningún secuestro
tardío genuino se pierde en este barrido.

Salvedades: las filas `lsmear-guard` de `ipopt-compo.csv` se corrieron antes
del arreglo que impide que la búsqueda anidada de `LoupFinderIpopt` le sume
decisiones al guardián, y los puntos de cambio se midieron después; `hs088` y
otras 3 no tienen punto de cambio (tiempo agotado midiendo) y cuentan como
`lsmear`. Confirmación directa: correr `--rules lsmear-guard:10` bajo esta
configuración.

```bash
python3 python/guard_horizon_ipopt.py results/ipopt-compo.csv results/guard-switch-ipopt.txt
```
