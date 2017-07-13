/* ============================================================================
 * I B E X - X-Taylor linear relaxation/restriction
 * ============================================================================
 * Copyright   : IMT Atlantique (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert, Ignacio Araya, Bertrand Neveu
 * Created     : July 01th, 2012
 * Updated     : July 13th, 2017
 * ---------------------------------------------------------------------------- */

#ifndef __IBEX_LINEARIZER_X_TAYLOR__
#define __IBEX_LINEARIZER_X_TAYLOR__

#include "ibex_Linearizer.h"
#include "ibex_System.h"

namespace ibex {

/**
 * \ingroup numeric
 *
 * \brief X-Taylor linearization technique.
 *
 * This class is an implementation of the X-Taylor algorithm
 *
 * \see "Inner Regions and Interval Linearizations for Global Optimization"
 *      G. Trombettoni, I. Araya, B. Neveu, G. Chabert
 *      Proc. of AAAI, AAAI Press, p. 99-104, 2011.
 */
class LinearizerXTaylor : public Linearizer {

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
	 * \brief Policy for selecting corner points.
	 *
	 * - INF        - Lower-left corner
	 * - SUP        - Upper-right corner
	 * - RANDOM     - One random point
	 * - RANDOM_OPP - One random point and its opposite. 2 points are involved,
	 *                therefore this option is for RELAX mode only.
	 */
	typedef enum  {INF, SUP, RANDOM, RANDOM_OPP } corner_policy;

	/**
	 * \brief Slope matrices computation mode.
	 *
	 * - TAYLOR: one gradient evaluation for all variables simultaneously
	 *           (less accurate but faster)
	 * - HANSEN: recursive evaluation with successive variable instantiation
	 *           (more accurate but longer)
	 */
	typedef enum  { TAYLOR, HANSEN } linear_mode;

	/** Default max_diam_deriv value, set to 1e6  **/
	static const double default_max_diam_deriv;

	/**
	 * \brief Creates the X_Taylor linearizer.
	 *
	 * \param sys             - The system (extended or not).
	 * \param mode            - Approximation type (RELAX | RESTRICT)
	 * \param policy          - Corner selection policy (X_INF | X_SUP | RANDOM | RANDOM_INV)
	 * \param lmode           - Slope matrix formula (TAYLOR | HANSEN)
	 * \param max_diam_deriv  - The maximum diameter of the box for for the linear solver (default value 1.e6).
	 * 	  				        Soplex may lose solutions when it is called with "big" domains.
	 */
	LinearizerXTaylor(const System& sys, approx_mode mode, corner_policy corner=RANDOM, linear_mode lmode=HANSEN, double max_diam_deriv=default_max_diam_deriv);

	/**
	 * \brief Deletes this.
	 */
	~LinearizerXTaylor();

	/**
	 * \brief Generation of the linear inequalities
	 */
	virtual int linearize(const IntervalVector& box, LinearSolver& lp_solver);

private:

	/**
	 * \brief Next corner to be chosen.
	 */
	typedef enum  {INF_X, SUP_X, RANDOM_X, OPPOSITE } corner_id;

	/**
	 * \brief Set the corner information "inf".
	 *
	 * \param inf - (output) whether the jth variable is set to lower bound (true)
	 *              or upper bound (false)
	 */
	void get_corner(corner_id id);

	/**
	 * \brief Get the point corresponding to "inf".
	 */
	IntervalVector get_corner_point(const IntervalVector& box);

	/**
	 * \brief Add the constraint ax<=b in the LP solver.
	 */
	bool check_and_add_constraint(const IntervalVector& box, const Vector& a, double b, LinearSolver& lp_solver);

	/**
	 * \brief Linearization (RELAX mode)
	 */
	int linear_relax(const IntervalVector& box, LinearSolver& lp_solver);

	/**
	 * \brief Linearization (RESTRICT mode)
	 */
	int linear_restrict(const IntervalVector& box, LinearSolver& lp_solver);

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
	 * \brief The vector of selected corners.
	 */
	std::vector<corner_id> corners;


	/**
	 * \brief Slope matrix formula.
	 */
	linear_mode lmode;

	/*
	 * Boolean array indicating which corner in direction i is used :
	 * true for inferior corner, false for superior one.
	 */
	bool* inf;

	/**
	 * Maximum diameter of the derivatives for calling linear solver
	 * (default value 1.e6)
	 */
	double max_diam_deriv;

};

} // end namespace ibex

#endif /* __IBEX_LINEARIZER_X_TAYLOR__ */

