#!/bin/sh
# Paso 2: dataset para aprender una regla por nodo, con el objetivo que el
# oráculo validó: la mejor variable suponiendo que después decide
# lsmear-guard:10 (results/oracle/README.md).
#
#   trayectoria : lsmear-guard:10, compo + ipoptxn (la búsqueda que se muestrea)
#   etiquetas   : un dive por candidato, continuando con lsmear-guard:10
#   presupuesto : 1000 nodos, arranca en 25 y duplica (BUDGET, BUDGET_START)
#   poda        : no por defecto: todos los candidatos con el mismo presupuesto,
#                 etiquetas exactas (sirven para imitar, ranking y regresión);
#                 PRUNE_ON=1 corta cada dive en el mejor hasta ese momento (2-3x
#                 más barato, etiquetas censuradas)
#   muestreo    : hasta 150 muestras (MAX_SAMPLES) por instancia, con
#                 p = min(1, 1.2*150/decisiones del árbol de lsmear-guard:10)
#                 (instances.txt), así cubren todo el árbol y no solo su
#                 primera mitad
#
# Estimado: ~12.600 muestras; ~60 h de CPU (cota pesimista), ~4 h en 16
# núcleos. La mitad de las instancias son árboles de <= 8 decisiones y dan
# pocas muestras; aportan sobre todo las ~90 grandes (ninguna familia > 8%).
#
# Instancias: las 221 que lsmear o lsmear-guard:10 cierran en
# results/ipopt-compo-fixed.csv; la 3.ª columna es la familia, para validar
# agrupando por familia. Retoma: salta los .jsonl completos.
#
#   BIN=build/bin/ibexopt-ml JOBS=16 results/dataset-guard10/collect.sh
cd "$(dirname "$0")/../.."
BIN=${BIN:-build/bin/ibexopt-ml}
JOBS=${JOBS:-8}
TLIM=${TLIM:-5400}
BUDGET=${BUDGET:-1000}
BUDGET_START=${BUDGET_START:-25}
MAX_SAMPLES=${MAX_SAMPLES:-150}
PRUNE="--no-prune"; [ -n "$PRUNE_ON" ] && PRUNE=""
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
