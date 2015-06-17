//============================================================================
//                                  I B E X
// File        : ibex_OptimCtc.h
// Author      : Jordan Ninin, Gilles Chabert
// License     : See the LICENSE file
// Created     : May 14, 2014
// Last Update : May 14, 2015
//============================================================================

#ifndef __IBEX_OPTIMCTC_H__
#define __IBEX_OPTIMCTC_H__

#include "ibex_MainOpti.h"
#include "ibex_Bsc.h"
#include "ibex_Ctc.h"
#include "ibex_CellDoubleHeap.h"
#include "ibex_Function.h"
#include "ibex_UnconstrainedLocalSearch.h"
#include "ibex_Optimizer.h"
namespace ibex {

/**
 * \ingroup strategy
 *
 * \brief Global OptimCtc.
 *
 * This class is an implementation of the global optimization algorithm
 *
 * \remark In all the comments of this class, "loup" means "lowest upper bound" of the criterion f
 * and "uplo" means "uppermost lower bound" of the criterion.
 */
class OptimCtc : MainOpti {
public:
	/**
	 *  \brief Create an OptimCtc.
	 *
	 *   \param ctc_out   - the contractor which remove the domain that does not contain any feasible solution
	 *   \param ctc_in    - the contractor which remove the domain that are entirely feasible
	 *   \param f_cost    - the objective function
	 *   \param bsc       - bisector for extended boxes
	 *
	 * And optionally:
	 *   \param prec          - absolute precision for the boxes (bisection control)
	 *   \param goal_rel_prec - relative precision of the objective (the OptimCtc stops once reached).
	 *   \pram  goal_abs_prec - absolute precision of the objective (the OptimCtc stops once reached).
	 *   \param critpr        - probability to choose the second criterion in node selection; integer in [0,100]. By default 50
	 *   \param crit          - second criterion in node selection (the first criterion is the minimum of the objective estimate). default value CellHeapOPtim::UB
	 *
	 */

	OptimCtc(Ctc& ctc_out, Ctc& ctc_in, Function& f_cost, Bsc& bsc,
			double prec = default_prec, double goal_rel_prec = default_goal_rel_prec,
			double goal_abs_prec =	default_goal_abs_prec);
	/**
	 * \brief Delete *this.
	 */
	virtual ~OptimCtc();

	/**
	 * \brief Return status of the optimizer
	 */
	//typedef enum {SUCCESS, INFEASIBLE, NO_FEASIBLE_FOUND, UNBOUNDED_OBJ, TIME_OUT} Status;

	/**
	 * \brief Run the optimization.
	 *
	 * \param init_box             The initial box
	 * \param obj_init_bound       (optional) can be set when an initial upper bound of the objective minimum is known a priori.
	 *                             This bound can be obtained, e.g., by a local solver. This is equivalent to (but more practical
	 *                             than) adding a constraint f(x)<=obj_init_bound.
	 *
	 * \return SUCCESS             If the global minimum (with respect to the precision required) has been found.
	 *                             In particular, at least one feasible point has been found, less than obj_init_bound, and in the time limit.
	 *
	 *         INFEASIBLE          if no feasible point exist less than obj_init_bound. In particular, the function returns INFEASIBLE
	 *                             if the initial bound "obj_init_bound" is LESS than the true minimum (this case is only possible if
	 *                             goal_abs_prec and goal_rel_prec are 0). In the latter case, there may exist feasible points.
	 *
	 *         NO_FEASIBLE_FOUND   if no feasible point could be found less than obj_init_bound. Contrary to INFEASIBLE,
	 *                             infeasibility is not proven here. Warning: this return value is sensitive to the goal_abs_prec and
	 *                             goal_rel_prec parameters. The upperbounding makes the optimizer only looking for points less than
	 *                             min { (1-goal_rel_prec)*obj_init_bound, obj_init_bound - goal_abs_prec }.
	 *
	 *         UNBOUNDED_OBJ       the objective function seems unbounded (tends to -oo).
	 *
	 *         TIMEOUT             time is out.
	 */
	Status_Opti optimize(const IntervalVector& init_box, double obj_init_bound=POS_INFINITY);


	/**   the contractor which remove the domain that does not contain any feasible solution */
	Ctc& _ctc_out;

	/**   the contractor which remove the domain that are entirely feasible */
	Ctc& _ctc_in;


protected:

	/**
	 * \brief Main procedure for processing a box.
	 *
	 * <ul>
	 * <li> contract and bound  the cell box (see contract_and_bound)
	 * <li> push the cell onto the heap or delete the cell in case of empty box detected
	 * </ul>
	 *
	 */
	void handle_cell(Cell& c);

	/**
	 * \brief Contract and bound procedure for processing a box.
	 *
	 * <ul>
	 * <li> contract the cell's box w.r.t the "loup",
	 * <li> contract with the contractor ctc,
	 * <li> search for a new loup,
	 * <li> call the first order contractor
	 * </ul>
	 *
	 */
	void contract_and_bound(Cell& c);

	/**
	 * \brief Monotonicity analysis.
	 *
	 * When f is increasing (resp. decreasing) w.r.t. variable x_i, the interval [x_i]
	 * is replaced by the lower bound (resp. upper bound) of [x_i].
	 */
	void monotonicity_analysis(IntervalVector& box, bool inner_found);


	/**
	 * \brief Quick check that the box is inside the feasible domain.
	 *
	 * The box must be a sub-box of the current cells's box (because constraints marked as
	 * entailed as skipped from the check)
	 */
	bool is_inner(const IntervalVector& box);


private:

	/**
	 * \brief add the entire list of intervalVector in the buffer
	 */
	void add_buffer(IntervalVector* list, int size);


};


} // end namespace ibex
#endif // __IBEX_OPTIMCTC_H__
