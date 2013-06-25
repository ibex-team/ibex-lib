//============================================================================
//                                  I B E X                                   
// File        : ibex_CtcART.h
// Author      : Jordan Ninin
// License     : See the LICENSE file
// Created     : May 19, 2013
// Last Update : May 19, 2013
//============================================================================


#ifndef __IBEX_CTC_ART_H__
#define __IBEX_CTC_ART_H__

#include "ibex_Ctc.h"
#include "ibex_System.h"
#include "ibex_NumConstraint.h"
#include "ibex_CtcLinearRelaxationIter.h"
#include "ibex_LinearSolver.h"
#include "ibex_Affine2.h"

#include <vector>

namespace ibex {

/** \ingroup ctcgroup
 * \brief ART contractor
 *
* This class is an implementation of the ART algorithm
 * \author Jordan Ninin
 * \date May 2013
 */

class CtcART : public CtcLinearRelaxationIter {

public:


	CtcART (const System& sys,	ctc_mode cmode=ALL_BOX,
			int max_iter=LinearSolver::default_max_iter, int time_out=LinearSolver::default_max_time_out,
			double eps=LinearSolver::default_eps, double max_diam_box=LinearSolver::default_max_diam_box);


	~CtcART ();

	/** ART iteration.
  Linearize the system and performs 2n calls to Simplex in order to reduce
  the 2 bounds of each variable */
	int linearization( IntervalVector & box, LinearSolver *mysolver);


};

} // end namespace ibex


#endif /* CTC_ARTITER_H_ */

