#!/bin/sh
# Paso 1: margen por nodo. Sigue la trayectoria de lsmear-guard:10 bajo compo +
# ipoptxn y, en los nodos muestreados, prueba todos los candidatos con el mismo
# presupuesto (--no-prune), así la elección de LSmear no fija el de los demás.
cd "$(dirname "$0")/../.."
BIN=${BIN:-build-fix/bin/ibexopt-ml}
mkdir -p results/headroom/samples
cat results/headroom/instances.txt | xargs -P ${JOBS:-2} -I{} sh -c '
  out=results/headroom/samples/$(basename {} .bch).jsonl
  [ -s "$out" ] && exit 0
  timeout 400 '"$BIN"' benchs/optim/all/{} --collect -o "$out.tmp" --bisector lsmear-guard --guard-horizon 10 \
     --relax both --loup ipoptxn --no-prune --budget 500 --budget-start 0 \
     --sample-prob 0.2 --max-samples 20 --random-seed 1 --timeout 300 >/dev/null 2>&1
  mv "$out.tmp" "$out"'
