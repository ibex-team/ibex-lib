//============================================================================
//                                  I B E X                                   
// File        : ibex_CtcLR.h
// Author      : Ignacio Araya, 
//               Bertrand Neveu, Gilles Trombettoni
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jul 20, 2012
// Last Update : March 19, 2013
//============================================================================


#ifndef __IBEX_CTC_LR_H__
#define __IBEX_CTC_LR_H__


#include "ibex_CtcLinearRelaxationIter.h"
#include "ibex_CtcART.h"
#include "ibex_CtcXNewton.h"

namespace ibex {

/** \ingroup ctcgroup
 * \brief LR contractor
 *
 * This class is an implementation of the LR algorithm
 * \author Ignacio Araya, Gilles Trombettoni, Jordan Ninin, Bertrand Neveu
 * \date February 2011
 */

class CtcLR : public CtcLinearRelaxationIter {

public:


	/** Creates the X_Newton
	 *
	 * \param sys The system (the extended system in case of optimization)
	 * \param cpoints The vector of corner selection in linearization (X_INF, X_SUP, RANDOM, RANDOM_INV)
	 * \param cmode X_NEWTON (contracts all the box) | LOWER_BOUNDING (in optimization only improves the left bound of the variable y)
	 * \param lmode AFFINE2 | TAYLOR | HANSEN | COMPO: linear relaxation method.
	 * \param max_iter_soplex : the maximum number of iterations for Soplex (default value 100)
	 * \param max_diam_deriv : the maximum diameter of the derivatives for calling Soplex (default value 1.e5)
	 * \param max_diam_box : the maximum diameter of the variables for calling Soplex (default value 1.e4) Soplex may lose solutions when it is called with "big" domains.
	 */

	CtcLR(const System& sys,ctc_mode cmode=ALL_BOX, linear_mode lmode=COMPO);

	/** Deletes this instance. */
	~CtcLR();

	/** LR iteration.
  Linearize the system and performs 2n calls to Simplex in order to reduce
  the 2 bounds of each variable */
	int linearization( IntervalVector & box, LinearSolver *mysolver);

private:


	/**  AFFINE2 | TAYLOR | HANSEN | COMPO : the linear relaxation method */
	linear_mode lmode;

	/** ART object to linearize	 */
	CtcART *myart;

	/** XNewton  object to linearize	 */
	CtcXNewton *myxnewton;

	/** The vector of corner selection in linearization (RANDOM, RANDOM_INV) by default */
	std::vector<CtcXNewton::corner_point> cpoints;
};

}

#endif // endif  __IBEX_CTC_LR_H__

