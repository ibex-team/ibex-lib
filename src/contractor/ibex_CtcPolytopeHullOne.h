//============================================================================
//                                  I B E X                                   
// File        : ibex_CtcPolytopeHull.h
// Authors     : Jordan Ninin, Bertrand Neveu, Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Oct 31, 2013
// Last Update : Oct 31, 2013
//============================================================================

#ifndef __IBEX_CTC_POLYTOPE_HULL0NE_H__
#define __IBEX_CTC_POLYTOPE_HULLONE_H__

#include "ibex_CtcPolytopeHull.h"

namespace ibex {

/**
 * \brief Contract the bounds of a box with respect to a polytope.
 *
 * The polytope is obtained by linearizing a system.
 * \see #LinearRelax.
 */
class CtcPolytopeHullOne : public CtcPolytopeHull {
public:

	/**
	 * \brief Creates the iteration
	 *
	 * \param lr       - The linear relaxation
	 * \param cmode    - ALL_BOX (contracts all variables in the box) | ONLY_Y (in optimization :only improves
	 *                   the left bound of the objective)
	 * \param max_iter - The maximum number of iterations of the linear solver (default value 100)
	 * \param timeout  - The timeout of the linear solver at each iteration    (default value 100 second)
	 * \param eps      - The accuracy required on the resolution of the linear program (default value 1e-10)
	 */

	CtcPolytopeHullOne(LinearRelax& lr, ctc_mode cmode=ALL_BOX, int max_iter=LinearSolver::default_max_iter,
			int time_out=LinearSolver::default_max_time_out, double eps=LinearSolver::default_eps,
			Interval limit_diam=LinearSolver::default_limit_diam_box);

	/**
	 * \brief Creates the contractor w.r.t. Ax<=b
	 *
	 * \param A        - Matrix in Ax<=b
	 * \param b        - Vector in Ax<=b
	 * \param max_iter - The maximum number of iterations of the linear solver (default value 100)
	 * \param timeout  - The timeout of the linear solver at each iteration    (default value 100 second)
	 * \param eps      - The accuracy required on the resolution of the linear program (default value 1e-10)
	 */
	CtcPolytopeHullOne(const Matrix& A, const Vector& b, int max_iter=LinearSolver::default_max_iter,
			int time_out=LinearSolver::default_max_time_out, double eps=LinearSolver::default_eps,
			Interval limit_diam=LinearSolver::default_limit_diam_box);

	virtual void contract(IntervalVector& box);

	virtual ~CtcPolytopeHullOne() { };


};

} // end namespace ibex
#endif // __IBEX_CTC_POLYTOPE_HULL_H__
