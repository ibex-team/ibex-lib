//============================================================================
//                                  I B E X
// File        : ibex_Optimizer.h
// Author      : Gilles Chabert, Bertrand Neveu
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : May 14, 2012
// Last Update : Jul 24, 2017
//============================================================================

#ifndef __IBEX_OPTIMIZER_H__
#define __IBEX_OPTIMIZER_H__

#include "ibex_Ctc.h"
#include "ibex_Bsc.h"
#include "ibex_LoupFinder.h"
#include "ibex_CellBufferOptim.h"
//#include "ibex_EntailedCtr.h"
#include "ibex_CtcKhunTucker.h"
#include "ibex_CovOptimData.h"

namespace ibex {

/**
 * \defgroup optim IbexOpt
 */

/**
 * \ingroup optim
 *
 * \brief Global Optimizer.
 *
 * This class is an (improved) implementation of the global optimization algorithm described
 * in the AAAI'11 paper <i>Inner Regions and Interval Linearizations for Global Optimization</i>
 * by Trombettoni et al.
 *
 * The optimizer uses operators that either work on the *original* or *extended* system.
 *
 * The extended system corresponds to the normalized system (inequalities are all "<=")
 * with the goal f(x) represented as a variable "y" and with the additional constraint
 * y=f(x). An *extended* box stores the domain [ylb,yub] where "ylb" is the lower bound
 * of f(x) in the box and "yub" its upper bound. (note: y is contracted with y <= ymax :
 * see #compute_ymax())
 *
 * \remark In all the comments of this class, "loup" means "lowest upper bound" of the criterion f
 * and "uplo" means "uppermost lower bound" of the criterion.
 */
class Optimizer {

public:

	/**
	 * \brief Return status of the optimizer
	 *
	 * See comments for optimize(...) below.
	 */
	typedef enum {SUCCESS, INFEASIBLE, NO_FEASIBLE_FOUND, UNBOUNDED_OBJ, TIME_OUT, UNREACHED_PREC} Status;

	/**
	 *  \brief Create an optimizer.
	 *
	 * Inputs:
	 *   \param n        - number of variables or the <b>original system</b>
	 *   \param ctc      - contractor for <b>extended<b> boxes (of size n+1)
	 *   \param bsc      - bisector for <b>extended<b> boxes (of size n+1)
	 *   \param finder   - upper-bounding procedure for the original system (n-sized boxes)
	 *   \param buffer   - buffer for <b>extended<b> boxes (of size n+1)
	 *   \param goal_var - index of the goal variable in an extended box.
	 *
	 * And optionally:
	 *   \param eps_x         - absolute precision for the boxes (bisection control)
	 *   \param rel_eps_f     - relative precision of the objective (the optimizer stops once reached).
	 *   \pram  abs_eps_f     - absolute precision of the objective (the optimizer stops once reached).
	 *
	 * \warning The optimizer relies on the contractor \a ctc to contract the domain of the goal variable
	 *          and increase the uplo. If this contractor never contracts this goal variable,
	 *          the optimizer will only rely on the evaluation of f and will be very slow.
	 *
	 */
	Optimizer(int n, Ctc& ctc, Bsc& bsc, LoupFinder& finder, CellBufferOptim& buffer,
			int goal_var,
			double eps_x=default_eps_x,
			double rel_eps_f=default_rel_eps_f,
			double abs_eps_f=default_abs_eps_f);

	/**
	 * \brief Delete *this.
	 */
	virtual ~Optimizer();

	/**
	 * \brief Run the optimization.
	 *
	 * \param init_box             The initial box
	 * \param obj_init_bound       (optional) can be set when an initial upper bound of the objective minimum is known a priori.
	 *                             This bound can be obtained, e.g., by a local solver. This is equivalent to (but more practical
	 *                             than) adding a constraint f(x)<=obj_init_bound.
	 *
	 * \return SUCCESS             if the global minimum (with respect to the precision required) has been found.
	 *                             In particular, at least one feasible point has been found, less than obj_init_bound, and in the
	 *                             time limit.
	 *
	 *         INFEASIBLE          if no feasible point exist less than obj_init_bound. In particular, the function returns INFEASIBLE
	 *                             if the initial bound "obj_init_bound" is LESS than the true minimum (this case is only possible if
	 *                             goal_abs_prec and goal_rel_prec are 0). In the latter case, there may exist feasible points.
	 *
	 *         NO_FEASIBLE_FOUND   if no feasible point could be found less than obj_init_bound. Contrary to INFEASIBLE,
	 *                             infeasibility is not proven here. Warning: this return value is sensitive to the abs_eps_f and
	 *                             rel_eps_f parameters. The upperbounding makes the optimizer only looking for points less than
	 *                             min { (1-rel_eps_f)*obj_init_bound, obj_init_bound - abs_eps_f }.
	 *
	 *         UNBOUNDED_OBJ       if the objective function seems unbounded (tends to -oo).
	 *
	 *         TIMEOUT             if time is out.
	 *
	 *         UNREACHED_PREC      if the search is over but the resulting interval [uplo,loup] does not satisfy the precision
	 *                             requirements. There are several possible reasons: the goal function may be too pessimistic
	 *                             or the constraints function may be too pessimistic with respect to the precision requirement
	 *                             (which can be too stringent). This results in tiny boxes that can neither be contracted nor
	 *                             used as new loup candidates. Finally, the eps_x parameter may be too large.
	 *
	 */
	Status optimize(const IntervalVector& init_box, double obj_init_bound=POS_INFINITY);

