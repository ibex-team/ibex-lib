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
* Hasta 50 muestras por instancia, dives de hasta 1000 nodos, 3600 s por
  instancia. Variables: `JOBS`, `TLIM`, `BUDGET`, `BUDGET_START`,
  `MAX_SAMPLES`, `NOPRUNE=1` (todos los candidatos con el presupuesto completo:
  etiquetas exactas para regresión, bastante más caro; con poda, las etiquetas
  alcanzan para imitar al mejor y para rankings por pares).
* Retoma: salta las instancias con `.jsonl` completo; los `.tmp` y `.log` no se
  versionan.
* Formato de cada muestra: el mismo de `--collect` (`python/README.md`): el
  nodo (features, `bisect_var` = la elección de `lsmear-guard:10`,
  `lsmear_var`, ...) y `labels`, un dive por candidato (`nodes`, `censored`,
  `max_depth`, ...).
