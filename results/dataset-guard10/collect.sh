#!/bin/sh
# Paso 2: dataset para aprender una regla por nodo, con el objetivo que el
# oráculo validó: la mejor variable suponiendo que después decide
# lsmear-guard:10 (results/oracle/README.md).
#
#   trayectoria : lsmear-guard:10, compo + ipoptxn (la búsqueda que se muestrea)
#   etiquetas   : un dive por candidato, continuando con lsmear-guard:10
#   presupuesto : 1000 nodos, arranca en 25 y duplica (BUDGET, BUDGET_START)
#   poda        : sí por defecto; NOPRUNE=1 da a todos el presupuesto completo
#                 (etiquetas exactas para regresión, bastante más caro)
#   muestreo    : p por instancia (instances.txt) ~ 100 puntos repartidos en el
#                 árbol, hasta 50 muestras (MAX_SAMPLES)
#
# Instancias: las 221 que lsmear o lsmear-guard:10 cierran en
# results/ipopt-compo-fixed.csv; la 3.ª columna es la familia, para validar
# agrupando por familia. Retoma: salta los .jsonl completos.
#
#   BIN=build/bin/ibexopt-ml JOBS=16 results/dataset-guard10/collect.sh
cd "$(dirname "$0")/../.."
BIN=${BIN:-build/bin/ibexopt-ml}
JOBS=${JOBS:-8}
TLIM=${TLIM:-3600}
BUDGET=${BUDGET:-1000}
BUDGET_START=${BUDGET_START:-25}
MAX_SAMPLES=${MAX_SAMPLES:-50}
PRUNE=""; [ -n "$NOPRUNE" ] && PRUNE="--no-prune"
OUT=results/dataset-guard10/samples
mkdir -p $OUT
export BIN TLIM BUDGET BUDGET_START MAX_SAMPLES PRUNE OUT
cut -d' ' -f1,2 results/dataset-guard10/instances.txt | xargs -P $JOBS -n2 sh -c '
  i=$0; p=$1; f=$OUT/$(basename $i .bch).jsonl
  [ -e "$f" ] && exit 0
  timeout $((TLIM+600)) $BIN benchs/optim/all/$i --collect -o "$f.tmp" \
     --bisector lsmear-guard --guard-horizon 10 --relax both --loup ipoptxn \
     --budget $BUDGET --budget-start $BUDGET_START $PRUNE \
     --sample-prob $p --max-samples $MAX_SAMPLES --random-seed 1 --timeout $TLIM \
     > "$f.log" 2>&1
  mv "$f.tmp" "$f"; echo "$i: $(grep -c . "$f") muestras"'
