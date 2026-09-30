# LSmear's largest-first fallback

`OptimLargestFirst` ranks by `diam/prec(i)` when the precision is a vector.
IbexOpt passes one whose entries are all 0 by default (`eps_x=0`): every key
is +inf, none beats the first, and it always returns the first bisectable
variable. LSmear falls back to it whenever the Jacobian has an infinite entry.

`paths.txt`: for each instance, LSmear (compo + ipoptxn, 5 s, first 3000
decisions): group (`sw`: lsmear-guard switches in results/guard-switch-ipopt.txt,
`ns`: it does not), instance, decisions, via largest first on an infinite
derivative, via largest first because no candidate, via the unbounded-box path.

    group  instances  any  >10%  >50%
    sw        86       51    45    41
    ns       190       10     7     3

The guard's hijack is, in about half of the cases where it switches, this
fallback bisecting the first variable over and over (ship-1: 2147 of 2809
decisions, x1 forty levels down). `lsmear-lffix` (OptimLargestFirstFixed:
widths when the precision is 0) solves ship-1 in 680 nodes; LSmear times out.

## The variants on the 10 usual instances (compo + ipoptxn, 120 s)

`variants-10.txt`, nodes (T = timeout):

    instance   lsmear  lffix  guard:10  guard-lffix  guard-next  avoid  grasp.8
    ship-1     T       680    486       680          4034        834    T
    dnieper    434     434    434       434          434         422    556
    ex6_2_8    13672   13672  13672     13672        13672       15140  13736
    schwefel5  T       698    956       698          1996        754    T
    mconcon    T       18     116       18           2128        18     T
    avgasb     54      54     54        54           54          54     50
    dipigri    212     212    212       212          212         200    206
    avgasa     188     188    188       188          188         182    198
    dualc2     146     146    146       146          146         140    152
    ex8_5_6    T       1852   2144      1852         1362        1272   T

lffix removes all four LSmear timeouts and matches LSmear wherever LSmear
never takes the fallback; the guard never switches on top of it. It beats
guard:10 on schwefel5, mconcon, ex8_5_6 and loses on ship-1. lsmear-avoid
(never the parent's variable, on any path) is close or better almost
everywhere, except ex6_2_8. guard-next is worse than guard:10 when it
matters. grasp, whose randomness only touches LSmear's own ranking, keeps the
timeouts.

## Tabu list along the branch (`lsmear-tabu --tabu-tenure k`)

`tabu-10.txt`; k=1 is lsmear-avoid. Nodes, and geometric mean relative to
guard:10 (with / without mconcon, where everything but guard:10 takes 18-20):

    instance   guard:10  lffix  k=1    k=2    k=3    k=5
    ship-1     486       680    834    2662   2832   756
    dnieper    434       434    422    634    314    616
    ex6_2_8    13672     13672  15140  37806  61426  22784
    schwefel5  956       698    754    710    642    748
    mconcon    116       18     18     20     20     18
    avgasb     54        54     54     58     56     58
    dipigri    212       212    200    204    192    260
    avgasa     188       188    182    186    196    200
    dualc2     146       146    140    142    134    126
    ex8_5_6    2144      1852   1272   2064   2676   1924
    geo        1         0.82   0.81   1.11   1.09   0.93
    w/o mconc  1         0.99   0.97   1.36   1.34   1.14

A longer tenure is worse: forbidding more than the parent's variable
overrides LSmear where it was right (ex6_2_8, which never takes the
fallback, 2.8x-4.5x with k=2,3).

## Tabu on capture (`lsmear-tabu --tabu-tenure T`)

The rule above is now `lsmear-recent`. `lsmear-tabu` makes a variable tabu
only when it captures LSmear (LSmear chooses the parent's variable again),
for T levels along the branch; T=1 is lsmear-avoid. `tabu-capture-10.txt`,
nodes (H: Ipopt hung inside one call, past the 120 s limit):

    instance   guard:10  lffix  T=1    T=2    T=3    T=5    T=10
    ship-1     486       680    834    1850   1994   1412   910
    dnieper    434       434    422    634    314    606    T
    ex6_2_8    13672     13672  15140  35792  75686  H      23534
    schwefel5  956       698    754    618    684    H      612
    mconcon    116       18     18     20     20     20     20
    avgasb     54        54     54     54     54     54     54
    dipigri    212       212    200    200    200    214    228
    avgasa     188       188    182    182    182    182    182
    dualc2     146       146    140    148    132    146    128
    ex8_5_6    2144      1852   1272   1936   1614   2202   1738

No tenure beats T=1: keeping a captured variable out after the capture point
also keeps it out where LSmear is right to come back to it (ex6_2_8 has
captures and needs the variable again: 2.6x-5.5x).
