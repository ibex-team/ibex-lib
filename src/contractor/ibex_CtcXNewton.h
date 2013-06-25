//============================================================================
//                                  I B E X                                   
// File        : ibex_CtcXNewton.h
// Author      : Ignacio Araya, 
//               Bertrand Neveu, Gilles Trombettoni
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jul 20, 2012
// Last Update : March 19, 2013
//============================================================================


#ifndef __IBEX_CTC_XNEWTON_H__
#define __IBEX_CTC_XNEWTON_H__

#include "ibex_Ctc.h"
#include "ibex_System.h"
#include "ibex_NumConstraint.h"
#include "ibex_CtcLinearRelaxationIter.h"
#include "ibex_LinearSolver.h"

#include <vector>

namespace ibex {

/** \ingroup ctcgroup
 * \brief X_Newton contractor 
 *
 * This class is an implementation of the X-Newton algorithm
 * \author Ignacio Araya, Gilles Trombettoni     
 * \date February 2011
 */

class CtcXNewton : public CtcLinearRelaxationIter {

public:


	typedef enum  {RANDOM_INV, NEG, INF_X, SUP_X, RANDOM, GREEDY1, GREEDY5, GREEDY6, BEST , MONO,NEGMONO, K4} corner_point;



	/** Creates the X_Newton
	 *
	 * \param sys The system (the extended system in case of optimization)
	 * \param cpoints The vector of corner selection in linearization (X_INF, X_SUP, RANDOM, RANDOM_INV)
	 * \param goal_ctr  (goal index for optimization, -1 for constraint solving)
	 * \param goal_var
	 * \param goal   (goal function pointer for optimization, NULL for constraint solving)
	 * \param cmode X_NEWTON (contracts all the box) | LOWER_BOUNDING (in optimization only improves the left bound of the variable y)
	 * \param lmode TAYLOR | HANSEN : linear relaxation method.
	 * \param max_iter_soplex : the maximum number of iterations for Soplex (default value 100)
	 * \param max_diam_deriv : the maximum diameter of the derivatives for calling Soplex (default value 1.e5)
	 * \param max_diam_box : the maximum diameter of the variables for calling Soplex (default value 1.e4) Soplex may lose solutions when it is called with "big" domains.
	 */


	CtcXNewton(const System& sys, std::vector<corner_point>& cpoints,
			ctc_mode cmode=ALL_BOX, linear_mode lmode=HANSEN, int max_iter=LinearSolver::default_max_iter,
			double max_diam_deriv=default_max_diam_deriv, double max_diam_box=LinearSolver::default_max_diam_box);


	/** Deletes this instance. */
	~CtcXNewton();


	/** The vector of corner selection in linearization (X_INF, X_SUP, RANDOM, RANDOM_INV) */
	std::vector<corner_point>& cpoints;

	/** X_Newton iteration.
  Linearize the system and performs 2n calls to Simplex in order to reduce
  the 2 bounds of each variable */
	int linearization( IntervalVector & box, LinearSolver *mysolver);

private:
	/* Computes the gradient G of the constraint ctr : special case if ctr==goal_ctr */
	void gradient_computation (IntervalVector& box, IntervalVector& G, int ctr);


	/** Default max_diam_deriv value, set to 1e5  **/
	static const double default_max_diam_deriv;

	/** max_diam_deriv : the maximum diameter of the derivatives for calling Soplex (default value 1.e5) */
	double max_diam_deriv;

	/** TAYLOR | HANSEN : the linear relaxation method */
	linear_mode lmode;

	/** stores the coefficients of linear constraints */
	IntervalMatrix linear_coef;

	/* for implementing RANDOM_INV one needs to store the last random corners */
	int* last_rnd;

	int* base_coin;

	/** indicates if the constraint is linear */
	bool* linear;


	/** Tries to add a linearization in the model mysoplex. Returns true if it is succesful */
	int X_Linearization(IntervalVector & box, int ctr, corner_point cpoint,  IntervalVector &G,
			int id_point, int& non_linear_vars, LinearSolver *mysolver);

	int X_Linearization(IntervalVector& box, int ctr, corner_point cpoint, CmpOp op,
			IntervalVector &G, int id_point, int& non_linear_vars, LinearSolver *mysolver);



	/*
used in greedy heuristics :  not implemented in v2.0
  inline double abs(double a){
    return (a>=0)? a:-a;
   }

  //Evaluation of the corner by using relation (4) in Taylorisation par intervalles convexe: premiers résultats, JFPC 
  REAL eval_corner(int ctr, int op, INTERVAL_VECTOR& G, bool* corner);

  //finds the corner with the minimal evaluation of relation (4) in Taylorisation par intervalles convexe: premiers résultats, JFPC 
  void best_corner(int ctr, int op, INTERVAL_VECTOR& G, bool* corner);
	 */


};

}

#endif

