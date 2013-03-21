//============================================================================
//                                  I B E X                                   
// File        : XNewton : fixpoint of compo of XNewtonIter and contractor
// Author      :Bertrand Neveu
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : March 19, 2013
// Last Update : March 19, 2013
//============================================================================

#include "ibex_CtcXNewton.h"
#include "ibex_CtcFixPoint.h"
#include "ibex_CtcCompo.h"
using namespace std;
namespace ibex {

/*! Default fixpoint ratio. */
  const double CtcXNewton::default_xnewton_ratio = 0.2;

  CtcXNewton::CtcXNewton (CtcXNewtonIter & xnewtoniter, Ctc & ctc, double ratio) :
    CtcFixPoint(* (new CtcCompo (xnewtoniter, ctc)), ratio) {}
  
} // end namespace ibex
  
