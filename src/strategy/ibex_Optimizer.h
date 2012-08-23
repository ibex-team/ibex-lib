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
#include "ibex_CtcUnion.h"
#include "ibex_Backtrackable.h"
#include "ibex_CellHeapOptim.h"
#include "ibex_System.h"

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
	 *   \param f     - The objective function f(x)
	 *   \param g     - the constraints (each constraint must be g_i(x)<=0)
	 *   \param bsc   - bisector for extended boxes
	 *   \param ctc   - contractor for the extended system
	 *
	 * And optionally:
	 *   \param prec          - absolute precision for the boxes (bisection control)
	 *   \param goal_rel_prec - relative precision of the objective (the optimizer stops once reached).
	 *   \pram  goal_abs_prec - absolute precision of the objective (the optimizer stops once reached).
	 *   \param sample_size   - number of samples taken when looking for a "loup"
	 *
	 * <ul> The extended system (see copy constructor of #ibex::System) contains:
	 * <li> (n+1) variables, x_1,...x_n,y. The index of y is #goal_var (==n if it is the last).
	 * <li> A (m+1)-valued function f:(x,y)->(y-f(x),g_0(x),...,g_m(x))
	 * <li> (m+1) constraints: y-f(x)=0, g_1(x)<=0, ..., g_m(x)<=0.
	 *      The index of y-f(x)=0 in the system is #goal_ctr (==0 if it is the first).
	 * </ul>
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
	 */
	void optimize(const IntervalVector& init_box);

	/**
	 * \brief Displays on standard output a report of the last call to #optimize(const IntervalVector&).
	 *
	 * Information provided:
	 * <ul><li>last value of the "loup"
	 *     <li>total number of cells created during the exploration
	 *     <li>total running time
	 * </ul>
	 */
	void report();

	/** Number of variables. */
	const int n;

	/** Number of constraints. */
	const int m;

	/**
	 * \brief Normalized system
	 *
	 * Corresponds to the input system (see constructor) but with all inequalities
	 * under the form g_i(x)<=0.
	 */
	System sys;

	/**
	 * */
	//System ext_sys;

	/**
	 * \brief Number of the goal constraint y-f(x)=0 in the extended system.
	 */
	int goal_ctr;

	/**
	 * \brief Index of the goal variable y in the extended system.
	 *
	 * This variables stores the interval [uplo,loup] where "uplo" is
	 * the uppermost lower bound on f(x) and "loup" the lowest upper
	 * bound.
	 */
	int goal_var;
	/*=======================================================================================================*/


	/** Bisector. */
	Bsc& bsc;

	/** Contractor for the extended system
	 * (y=f(x), g_1(x)<=0,...,g_m(x)<=0). */
	Ctc& ctc;

	/** Cell buffer. */
	CellHeapOptim buffer;

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
	 * The value can be fixed by the user. By default: false. */
	bool trace;

	/**
	 * \brief Time limit.
	 *
	 * Maximum CPU time used by the strategy.
	 * This parameter allows to bound time consumption.
	 * The value can be fixed by the user.
	 */
	double timeout;

	/** Default bisection precision: 1e-07 */
	static const double default_prec;

	/** Default goal relative precision */
	static const double default_goal_rel_prec;

	/** Default goal absolute precision */
	static const double default_goal_abs_prec;

	/** Default sample size */
	static const int default_sample_size;

	/** Name of the goal variable ("y"). */
	static const char* goal_name;

protected:
	/**
	 * \brief Main procedure for processing a box.
	 *
	 * <ul>
	 * <li> contract the cell's box w.r.t the "loup", with the contractor ctc,
	 * <li> search for a new loup,
	 * <li> push the box onto the heap.
	 * </ul>
	 *
	 * Return true iff the loup has been decreased
	 */
	bool contract_and_bound(Cell& c);

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
	 */
	bool is_feasible(const IntervalVector& box);

	/**
	 * \brief Quick check that the box is inside g(x)<=0.
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

	/*
	 * \brief First method for probing
	 *
	 * Take random points in any directions.
	 *
	 * \param is_inner - If true, the box is already known to be an inner box so there
	 *                   is no need to check constraints again.
	 *
	 * \return true in case of success, i.e., if the loup has been decreased.
	 */
	bool random_probing (const IntervalVector& box, bool is_innner);

	/*
	 * \brief Second method for probing
	 *
	 * Pick equidistant points in the opposite direction of the
	 * gradient calculated at a starting point "start".
	 *
	 * This function is simpler than a "gradient descent" since the step is calculated
	 * by the simple geometric rule explained below (we do not use rules like Wolfe's criterion, etc.).
	 *
	 * This function can be called in two different modes:
	 * - recursive=false: all the points are taken on the same half-line, starting from "start"
	 *                    and ending at a facet of the box.
	 * - recursive=true:  as soon as the "loup" is modified, a new gradient is calculated and the
	 *                    line changes accordingly. This variant is more similar to a gradient descent.
	 *
	 * return true if the loup has been modified.
	 */
	bool line_probing(const IntervalVector& box, const Vector& start, bool is_inner, int sample_size, bool recursive);

	/*
	 * \brief Update loup either using line_probing or random_probing.
	 */
	bool update_loup_probing(const IntervalVector& box);

	/**
	 * \brief Update loup using inner linearizations.
	 */
	bool update_loup_simplex(const IntervalVector& box);

	/**
	 * \brief Main procedure for updating the loup.
	 */
	bool update_loup(const IntervalVector& box);

	/**
	 * \brief Display the loup (for debug)
	 */
	void trace_loup();
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

private:

	/** Inner contractor (for the negation of g) */
	CtcUnion* is_inside;

	/** The "loup" (lowest upper bound of the criterion) */
	double loup;

	/** The point satisfying the constraints corresponding to the loup */
	Vector loup_point;

	/** Lower bound of the small boxes taken by the precision contractor */
	double uplo_of_epsboxes;

	/** Miscellaneous */
	int nb_simplex;
	int nb_rand;
	double diam_simplex;
	double diam_rand;
	int nb_inhc4;
	double diam_inhc4;

};


} // end namespace ibex
#endif // __IBEX_OPTIMIZER_H__
