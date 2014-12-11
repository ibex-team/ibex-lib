//============================================================================
//                                  I B E X                                   
// File        : ibex_LinearRelaxFixed.h
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Aug 22, 2014
//============================================================================

#ifndef __IBEX_LINEAR_RELAX_FIXED_H__
#define __IBEX_LINEAR_RELAX_FIXED_H__

#include "ibex_LinearRelax.h"

namespace ibex {

/**
 * \ingroup numeric
 * \brief Fixed linear relaxation Ax<=b
 */
class LinearRelaxFixed : public LinearRelax {
public:
	/**
	 * \brief Create the linear inequalities Ax<=b.
	 */
	LinearRelaxFixed(const Matrix& A, const Vector& b);

	/**
	 * \brief Add the inequalities in the solver
	 */
	int linearization(const IntervalVector& box, LinearSolver& lp_solver);

	/**
	 * \brief Generation of a linear approximation of the inner region
	 *
	 */
	int inlinearization(const IntervalVector& box, LinearSolver& lp_solver);

	/**
	 * \brief Generation of a linear approximation of the linear objective function
	 *
	 */
	bool goal_linearization(const IntervalVector& box, LinearSolver& lp_solver);

private:
	/** The matrix */
	Matrix A;
	/** The vector */
	Vector b;

};


} // namespace ibex

#endif // __IBEX_LINEAR_RELAX_FIXED_H__
