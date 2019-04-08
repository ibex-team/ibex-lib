//============================================================================
//                                  I B E X                                   
// File        : ibex_LinearizerFixed.h
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Aug 22, 2014
//============================================================================

#ifndef __IBEX_LINEARIZER_FIXED_H__
#define __IBEX_LINEARIZER_FIXED_H__

#include "ibex_Linearizer.h"

namespace ibex {

/**
 * \ingroup numeric
 *
 * \brief Fixed linear system Ax<=b
 */
class LinearizerFixed : public Linearizer {
public:
	/**
	 * \brief Create the linear inequalities Ax<=b.
	 */
	LinearizerFixed(const Matrix& A, const Vector& b);

	/**
	 * \brief Add the inequalities in the LP solver.
	 */
	int linearize(const IntervalVector& box, LPSolver& lp_solver);

protected:

	/** The matrix */
	Matrix A;

	/** The vector */
	Vector b;
};

} // namespace ibex

#endif // __IBEX_LINEARIZER_FIXED_H__
