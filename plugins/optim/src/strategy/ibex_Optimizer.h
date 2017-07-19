//============================================================================
//                                  I B E X
// File        : ibex_Optimizer.h
// Author      : Gilles Chabert, Bertrand Neveu
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : May 14, 2012
// Last Update : May 14, 2012
//============================================================================

#ifndef __IBEX_OPTIMIZER_H__
#define __IBEX_OPTIMIZER_H__

#include "ibex_LinearizerCombo.h"
#include "ibex_LoupFinder.h"
#include "ibex_Bsc.h"
#include "ibex_CtcHC4.h"
#include "ibex_Ctc3BCid.h"
#include "ibex_CtcUnion.h"
#include "ibex_Backtrackable.h"
#include "ibex_CellCostFunc.h"
#include "ibex_CellDoubleHeap.h"
#include "ibex_NormalizedSystem.h"
#include "ibex_ExtendedSystem.h"
#include "ibex_EntailedCtr.h"
#include "ibex_LinearSolver.h"
#include "ibex_CtcKhunTucker.h"
#include "ibex_Random.h"
#include "ibex_FritzJohnCond.h"

namespace ibex {

/**
 * \defgroup optim IbexOpt
 */

/**
 * \ingroup optim
 *
 * \brief Global Optimizer.
 *
 * This class is an implementation of the global optimization algorithm described
 * in the AAAI'11 paper <i>Inner Regions and Interval Linearizations for Global Optimization</i>
 * by Trombettoni et al.
 *
 * \remark In all the comments of this class, "loup" means "lowest upper bound" of the criterion f
 * and "uplo" means "uppermost lower bound" of the criterion.
 */
class Optimizer {
public:
	/**
	 *  \brief Create an optimizer.
	 *
	 *   \param sys   - the system to optimize
	 *   \param bsc   - bisector for extended boxes
	 *   \param ctc   - contractor for the <b>extended system</b> (see below)
	 *
	 * And optionally:
	 *   \param prec          - absolute precision for the boxes (bisection control)
	 *   \param goal_rel_prec - relative precision of the objective (the optimizer stops once reached).
	 *   \pram  goal_abs_prec - absolute precision of the objective (the optimizer stops once reached).
	 *   \param sample_size   - number of samples taken when looking for a "loup"
	 *   \param equ_eps       - thickness of equations when relaxed to inequalities
	 *   \param rigor         - look for points that strictly satisfy equalities. By default: false
	 *   \param critpr        - probability to choose the second criterion in node selection; integer in [0,100]. By default 50
	 *   \param crit          - second criterion in node selection (the first criterion is the minimum of the objective estimate). default value CellHeapOPtim::UB
	 *
	 * <ul> The extended system (see ExtendedSystem constructor) contains:
	 * <li> (n+1) variables, x_1,...x_n,y. The index of y is #goal_var (==n).
	 * <li> A (m+1)-valued function f:(x,y)->(y-f(x),g_0(x),...,g_m(x))
	 * <li> (m+1) constraints: y-f(x)=0, g_1(x)<=0, ..., g_m(x)<=0.
	 * </ul>
	 *
	 * \warning The optimizer relies on the contractor \a ctc to contract the domain of the goal variable and increase the uplo.
	 * If this contractor never contracts this goal variable, the optimizer will only rely on the evaluation of f  and will be very slow.
	 *
	 */
	Optimizer(System& sys, Ctc& ctc, Bsc& bsc, /*LoupFinder& finder, */double prec=default_prec,
			double goal_rel_prec=default_goal_rel_prec, double goal_abs_prec=default_goal_abs_prec,
			double equ_eps=default_equ_eps, bool rigor=false, int critpr=50,CellCostFunc::criterion crit= CellCostFunc::UB);

	/**
	 * \brief Delete *this.
	 */
	virtual ~Optimizer();

	/**
	 * \brief Return status of the optimizer
	 */
	typedef enum {SUCCESS, INFEASIBLE, NO_FEASIBLE_FOUND, UNBOUNDED_OBJ, TIME_OUT} Status;

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
	Status optimize(const IntervalVector& init_box, double obj_init_bound=POS_INFINITY);

	/**
	 * \brief Displays on standard output a report of the last call to #optimize(const IntervalVector&).
	 *
	 * Information provided:
	 * <ul><li> interval of the cost  [uplo,loup]
	 *     <li> the best feasible point found
	 *     <li>total running time
	 *     <li>total number of cells created during the exploration
	 * </ul>
	 */
	void report();

	/**
	 * \brief Displays on standard output a report of the last call to #optimize(const IntervalVector&).
	 *
	 * Information provided:
	 * <ul><li> interval of the cost  [uplo,loup] in case of termination due to timelimit
	 *     <li>total running time
	 *     <li>total number of cells created during the exploration
	 * </ul>
	 */
	
	void time_cells_report();
	
	/**
	 * \brief Displays on standard output a report of the last call to #optimize(const IntervalVector&).
	 *
	 * Information provided:
	 * <ul><li> interval of the cost  [uplo,loup]
	 *     <li>total running time
	 * </ul>
	 */
	void report_perf();

	/**
	 * \brief The original system
	 *
	 * \warning kept by reference.
	 */
	System& user_sys;

	NormalizedSystem normalized_user_sys;

	/** Number of variables. */
	const int n;

	/**
	 * \brief The extended system
	 *
	 * Corresponds to the normalized system with the goal f(x)
	 * represented as a variable "y" and with the additional constraint
	 * y=f(x). The domain of y in an extended box stores the interval [ylb,yub] where
	 * "ylb" is the lower bound of f(x) in the box and "yub" its
	 * upper bound.  (y is contracted with y <= ymax : see compute_ymax)
	 *
	 * The index of y is ext_sys.nb_var.
	 * See #ibex::ExtendedSystem::goal_var.
	 */
	ExtendedSystem ext_sys;

