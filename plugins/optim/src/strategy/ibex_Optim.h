//============================================================================
//                                  I B E X
// File        : ibex_Optimizer.h
// Author      : Gilles Chabert, Bertrand Neveu
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : May 14, 2012
// Last Update : May 14, 2012
//============================================================================

#ifndef __IBEX_OPTIM__
#define __IBEX_OPTIM__

#include "ibex_Interval.h"
#include "ibex_CellDoubleHeap.h"

namespace ibex {

/**
 * \ingroup strategy
 *
 * \brief  Optimizer.
 *
 */
class Optim {
public:

	Optim( int n, CellDoubleHeap* buffer, double prec=default_prec,
			double goal_rel_prec=default_goal_rel_prec, double goal_abs_prec=default_goal_abs_prec,
			  int sample_size=default_sample_size);

	/**
	 * \brief Delete *this.
	 */
	virtual ~Optim();

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
	virtual Status optimize(const IntervalVector& init_box, double obj_init_bound=POS_INFINITY)=0;

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


	/** Number of variables. */
	const int n;

	/** Cell buffers.
	Two buffers are used for node selection. the first one corresponds to minimize  the minimum of the objective estimate,
	the second one to minimize another criterion (by default the maximum of the objective estimate).
	The second one is chosen at each node with a probability critpr/100 (default value critpr=50)
	 */
	CellDoubleHeap *buffer;

	/**
	 * \brief Index of the goal variable y in the extended box.
	 *
	 */

	/** Precision (bisection control) */
	const double prec;

	/** Relative precision on the objective */
	const double goal_rel_prec;

	/** Absolute precision on the objective */
	const double goal_abs_prec;

	/** Number of samples used to update the loup */
	const int sample_size;

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

	/** Default sample size */
	static const int default_sample_size;

	/** Default epsilon applied to equations */
	static const double default_equ_eps;

	/** Default tolerance increase ratio for the pseudo-loup. */
	static const double default_loup_tolerance;

	/** Default Timeout. */
	static const double default_timeout;
	/**
	 * \brief The "loup" (lowest upper bound of the criterion)
	 *
	 * In rigor mode, represents the real-loup (not the pseudo-loup).
	 */
	double loup;

	/** The "uplo" (uppermost lower bound of the criterion) */
	double uplo;

	/** The point satisfying the constraints corresponding to the loup */
	Vector loup_point;

	/** Number of cells put into the heap (which passed through the contractors)  */
	int nb_cells;

protected:

	/**
	 * \brief Update the uplo of non bisectable boxes
	 */
	void update_uplo_of_epsboxes(double ymin);

	/**
	 * \brief Update the uplo
	 */
	void update_uplo();

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


	/** Lower bound of the small boxes taken by the precision */
	double uplo_of_epsboxes;


};


} // end namespace ibex
#endif // __IBEX_OPTIM__
