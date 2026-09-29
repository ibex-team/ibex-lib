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

## Resultado (`summary.txt`)

590 nodos informativos en 38 de las 45 instancias; en otros 155 ningún dive
terminó con 500 nodos (no dicen nada y quedan fuera, lo que sesga hacia los
nodos fáciles). Mediana de 10 candidatos por nodo; el peor candidato cuesta
2× el mejor (mediana).

| regla | regret (geo por instancia) | elige el mejor | ≥2× | ≥10× |
|---|---|---|---|---|
| `lsmear-guard:10` | 1.58 | 37% | 25% | 3% |
| `lsmear` | 1.54 | 40% | 21% | 2% |
| `smearsumrel` | 1.60 | 36% | 24% | 2% |
| `roundrobin` | 1.67 | 34% | 27% | 2% |

* **Hay margen por nodo:** en cada decisión la mejor variable ahorra ~1/3 del
  subárbol frente a la que eligen las reglas, y las reglas aciertan la mejor en
  poco más de un tercio de los nodos.
* **Pero las reglas son casi indistinguibles un paso adelante** (1.54–1.67),
  mientras que de punta a punta difieren mucho (`lsmear-guard:10` cierra 220,
  `lsmear` 207). La diferencia que importa —el secuestro— es de trayectoria, no
  de un paso: el regret de un paso no la ve.
* Consecuencia para el paso 2: aprender a minimizar el regret de un paso con la
  continuación fija no alcanza —ya pasó con el modelo de atención (1.12× por
  nodo, árboles 2× más grandes)—. Hace falta `--dive-rule model` (que el dive
  continúe con la regla aprendida) y evaluar siempre de punta a punta.
* El margen no crece con la profundidad (1.35–1.71).
