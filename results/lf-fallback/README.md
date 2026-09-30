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
