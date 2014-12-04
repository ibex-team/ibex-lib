//============================================================================
//                                  I B E X                                   
// File        : ibex_UserFriendlyOptimizer.h
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Dec 4, 2014
//============================================================================

#ifndef __IBEX_USER_FRIENDLY_OPTIMIZER_H__
#define __IBEX_USER_FRIENDLY_OPTIMIZER_H__

#include "ibex_Optimizer.h"

namespace ibex {

/**
 * \ingroup strategy
 *
 * \brief user-friendly configurable optimizer.
 *
 * All options are set with simple strings.
 */
class UserFriendlyOptimizer : public Optimizer {
public:

	/**
	 * \brief Create an optimizer with all arguments passed as strings.
	 *
	 * \param args[0] - name of the file where the system is stored
	 * \param args[1] - contraction strategy: in { "hc4" ; "acidhc4" ; "3bcidhc4" }
	 * \param args[2] - linear relaxation:    in { "art" ; "compo" ; "xn" }
	 * \param args[3] - bisection:            in { "roundrobin" ; "largestfirst" ; "smearsum" ; "smearmax" ; "smearsumrel" ;  "smearmaxrel" }
	 * \param args[4] - absolution precision on x
	 * \param args[5] - relative goal precision
	 * \param args[6] - absolute goal precision
	 * \param args[7] - sample size
	 * \param args[8] - time limit
	 * \param args[9] - thickness of equations (non-rigor mode)
	 */
	UserFriendlyOptimizer(const char* args[10]);


	/**
	 * \brief Delete this.
	 */
	virtual ~UserFriendlyOptimizer();

	/**
	 * \brief Run the optimization on the system initial box.
	 *
	 * \see ibex::Optimizer::optimize(const IntervalVector&, double);
	 */
	Status optimize();

private:

	void* data;
};

} // namespace ibex

#endif // __IBEX_USER_FRIENDLY_OPTIMIZER_H__
