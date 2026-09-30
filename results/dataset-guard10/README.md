# Paso 2: dataset con la continuación correcta

El oráculo de un paso (`results/oracle/README.md`) mostró qué imitar: la mejor
variable **suponiendo que después decide `lsmear-guard:10`**. Con LSmear como
continuación la etiqueta hereda el secuestro; con el guardián, imitarla
perfectamente da árboles de ~0.55× los de `lsmear-guard:10`.

`collect.sh` recolecta ese dataset: sigue la trayectoria de `lsmear-guard:10`
bajo compo + ipoptxn (la misma de `--solve`, nodo por nodo: los dives restauran
el estado de la búsqueda, el guardián y el calendario de Ipopt) y en los nodos
muestreados corre un dive por candidato que continúa con `lsmear-guard:10`.

    git pull origin ml-branching
    cd build && cmake .. -DLP_LIB=soplex -DIBEX_WITH_IPOPT=ON && make -j16 ibexopt-ml && cd ..
    BIN=build/bin/ibexopt-ml JOBS=16 results/dataset-guard10/collect.sh
    git add results/dataset-guard10/samples/*.jsonl && git commit -m "dataset: guard10 samples" && git push

* 221 instancias (las que `lsmear` o `lsmear-guard:10` cierran en
  `results/ipopt-compo-fixed.csv`), 131 familias; `instances.txt` trae la
  probabilidad de muestreo (~100 puntos repartidos por árbol) y la familia, para
  validar agrupando por familia.
* Hasta 150 muestras por instancia, repartidas en todo el árbol
  (p = min(1, 1.2·150 / decisiones del árbol de `lsmear-guard:10`)); dives de
  hasta 1000 nodos **sin poda** (etiquetas exactas); 5400 s por instancia.
  Variables: `JOBS`, `TLIM`, `BUDGET`, `BUDGET_START`, `MAX_SAMPLES`,
  `PRUNE_ON=1` (poda: 2–3× más barato, etiquetas censuradas).
* Estimado: ~12.600 muestras, ~60 h de CPU (cota pesimista), ~4 h en 16
  núcleos. Estimado con el costo por muestra del paso 1 (0.7 s mediana, 7.8 s
  p90 sin poda con 500 nodos) y el tamaño de los árboles en
  `results/ipopt-compo-fixed.csv`. La mitad de las instancias tiene ≤ 8
  decisiones y aporta pocas muestras; el grueso sale de ~90 instancias
  grandes, sin que ninguna familia pase del 8% (`ex6_2`).
* Probado en `ex6_2_8` (~6800 decisiones, p = 0.026): 150 muestras en 16 s,
  profundidades 6–25 (mediana 14), 1% de muestras sin ningún dive cerrado
  (23% con el presupuesto de 500 del paso 1), y la trayectoria intacta (13 672
  nodos, igual que `--solve`). Con `--stop-at-max-samples` (lo que usa el
  script) las 150 muestras son idénticas y la búsqueda termina en 11 986
  nodos en vez de 13 672.
* Retoma: salta las instancias con `.jsonl` completo; los `.tmp` y `.log` no se
  versionan.
* Formato de cada muestra: el mismo de `--collect` (`python/README.md`): el
  nodo (features, `bisect_var` = la elección de `lsmear-guard:10`,
  `lsmear_var`, ...) y `labels`, un dive por candidato (`nodes`, `censored`,
  `max_depth`, ...).
