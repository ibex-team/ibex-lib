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

#include "ibex_Setting.h"

#ifdef _IBEX_WITH_AFFINE_
#include "ibex_LinearRelaxAffine2.h"
#endif

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
	typedef enum { XNEWTON, TAYLOR, HANSEN, ART, AFFINE2, COMPO } linear_mode;

	/**
	 * \brief Creates the combination
	 *
	 * \param sys The system (the extended system in case of optimization)
	 * \param lmode AFFINE2 | TAYLOR | HANSEN | COMPO: linear relaxation method.
	 */

#ifdef _IBEX_WITH_AFFINE_
	LinearRelaxCombo(const System& sys, linear_mode lmode=COMPO);
#else
	LinearRelaxCombo(const System& sys, linear_mode lmode=XNEWTON);
#endif


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

	/** XNewton  object to linearize	 */
	LinearRelaxXTaylor *myxnewton;

#ifdef _IBEX_WITH_AFFINE_
	/** ART object to linearize	 */
	LinearRelaxAffine2 *myart;
#endif

	/** The vector of corner selection in linearization (RANDOM, RANDOM_INV) by default */
	std::vector<LinearRelaxXTaylor::corner_point> cpoints;
};

}

#endif // __IBEX_LINEAR_RELAX_COMBO_H__

