//============================================================================
//                                  I B E X
// File        : ibex_MainOpti.h
// Author      : Gilles Chabert, Bertrand Neveu
// Author      : Gilles Chabert, Bertrand Neveu, Jordan Ninin
// Copyright   : Ecole des Mines de Nantes (France)
// Copyright   : ENSTA-Bretagne (France)
// License     : See the LICENSE file
// Created     : May 14, 2012
// Last Update : June 15, 2015
//============================================================================

#ifndef __IBEX_MAINOPTI_H__
#define __IBEX_MAINOPTI_H__

#include "ibex_Bsc.h"
#include "ibex_Ctc.h"
#include "ibex_CellDoubleHeap.h"
#include "ibex_Function.h"
#include "ibex_UnconstrainedLocalSearch.h"

namespace ibex {

/**
 * \brief Return status of the MainOpti
 */
typedef enum {SUCCESS, INFEASIBLE, NO_FEASIBLE_FOUND, UNBOUNDED_OBJ, TIME_OUT} Status_Opti;

/**
 * \ingroup strategy
 *
 * \brief Global MainOpti.
 *
 *
 * \remark In all the comments of this class, "loup" means "lowest upper bound" of the criterion f
 * and "uplo" means "uppermost lower bound" of the criterion.
 */
class MainOpti {
public:
	/**
	 *  \brief Create an MainOpti.
	 *
	 *	 \param f_cost - the objective function
	 *   \param bsc    - bisector for extended boxes
	 *   \param buffer - THe dynamic list to store elements
	 *
	 * And optionally:
	 *   \param prec          - absolute precision for the boxes (bisection control)
	 *   \param goal_rel_prec - relative precision of the objective (the MainOpti stops once reached).
	 *   \pram  goal_abs_prec - absolute precision of the objective (the MainOpti stops once reached).
	 *
	 *
	 * \warning The MainOpti relies on the contractor \a ctc to contract the domain of the goal variable and increase the uplo.
	 * If this contractor never contracts this goal variable, the MainOpti will only rely on the evaluation of f  and will be very slow.
	 *
	 */
	MainOpti(Function& f_cost, Bsc& bsc, CellDoubleHeap* buffer, double prec=default_prec, double goal_rel_prec=default_goal_rel_prec, double goal_abs_prec=default_goal_abs_prec);

	/**
	 * \brief Delete *this.
	 */
	virtual ~MainOpti();


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
	 *                             goal_rel_prec parameters. The upperbounding makes the MainOpti only looking for points less than
	 *                             min { (1-goal_rel_prec)*obj_init_bound, obj_init_bound - goal_abs_prec }.
	 *
	 *         UNBOUNDED_OBJ       the objective function seems unbounded (tends to -oo).
	 *
	 *         TIMEOUT             time is out.
	 */
	virtual Status_Opti optimize(const IntervalVector& init_box, double obj_init_bound=POS_INFINITY)=0;

	/**
	 * \brief Displays on standard output a report of the last call to #optimize(const IntervalVector&).
	 *
	 * Information provided:
	 * <ul><li> interval of the cost  [uplo,loup]
	 *     <li> the best feasible point found
	 *     <li> total running time
	 *     <li> total number of cells created during the exploration
	 * </ul>
	 */
	void report() const;

	/**
	 * \brief Displays on standard output a report of the last call to #optimize(const IntervalVector&).
	 *
	 * Information provided:
	 * <ul><li> interval of the cost  [uplo,loup] in case of termination due to timelimit
	 *     <li> total running time
	 *     <li> total number of cells created during the exploration
	 * </ul>
	 */
	void time_cells_report() const;
	
	/**
	 * \brief Displays on standard output a report of the last call to #optimize(const IntervalVector&).
	 *
	 * Information provided:
	 * <ul><li> interval of the cost  [uplo,loup]
	 *     <li>total running time
	 * </ul>
	 */
	void report_perf() const;


	/** Number of variables. */
	const int n;

	/** Bisector. */
	Bsc& bsc;


