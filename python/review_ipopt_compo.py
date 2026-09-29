#!/usr/bin/env python3
"""Review of the compo + ipoptxn sweep (results/ipopt-compo.csv).

  1. ranking by collection, the oracle and the gap each rule closes, with
     bootstrap intervals (lsmear-guard:10 from the switch points, see
     guard_horizon_ipopt.py);
  2. ref-ipopt against lsmear-guard:10: what each closes that the other does
     not, and the virtual two-rule portfolio;
  3. answers that are wrong or belong to another problem, and the ranking
     with them discounted;
  4. instances nobody closes, and those where no rule finds a feasible point.

    python3 python/review_ipopt_compo.py
"""
import os
os.chdir(os.path.join(os.path.dirname(os.path.abspath(__file__)), ".."))
import pandas as pd, numpy as np, sys, re, math
sys.path.insert(0,os.path.join(os.path.dirname(os.path.abspath(__file__)))); from experiment_bisectors import solved
L=600.
d=pd.read_csv('results/ipopt-compo.csv').drop_duplicates(['instance','rule'],keep='last')
d['ok']=[solved(r) for r in d.to_dict('records')]
d['par2']=np.where(d.ok,d.time,2*L)
P=d.pivot(index='instance',columns='rule',values='par2'); S=d.pivot(index='instance',columns='rule',values='ok').fillna(False).astype(bool)
N=d.pivot(index='instance',columns='rule',values='nodes')
sets=d.drop_duplicates('instance').set_index('instance')['set']
# add guard:10
sw={}
for line in open('results/guard-switch-ipopt.txt'):
    n,_,js=line.partition(' '); m=re.search(r'"guard_switched_at":(-?\d+)',js)
    if m: v=int(m.group(1)); sw[n]=v if v>=0 else math.inf
sw=pd.Series(sw).reindex(P.index); h=sw<=10
P['guard:10']=P['lsmear-guard'].where(h,P.lsmear); S['guard:10']=S['lsmear-guard'].where(h,S.lsmear)
coc=(sets.reindex(P.index)=='coconutbenchmark-library2')

print("== 1. ranking ==")

print(sets.value_counts().to_string())
rows=[]
for r in P.columns:
    rows.append((r,S[r].sum(),S[r][coc].sum(),S[r][~coc].sum(),P[r].mean(),P[r][coc].mean(),P[r][~coc].mean()))
t=pd.DataFrame(rows,columns=['regla','res','res coc','res resto','PAR2','PAR2 coc','PAR2 resto']).sort_values('PAR2')
print(t.round(1).to_string(index=False))
arms=[c for c in P.columns if not c.startswith('ref') and '@' not in c and c!='guard:10']
print("\noráculo sobre", arms)
orc=P[arms].min(axis=1); print("  res",S[arms].any(axis=1).sum(),"PAR2 %.1f"%orc.mean(), " coc %d resto %d"%(S[arms].any(axis=1)[coc].sum(),S[arms].any(axis=1)[~coc].sum()))
ls=P.lsmear
for r in ['lsmear-guard','guard:10','roundrobin','ref-ipopt']:
    print("brecha lsmear->oráculo cerrada por %-12s %.1f%%  (coc %.1f%%, resto %.1f%%)"%(r,*[100*(ls[m].sum()-P[r][m].sum())/(ls[m].sum()-orc[m].sum()) for m in (coc|~coc,coc,~coc)]))
# bootstrap
rs=np.random.RandomState(0); n=len(P)
for a,b in [('guard:10','lsmear'),('guard:10','roundrobin'),('guard:10','lsmear-guard'),('ref-ipopt','guard:10')]:
    x,y=P[a].values,P[b].values; D=[]
    for _ in range(4000):
        i=rs.randint(0,n,n); D.append(y[i].mean()-x[i].mean())
    print("PAR2(%s)-PAR2(%s): %.1f  IC95 [%.1f, %.1f]  P(>0)=%.3f"%(b,a,y.mean()-x.mean(),*np.percentile(D,[2.5,97.5]),np.mean(np.array(D)>0)))
print("\n== 2. ref-ipopt vs guard:10 ==")

g,r=S['guard:10'],S['ref-ipopt']
T=pd.DataFrame({'set':sets,'lsmear':P.lsmear,'guard:10':P['guard:10'],'ref':P['ref-ipopt'],'rr':P.roundrobin,'n_g':N['lsmear-guard'],'n_ref':N['ref-ipopt']})
print("solo ref-ipopt (%d):"%(r&~g).sum()); print(T[r&~g].round(1).to_string())
print("\nsolo guard:10 (%d):"%(g&~r).sum()); print(T[g&~r].round(1).to_string())
both=g&r; x=P['guard:10'][both]; y=P['ref-ipopt'][both]
q=np.exp(np.mean(np.log(np.maximum(y,1e-2)/np.maximum(x,1e-2)))); print("\nambos (%d): geo tiempo ref/guard %.2f; ref más rápido >1.5x y >5s en %d, guard en %d"%(both.sum(),q,((y<x/1.5)&(x-y>5)).sum(),((x<y/1.5)&(y-x>5)).sum()))
nb=both&(N['lsmear-guard']>0)
print("portafolio virtual min(guard:10, ref): res %d PAR2 %.1f"%((g|r).sum(),np.minimum(P['guard:10'],P['ref-ipopt']).mean()))
print("\n== 3-4. respuestas inválidas, instancias sin cerrar ==")

# descontar respuestas inválidas: incorrectas u otro problema
inval={('eigmaxc.bch','lsmear-guard'),('eigmaxc.bch','guard:10'),('ex8_2_4.bch','ref-ipopt'),('haldmads.bch','ref-ipopt'),
       ('test_infinity3.bch','ref-ipopt'),('test_infinity4.bch','ref-ipopt'),('ex8_4inf-1.bch','ref-ipopt')}
for rr in ['ref-ipopt-dh']:
    for i in ['ex8_2_4.bch','haldmads.bch','test_infinity3.bch','test_infinity4.bch','ex8_4inf-1.bch']:
        if S.at[i,rr]: inval.add((i,rr))
P2,S2=P.copy(),S.copy()
for i,r in inval:
    if r in P2 and S2.at[i,r]: P2.at[i,r]=2*L; S2.at[i,r]=False
print("respuestas invalidadas:",sorted(inval))
for r in ['ref-ipopt','ref-ipopt-dh','guard:10','lsmear-guard','roundrobin','lsmear']:
    print("%-13s res %d -> %d   PAR2 %.1f -> %.1f   coc %d resto %d"%(r,S[r].sum(),S2[r].sum(),P[r].mean(),P2[r].mean(),S2[r][coc].sum(),S2[r][~coc].sum()))
print("portafolio guard:10 + ref-ipopt (válido): res %d"%(S2['guard:10']|S2['ref-ipopt']).sum())
# no incumbent anywhere
fin=d.assign(f=np.isfinite(d.loup)&(d.loup.abs()<1e100)).pivot(index='instance',columns='rule',values='f')
none=~S.drop(columns=['guard:10']).any(axis=1)
noinc=none & ~fin.any(axis=1)
print("\nnadie cierra: %d; de esas sin ningún punto factible en ninguna regla: %d"%(none.sum(),noinc.sum()))
print(sorted(noinc[noinc].index))
