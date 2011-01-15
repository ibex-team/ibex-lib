/*---------------------------------------------------------------------------------
 * 
 * Monotonicity Contractor  
 * ------------------------
 *
 * Copyright (C) 2009 Gilles Chabert
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

#include "IbexOctum.h"

namespace ibex {

/** Reverses what has been modified before backtracking */
MonotonicityFlags::~MonotonicityFlags() { 
  if (root) {
    for (hash_map<int,int*>::const_iterator it=flags.begin(); it!=flags.end(); it++)
      delete[] it->second;
    delete &flags;
  } else {
    for (vector<pair<int,int> >::const_iterator it=changes.begin(); it!=changes.end(); it++)
      flags[it->first][it->second]=0; // reverse
  }
}

/** Add a new entry for equation \a equ_num involving
 * \a nb_var variables. */
void MonotonicityFlags::add(int equ_num, int nb_var) {
  flags.insert(std::pair<int,int*>(equ_num, new int[nb_var]));
  for (int i=0; i<nb_var; i++) // i=counter for the variables involved in this equation
    flags[equ_num][i]=0;    
}

Octum::Octum(const Equality& equ, Space& space) : 
  Operator(space), equ(equ), newton(equ,space), hc4rev(equ,space), equ_num(equ.get_env_num()),
  nb_var(equ.adj.size()) { 
  
  init();
}
  
Octum::Octum(const Octum& m) : 
  Operator(m.space), Contractor(m), equ(m.equ), newton(m.newton), hc4rev(m.hc4rev), equ_num(m.equ_num),
  nb_var(m.nb_var) { 
  
  init();  
}

Octum::~Octum() {
  delete[] variables;
}

void Octum::contract() {
  bool mono=true;
  INTERVAL_VECTOR inf=space.box;
  INTERVAL_VECTOR sup=space.box;

  /*------------------------------------------------------------*
  // solution with monotonicity flags 
  //----------------------------------*/
  /*
  if (current_cell()!=NULL) {
    Cell& cell(*current_cell());
    MonotonicityFlags f = cell.get<MonotonicityFlags>(); 
    
    if (!f.registered(equ_num)) { //create_flags_entry(f);
      f.add(equ_num, nb_var);    
      mono=false;
    }
    
    for (int i=0; mono && i<nb_var; i++) {
      int var=variables[i];

      switch (f.get_flag(equ_num,i)) {
      case -1: inf(var+1)=Inf(space.box(var+1));
	sup(var+1)=Sup(space.box(var+1));
	break;
      case +1: inf(var+1)=Sup(space.box(var+1));
	sup(var+1)=Inf(space.box(var+1));
	break;
      case  0: mono=false;
	break;		 
      }
    }
  } else {
  */

  /*------------------------------------------------------------*
  // solution without monotonicity flags
  //-------------------------------------*/
  INTERVAL_VECTOR G(space.nb_var());
  try {
    equ.gradient(space,G);
  } catch(NotDifferentiableException) {
    mono=false;
  } catch(UnboundedResultException) {
    mono=false;
  }

  for (int i=0; mono && i<nb_var; i++) {
    int var=variables[i];
    if (Inf(G(var+1))>0) {
      inf(var+1)=Inf(space.box(var+1));
      sup(var+1)=Sup(space.box(var+1));
    } else if (Sup(G(var+1))<0) {
      inf(var+1)=Sup(space.box(var+1));
      sup(var+1)=Inf(space.box(var+1));
    } else { 
      mono=false;
    }
  }
  /*------------------------------------------------------------*/

  if (!mono) return hc4rev.contract();


  INTERVAL_VECTOR y=space.box; // will contain the result
 
  space.box=inf;
  INTERVAL img_inf=equ.eval(space);
  space.box=sup;
  INTERVAL img_sup=equ.eval(space);

  if (Inf(img_inf)>0 || Sup(img_sup)<0) throw EmptyBoxException();
  
  bool sup_ok=false;
  bool inf_ok=false;
  
  for (int i=0; i<nb_var; i++) {
    bool curr_sup = sup_ok;
    int var=variables[i];
    REAL prec = 0.01 * Diam(space.box(var+1));  // relative precision : 1%
    if (!inf_ok) {
      try {
	space.box = sup;
	space.box(var+1)=y(var+1);

	newton.monotone_contract(var, G (var+1), prec);

	if (Inf(G(var+1))>0)
	  //	if (f.get_flag(equ_num,i)==1) 
	  y(var+1) = INTERVAL(Inf(space.box(var+1)), Sup(y(var+1)));
	else
	  y(var+1) = INTERVAL(Inf(y(var+1)), Sup(space.box(var+1)));

	sup_ok=true;
      } catch(EmptyBoxException) { }
    } 
    
    if (!curr_sup) {
      try {
	space.box = inf;
	space.box(var+1)=y(var+1);
        
	newton.monotone_contract(var, G(var+1), prec);

	if (Inf(G(var+1))>0) 
	  //	if (f.get_flag(equ_num,i)==1) 
	  y(var+1) = INTERVAL(Inf(y(var+1)), Sup(space.box(var+1)));
	else
	  y(var+1) = INTERVAL(Inf(space.box(var+1)), Sup(y(var+1)));

	inf_ok=true;
      } catch(EmptyBoxException) { }
    }
  }

  space.box = y;
}

}
