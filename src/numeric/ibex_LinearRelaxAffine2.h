//============================================================================
//                                  I B E X                                   
// File        : ibex_CtcART.h
// Author      : Jordan Ninin
// License     : See the LICENSE file
// Created     : May 19, 2013
// Last Update : May 19, 2013
//============================================================================


#ifndef __IBEX_LINEAR_RELAX_AFFINE2_H__
#define __IBEX_LINEAR_RELAX_AFFINE2_H__

#include "ibex_System.h"
#include "ibex_LinearRelax.h"
#include "ibex_Affine2.h"

#include <vector>

namespace ibex {

/** \ingroup numeric
 * \brief Affine-based linearization
 *
 * This class is an implementation of the ART algorithm
 * \author Jordan Ninin
 * \date May 2013
 */

class LinearRelaxAffine2 : public LinearRelax {

public:

	LinearRelaxAffine2 (const System& sys);

	~LinearRelaxAffine2 ();

	/** ART iteration.
  Linearize the system and performs 2n calls to Simplex in order to reduce
  the 2 bounds of each variable */
	int linearization( IntervalVector & box, LinearSolver *mysolver);

};

} // end namespace ibex

#endif /* __IBEX_LINEAR_RELAX_AFFINE2_H__ */

