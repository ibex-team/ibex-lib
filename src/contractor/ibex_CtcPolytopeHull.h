//============================================================================
//                                  I B E X                                   
// File        : ibex_CtcPolytopeHull.h
// Authors     : Jordan Ninin, Bertrand Neveu, Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Oct 31, 2013
// Last Update : Oct 31, 2013
//============================================================================

#ifndef __IBEX_CTC_POLYTOPE_HULL_H__
#define __IBEX_CTC_POLYTOPE_HULL_H__

#include "ibex_Linearizer.h"
#include "ibex_Ctc.h"
#include "ibex_LPSolver.h"
#include "ibex_BitSet.h"

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
	 * \brief Creates the iteration
	 *
	 * \param lr         - The linear relaxation
	 * \param max_iter   - The maximum number of iterations of the linear solver (default value 100)
	 * \param timeout    - The timeout of the linear solver at each iteration    (default value 100 second)
	 * \param eps        - The accuracy required on the resolution of the linear program (default value 1e-10)
	 * \param limit_diam - The contractor does nothing if the diameter does not respect these bounds
	 */

	CtcPolytopeHull(Linearizer& lr, int max_iter=LPSolver::default_max_iter,
			int time_out=LPSolver::default_max_time_out, double eps=LPSolver::default_eps,
			Interval limit_diam=LPSolver::default_limit_diam_box);

	/**
	 * \brief Creates the contractor w.r.t. Ax<=b
	 *
	 * \param A        - Matrix in Ax<=b
	 * \param b        - Vector in Ax<=b
	 * \param max_iter - The maximum number of iterations of the linear solver (default value 100)
	 * \param timeout  - The timeout of the linear solver at each iteration    (default value 100 second)
	 * \param eps      - The accuracy required on the resolution of the linear program (default value 1e-10)
	 * \param limit_diam - The contractor does nothing if the diameter does not respect these bounds
	 */
	CtcPolytopeHull(const Matrix& A, const Vector& b, int max_iter=LPSolver::default_max_iter,
			int time_out=LPSolver::default_max_time_out, double eps=LPSolver::default_eps,
			Interval limit_diam=LPSolver::default_limit_diam_box);

	/**
	 * \brief Delete this.
	 */
	virtual ~CtcPolytopeHull();

	/**
	 * \brief Contract the box.
	 *
	 * Linearize the system and performs 2n calls to Simplex in order to reduce
	 * the 2 bounds of each variable
	 */
	virtual void contract(IntervalVector& box);

	/**
	 * \brief Set the variable to be contracted.
	 *
	 * This allows, for example, to only contract the variable corresponding
	 * to the objective in an extended system.
	 *
	 * TODO: allow to specify (left/right) bounds and update the default
	 * optimizer contractor strategy so that only the lower bound of the
	 * goal variable is contracted.
	 */
	void set_contracted_vars(const BitSet& vars);

#ifndef _IBEX_WITH_NOLP_

protected:

	/**
	 * Achterberg heuristic for choosing the next variable  and which bound to optimize
	 */
	bool choose_next_variable(IntervalVector &box,  int & nexti, int & infnexti, int* inf_bound, int* sup_bound);

	/**
	 * TODO: add comment.
	 */
	void optimizer(IntervalVector &box);

	/**
	 * \brief The linearization technique
	 */
	Linearizer& lr;

	/**
	 * TODO: add comment
	 */
	const Interval limit_diam_box;

	/**
	 * \brief  The linear solver that will be used
	 */
	LPSolver mylinearsolver;

	/**
	 * \brief Contracted variables (by default: all)
	 */
	BitSet contracted_vars;

private:
	bool own_lr; // for memory cleanup

#endif /// end _IBEX_WITH_NOLP_
};

} // end namespace ibex

#endif // __IBEX_CTC_POLYTOPE_HULL_H__
