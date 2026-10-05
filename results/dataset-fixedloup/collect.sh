#!/bin/sh
# Paso 2 con etiquetas de bisección pura: como results/dataset-lffix, pero con
# el loup fijado en el óptimo desde el inicio (--initial-loup, optima.txt).
# Así el tamaño de cada dive depende de cómo se bisecta y no de cuándo el loup
# finder encuentra un buen punto (results/fixed-loup/README.md: con el loup
# fijo SB deja de explotar y la explosión de mconcon desaparece).
#
#   trayectoria : lsmear-lffix, compo + ipoptxn, --initial-loup <óptimo>
#   etiquetas   : un dive por candidato, continuando con lsmear-lffix
#   el resto    : como dataset-lffix (presupuesto 1000 desde 25 duplicando,
#                 sin poda, hasta 150 muestras por instancia y la búsqueda se
#                 corta al llegar)
#
# optima.txt: el mejor loup de results/ipopt-compo-fixed.csv entre corridas
# que cerraron (las 221 tienen uno). Las probabilidades de muestreo son las de
# dataset-guard10: con el loup conocido los árboles suelen ser más chicos (salen
# menos muestras) y alguno más grande (dnieper).
#
#   git pull origin ml-branching && make -C build -j16
#   BIN=build/bin/ibexopt-ml JOBS=16 results/dataset-fixedloup/collect.sh
# Retoma: salta los .jsonl completos.
cd "$(dirname "$0")/../.."
BIN=${BIN:-build/bin/ibexopt-ml}
JOBS=${JOBS:-8}
TLIM=${TLIM:-5400}
BUDGET=${BUDGET:-1000}
BUDGET_START=${BUDGET_START:-25}
MAX_SAMPLES=${MAX_SAMPLES:-150}
PRUNE="--no-prune"; [ -n "$PRUNE_ON" ] && PRUNE=""
OUT=results/dataset-fixedloup/samples
mkdir -p $OUT
$BIN --help 2>/dev/null | grep -q lsmear-lffix || { echo "$BIN no conoce lsmear-lffix: recompila"; exit 1; }
export BIN TLIM BUDGET BUDGET_START MAX_SAMPLES PRUNE OUT
cut -d' ' -f1,2 results/dataset-guard10/instances.txt | while read i p; do
  echo "$i $p $(grep "^$i " results/dataset-fixedloup/optima.txt | cut -d' ' -f2)"
done | xargs -P $JOBS -n3 sh -c '
  i=$0; p=$1; lo=$2; f=$OUT/$(basename $i .bch).jsonl
  [ -e "$f" ] && exit 0
  timeout $((TLIM+600)) $BIN benchs/optim/all/$i --collect -o "$f.tmp" \
     --bisector lsmear-lffix --relax both --loup ipoptxn --initial-loup $lo \
     --budget $BUDGET --budget-start $BUDGET_START $PRUNE \
     --sample-prob $p --max-samples $MAX_SAMPLES --stop-at-max-samples --random-seed 1 --timeout $TLIM \
     > "$f.log" 2>&1
  mv "$f.tmp" "$f"; echo "$i: $(grep -c . "$f") muestras"'