	/** Cell buffers.
	Two buffers are used for node selection. the first one corresponds to minimize  the minimum of the objective estimate,
	the second one to minimize another criterion (by default the maximum of the objective estimate).
	The second one is chosen at each node with a probability critpr/100 (default value critpr=50)
	 */
	CellDoubleHeap* buffer;


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

	/** Default sample size */
	static const int default_sample_size;

	/** Default epsilon applied to equations */
	static const double default_equ_eps;

	/** Default tolerance increase ratio for the pseudo-loup. */
	static const double default_loup_tolerance;

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
	 * \brief Return an upper bound of f(x).
	 *
	 * Return +oo if x is outside the definition domain of f.
	 */
	inline double goal(const Vector& x) const {
		Interval fx=f_cost.eval(x);
		if (fx.is_empty())  // means: outside of the definition domain of the function
			return POS_INFINITY;
		else
			return fx.ub();
	}

	/**
	 * \brief Main procedure for processing a box.
	 *
	 * <ul>
	 * <li> contract and bound  the cell box (see contract_and_bound)
	 * <li> push the cell onto the heap or delete the cell in case of empty box detected
	 * </ul>
	 *
	 */
	virtual void handle_cell(Cell& c, const IntervalVector& init_box)=0;

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
	virtual void contract_and_bound(Cell& c, const IntervalVector& init_box)=0;


	/**
	 * \brief Procedure for updating the loup with a local optimizer on each box of \a list_box.
	 */
	bool localsearch(const IntervalVector* list_box, int nb);
	bool localsearch(const IntervalVector list_box);

	/**
	 * \brief Update the uplo of non bisectable boxes
	 */
	void update_uplo_of_epsboxes(double ymin);

	/**
	 * \brief Update the uplo
	 */
	void update_uplo();


	/*=======================================================================================================*/
	/*             Functions to update the loup (see ibex_OptimProbing and ibex_OptimSimplex)                */
	/*=======================================================================================================*/

	/**
	 * \brief Monotonicity analysis.
	 *
	 * When f is increasing (resp. decreasing) w.r.t. variable x_i, the interval [x_i]
	 * is replaced by the lower bound (resp. upper bound) of [x_i].
	 */
	virtual void monotonicity_analysis(IntervalVector& box, bool inner_found)=0;


	/**
	 * \brief Quick check that the box is inside the feasible domain.
	 *
	 * The box must be a sub-box of the current cells's box (because constraints marked as
	 * entailed as skipped from the check)
	 */
	virtual bool is_inner(const IntervalVector& box)=0;

	/**
	 * \brief Try to reduce the "loup" with a candidate point.
	 *
	 * \param pt       - The candidate point.
	 * \param is_inner - If true, the point is already known to be inner so there
	 *                   is no need to check constraint satisfaction again. False
	 *                   means "unknown" and a quick check (see
	 *                   #is_inner(const IntervalVector&)) is performed.
	 *
	 * \note In rigorous mode, the equalities have to be checked anyway (even if
	 *       is_inner==true) because the innership is only wrt the relaxed system.
	 *       In this case, the resulting loup_point may be different than \a pt (the
	 *		 procedure used to check satisfiability
	 * \return true in case of success, i.e., if the loup has been decreased.
	 */
	bool check_candidate(const Vector& pt, bool is_inner);


	/**
	 * \brief Computes and returns  the value ymax (the loup decreased with the precision)
	 * the heap and the current box are actually contracted with y <= ymax
	 *
	 */
	double compute_ymax () const;

	bool loup_changed;

	/**
	 * \brief The bound on the objective given by the user, +oo otherwise.
	 *
	 * Used to see if at least a loup-point has been found.
	 *
	 */
	double initial_loup;


	/** The goal (if any, otherwise NULL) */
	Function& f_cost;


	/** Lower bound of the small boxes taken by the precision */
	double uplo_of_epsboxes;


	/** a local minimizer to perform optimization on the feasible domain. */
	UnconstrainedLocalSearch localopti;


};


} // end namespace ibex
#endif // __IBEX_MAINOPTI_H__