	/**
	 * \brief True iff there is an equality.
	 */
	bool has_equality;

	/** Contractor for the extended system
	 * (y=f(x), g_1(x)<=0,...,g_m(x)<=0). */
	Ctc& ctc;

	/** Bisector. */
	Bsc& bsc;

	/** Loup finder algorithm. */
	LoupFinder& loup_finder;

	/** Cell buffers.
	Two buffers are used for node selection. the first one corresponds to minimize  the minimum of the objective estimate,
	the second one to minimize another criterion (by default the maximum of the objective estimate).
	The second one is chosen at each node with a probability critpr/100 (default value critpr=50)
	 */
	CellDoubleHeap buffer;

	/** Precision (bisection control) */
	const double prec;

	/** Relative precision on the objective */
	const double goal_rel_prec;

	/** Absolute precision on the objective */
	const double goal_abs_prec;

	/** Trace activation flag.
	 * The value can be fixed by the user. By default: 0  nothing is printed
	 1 for printing each better found feasible point
	  2 for printing each handled node */
	int trace;

	/**
	 * \brief Time limit.
	 *
	 * Maximum CPU time used by the strategy.
	 * This parameter allows to bound time consumption.
	 * The value can be fixed by the user.
	 */
	double timeout;

	/* Remember running time of the last exploration */
	double time;

	void time_limit_check();

	/** Default bisection precision: 1e-07 */
	static const double default_prec;

	/** Default goal relative precision */
	static const double default_goal_rel_prec;

	/** Default goal absolute precision */
	static const double default_goal_abs_prec;

	/** Default epsilon applied to equations */
	static const double default_equ_eps;

	/**
	 * \brief The "loup" (lowest upper bound of the criterion)
	 *
	 * In rigor mode, represents the real-loup (not the pseudo-loup).
	 */
	double loup;

	/**
	 * \brief The pseudo-loup.
	 *
	 * Represents, in rigor mode only, the loup for the relaxed problem.
	 */
	double pseudo_loup;

	/** The "uplo" (uppermost lower bound of the criterion) */
	double uplo;

	/** The point satisfying the constraints corresponding to the loup */
	Vector loup_point;

	/** Rigor mode: the box satisfying the constraints corresponding to the loup */
	IntervalVector loup_box;
	
	/** Number of cells put into the heap (which passed through the contractors)  */
	int nb_cells;

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
	void handle_cell(Cell& c, const IntervalVector& init_box);

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
	void contract_and_bound(Cell& c, const IntervalVector& init_box);

	/**
	 * \brief First order contraction procedure for processing a box.
	 *
	 * <ul>
	 * <li>  gradient=0 contraction for unconstrained optimization ;
	 * <li>  first order test for constrained optimization (useful only when there are no equations replaced by inequalities)
	 * </ul>
	 *
	 */

	virtual void firstorder_contract ( IntervalVector& box, const IntervalVector& init_box);

	/**
	 * \brief Update the entailed constraint for the current box
	 *
	 * \return false if unsatisfiability is detected, true otherwise.
	 */
	//bool update_entailed_ctr(const IntervalVector& box);

	/**
	 * \brief Update the uplo of non bisectable boxes
	 */
	void update_uplo_of_epsboxes(double ymin);

	/**
	 * \brief Update the uplo
	 */
	void update_uplo();


	/**
	 * \brief Main procedure for updating the loup.
	 */
	bool update_loup(const IntervalVector& box);


	/*=======================================================================================================*/
	/*             Functions to update the loup (see ibex_OptimProbing and ibex_OptimSimplex)                */
	/*=======================================================================================================*/

	/**
	 * Look for a loup box (in rigor mode) starting from a pseudo-loup.
	 *
	 * Start from the last loup point found in relaxed mode.
	 */
	bool update_real_loup();

	/**
	 * \brief Display the loup (for debug)
	 */
	void trace_loup(bool inner_found);
	/*=======================================================================================================*/
	/*                                Functions to manage the extended CSP                                   */
	/*=======================================================================================================*/

	/**
	 * \brief Load a (n-dimensional) box into an (n+1-dimensional) extended box
	 *
	 *  The goal variable is skipped.
	 */
	void write_ext_box(const IntervalVector& box, IntervalVector& ext_box);

	/**
	 * \brief Load an extended (n+1-dimensional) box into a (n-dimensional) box
	 *
	 *  The goal variable is skipped.
	 */
	void read_ext_box(const IntervalVector& ext_box, IntervalVector& box);

	/**
	 * \brief Symbolic gradient of the objective
	 */
	Function* df;

	/**
	 * \brief Computes and returns  the value ymax (the loup decreased with the precision)
	 * the heap and the current box are actually contracted with y <= ymax
	 *
	 */
	double compute_ymax ();

	bool loup_changed;

	/**
	 * \brief The bound on the objective given by the user, +oo otherwise.
	 *
	 * Used to see if at least a loup-point has been found.
	 *
	 */
	double initial_loup;
	
private:

	/** Rigor mode (eps_equ==0) */
	const bool rigor;

	/** Lower bound of the small boxes taken by the precision */
	double uplo_of_epsboxes;

	/** Currently entailed constraints */
	//EntailedCtr* entailed;

	//void fritz_john_contract(IntervalVector& box);

	//!! warning: sys.box should be properly set before call to constructor !!
	CtcKhunTucker kkt;
};

} // end namespace ibex

#endif // __IBEX_OPTIMIZER_H__
