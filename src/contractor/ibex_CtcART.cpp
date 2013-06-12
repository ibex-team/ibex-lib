//============================================================================
//                                  I B E X                                   
// File        : ibex_CtcART.cpp
// Author      : Jordan Ninin
// License     : See the LICENSE file
// Created     : May 19, 2013
// Last Update : May 19, 2013
//============================================================================

#include "ibex_CtcART.h"
#include "ibex_CtcFixPoint.h"
#include "ibex_CtcCompo.h"
using namespace std;
namespace ibex {

/*! Default fixpoint ratio. */
  const double CtcART::default_art_ratio = 0.2;

  CtcART::CtcART (CtcARTiter & iter, Ctc & ctc, double ratio) :
    CtcFixPoint(* (new CtcCompo (iter, ctc)), ratio) {}

} // end namespace ibex

