//============================================================================
//                                  I B E X                                   
// File        : ibex_Solver.h
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : May 13, 2012
// Last Update : May 13, 2012
//============================================================================

#ifndef IBEX_SOLVER_H_
#define IBEX_SOLVER_H_

#include "ibex_Ctc.h"
#include "ibex_CtcPrecision.h"
#include "ibex_Strategy.h"

#include <vector>

namespace ibex {

class Solver : public Strategy {
public:
	Solver(Ctc& ctc, double prec);

	// This field allows to bound the number of solutions.
	std::vector<IntervalVector> solve(const IntervalVector& init_box);

	/** Contractor. */
	Ctc& ctc;

	/** Precision */
	CtcPrecision prec;
};

} // end namespace ibex
#endif // IBEX_SOLVER_H_
