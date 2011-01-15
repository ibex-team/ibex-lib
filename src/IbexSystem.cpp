/*---------------------------------------------------------------------------------
 * 
 * System of equations
 * -------------------
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

#include "IbexEnv.h"
#include "IbexSystem.h"
#include "IbexSystemFactory.h"
#include <typeinfo>

namespace ibex {

void System::check_constraints() {
  for (int i=0; i<nb_ctr(); i++)
    try {
      dynamic_cast<const ArithConstraint&>(CSP::ctr(i));
    } catch(std::bad_cast) {
      throw NonRecoverableException("One constraint of the system is not arithmetical");
    }
}

System::System(const vector<const ArithConstraint*>& ctrs, Space &space) : 
  CSP((const vector<const Constraint*>&) ctrs,space) { }

System::System(const vector<const ArithConstraint*>& ctrs, SpaceFactory &fac) : 
  CSP((const vector<const Constraint*>&)ctrs, fac) { }

System::System(SpaceFactory &fac) : 
  CSP(fac) { 
  check_constraints();
}

System::System(const char* filename) : CSP(filename) { 
  check_constraints();
}

System::System(const System& sys) : CSP(sys) { }

void System::eval_pt(INTERVAL_VECTOR& f, const VECTOR& pt) const {
  INTERVAL_VECTOR savebox = space.box;
  (INTERVAL_VECTOR&) space.box = pt;
  eval(f);
  (INTERVAL_VECTOR&) space.box = savebox;
}

void System::jacobian(INTERVAL_MATRIX& J) const {

  if (RowDimension(J)!=nb_ctr() || ColDimension(J)!=nb_var()) throw NonRecoverableException("Bad argument dimension");

  for (int i=0; i<nb_ctr(); i++) {    
    for (int j=0; j<nb_var(); j++) {
      J(i+1,j+1) = 0.0;
      space.ent(IBEX_VAR,j).deriv = & J(i+1,j+1); 
    }
    ctr(i).gradient(space); 
  }    
}

void System::hansen_matrix(INTERVAL_MATRIX& H) const {
  
  if (RowDimension(H)!=nb_ctr() || ColDimension(H)!=nb_var()) throw NonRecoverableException("Bad argument dimension");

//   int p[nb_var()];
//   box.diameter_sort(p);
  
  INTERVAL_VECTOR save(space.box);
  INTERVAL_VECTOR grad(nb_var());
  (INTERVAL_VECTOR&) space.box = Mid(space.box);

  for (int var=0; var<nb_var(); var++) {

    ((INTERVAL_VECTOR&) space.box)(var+1) = save(var+1);

    for (int i=0; i<nb_ctr(); i++) {
      for (int j=0; j<nb_var(); j++) {
	grad(j+1)=0.0;
	space.ent(IBEX_VAR,j).deriv = &grad(j+1); 
      }
      try {
	ctr(i).gradient(space); 
      } 
      
      catch(EvalException& e) {
	(INTERVAL_VECTOR&) space.box = save;
	throw e;
      }      
      H(i+1, var+1)=grad(var+1);
    }
    
  }  
  (INTERVAL_VECTOR&) space.box = save;
}

} // end namespace
