#!/bin/sh
# Oráculo de un paso de punta a punta: en cada nodo de la búsqueda real, un dive
# por candidato (presupuesto 1000, poda, continuación = el bisector) y se bisecta
# en el que cierra con menos nodos. Dos brazos: continuación lsmear (como las
# etiquetas originales) y continuación lsmear-guard:10. compo + ipoptxn.
cd "$(dirname "$0")/../.."
BIN=${BIN:-build-fix/bin/ibexopt-ml}
OUT=results/oracle/oracle.jsonl
touch $OUT
for arm in lsmear guard10; do while read i; do echo "$arm $i"; done < results/oracle/instances.txt; done |
xargs -P ${JOBS:-2} -n2 sh -c '
  arm=$0; i=$1; grep -q "\"instance\": \"$i\", \"arm\": \"$arm\"" '"$OUT"' && exit 0
  if [ $arm = lsmear ]; then b="--bisector lsmear"; else b="--bisector lsmear-guard --guard-horizon 10"; fi
  r=$(timeout 1100 '"$BIN"' benchs/optim/all/$i --solve --oracle --budget 1000 $b --relax both --loup ipoptxn \
       --random-seed 1 --timeout 900 --max-nodes 20000 2>/dev/null | tail -1)
  [ -z "$r" ] && r="{\"status\": \"killed\"}"
  echo "{\"instance\": \"$i\", \"arm\": \"$arm\", \"result\": $r}" >> '"$OUT"'
'