	/**
	 * \brief Continue optimization.
	 *
	 * Load intermediate data from a COV structure, and continue optimization.
	 *
	 * See above for comments on return values.
	 */
	Status optimize(const CovOptimData& cov, double obj_init_bound=POS_INFINITY);


	/* \brief Continue optimization.
	 *
	 * Load intermediate data from a COV file, and continue optimization.
	 *
	 * See above for comments on return values.
	 */
	Status optimize(const char* cov_file, double obj_init_bound=POS_INFINITY);

	/* =========================== Output ============================= */

	/**
	 * \brief Displays on standard output a report of the last call to optimize(...).
	 *
	 * Information provided:
	 * <ul><li> interval of the cost  [uplo,loup]
	 *     <li> the best feasible point found
	 *     <li> total running time
	 *     <li> total number of cells (~boxes) created during the exploration
	 * </ul>
	 */
	void report();
	
	/**
	 * \brief Get the status.
	 *
	 * \return the status of last call to optimize(...).
	 */
	Status get_status() const;

	/**
	 * \brief Get the "uplo" (<= f*).
	 *
	 * "uplo" stands for *up*permost *lo*wer bound of the minimum f*.
	 *
	 * \return the uplo of the last call to optimize(...).
	 */
	double get_uplo() const;

	/**
	 * \brief Get the "loup" (>= f*).
	 *
	 * "loup" stands for *lo*west *up*per bound of the minimum f*.
	 *
	 * \return the loup of the last call to optimize(...).
	 */
	double get_loup() const;

	/**
	 * \brief Get x* (== argmin).
	 *
	 * \note If the loup-finder is not rigorous, this point is only the
	 *       argmin of the eps-h-relaxed problem. See #LoupFinder.
	 *
	 * \return the argmin of the last call to optimize(...).
	 */
	const IntervalVector& get_loup_point() const;

	/**
	 * \brief Get the time spent.
	 *
	 * \return the total CPU time of last call to optimize(...)
	 */
	double get_time() const;

	/**
	 * \brief Get the number of cells.
	 *
	 * \return the number of cells generated by the last call to optimize(...).
	 */
	size_t get_nb_cells() const;

	/**
	 * \brief Get the relative precision on the objective obtained after last execution.
	 */
	double get_obj_rel_prec() const;

	/**
	 * \brief Get the absolute precision on the objective obtained after last execution.
	 */
	double get_obj_abs_prec() const;

	/**
	 * \brief Get all optimization data.
	 */
	const CovOptimData& get_data() const;

	/* =========================== Settings ============================= */

	/**
	 * \brief Number of variables.
	 */
	const int n;

	/**
	 * \brief Index of the goal variable.
	 *
	 * See #ExtendedSystem.goal_var().
	 */
	const int goal_var;

	/**
	 * \brief Contractor for the extended system.
	 *
	 * The extended system:
	 * (y=f(x), g_1(x)<=0,...,g_m(x)<=0).
	 */
	Ctc& ctc;

	/**
	 * \brief Bisector.
	 *
	 * Must work on extended boxes.
	 */
	Bsc& bsc;

	/**
	 * \brief Loup finder algorithm.
	 *
	 * Must work on the original system.
	 */
	LoupFinder& loup_finder;

	/**
	 * Cell buffer.
	 */
	CellBufferOptim& buffer;

	/** Precision (bisection control) */
	const double eps_x;

	/** Default bisection precision: 0. */
	static constexpr double default_eps_x = 0;

	/** Relative precision on the objective */
	const double rel_eps_f;

	/** Default goal relative precision: 1e-3. */
	static constexpr double default_rel_eps_f = 1e-03;

	/** Absolute precision on the objective */
	const double abs_eps_f;

	/** Default goal absolute precision: 1e-7. */
	static constexpr double default_abs_eps_f = 1e-07;;

	/**
	 * \brief Trace activation flag.
	 *
	 * The value can be fixed by the user.
	 * - 0 (by default): nothing is printed
	 * - 1:              prints every loup/uplo update.
	 * - 2:              prints also each handled node (warning: can generate very
	 *                   long trace).
	 */
	int trace;

	/**
	 * \brief Time limit.
	 *
	 * Maximum CPU time used by the strategy.
	 * This parameter allows to bound time consumption.
	 * The value can be fixed by the user.
	 */
	double timeout;

	/**
	 * \brief Whether the output has to be in the extended space.
	 *
	 * If true, the COV structure generated by the optimizer will
	 * be in the extended space (variables+objective values).
	 * If false, the structure will be in the original space (variables
	 * only).
	 */
	bool extended_COV;

