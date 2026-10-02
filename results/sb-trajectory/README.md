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
   choice on the same nodes (sum of dive nodes, SB vs lffix choice):
   schwefel5 5606 vs 4774, ex8_5_6 4804 vs 4184, mconcon 4632 vs 4504; and
   on lffix's trajectory too on dipigri and ex8_5_6. The geometric mean hides
   it: SB is better on many small subtrees and worse on the large ones.
3. The trees go deep: max sampled depth 148 (mconcon; lffix: 8) and 85
   (schwefel5; lffix: 65). A per-decision loss of a few percent over lffix
   compounds level after level.
4. Not the probes: running every SB probe but bisecting where lffix says gives
   lffix's tree (schwefel5 680, mconcon 18). Not the ties either: SB's key
   ties at the top on 30-78% of the nodes, but breaking them by LSmear rank
   (tolerance 1e-6, kept in the code) barely changes it (mconcon 564,
   schwefel5 38654).

So the offline metric was the wrong one. A rule replacing the base bisector
everywhere must not be worse than the base's choice where subtrees are large
(the policy-improvement condition): measure the arithmetic sum of label
nodes against the base's choice, not the geometric regret against the best.
The one-step oracle meets it exactly (0.55x); SB and the models imitating it
do not.
