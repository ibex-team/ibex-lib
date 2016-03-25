//============================================================================
//                                  I B E X                                   
// File        : ibex_LinearRelaxXTaylor.h
// Author      : Ignacio Araya, Bertrand Neveu,
//               Gilles Trombettoni, Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jul 20, 2012
// Last Update : Jul 02, 2013 (Gilles Chabert)
//============================================================================


#ifndef __IBEX_LINEAR_RELAX_X_TAYLOR__
#define __IBEX_LINEAR_RELAX_X_TAYLOR__

#include "ibex_System.h"
#include "ibex_LinearRelax.h"

#include <vector>

namespace ibex {

/**
 * \ingroup numeric
 * \brief X_Taylor contractor
 *
 * This class is an implementation of the X-Taylor algorithm
 */

class LinearRelaxXTaylor : public LinearRelax {

public:

	/**
	 * TODO: add comment
	 */
	typedef enum  { TAYLOR, HANSEN } linear_mode;

	/**
	 * \brief Strategies for selecting corner points.
	 */
	typedef enum  {RANDOM_INV, NEG, INF_X, SUP_X, RANDOM, GREEDY1, GREEDY5, GREEDY6, BEST , MONO,NEGMONO, K4} corner_point;

	/** Default max_diam_deriv value, set to 1e6  **/
	static const double default_max_diam_deriv;

	/**
	 * \brief Creates the X_Taylor contractor.
	 *
	 * \param sys             - The system (extended or not).
	 * \param cpoints         - The vector of corner selection in linearization (X_INF, X_SUP, RANDOM, RANDOM_INV)
	 * \param lmode           - TAYLOR | HANSEN : linear relaxation method.
	 * \param max_diam_deriv  - The maximum diameter of the box for for the linear solver (default value 1.e6).
	 * 	  				        Soplex may lose solutions when it is called with "big" domains.
	 */
	LinearRelaxXTaylor(const System& sys, std::vector<corner_point>& cpoints,
			linear_mode lmode=HANSEN, double max_diam_deriv=default_max_diam_deriv);

	/**
	 * \brief Deletes this instance.
	 */
	~LinearRelaxXTaylor();


	/**
	 * \brief Generation of the linearized system
	 *
	 * Linearize the system and performs 2n calls to Simplex in order to reduce
	 * the 2 bounds of each variable
	 */
	int linearization(const IntervalVector& box, LinearSolver& lp_solver);

	/**
	 * \brief Generation of a linear approximation of the inner region
	 *
	 */
	int inlinearization(const IntervalVector& box, LinearSolver& lp_solver);

	/**
	 * \brief Generation of a linear approximation of the linear objective function
	 *
	 */
	bool goal_linearization(const IntervalVector& box, LinearSolver& lp_solver);

private:

	/**
	 * \brief The vector of corner selection in linearization
	 *
	 * Can be either X_INF, X_SUP, RANDOM, or RANDOM_INV.
	 */
	std::vector<corner_point> cpoints;

	/**
	 * \brief The system
	 */
	const System& sys;

	/**
	 * \brief Goal constraint (in case of extended system, -1 otherwise).
	 */
	const int goal_ctr;

	/** Maximum diameter of the derivatives for calling linear solver (default value 1.e5) */
	double max_diam_deriv;

	/** TAYLOR | HANSEN : the linear relaxation method */
	linear_mode lmode;

	/** Stores the coefficients of linear constraints */
	IntervalMatrix linear_coef;

	/* For implementing RANDOM_INV one needs to store the last random corners */
	int* last_rnd;

	bool* base_coin;

	/** Indicates if the constraint is linear wrt to each variable */
	bool** linear;

	/** Indicates if the constraint is linear wrt to each variable */
	bool* linear_ctr;

	/**
	 * Initialize the #linear and #linear_coef fields.
	 */
	void init_linear_coeffs();

	/**
	 * \brief Tries to add a linearization in the model mysoplex.
	 *
	 * \return 0 only when the linearization is not performed
	 */
	int X_Linearization(const IntervalVector & box, int ctr, corner_point cpoint,  IntervalVector &G,
			int id_point, int& non_linear_vars, LinearSolver& lp_solver);

	int X_Linearization(const IntervalVector& box, int ctr, corner_point cpoint, CmpOp op,
			IntervalVector &G, int id_point, int& non_linear_vars, LinearSolver& lp_solver);

	/**
	 * \brief Random hoice of a corner
	 */
	bool choose_corner(const IntervalVector& box, IntervalVector& x_corner,	bool* corner);

	/**
	 * \brief Symbolic Jacobian
	 */
	Function* df;

//	// used in greedy heuristics :  not implemented in v2.0
//	inline double abs(double a){
//		return (a>=0)? a:-a;
//	}
//
//	//Evaluation of the corner by using relation (4) in Taylorisation par intervalles convexe: premiers résultats, JFPC
//	double eval_corner(int ctr, int op, INTERVAL_VECTOR& G, bool* corner);
//
//	//finds the corner with the minimal evaluation of relation (4) in Taylorisation par intervalles convexe: premiers résultats, JFPC
//	void best_corner(int ctr, int op, INTERVAL_VECTOR& G, bool* corner);

};

} // end namespace ibex

#endif /* __IBEX_LINEAR_RELAX_X_TAYLOR__ */

