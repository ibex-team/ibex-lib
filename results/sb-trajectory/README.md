# Why strong branching is good offline and bad end to end

Strong branching (SB: most pruned children, then least open volume) has
regret 1.085 against the dive labels offline, but end to end it blows up on
some instances (lsmear-lffix as base, 300 s):

    instance   SB       lffix
    schwefel5  36910    698
    mconcon    678      18
    ex8_5_6    1268     1852
    dipigri    126      212
    ship-1     24       680
    dnieper    28       434

Samples collected along SB's own trajectory (`--collect --oracle
--oracle-score sb --bisector lsmear-lffix`, labels as in dataset-lffix):
`*.jsonl` here.

1. Per node, SB stays close to the best on its own trajectory too (geo regret,
   `python/sb_regret_by_instance.py`): schwefel5 1.153, mconcon 1.071,
   dipigri 1.093, ex8_5_6 1.142.
2. But summed, node by node, SB's choice is worse than lsmear-lffix's own
   choice on these nodes (sum of dive nodes, SB vs lffix choice):
   schwefel5 5606 vs 4774, ex8_5_6 4804 vs 4184, mconcon 4632 vs 4504.
3. The trees go deep: max sampled depth 148 (mconcon; lffix: 8) and 85
   (schwefel5; lffix: 65). A per-decision loss of a few percent over lffix
   compounds level after level.
4. Not the probes: running every SB probe but bisecting where lffix says gives
   lffix's tree (schwefel5 680, mconcon 18). Not the ties either: SB's key
   ties at the top on 30-78% of the nodes, but breaking them by LSmear rank
   (tolerance 1e-6, kept in the code) barely changes it (mconcon 564,
   schwefel5 38654).

5. On lsmear-lffix's trajectory the same sum says SB is better than the base
   (dataset-lffix, `python/oracle_sbfeat_offline.py`, last two columns: sum
   of the choice's dive nodes over the base choice's, geo over instances, and
   share of instances above 1):

       oracle            0.757   0%
       SB rule           0.846   8%
       model A+P         0.848   6%
       model A+P+P2      0.842  10%
       model A           1.035  36%

So it is a distribution shift, the classic one of imitation: judged on the
nodes the base visits, SB improves on the base; deciding everywhere, it
visits other nodes, where it is worse than the base (schwefel5 5606 vs 4774),
and the deep trees compound it. Any offline score on dataset-lffix has the
same blind spot. What would close it: collect on the rule's own trajectory and
label with the base continuation, retrain on the union (DAgger); and/or
deviate from the base only where the predicted gain is large.

## Conservative strong branching (`--sb-ratio r`)

Leave lsmear-lffix's choice only when SB's probe is clearly better than the
base's: more pruned children, or as many with at most r times the open
volume. `sb-ratio-10.txt` (300 s; dev = decisions taken from SB / probed):

    instance   lffix  SB      r=0.7  r=0.5  r=0.3  r=0.1
    ship-1     680    24      696    226    512    134
    dnieper    434    28      28     28     28     22
    ex6_2_8    13672  15866   13738  13146  13126  13126
    schwefel5  698    36910   498    532    494    494
    mconcon    18     678     560    564    556    334
    avgasb     54     36      34     34     34     34
    dipigri    212    126     130    98     152    170
    avgasa     188    156     180    160    158    162
    dualc2     146    152     130    132    140    146
    ex8_5_6    1852   1268    1434   1212   1184   1424
    geo/lffix  1      1.02    0.91   0.77   0.87   0.73
    w/o mconc  1      0.68    0.61   0.51   0.58   0.51

The schwefel5 blow-up is gone (494 vs 36910) and ex6_2_8 no longer loses;
mconcon still does (deviations on "more pruned children", which r does not
gate). Nodes only: every node is still probed, so time is far above lffix
(dnieper 25 s vs 6 s, mconcon 12 s vs 0.1 s).

`--sb-vol-only` (judge by open volume alone, also when SB prunes more):
`sb-ratio-volonly-10.txt`. mconcon is not fixed (r=0.5: 564, r=0.1: 336, vs
18); geo/lffix 0.87 and 0.91 (0.58 and 0.65 without mconcon): no better than
the default rule.

## Offline: can a model tell when to deviate, without probes?

`python/deviate_offline.py` (dataset-lffix): predict t_j = log(y_j/y_base)
per candidate, deviate to the argmin only if the prediction is below
log(thr). Out of fold, by family; sum of the chosen candidates' dive nodes
over the base's (geo over instances), instances above 1, deviation rate:

    oracle                              0.757    0%   46%
    conservative oracle thr=0.5         0.855    0%    9%
    conservative oracle thr=0.7         0.773    0%   26%
    model A (30 features)  thr=0.9      0.991   21%   12%
    model A                thr=0.7      1.002    5%    1%
    model A+P (probe)      thr=0.9      0.837    3%   23%
    model A+P              thr=0.7      0.854    2%   15%

The headroom of a conservative rule is real (deviating on 9% of the nodes is
worth 0.855), but the 30 cheap features cannot find those nodes: the model
either never deviates or deviates at random. With the probe it gets most of
it, which is what --sb-ratio already does end to end, at the probe's cost.

## Cheaper probes (`--oracle-score hc4`, `--probe-ctc`, offline on dataset-lffix)

`python/hc4probe_offline.py`, `hc4probe-offline.txt`. Slice the box along the
candidate (parts) and dims-1 LSmear partners, contract each piece, score by
pieces emptied then volume left; sum/base, conservative rules as before:

    probe contractor                     ms/cand   always   cons. r=0.25
    HC4 alone, 1x2 .. 2x4                0.2-1.0   1.05     1.02-1.04
    HC4 + polytope hull, 1x2 / 1x4       8 / 12    1.05     1.04 / 1.03
    the search's contractor, 1x2         13        1.007    0.984
    strong-branching dive step (labels)  -         0.846    0.837

None of them is the signal. Even the full contractor on the two children
(0.984) is far from the dive step of the labels (0.846). What the dive step
has and a contraction does not: the upper bounding (loup finder, Ipopt
sometimes) and the loup propagating from one child to the next. Being tested
as `--probe-ctc proc`.

Also offline: scoring the children by the rise of the goal lower bound
instead of volume is worse (0.869-0.887 vs 0.846).

### The offline probe evaluation is not trustworthy for process-based probes

End to end, `--probe-ctc proc --probe-parts 2 --sb-ratio 0.1` (contraction
and upper bounding on the two halves, conservative) is the strong-branching
conservative rule: `proc-e2e-10.txt` vs `sb-ratio-10.txt` r=0.1 -- dnieper
26/22, ex6_2_8 13000/13126, schwefel5 458/494, mconcon 334/334, ex8_5_6
1346/1424, dipigri 124/170, avgasb 40/34; only ship-1 differs (572 vs 134).
Yet offline the same probe scores 0.962 and the labels' dive step 0.837: the
offline harness replays each node from a saved state (Ipopt schedule, RNG,
ACID tuning), which changes what the loup finder does in the children (the
probe and the labels agree on the pruned count on 84% of the pairs and on
the volume on 50%). Offline numbers for probes that call the loup finder are
not comparable with the labels; the pure contractors (HC4, LP, full) do not
call it and their "no signal" stands.

### Upper bounding is part of what strong branching buys

lsmear-lffix with Ipopt called at every loup-finder call (`--ipopt-freq 1`)
instead of every 100th:

    instance   /100          /10           /1
    ship-1     680  1.9s     654  3.0s     6    1.0s
    dnieper    434  8.3s     654  14.6s    434  28.3s
    mconcon    18   0.1s     18   0.1s     26   1.2s
    schwefel5  698  0.8s     698  0.9s     682  2.1s
    dipigri    212  0.6s     224  0.7s     216  1.5s

ship-1's gain under strong branching (24 nodes) was upper bounding: its 2n
probes per node are 2n loup-finder calls. dnieper's (28 vs 434) is not: more
Ipopt does nothing there, the branching does.
