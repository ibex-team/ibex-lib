# El oráculo de un paso, de punta a punta

¿Las etiquetas de los dives son un buen objetivo? El modelo de atención que las
imitaba tenía un regret por nodo de 1.12× y aun así armaba árboles 2× más
grandes. Aquí se usan las etiquetas mismas como bisector en **todos** los nodos
(`ibexopt-ml --solve --oracle`): en cada nodo real se corre un dive por
candidato (bisectar en x_j y explorar los dos hijos en profundidad con la regla
de continuación) y se bisecta en el que cerró con menos nodos. Los dives no
tocan la búsqueda, así que los nodos son el tamaño del árbol que arma esa
elección. compo + ipoptxn, binario con los arreglos del LP; se compara con
`results/ipopt-compo-fixed.csv` (misma configuración y máquina, nodos
deterministas).

54 instancias: 48 que `lsmear` cierra en 50–3000 nodos y las 6 secuestradas que
`lsmear-guard:10` cierra en ≤ 3000 (`ship-1`, `schwefel5`, `schwefel5-abs`,
`ex8_5_6`, `mconcon`, `ex8_2_4`). Tres brazos:

| brazo | continuación del dive | cómo elige |
|---|---|---|
| `lsmear` | LSmear (como las etiquetas originales) | menos nodos, presupuesto hasta 1000, poda |
| `guard10` | `lsmear-guard:10` | ídem |
| `lazy50` | LSmear | el dive que llegó **menos profundo**, 50 nodos fijos, sin poda (`--oracle-score depth`) |

    results/oracle/run.sh             # ARMS, INSTANCES, OUT, JOBS
    python3 results/oracle/analyze.py

## Resultados

**Con continuación LSmear, en las instancias normales, imitar las etiquetas sí
paga.** Sobre las 45 que cierran el oráculo y `lsmear`: árboles de 0.67× en
media geométrica (mediana 0.77), peor en 3 y nunca por más de 1%. El fracaso del
modelo anterior era de imitación, no del objetivo.

**En las secuestradas, ese oráculo no sirve: hereda el secuestro de la
continuación.** No cierra ninguna de las 6. Como cada dive sigue con LSmear, que
está secuestrado, casi ningún dive termina y el oráculo cae en LSmear
(`schwefel5`: 5897 de 5998 decisiones). La etiqueta falla justo donde más
importa: el objetivo debe continuar con una regla que no se secuestre.

**El lazy por profundidad es más barato y más arriesgado.** En las normales,
0.72× frente a `lsmear` (0.67 el oráculo completo), pero peor en 9 de 45 (hasta
1.42×). Escapa del secuestro en 2 de las 6 (`ship-1` 544 nodos, `ex8_5_6` 1140
frente a 2144 de `lsmear-guard:10`): aunque ningún dive cierre, la profundidad
sigue distinguiendo. En las otras 4 se le acaba el tiempo por el costo de los
dives, así que no se sabe.

Tres instancias (`dixchlng`, `eigencco`, `eigmaxc`) no cierran con ningún
oráculo por costo: 40–400 nodos en 900 s, cada nodo con decenas de dives con
Ipopt. No es tamaño de árbol.
