/* ============================================================================
 * I B E X - AbsTaylor linearizer
 * ============================================================================
 *
 * Author(s)   : Ignacio Araya, Victor Reyes
 * Created     : May 2021
 * Updated     : May 2021
 * ---------------------------------------------------------------------------- */

#ifndef __IBEX_LINEARIZER_ABS_TAYLOR__
#define __IBEX_LINEARIZER_ABS_TAYLOR__

#include "ibex_Linearizer.h"
#include "ibex_System.h"


namespace ibex {

/**
 * \ingroup numeric
 *
 * \brief AbsTaylor linearization technique.
 *
 * This class is an implementation of the AbsTaylor algorithm
 *
 * \see  Reyes, V., & Araya, I. (2021). AbsTaylor: upper
 * bounding with inner regions in nonlinear continuous global optimization problems.
 * Journal of Global Optimization, 79(2), 413-429.
 *
 */
class LinearizerAbsTaylor : public Linearizer {

public:
	/**
	 * \brief Policy for selecting the expansion point.
	 *
	 * - MID        - The mid point of the box
	 * - RANDOM     - A random point inside the box
	 */
	typedef enum  {MID, RANDOM} point_policy;
	/**
	 * \brief Creates the X_Taylor linearizer.
	 *
	 * \param sys             - The system (extended or not).
	 */
	LinearizerAbsTaylor(const System& sys, point_policy = MID);

	/**
	 * \brief Deletes this.
	 */
	~LinearizerAbsTaylor();

	/**
	 * \brief Generation of the linear inequalities
	 *
	 * \param box             - The current box
	 * \param lp_solver       - The linear solver.
	 */
	virtual int linearize(const IntervalVector& box, LPSolver& lp_solver);

private:

	/**
	 * \brief Linearization (RESTRICT mode)
	 */
	int linear_restrict(const IntervalVector& box);

	/**
	 * \brief Linearize a constraint g(x)<=0 inside a box, from the midpoint.
	 *
	 * \param dg_box:   dg([box])
	 * \param g_mid: g(mid)
	 */
	int linearize_leq_mid(const IntervalVector& box, const Vector& point, const IntervalVector& dg_box, const Interval& g_mid);

	/**
	 * \brief Add the constraint ax<=b in the LP solver.
	 */
	int check_and_add_constraint(const IntervalVector& box, const Vector& a, double b);

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
	 * Current LP solver
	 */
	LPSolver* lp_solver;
	/*
	 * Point policy
	 */
	point_policy point;
};

} // end namespace ibex

#endif /* __IBEX_LINEARIZER_ABS_TAYLOR__ */
