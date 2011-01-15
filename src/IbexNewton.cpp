/*---------------------------------------------------------------------------------
 * 
 * Multivariate Newton operator
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
#include "IbexNewton.h"
#include "IbexLinear.h"

namespace ibex {

/** Default criterion used to enfore interval Newton iteration. */
#define __IBEX_DEFAULT_NEWTON_CEIL 0.01

/*! Default precision of Newton iteration. */
#define __IBEX_DEFAULT_NEWTON_PRECISION 1e-07

/*! Default criterion used by Gauss-Seidel. */
#define __IBEX_DEFAULT_NEWTON_RATIO_GAUSS_SEIDEL  1e-04 

Newton::Newton(const System& sys, REAL ceil, REAL prec, REAL ratio_gauss_seidel) :  
  Operator(sys.space), sys(sys), ceil(ceil), prec(prec), ratio_gauss_seidel(ratio_gauss_seidel) { 
  
  for (int i=0; i<sys.nb_ctr(); i++)
    if (!sys.ctr(i).is_equality()) throw NonRecoverableException("Cannot create Newton contractor with inequalities");
}

Newton::Newton(const Newton& newt) : 
  Operator(newt.space), Contractor(newt), sys(newt.sys), ceil(newt.ceil), prec(newt.prec), ratio_gauss_seidel(newt.ratio_gauss_seidel) { 
}


bool Newton::iterate() {

  INTERVAL_VECTOR& box(sys.box);

  if (sys.nb_ctr()!=sys.nb_var() || !(box.max_diameter()<=ceil)) return false;

  INTERVAL_MATRIX J(sys.nb_ctr(), sys.nb_var());
  INTERVAL_VECTOR y(sys.nb_var());
  INTERVAL_VECTOR y1(sys.nb_var());
  INTERVAL_VECTOR mid(sys.nb_var());
  INTERVAL_VECTOR Fmid(sys.nb_ctr());
  bool reducted=false;
  REAL gain;
  y1= Mid(box);
  do {

    try {
      sys.hansen_matrix(J);
    } 
    //may also throw EmptyBoxException
    catch (EvalException) {
      return reducted; // should be false
    }
  
    mid = Mid(box);
    
    sys.eval_mid(Fmid);
    
    y = mid-box;
    if (y==y1) break;
    y1=y;

    try {
      Precond(J, Fmid);
      GaussSeidel(J, Fmid, y, ratio_gauss_seidel);
      if (y.empty()) throw EmptyBoxException();
    }
    catch (LinearException) {
      return reducted; // should be false
    }

    INTERVAL_VECTOR box2=mid-y;

    if (!(box2 &= box)) throw EmptyBoxException(); 

    gain = box.maxdelta(box2);
    
    if (gain >= prec) reducted = true;
    
    //    if (reducted) box = box2;
    box=box2;

  }
  while (gain >= prec);
  return reducted;
}

const REAL Newton::default_ceil = __IBEX_DEFAULT_NEWTON_CEIL;
const REAL Newton::default_prec = __IBEX_DEFAULT_NEWTON_PRECISION;
const REAL Newton::default_ratio_gauss_seidel=__IBEX_DEFAULT_NEWTON_RATIO_GAUSS_SEIDEL;

} // end namespace
