//============================================================================
//                                  I B E X                                   
// File        : ibex_CellHeapOptim.cpp
// Author      : Gilles Chabert, Bertrand Neveu
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : May 14, 2012
// Last Update : May 14, 2012
//============================================================================

#include "ibex_CellHeapOptim.h"
#include "ibex_Optimizer.h"

namespace ibex {

/** The cost is already computed, it is the min of the
 * last variable corresponding to the objective */
double CellHeapOptim::cost(const Cell& c) const {
  return c.get<OptimCrit>().y.lb();
}


} // end namespace ibex
