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
#include "ibex_AffineEval.h"

#include <vector>

namespace ibex {

/**
 * \ingroup numeric
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

	/**
	 * \biref  ART iteration.
	 *
	 *  Linearize the system and performs 2n calls to Simplex in order to reduce the 2 bounds of each variable
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
	 * \brief The system
	 */
	const System& sys;

	/**
	 * \brief Affine evaluator for the goal function (if any)
	 */
	AffineEval<AF_Default>* goal_af_evl;

	/**
	 * \brief Affine evaluators for the constraints functions
	 */
	AffineEval<AF_Default>** ctr_af_evl;
};

} // end namespace ibex

#endif /* __IBEX_LINEAR_RELAX_AFFINE2_H__ */

