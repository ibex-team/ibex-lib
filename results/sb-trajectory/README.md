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
