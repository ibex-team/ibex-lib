//============================================================================
//                                  I B E X                                   
// File        : ibex_CtcPolytopeHull.h
// Authors     : Jordan Ninin, Bertrand Neveu, Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Oct 31, 2013
// Last Update : Oct 31, 2013
//============================================================================

#ifndef __IBEX_CTC_POLYTOPE_HULL_H__
#define __IBEX_CTC_POLYTOPE_HULL_H__

#include "ibex_Ctc.h"
#include "ibex_LinearRelax.h"
#include "ibex_LinearSolver.h"

namespace ibex {

/**
 * \brief Contract the bounds of a box with respect to a polytope.
 *
 * The polytope is obtained by linearizing a system.
 * \see #LinearRelax.
 */
class CtcPolytopeHull : public Ctc {
public:

	/**
	 * \brief Contraction mode.
	 *
	 * States if all the variables or just the one corresponding
	 * to the objective should be contracted.
	 */
	typedef enum  {  ONLY_Y, ALL_BOX } ctc_mode;

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

	CtcPolytopeHull(LinearRelax& lr, ctc_mode cmode=ALL_BOX, int max_iter=LinearSolver::default_max_iter,
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
	CtcPolytopeHull(const Matrix& A, const Vector& b, int max_iter=LinearSolver::default_max_iter,
			int time_out=LinearSolver::default_max_time_out, double eps=LinearSolver::default_eps,
			Interval limit_diam=LinearSolver::default_limit_diam_box);

	virtual void contract(IntervalVector& box);

	virtual ~CtcPolytopeHull();

protected:

#ifndef _IBEX_WITH_NOLP_
	/**
	 * Achterberg heuristic for choosing the next variable  and which bound to optimize
	 */
	bool choose_next_variable(IntervalVector &box,  int & nexti, int & infnexti, int* inf_bound, int* sup_bound);

	/**
	 * TODO: add comment
	 */
	void optimizer(IntervalVector &box);


#endif /// end _IBEX_WITH_NOLP_

	/**
	 * \brief The linearization technique
	 */
	LinearRelax& lr;

	/**
	 * \brief Index of the variable corresponding to the objective function
	 *
	 * -1 if none (unconstrained problem). */
	const int goal_var;

	/**
	 * \brief Indicates if in optimization only the objective is contracted
	 * (cmode=ONLY_Y) or all the box (ALL_BOX)
	 */
	const ctc_mode cmode;

	/**
	 * TODO: add comment
	 */
	const Interval limit_diam_box;


	/**
	 * \brief  The linear solver that will be use
	 */
	LinearSolver *mylinearsolver;

private:
	bool own_lr;

};

} // end namespace ibex
#endif // __IBEX_CTC_POLYTOPE_HULL_H__
