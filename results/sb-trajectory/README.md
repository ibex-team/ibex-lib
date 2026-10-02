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
