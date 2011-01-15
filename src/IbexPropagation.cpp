
/*---------------------------------------------------------------------------------
 * 
 * Generic Propagation-based contractor (Hull consistency, Box consistency, etc.)
 * ----------------------------------------------------------------------------
 *
 * Copyright (C) 2007-2009 Gilles Chabert
 * 
 * This file is part of IBEX.
 *
 * IBEX is free software; you can redistribute it and/or modify it under the terms of 
 * the GNU General Public License as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later version.
 *
 * IBEX is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; 
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
 * PURPOSE. See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with IBEX; 
 * if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, 
 * Boston, MA 02110-1301, USA 
 *
 --------------------------------------------------------------------------------*/

#include "IbexPropagation.h"

namespace ibex {

/*! Default propagation ratio. */
#define __IBEX_DEFAULT_RATIO_PROPAG           0.1

Propagation::Propagation(const ContractorList& cl, Space& space, REAL ratio, bool incremental) : 
  Operator(space), list(cl), ratio(ratio), incremental(incremental), 
  adj(list, space), propQ(list.size(), space.nb_var()) {

  for (int i=0; i<list.size(); i++)
    list(i).contract_floor=0;
}

Propagation::Propagation(const Propagation& p) : 
  Operator(p.space), Contractor(p), list(p.list), ratio(p.ratio), incremental(p.incremental), 
  adj(p.list, p.space), propQ(p.list.size(),p.space.nb_var()) { 
}

void Propagation::contract() {
  const Indicators* indic=current_indic();
  if (incremental && indic) 
    if (indic->impact==NO_VAR)
      return;
    else
      contract(indic->impact);
  else 
    contract(ALL_VARS);
}

void  Propagation::contract(int start) {
  int  c,v;                          // current constraint & variable
  int  old_c = -1;

  INTERVAL_VECTOR& box(space.box);
  INTERVAL_VECTOR propbox(box);  // variables domains before last propagation

  /********************************/
  /* option "fine" propagation  */
  INTERVAL_VECTOR projbox(box); // variables domains before last projection
  /********************************/

  //   VECTOR thres(_nb_var);        // threshold for propagation
  //   for (int i=1; i<=_nb_var; i++) {
  //     thres(i) = ratio*Diam(box(i));
  //     if (thres(i)<w) thres(i)=w;
  //   }
  Indicators p;

  if (start==ALL_VARS) adj.init(propQ);
  else adj.propagate(propQ,start);

  while (!propQ.empty()) {

    propQ.pop(c,v);

    //cout << "Narrowing for (c" << c << ", v" << v << ")" << endl;
//     cout << "Narrowing for " << list(c) << " on " << space.env.symbol_name(space.key(IBEX_VAR, v)) << " -> " << space.box(v+1) << endl;
//     space.set_output_flags(31);
//     cout << space << endl;
    //projbox(v+1) = box(v+1);

    if (c!=old_c)
      for (int i=0; i<adj.ctr_nb_vars(c); i++)
	projbox(adj.ctr_ith_var(c,i)+1) = box(adj.ctr_ith_var(c,i)+1);

    p.scope = v;
    if (c==old_c) p.impact = NO_VAR;
    else {
      p.impact = ALL_VARS;
      old_c = c;
    }

    /********************************/
    /* "fine" propagation option  */
    /********************************/
    try {
      list(c).contract(space, p);
    }
    catch (EmptyBoxException e) {
      propQ.flush();
      throw e;
    }

    //cout << "  =>" << space.box(v+1) << endl;

    if (propbox(v+1).ratiodelta(box(v+1))>=ratio) {
      //cout << "before prop q=" << propQ << endl;
      /********************************/
      /* "fine" propagation option  */      
      if (projbox(v+1).ratiodelta(box(v+1))>=ratio) {
	adj.propagate(propQ,c,v);
      } else {
      /********************************/
	adj.propagate(propQ,v); //(c,v); 
      //       cout << "q=" << propQ << endl;
      }
      propbox(v+1) = box(v+1); 
    }    
  } 

  /* we cancel the "residual" contractions 
   * in order to trigger more propagation next
   * time this function is called. This action
   * is meaningful when the contract floor is
   * small w.r.t the ratio here. */
  //   if (!reducted) box = propbox; // restore domains
  
}

const REAL Propagation::default_ratio = __IBEX_DEFAULT_RATIO_PROPAG;

} // end namespace
