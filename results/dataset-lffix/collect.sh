#!/bin/sh
# Paso 2, rehecho con lsmear-lffix: LSmear cuyo fallback largest-first compara
# anchos (results/lf-fallback). El dataset de results/dataset-guard10 tiene sus
# trayectorias y etiquetas contaminadas por ese bug (la continuación
# lsmear-guard:10 cae en él dentro de los dives).
#
#   trayectoria : lsmear-lffix, compo + ipoptxn
#   etiquetas   : un dive por candidato, continuando con lsmear-lffix
#   el resto    : como results/dataset-guard10/collect.sh (presupuesto 1000
#                 desde 25 duplicando, sin poda, hasta 150 muestras por
#                 instancia y la búsqueda se corta al llegar)
#
# Instancias y probabilidades de muestreo: las de dataset-guard10
# (instances.txt, calculadas con los árboles de lsmear-guard:10; donde lffix
# cambia el árbol mucho saldrán algo más o menos de 150).
#
# Requiere recompilar (lsmear-lffix es nuevo):
#   git pull origin ml-branching && make -C build -j16
#   BIN=build/bin/ibexopt-ml JOBS=16 results/dataset-lffix/collect.sh
# Retoma: salta los .jsonl completos.
cd "$(dirname "$0")/../.."
BIN=${BIN:-build/bin/ibexopt-ml}
JOBS=${JOBS:-8}
TLIM=${TLIM:-5400}
BUDGET=${BUDGET:-1000}
BUDGET_START=${BUDGET_START:-25}
MAX_SAMPLES=${MAX_SAMPLES:-150}
PRUNE="--no-prune"; [ -n "$PRUNE_ON" ] && PRUNE=""
OUT=results/dataset-lffix/samples
mkdir -p $OUT
$BIN --help 2>/dev/null | grep -q lsmear-lffix || { echo "$BIN no conoce lsmear-lffix: recompila"; exit 1; }
export BIN TLIM BUDGET BUDGET_START MAX_SAMPLES PRUNE OUT
cut -d' ' -f1,2 results/dataset-guard10/instances.txt | xargs -P $JOBS -n2 sh -c '
  i=$0; p=$1; f=$OUT/$(basename $i .bch).jsonl
  [ -e "$f" ] && exit 0
  timeout $((TLIM+600)) $BIN benchs/optim/all/$i --collect -o "$f.tmp" \
     --bisector lsmear-lffix --relax both --loup ipoptxn \
     --budget $BUDGET --budget-start $BUDGET_START $PRUNE \
     --sample-prob $p --max-samples $MAX_SAMPLES --stop-at-max-samples --random-seed 1 --timeout $TLIM \
     > "$f.log" 2>&1
  mv "$f.tmp" "$f"; echo "$i: $(grep -c . "$f") muestras"'
