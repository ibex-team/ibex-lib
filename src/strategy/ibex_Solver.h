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
#include "ibex_Bsc.h"
#include "ibex_CellBuffer.h"
#include "ibex_SubPaving.h"

#include <vector>

namespace ibex {

/** \ingroup strategy
 * \brief Solver
 */
class Solver {
public:
	/**
	 * \brief Build a solver.
	 */
	Solver(Ctc& ctc, Bsc& bsc, CellBuffer& buffer, double prec);

	/**
	 * \brief Solve the system.
	 */
	std::vector<IntervalVector> solve(const IntervalVector& init_box);

	/** Contractor. */
	Ctc& ctc;

	/** Bisector. */
	Bsc& bsc;

	/** Cell buffer. */
	CellBuffer& buffer;

	/** Precision of solutions. */
	double prec;

	/** Maximal number of cells created by the solver.
	 * This parameter allows to bound time complexity.
	 * The value can be fixed by the user. By default, it is -1 (no limit). */
	long cell_limit;

	/**
	 * \brief Trace activation flag.
	 *
	 * A flag for printing the trace. If set, the top of the buffer is printed
	 * on the standard output each time a new cell is created. Default value is \c false.
	 */
	bool trace;

	int nb_cells;
};

} // end namespace ibex
#endif // IBEX_SOLVER_H_
