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
