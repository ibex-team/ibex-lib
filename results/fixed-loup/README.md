# Bisection alone: the loup fixed at the optimum

`--initial-loup` set to the optimum found by lsmear-lffix (`optima.txt`), so
that the tree depends on the branching (and the contraction), not on when
the upper bounding finds a good point. 10 usual instances, compo + ipoptxn,
300 s; `table.txt`, `runs-10.jsonl`. Nodes:

    instance   lffix   guard10  SB      SB cons r=0.1  oracle (lffix dives)
    avgasa     174     174      162     160            134
    avgasb     40      40       34      36             38
    dipigri    204     204      102     136            78
    dnieper    24058   24058    34      58             timeout at 26
    dualc2     140     140      188     144            94
    ex6_2_8    13692   13692    15372   13146          killed
    ex8_5_6    1532    1810     1144    1330           946
    mconcon    10      8        8       8              8
    schwefel5  730     622      440     458            548
    ship-1     0 (the optimum prunes the root)

Geometric mean over lffix (8 instances, dnieper aside): guard10 0.97,
SB 0.83 (worst 1.34), SB conservative 0.84 (worst 1.03), oracle 0.68 on
the 7 it closes (worst 0.95).

1. mconcon's blow-up under SB (678 vs 18) was upper bounding entirely: with
   the optimum known every rule takes 8-10 nodes.
2. With the loup fixed, SB does not blow up anywhere (worst 1.34, dualc2);
   the conservative rule never loses more than 3%.
3. dnieper's 24058 under lffix is an artifact: with a tight loup from the
   start ACID tunes itself to shave 0 variables (IBEX_ACID_TRACE: nbcidvar=0
   after 24 tunings, vs 20 without the optimum) and the contraction weakens
   for the whole search. Any initial loup within ~7% does it (20000 too);
   1e9 does not (434). SB's gain there is real branching: it closes in ~30
   nodes before ACID ends its first tuning phase, vs 434 for lffix with ACID
   fully on. Hence the dataset fixes the loup inside the dives only
   (--dive-loup), and the search tunes ACID as usual.
4. The bisection headroom is real: the oracle at 0.68, SB at 0.83.
