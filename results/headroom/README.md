# Paso 1: margen por nodo

¿Cuánto se gana eligiendo mejor la variable en cada nodo, frente a elegir la
regla una vez por instancia? Se sigue la trayectoria de `lsmear-guard:10` bajo
compo + ipoptxn (con los arreglos del LP) en 45 instancias, una por familia, de
las que `lsmear` o `lsmear-guard:10` cierran en 1–150 s. En hasta 20 nodos por
instancia se prueban **todos** los candidatos con el mismo presupuesto (500
nodos, `--no-prune`), así la elección de LSmear no fija el presupuesto de los
demás (el sesgo de las etiquetas del dataset original).

    results/headroom/collect.sh          # samples/<instancia>.jsonl
    python3 results/headroom/analyze.py  # regret por regla

Limitación: el dive continúa con la regla que decide (un paso de mejora sobre
`lsmear-guard:10`), no con la regla que eligió el primer paso.
