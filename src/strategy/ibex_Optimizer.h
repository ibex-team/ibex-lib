//============================================================================
//                                  I B E X                                   
// File        : ibex_Optimizer.h
// Author      : Gilles Chabert, Bertrand Neveu
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : May 14, 2012
// Last Update : May 14, 2012
//============================================================================

#ifndef __IBEX_OPTIMIZER_H__
#define __IBEX_OPTIMIZER_H__

#include "ibex_Bsc.h"
#include "ibex_CtcHC4.h"
#include "ibex_Ctc3BCid.h"
#include "ibex_CtcUnion.h"
#include "ibex_Backtrackable.h"
#include "ibex_CellHeapOptim.h"
#include "ibex_System.h"
#include "ibex_ExtendedSystem.h"
#include "ibex_EntailedCtr.h"
#include "ibex_LinearSolver.h"

namespace ibex {

/**
 * \ingroup strategy
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

	Optimizer(System& sys, Bsc& bsc, Ctc& ctc, double prec=default_prec,
			double goal_rel_prec=default_goal_rel_prec, double goal_abs_prec=default_goal_abs_prec,
			int sample_size=default_sample_size);


	/**
	 * \brief Delete *this.
	 */
	~Optimizer();

	/**
	 * \brief Run the optimization.

	 * \param init_box    -  the initial box
	 */
	void optimize(const IntervalVector& init_box);

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
	 * <ul><li> interval of the cost  [uplo,loup]
	 *     <li>total running time
	 * </ul>
	 */
	void report_perf();

	/** Number of variables. */
	const int n;

	/** Number of constraints. */
	const int m;

	/**
	 * \brief The normalized system
	 *
	 * Corresponds to the system (see constructor) with all inequalities
	 * under the form g_i(x)<=0.
	 */
	System sys;

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

	/** Bisector. */
	Bsc& bsc;

	/** Contractor for the extended system
	 * (y=f(x), g_1(x)<=0,...,g_m(x)<=0). */
	Ctc& ctc;




	/** Cell buffer. */
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

	/** Number of samples used to update the loup */
	const int sample_size;

	/** Flag for applying monotonicity analysis.
	 * The value can be fixed by the user. By default: true. */
	bool mono_analysis_flag;

	/** Flag for applying inHC4.
	 * If true, apply inHC4. Otherwise, apply is_inside.
	 * The value can be fixed by the user. By default: true. */
	bool in_HC4_flag;


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


	/** The "loup" (lowest upper bound of the criterion) */
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
		Interval fx=sys.goal->eval(x);
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
	 * \brief Contraction procedure for processing a box.
	 *
	 * <ul>
	 * <li> contract with the contractor ctc,
	 * </ul>
	 *
         */

	 virtual void contract(IntervalVector& box, const IntervalVector& init_box );


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
	 */
	void update_entailed_ctr(const IntervalVector& box);


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
	void update_loup(const IntervalVector& box);


	/*=======================================================================================================*/
	/*             Functions to update the loup (see ibex_OptimProbing and ibex_OptimSimplex)                */
	/*=======================================================================================================*/
	
	/**
	 * \brief Monotonicity analysis.
	 *
	 * When f is increasing (resp. decreasing) w.r.t. variable x_i, the interval [x_i]
	 * is replaced by the lower bound (resp. upper bound) of [x_i].
	 */
	void monotonicity_analysis(IntervalVector& box);

	/**
	 * \brief Quick check that the box is not infeasible.
	 *
	 * The box must be a sub-box of the current cells's box (because constraints marked as
	 * entailed as skipped from the check)
	 */
	bool is_feasible(const IntervalVector& box);

	/**
	 * \brief Quick check that the box is inside g(x)<=0.
	 *
	 * The box must be a sub-box of the current cells's box (because constraints marked as
	 * entailed as skipped from the check)
	 */
	bool is_inner(const IntervalVector& box);


	/**
	 * \brief Reduce the box to an inner box using inHC4 algorithm.
	 *
	 * \return true if the resulting box is proven to be inner, false otherwise.
	 *
	 * Note: even if the result is false, a new box is constructed.
	 */
	bool in_HC4(IntervalVector& box);

	/**
	 * \brief Try to reduce the "loup" with a candidate point.
	 *
	 * \param pt       - The candidate point.
	 * \param is_inner - If true, the point is already known to be inner so there
	 *                   is no need to check constraint satisfaction again. False
	 *                   means "unknown" and a quick check (see
	 *                   #is_inner(const IntervalVector&)) is performed.
	 *
	 * \return true in case of success, i.e., if the loup has been decreased.
	 */
	bool check_candidate(const Vector& pt, bool is_inner);

	/**
	 * \brief First method for probing
	 *
	 * Take random points in any directions.
	 *
	 * \param box  the box in which a random point is searched for (the found inner box if is_inner i strue)
	 * \param fullbox the box where an intensification is made (in the current version only in case of unconstrained optimization)
	 * \param is_inner - If true, the box is already known to be an inner box so there
	 *                   is no need to check constraints again.
	 *
	 * \return true in case of success, i.e., if the loup has been decreased.
	 */

	bool random_probing (const IntervalVector& box, const IntervalVector& fullbox, bool is_innner);

	/**
	 * \brief Perform a dichotomic search of a minimum in a line (see Hansen's book).
	 *
	 * The search is performed on the segment delimited by the current loup-point and \a end_point.
	 *
	 * If \a exit_if_above_loup is true, the search stops as soon as we fall on
	 * a candidate x with f(x)>loup.
	 */
	bool dichotomic_line_search(const Vector& end_point, bool exit_if_above_loup);

	/**
	 * \brief Second method for probing
	 *
	 * Performs a dichotomic search between the current loup-point and its projection on the
	 * facet of the input box in the opposite direction of its gradient.
	 *
	 * return true if the loup has been modified.
	 */
	bool line_probing(const IntervalVector& box);

	/**
	 * \brief Update loup either using line_probing or random_probing.
	 *
	 * Main function for probing ;
	 * search for an inner box ;
	 * call line_probing or random_probing. (in the current version, random probing is called)
	 * return true if the loup has been modified.
	 */
	bool update_loup_probing(const IntervalVector& box);

	/**
	 * \brief Update loup using inner linearizations.
	 * return true if the loup has been modified.
	 */
	bool update_loup_simplex(const IntervalVector& box);


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
	Function df;


	/** 
	 * \brief Computes and returns  the value ymax (the loup decreased with the precision)
	 * the heap and the current box are actually contracted with y <= ymax
	 *
	 */
	double compute_ymax ();

	bool loup_changed;

private:

	/** linear solver used in ibex_OptimSimplex.cpp_ */
	LinearSolver *mylp;

	/** Inner contractor (for the negation of g) */
	CtcUnion* is_inside;

	/** Lower bound of the small boxes taken by the precision */
	double uplo_of_epsboxes;

	/** Number of cells put into the heap (which passed through the contractors)  */
	int nb_cells;

	/** Currently entailed constraints */
	EntailedCtr* entailed;

	/** Miscellaneous   for statistics */
	int nb_simplex;
	int nb_rand;
	double diam_simplex;
	double diam_rand;
	int nb_inhc4;
	double diam_inhc4;


};


} // end namespace ibex
#endif // __IBEX_OPTIMIZER_H__
