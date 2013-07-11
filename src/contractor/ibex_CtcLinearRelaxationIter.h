//============================================================================
//                                  I B E X
//
// File        : ibex_CtcLinearRelaxation.h
// Author      : Ignacio Araya Bertrand Neveu, Gilles Trombettoni, Jordan Ninin
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jul 20, 2012
// Last Update : May 15, 2013
//============================================================================

#ifndef __IBEX_CTC_LINEAR_RELAXATION_ITER_H__
#define __IBEX_CTC_LINEAR_RELAXATION_ITER_H__

#include "ibex_Ctc.h"
#include "ibex_System.h"
#include "ibex_NumConstraint.h"
#include "ibex_LinearSolver.h"

namespace ibex {

/**
 * \ingroup ctcgroup
 *
 * \brief Linear relaxation contractor
 *
 * This in an abstract class for linear relaxation-based contractors (AF-based, X-Newton)
 *
 */

class CtcLinearRelaxationIter : public Ctc {

public:
	typedef enum  {  ONLY_Y, ALL_BOX } ctc_mode;

	typedef enum   { ART, AFFINE2, XNEWTON, TAYLOR, HANSEN, COMPO } linear_mode;

	/**
	 * \brief Creates the iteration
	 *
	 * \param sys      - The system
	 * \param cmode      ALL_BOX (contracts all variables in the box) | ONLY_Y (in optimization :only improves
	 *                   the left bound of the objective)
	 * \param max_iter - The maximum number of iterations of the linear solver (default value 100)
	 * \param max_diam - The maximum diameter of the box for for the linear solver (default value 1.e6)
	 */
	CtcLinearRelaxationIter(const System& sys, ctc_mode cmode=ALL_BOX,
			int max_iter=LinearSolver::default_max_iter, int time_out=LinearSolver::default_max_time_out,
			double eps=LinearSolver::default_eps, Interval limit_diam=LinearSolver::default_limit_diam_box,
			bool init_lp=true);

	/**
	 * \brief Delete *this
	 */
	~CtcLinearRelaxationIter ();

	/**
	 * \brief Basic iteration of the LR-based contractor.
	 *
	 * Linearize the system and performs calls to Simplex.
	 * Apply contraction. It must be implemented in the subclasses
	 */
	void contract( IntervalVector& box);

	/**
	 * \brief The linearization technique.
	 *
	 * It must be implemented in the subclasses.
	 */
	virtual int linearization(IntervalVector& box, LinearSolver *mysolver) =0;

protected:

	/**
	 * \brief The linearization technique.
	 *
	 * It must be implemented in the subclasses
	 */
	int linearization(IntervalVector& box);

	/**
	 * \brief The system
	 */
	const System& sys;

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

	const Interval limit_diam_box;

	/**
	 * \brief  The linear solver that will be use
	 */
	LinearSolver *mylinearsolver;

	/**
	 * Neumaier Shcherbina postprocessing in case of optimal solution found : the result obj is made reliable
	 */
	void NeumaierShcherbina_postprocessing (int nr, int var, Interval & obj, IntervalVector& box, Matrix & As, IntervalVector& B,  Vector &dual_solution, bool minimization);

	/**
	 *  Neumaier Shcherbina postprocessing in case of infeasibilty found by LP  returns true if the infeasibility is proved
	 */
	bool  NeumaierShcherbina_infeasibilitytest ( int nr, IntervalVector& box, Matrix & As, IntervalVector& B, Vector & infeasible_dir);

	/**
	 * Achterberg heuristic for choosing the next variable  and which bound to optimize
	 */
	bool choose_next_variable ( IntervalVector &box,  int & nexti, int & infnexti, int* inf_bound, int* sup_bound);

	/**
	 * Call to linear solver
	 */
	LinearSolver::Status_Sol run_simplex(IntervalVector &box, LinearSolver::Sense sense, int var, Interval & obj, double bound);

	void optimizer(IntervalVector &box);

	/**
	 * Check if the constraint is satisfied in the box : in this case, no linear relaxation is made.
	 */
	bool isInner(IntervalVector & box, const System& sys, int j); /* redoundant method? */

};

} // end namespace ibex

#endif /* __IBEX_CTC_LINEAR_RELAXATION_ITER_H__ */
