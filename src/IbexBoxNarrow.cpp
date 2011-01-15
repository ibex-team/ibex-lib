/*---------------------------------------------------------------------------------
 * 
 * Box narrowing operator
 * ----------------------
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

#include "IbexBoxNarrow.h"
#include "IbexUnivNewton.h"

namespace ibex {

/*! Default criterion used by the box narrowing operator. */
#define __IBEX_DEFAULT_BOX_VAR_RATIO     0.03

BoxNarrow::BoxNarrow(const Equality& equ, Space& space, REAL var_ratio, REAL w_newton) : 
  Operator(space), equ(equ), var_ratio(var_ratio), newton(equ, space, w_newton), hc4revise(equ, space), occ_mult(new bool[space.nb_var()]), last_var(NO_VAR) {
  for (int i=0; i<space.nb_var(); i++) {
    hash_map<int,int>::const_iterator it=equ.adj.find(space.key(IBEX_VAR,i));
    occ_mult[i] = (it!=equ.adj.end() && it->second>1);
  }
}

BoxNarrow::BoxNarrow(const BoxNarrow& b) : 
  Operator(b.space), Contractor(b), equ(b.equ), var_ratio(b.var_ratio), newton(b.newton), hc4revise(equ, space), occ_mult(new bool[space.nb_var()]), last_var(b.last_var)  {
  for (int i=0; i<space.nb_var(); i++) {
    occ_mult[i] = b.occ_mult[i];
  }  
}

BoxNarrow::~BoxNarrow() {
  delete[] occ_mult;
}

void BoxNarrow::narrow(int var, bool left, REAL w) {
  
  INTERVAL& x(space.box(var+1));

  //cout << "calling newton on variable " << var << endl;  
  newton.contract(var,w); // may throw EmptyBoxException

  if (Diam(x)<w) return;

  REAL l = Inf(x);
  REAL m = Mid(x);
  REAL s = Sup(x);
  
  if (left) {
    
    x=INTERVAL(l,l+w); // bound evaluation. Could also be Succ(l) instead of l+w
    if (equ.feasible(space)) {
      x=INTERVAL(l,s); // restore the box (see comment above)
      return; 
    }

    x = INTERVAL(l,m);
    try {
      narrow(var,left,w);
      x=INTERVAL(Inf(x),s);
    }
    catch (EmptyBoxException) {
      x=INTERVAL(m,s);
      narrow(var,left,w);
    } 
  } else {

    x=INTERVAL(s-w,s); // bound evaluation. Could also be Pred(s) instead of s-w
    if (equ.feasible(space)) {
      x=INTERVAL(l,s); // restore x
      return;
    }

    x=INTERVAL(m,s);

    try {
      narrow(var,left,w);
      x=INTERVAL(l,Sup(x));
    }
    catch (EmptyBoxException) {
      x=INTERVAL(l,m);
      narrow(var,left,w);
    }
  }
}

void BoxNarrow::contract() {

  const Indicators* indic=current_indic();

  if (!indic) {
    Contractor::contract(Indicators(ALL_VARS, ALL_VARS));
    return;
  }

  int var = indic->scope;

  if (var==NO_VAR) return;    // who would call a contractor on NO variable?

  if (var==ALL_VARS) {
    Contractor::contract(Indicators(indic->impact, 0));
    for (int i=1; i<space.nb_var(); i++)
      Contractor::contract(Indicators(NO_VAR,i));
    return;
  }

  if (indic->impact==NO_VAR) {
    if (!occ_mult[var]) { 
      if (last_var!=NO_VAR && !occ_mult[last_var]) return; 
      /* Rem: the first condition is only for security. 
       * If indic->impact==NO_VAR, last_var should not be NO_VAR, 
       * since there is no meaning of providing a specific impact 
       * on the first call. */
    }
    else {
      if (last_var==var) return;
    }      
  }
  last_var = var;
  narrow(var);
}

void BoxNarrow::narrow(int var) {
  hc4revise.contract(); 
  if (!occ_mult[var]) return;
  REAL w = var_ratio*Diam(space.box(var+1));
  if (w<newton.w) w=newton.w; 
  narrow(var, true, w);
  narrow(var, false, w);
}

const REAL BoxNarrow::default_var_ratio = __IBEX_DEFAULT_BOX_VAR_RATIO;

} // end namespace
