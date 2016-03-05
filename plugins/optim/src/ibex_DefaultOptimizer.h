//============================================================================
//                                  I B E X                                   
// File        : ibex_DefaultOptimizer.h
// Author      : Gilles Chabert, Bertrand Neveu
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Aug 27, 2012
// Last Update : Jul 06, 2014
//============================================================================

#ifndef __IBEX_DEFAULT_OPTIMIZER_H__
#define __IBEX_DEFAULT_OPTIMIZER_H__

#include "ibex_Optimizer.h"
#include "ibex_CtcCompo.h"

namespace ibex {

/**
 * \ingroup strategy
 * \brief Default optimizer.
 */
class DefaultOptimizer : public Optimizer {
public:
	/**
	 * \brief Create a default optimizer.
	 *
	 * \param sys       - The system to optimize
	 * \param prec      - Stopping criterion for box splitting (absolute precision)
	 * \param goal_prec - Stopping criterion for the objective (relative precision)
	 */
    DefaultOptimizer(System& sys, double prec, double goal_prec);

	/**
	 * \brief Delete *this.
	 */
    ~DefaultOptimizer();

private:
    /**
     * The contractor: hc4 + acid(hc4) + xnewton
     */
	Ctc&  ctc(System& sys, System& ext_sys,double prec);

	//	std::vector<CtcXNewton::corner_point>* default_corners ();

	void* data;
};

} // end namespace ibex

#endif // __IBEX_DEFAULT_OPTIMIZER_H__
