/*
 * ibex_LinearizerParallelTaylor.h
 *
 *  Created on: Aug 7, 2019
 *      Author: iaraya
 */

#ifndef __IBEX_LINEARIZER_PARALLEL_TAYLOR__
#define __IBEX_LINEARIZER_PARALLEL_TAYLOR__

#include "ibex_Linearizer.h"
#include "ibex_System.h"
#include "ibex_BxpSystemCache.h"

namespace ibex {

/**
 * \ingroup numeric
 *
 * \brief Parallel linearization technique.
 *
 * This class is an implementation of the Paralle linearization methodithm
 *
 * \see "Inner Regions and Interval Linearizations for Global Optimization"
 *      G. Trombettoni, I. Araya, B. Neveu, G. Chabert
 *      Proc. of AAAI, AAAI Press, p. 99-104, 2011.
 */
class LinearizerParallelTaylor : public Linearizer {

public:

	/**
	 * \brief Approximation type.
	 *
	 * If f(x)<=0 denotes the non-linear constraints
	 * and Ax<=b the linear approximation, then:
	 *
	 * - RELAX:    f(x)<=0 => Ax<=b
	 * - RESTRICT: f(x)<=0 <= Ax<=b
	 */
	typedef enum  { RELAX, RESTRICT } approx_mode;


	/**
	 * \brief Slope matrices computation formula.
	 *
	 * - TAYLOR: one gradient evaluation for all variables simultaneously
	 *           (less accurate but faster)
	 * - HANSEN: recursive evaluation with successive variable instantiation
	 *           (more accurate but longer)
	 */
	typedef enum  { TAYLOR, HANSEN } slope_formula;

	/**
	 * \brief Creates the X_Taylor linearizer.
	 *
	 * \param sys             - The system (extended or not).
	 * \param mode            - Approximation type (RELAX | RESTRICT)
	 * \param corners         - Corner selection policy (INF | SUP | RANDOM | RANDOM_OPP)
	 * \param slope           - Slope matrix formula (TAYLOR | HANSEN)
	 * \param max_diam_deriv  - The maximum diameter of the box for the linear solver (default value 1.e6).
	 * 	  				        Soplex may lose solutions when it is called with "big" domains.
	 */
	LinearizerParallelTaylor(const System& sys, approx_mode mode, slope_formula slope=HANSEN);

	/**
	 * \brief Deletes this.
	 */
	~LinearizerParallelTaylor() { };

	/**
	 * \brief Generation of the linear inequalities
	 */
	virtual int linearize(const IntervalVector& box, LPSolver& lp_solver);

	/**
	 * \brief Generation of the linear inequalities
	 */
	virtual int linearize(const IntervalVector& box, LPSolver& lp_solver, BoxProperties& prop);

	/**
	 * \brief Add BxpActiveCtrs
	 */
	virtual void add_property(const IntervalVector& init_box, BoxProperties& prop);

private:

	/**
	 * \brief Linearization (RELAX mode)
	 */
	int linear_relax(const IntervalVector& box, const BitSet& active);

	/**
	 * \brief Add the constraint ax<=b in the LP solver.
	 */
	int check_and_add_constraint(const IntervalVector& box, const Vector& a, double b, CmpOp op);

	/**
	 * \brief The system
	 */
	const System& sys;

	/**
	 * \brief Number of (real-valued) constraints
	 */
	int m;

	/**
	 * \brief Goal constraint (in case of extended system, -1 otherwise).
	 */
	const int goal_ctr;

	/**
	 * \brief Approximation mode.
	 */
	approx_mode mode;



	/**
	 * \brief Slope matrix formula.
	 */
	slope_formula slope;



	/**
	 * Current LP solver
	 */
	LPSolver* lp_solver;

};

} // end namespace ibex

#endif /* NUMERIC_IBEX_LINEARIZERPARALLELTAYLOR_H_ */
