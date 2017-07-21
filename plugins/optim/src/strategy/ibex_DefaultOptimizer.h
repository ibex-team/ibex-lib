//============================================================================
//                                  I B E X                                   
// File        : ibex_DefaultOptimizer.h
// Author      : Gilles Chabert, Bertrand Neveu
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Aug 27, 2012
// Last Update : Jul 21, 2017
//============================================================================

#ifndef __IBEX_DEFAULT_OPTIMIZER_H__
#define __IBEX_DEFAULT_OPTIMIZER_H__

#include "ibex_Optimizer.h"
#include "ibex_CtcCompo.h"

namespace ibex {

/**
 * \ingroup optim
 *
 * \brief Default optimizer.
 */
class DefaultOptimizer : public Optimizer {
public:
	/**
	 * \brief Create a default optimizer.
	 *
	 * \param sys        - The system to optimize
	 * \param eps_x      - Stopping criterion for box splitting (absolute precision)
	 * \param rel_eps_f  - Relative precision on the objective
	 * \param abs_eps_f  - Absolute precision on the objective
	 */
    DefaultOptimizer(const System& sys,
    		double eps_x=Optimizer::default_prec,
    		double rel_eps_f=Optimizer::default_goal_rel_prec,
			double abs_eps_f=Optimizer::default_goal_abs_prec,
			double eps_h=Optimizer::default_equ_eps,
			bool rigor=false);

	/**
	 * \brief Delete *this.
	 */
    ~DefaultOptimizer();

private:

    /**
     * The contractor: HC4 + acid(HC4) + X-Newton
     */
	Ctc& ctc(const System& sys, const System& ext_sys, double prec);

	void* data; // keep track of data, for memory cleanup
};

} // end namespace ibex

#endif // __IBEX_DEFAULT_OPTIMIZER_H__
