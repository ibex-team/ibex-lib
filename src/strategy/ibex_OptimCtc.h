//============================================================================
//                                  I B E X
// File        : ibex_OptimCtc.h
// Author      : Jordan Ninin, Gilles Chabert
// License     : See the LICENSE file
// Created     : May 14, 2014
// Last Update : May 14, 2014
//============================================================================

#ifndef __IBEX_OPTIMCTC_H__
#define __IBEX_OPTIMCTC_H__

#include "ibex_Bsc.h"
#include "ibex_Ctc.h"
#include "ibex_HC4Revise.h"
#include "ibex_Backtrackable.h"
#include "ibex_CellHeapOptim.h"
#include "ibex_OptimCell.h"
#include "ibex_Function.h"
#include "ibex_UnconstrainedLocalSearch.h"
#include <string>
#include "ibex_BitSet.h"
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
class OptimCtc {
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
	 * \brief Run the optimization.

	 * \param init_box       -  the initial box
	 * \param obj_init_bound - (optional) can be set when an initial upper
	 *                         bound of the objective minimum is known a priori.
	 *                         (this bound can be obtained, e.g., by a local solver).
	 */
	void optimize(const IntervalVector& init_box, double obj_init_bound =POS_INFINITY);

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
	 *     <li> total running time
	 * </ul>
	 */
	void report_perf();

	/** Number of variables. */
	const int n;

	 /**   the contractor which remove the domain that does not contain any feasible solution */
	Ctc& _ctc_out;

	 /**   the contractor which remove the domain that are entirely feasible */
	Ctc& _ctc_in;

	 /**  the objective function  */
	Function& _f_cost;

	/** a local minimizer to perform optimization on the feasible domain. */
	UnconstrainedLocalSearch _localopti;

	/** Bisector. */
	Bsc& bsc;

	CellHeapOptim buffer;

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

	/** Trace activation flag.
	 * The value can be fixed by the user. By default: 0  nothing is printed
	 1 for printing each better found feasible point
	 2 for printing each handled node */
	int trace;

	/** Probability to choose the second criterion in node selection in percentage
	 * integer in [0,100] default value 50
	 * the value 0 corresponds to use a single criterion for node selection (the classical one : minimizing the lower bound of the estimate of the objective)
	 * the value 100 corresponds to use a single criterion for node selection (the second one used in buffer2) */
	 int critpr;

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

	/** Number of cells put into the heap (which passed through the contractors)  */
	int nb_cells;

	/**
	 * \brief The "loup" (lowest upper bound of the criterion)
	 */
	double loup;


	/** The "uplo" (uppermost lower bound of the criterion) */
	double uplo;

	/** The point satisfying the constraints corresponding to the loup */
	Vector loup_point;


protected:
		/**
		 * \brief Return an upper bound of f(x).
		 *
		 * Return +oo if x is outside the definition domain of f.
		 */
		inline double goal(const Vector& x) const {
			Interval fx=_f_cost.eval(x);
			if (fx.is_empty()) // means: outside of the definition domain of the function
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
		void handle_cell(OptimCell& c, const IntervalVector& init_box);

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
		void contract_and_bound(OptimCell& c, const IntervalVector& init_box);

		/**
		 * \brief Contraction procedure for processing a box.
		 *
		 * <ul>
		 * <li> contract with the contractor ctc,
		 * </ul>
		 *
		 */
		 void contract(IntervalVector& box);

		/**
		 * \brief Update the uplo of non bisectable boxes
		 */
		void update_uplo_of_epsboxes(double ymin);

		/**
		 * \brief Update the uplo
		 */
		void update_uplo();


		/**
		 * \brief Procedure for updating the loup with a local optimizer on each box of \a list_box.
		 */
		bool localsearch(const IntervalVector* list_box, int nb) ;


		/**
		 * \brief Procedure for trying to update the loup with a given point.
		 */
		bool direct_try(const Vector point) ;

		bool loup_changed;

		/**
		 * \brief Computes and returns  the value ymax (the loup decreased with the precision)
		 * the heap and the current box are actually contracted with y <= ymax
		 *
		 */
		double compute_ymax ();
	

		void compute_pf(OptimCell& c);

		/**
		 * \brief add the entire list of intervalVector in the buffer
		 */
		void add_buffer_pf(IntervalVector* list, int size);

	
private:


	/** Lower bound of the small boxes taken by the precision */
	double uplo_of_epsboxes;

	void draw_vibes( const IntervalVector& X0, const IntervalVector& X,const std::string color);

};


} // end namespace ibex
#endif // __IBEX_OPTIMCTC_H__
