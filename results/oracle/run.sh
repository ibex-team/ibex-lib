#!/bin/sh
# Oráculo de un paso de punta a punta: en cada nodo de la búsqueda real, un dive
# por candidato (presupuesto 1000, poda, continuación = el bisector) y se bisecta
# en el que cierra con menos nodos. Dos brazos: continuación lsmear (como las
# etiquetas originales) y continuación lsmear-guard:10. compo + ipoptxn.
# Tercer brazo, lazy50 (ARMS=lazy50): presupuesto fijo de 50 nodos sin poda, y
# se elige el candidato cuyo dive llegó menos profundo (--oracle-score depth).
# Reparto entre máquinas: INSTANCES elige la lista y OUT el archivo de salida
# (oracle-local.jsonl, oracle-cloud.jsonl); analyze.py lee todos los oracle*.jsonl.
cd "$(dirname "$0")/../.."
BIN=${BIN:-build-fix/bin/ibexopt-ml}
OUT=${OUT:-results/oracle/oracle.jsonl}
INSTANCES=${INSTANCES:-results/oracle/instances.txt}
touch $OUT
for arm in ${ARMS:-lsmear guard10}; do while read i; do echo "$arm $i"; done < $INSTANCES; done |
xargs -P ${JOBS:-2} -n2 sh -c '
  arm=$0; i=$1; grep -q "\"instance\": \"$i\", \"arm\": \"$arm\"" '"$OUT"' && exit 0
  case $arm in
    lsmear)  b="--bisector lsmear --budget 1000" ;;
    guard10) b="--bisector lsmear-guard --guard-horizon 10 --budget 1000" ;;
    lazy50)  b="--bisector lsmear --oracle-score depth --budget 50" ;;
  esac
  r=$(timeout 1100 '"$BIN"' benchs/optim/all/$i --solve --oracle $b --relax both --loup ipoptxn \
       --random-seed 1 --timeout 900 --max-nodes 20000 2>/dev/null | tail -1)
  [ -z "$r" ] && r="{\"status\": \"killed\"}"
  echo "{\"instance\": \"$i\", \"arm\": \"$arm\", \"result\": $r}" >> '"$OUT"'
'
