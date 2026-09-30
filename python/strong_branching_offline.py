#!/usr/bin/env python3
"""Offline: how well do one-level lookahead (strong branching) scores pick the
best candidate of each sample of results/dataset-guard10? Uses the children
boxes and statuses every label already carries.

    python3 python/strong_branching_offline.py
"""
import glob, json, os, math, numpy as np, pandas as pd
g=lambda x: float(np.exp(np.mean(np.log(x))))
def logvol(box, parent, skip):
    s=0.0
    for j,(a,b) in enumerate(box):
        if j==skip: continue
        pa,pb=parent[j]
        dp=pb-pa; d=b-a
        if not (math.isfinite(dp) and dp>0 and math.isfinite(d)): continue
        s+=math.log(max(d,1e-300)/dp)
    return s
rows=[]
for f in sorted(glob.glob(os.path.join(os.path.dirname(os.path.abspath(__file__)),'..','results','dataset-guard10','samples','*.jsonl'))):
    inst=os.path.basename(f)[:-6]
    for line in open(f):
        s=json.loads(line); L=[l for l in s['labels'] if l['valid']]
        done=[l['nodes'] for l in L if not l['censored']]
        if len(L)<2 or not done: continue
        best=min(done); n=s['node']
        parent=[(v['lb'],v['ub']) for v in n['vars']]
        gv=next((j for j,v in enumerate(n['vars']) if v.get('is_goal')),-1)
        c=[]
        for l in L:
            y=l['nodes'] if not l['censored'] else 2*l['budget_used']
            st=[l['left_status'],l['right_status']]; ch=[l['left'],l['right']]
            npr=sum(x!='open' for x in st)
            lv=[logvol(b,parent,gv) if st[k]=='open' and b else -700.0 for k,b in enumerate(ch)]
            ylb=[(b[gv][0] if st[k]=='open' and b else math.inf) for k,b in enumerate(ch)]
            y0=parent[gv][0]
            dl=[max(1e-9,min(yl,1e300)-y0) if math.isfinite(y0) else 1e-9 for yl in ylb]
            c.append(dict(var=l['var'],r=y/best,pruned=npr,sumvol=np.logaddexp(lv[0],lv[1]),maxvol=max(lv),
                          prodlb=math.log(dl[0])+math.log(dl[1]),minlb=min(math.log(dl[0]),math.log(dl[1])),
                          guard=l['var']==n.get('bisect_var'),lsmear=l['var']==n.get('lsmear_var')))
        C=pd.DataFrame(c)
        pick={'lsmear-guard:10':C[C.guard], 'lsmear':C[C.lsmear], 'oráculo':C.loc[[C.r.idxmin()]],
              'más hijos podados, luego menor vol. total':C.sort_values(['pruned','sumvol'],ascending=[False,True]).head(1),
              'menor volumen total de los hijos':C.loc[[C.sumvol.idxmin()]],
              'menor volumen del hijo más grande':C.loc[[C.maxvol.idxmin()]],
              'mayor producto de mejoras de cota (SB de MILP)':C.loc[[C.prodlb.idxmax()]],
              'mayor mejora de cota del peor hijo':C.loc[[C.minlb.idxmax()]]}
        r={'instance':inst}
        for k,v in pick.items():
            if len(v): r[k]=float(v.r.iloc[0])
        rows.append(r)
T=pd.DataFrame(rows)
print("%d muestras, %d instancias"%(len(T),T.instance.nunique()))
for k in [c for c in T.columns if c!='instance']:
    x=T[k].dropna(); per=T.groupby('instance')[k].apply(lambda v:g(v.dropna()) if v.notna().any() else np.nan).dropna()
    print("  %-48s regret %.3f  mejor %3.0f%%  >=2x %3.0f%%"%(k,g(per),100*(x<=1.0001).mean(),100*(x>=2).mean()))