	/**
	 * \brief Whether anticipated upper bounding has to be applied.
	 *
	 * If true, the search space is not only contracted w.r.t.
	 *         f(x) <= loup
	 * but
	 *         f(x) <= loup - eps
	 * where eps is the required precision on the objective
	 * (the cumul of absolute and relative precision).
	 *
	 * Default value: true.
	 */
	bool anticipated_upper_bounding;

protected:
	/*
	 * \brief Initialize the optimizer from a single box.
	 */
	void start(const IntervalVector& init_box, double obj_init_bound=POS_INFINITY);

	/*
	 * \brief Initialize the optimizer from a COV input file.
	 */
	void start(const CovOptimData& cov, double obj_init_bound=POS_INFINITY);

	/**
	 * \brief Run the optimizer (once started).
	 */
	Status optimize();

	/**
	 * \brief Main procedure for processing a box.
	 *
	 * <ul>
	 * <li> contract the cell box and try to find a new loup (see contract_and_bound)
	 * <li> push the cell into the buffer or delete the cell in case of empty box detected.
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
	 * <li> (optional) call the first order contractor
	 * </ul>
	 *
	 */
	void contract_and_bound(Cell& c);

	/**
	 * \brief Update the entailed constraint for the current box
	 *
	 * \return false if unsatisfiability is detected, true otherwise.
	 */
	//bool update_entailed_ctr(const IntervalVector& box);

	/**
	 * \brief Update the uplo of tiny (ex: non-bisectable) boxes.
	 */
	void update_uplo_of_epsboxes(double ymin);

	/**
	 * \brief Update the uplo
	 */
	void update_uplo();

	/**
	 * \brief Main procedure for updating the loup.
	 */
	bool update_loup(const IntervalVector& box, BoxProperties& prop);

	/**
	 * \brief Computes and returns  the value ymax (the loup decreased with the precision)
	 * the heap and the current box are actually contracted with y <= ymax
	 *
	 */
	double compute_ymax ();

	/**
	 * \brief Check time is not out.
	 */
	void time_limit_check();

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

	/** Currently entailed constraints */
	//EntailedCtr* entailed;

	//!! warning: sys.box should be properly set before call to constructor !!
	//CtcKhunTucker kkt;

	/* Remember return status of the last optimization. */
	Status status;

	/** The current uplo. */
	double uplo;

	/** Lower bound of the small boxes taken by the precision. */
	double uplo_of_epsboxes;

	/** The current loup. */
	double loup;

	/**
	 * The point satisfying the constraints corresponding to the loup.
	 * If the loup-finder is rigorous, this point is a (non-degenerated) box.
	 */
	IntervalVector loup_point;

	/**
	 * \brief The bound on the objective given by the user, +oo otherwise.
	 *
	 * Used to see if at least a loup-point has been found.
	 *
	 */
	double initial_loup;

	/** True if loup has changed in the last call to handle_cell(..) */
	bool loup_changed;

	/* CPU running time of the current optimization. */
	double time;

	/** Number of cells pushed into the heap (which passed through the contractors) */
	size_t nb_cells;

	/** Result. */
	CovOptimData* cov;
};

inline Optimizer::Status Optimizer::get_status() const { return status; }

inline double Optimizer::get_uplo() const { return uplo; }

inline double Optimizer::get_loup() const { return loup; }

inline const IntervalVector& Optimizer::get_loup_point() const { return loup_point; }

inline double Optimizer::get_time() const { return time; }

inline size_t Optimizer::get_nb_cells() const { return nb_cells; }

inline double Optimizer::get_obj_rel_prec() const {
	if (loup==POS_INFINITY)
		return POS_INFINITY;
	else if (loup==0)
		if (uplo<0) return POS_INFINITY;
		else return 0;
	else
		return (loup-uplo)/(fabs(loup));
}

inline double Optimizer::get_obj_abs_prec() const {
	return loup-uplo;
}

inline const CovOptimData& Optimizer::get_data() const {
	if (!cov) ibex_error("[Optimizer] no data (run optimizer first)");
	return *cov;
}

//inline std::ostream& operator<<(std::ostream& os, const Optimizer::Status s) {
//	switch (s) {
//	case Optimizer::SUCCESS:
//		return os << "SUCCESS";
//		break;
//	case Optimizer::INFEASIBLE:
//		return os << "INFEASIBLE";
//		break;
//	case Optimizer::NO_FEASIBLE_FOUND:
//		return os << "NO_FEASIBLE_FOUND";
//		break;
//	case Optimizer::UNBOUNDED_OBJ:
//		return os << "UNBOUNDED_OBJ";
//		break;
//	case Optimizer::TIME_OUT:
//		return os << "TIME_OUT";
//		break;
//	case Optimizer::UNREACHED_PREC:
//		return os << "UNREACHED_PREC";
//		break;
//	default:
//		break;
//	}
//	return os;
//}


} // end namespace ibex

#endif // __IBEX_OPTIMIZER_H__
