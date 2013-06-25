//============================================================================
//                                  I B E X                                   
// File        : CtcLinearRelaxation : fixpoint of compo of XNewtonIter and contractor
// Author      : Bertrand Neveu, Jordan Ninin
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : March 19, 2013
// Last Update : June 24, 2013
//============================================================================

#include "ibex_CtcLinearRelaxation.h"

namespace ibex {

/*! Default fixpoint ratio. */
  const double CtcLinearRelaxation::default_relax_ratio = 0.2;

  CtcLinearRelaxation::CtcLinearRelaxation (CtcLinearRelaxationIter &iter, Ctc & ctc, double ratio) :
    CtcFixPoint(* (new CtcCompo (iter, ctc)), ratio) {}
  
} // end namespace ibex
  
