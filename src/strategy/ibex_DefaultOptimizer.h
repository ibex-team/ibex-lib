//============================================================================
//                                  I B E X                                   
// File        : ibex_DefaultOptimizer.h
// Author      : Gilles Chabert, Bertrand Neveu
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Aug 27, 2012
// Last Update : Sep 28, 2012
//============================================================================

#ifndef __IBEX_DEFAULT_OPTIMIZER_H__
#define __IBEX_DEFAULT_OPTIMIZER_H__

#include "ibex_Optimizer.h"
#include "ibex_CtcCompo.h"
#include "ibex_XNewton.h"

namespace ibex {

/**
 * \brief Default optimizer.
 */
class DefaultOptimizer : public Optimizer {
public:
	/**
	 * \brief Create a default optimizer.
	 */
    DefaultOptimizer(System& sys, System & ext_sys, double prec, double goal_prec);

	/**
	 * \brief Delete *this.
	 * \param sys The system to optimize
	 * \param ext_sys The extended_system  (the objective is added in the system as the last variable and the first constraint 
            and is used for contraction and bisection)
	 * \param  the precision : stopping criterion for box splitting and for the objective (used as absolute and relative precisions)
	 */
    ~DefaultOptimizer();

private:
	// -------- information stored for cleanup ----------
	CtcCompo* __ctc;
	Bsc* __bsc;
	Array<Ctc>*  contractor_list (System& sys, System& ext_sys,double prec);
	std::vector<X_Newton::corner_point>* default_corners ();
};



} // end namespace ibex
#endif // __IBEX_DEFAULT_OPTIMIZER_H__
