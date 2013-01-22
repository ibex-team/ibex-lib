/*---------------------------------------------------------------------------------
 * 
 * Newton prover
 * ----------------------------
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

#include "IntervalVector.h"
#include "IbexNewtonProver.h"
#include "IbexLinear.h"

namespace ibex {

NewtonProver::NewtonProver(const System& sys) : Operator(sys.space), sys(sys) { 

  for (int i=0; i<sys.nb_ctr(); i++)
    if (!sys.ctr(i).is_equality()) throw NonRecoverableException("Cannot create Newton contractor with inequalities");
}

NewtonProver::NewtonProver(const NewtonProver& newt) : 
  Operator(newt.space), sys(newt.sys) { }


void NewtonProver::contract() {

  INTERVAL_VECTOR& box(sys.box);

  int n=sys.nb_var();

  if (sys.nb_ctr()!=n) return;

  INTERVAL_MATRIX J(n, n);
  INTERVAL_VECTOR y(n);
  INTERVAL_VECTOR mid(n);
  INTERVAL_VECTOR Fmid(n);
  bool reducted=false;
  REAL gain;

  try {
    sys.hansen_matrix(J);
  } 
  //may also throw UnfeasibilityException
  catch (EvalException) {
    cout << "eval exception\n";
    return;
  }
  catch (UnfeasibilityException) {
    cout << "unfeasibility\n";
    return;
  }
  
  mid = Mid(box);
    
  sys.eval_mid(Fmid);

  try {
    //cout << "before precond: J=" << J << " Fmid=" << Fmid << endl;
    Precond(J, Fmid);
    //cout << "after precond: J=" << J << " Fmid=" << Fmid << endl;
    HansenBliek(J, Fmid, y);

    cout << "y=" << y << " mid-box=" << mid-box << endl;

    if (y.included(mid-box)) {
      cout << "prover success!!!\n";
      throw UnfeasibilityException();
    }

    if (y.empty()) {
      return; 
    } //throw UnfeasibilityException();
  }
  catch (LinearException) {
    cout << "linear exception\n";
    return;
  }
}


} // end namespace
