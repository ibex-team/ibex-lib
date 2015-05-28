//============================================================================
//                                  I B E X                                   
// File        : ibex_CtcLR.h
// Author      : Ignacio Araya, Bertrand Neveu,
//               Gilles Trombettoni
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jul 20, 2012
// Last Update : March 19, 2013
//============================================================================


#ifndef __IBEX_LINEAR_RELAX_COMBO_H__
#define __IBEX_LINEAR_RELAX_COMBO_H__

#include "ibex_LinearRelaxAffine2.h"
#include "ibex_LinearRelaxXTaylor.h"

namespace ibex {

/**
 * \ingroup numeric
 * \brief LR relaxation
 *
 * This class is an implementation of the LR algorithm
 * \author Ignacio Araya, Gilles Trombettoni, Jordan Ninin, Bertrand Neveu
 * \date February 2011
 */

class LinearRelaxCombo : public LinearRelax {

public:

	/**
	 * TODO: add comment
	 */
	typedef enum   { ART, AFFINE2, XNEWTON, TAYLOR, HANSEN, COMPO } linear_mode;

	/**
	 * \brief Creates the combination
	 *
	 * \param sys The system (the extended system in case of optimization)
	 * \param lmode AFFINE2 | TAYLOR | HANSEN | COMPO: linear relaxation method.
	 */

	LinearRelaxCombo(const System& sys, linear_mode lmode=COMPO);

	/**
	 * \brief Deletes this instance.
	 */
	~LinearRelaxCombo();

	/**
	 * \brief Linearization.
  	 *
  	 * Linearize the system.
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

	/**  AFFINE2 | TAYLOR | HANSEN | COMPO : the linear relaxation method */
	linear_mode lmode;

	/** ART object to linearize	 */
	LinearRelaxAffine2 *myart;

	/** XNewton  object to linearize	 */
	LinearRelaxXTaylor *myxnewton;

	/** The vector of corner selection in linearization (RANDOM, RANDOM_INV) by default */
	std::vector<LinearRelaxXTaylor::corner_point> cpoints;
};

}

#endif // __IBEX_LINEAR_RELAX_COMBO_H__

