//============================================================================
//                                  I B E X                                   
// File        : ibex_DefaultOptimizer.h
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Aug 27, 2012
// Last Update : Aug 27, 2012
//============================================================================

#ifndef __IBEX_DEFAULT_OPTIMIZER_H__
#define __IBEX_DEFAULT_OPTIMIZER_H__

#include "ibex_Optimizer.h"

namespace ibex {

/**
 * \brief Default optimizer.
 */
class DefaultOptimizer : public Optimizer {
public:
	/**
	 * \brief Create a default optimizer.
	 */
	DefaultOptimizer(System& sys);

	/**
	 * \brief Delete *this.
	 */
	~DefaultOptimizer();

private:
	// -------- information stored for cleanup ----------
	Ctc* __ctc;
	Bsc* __bsc;
	System* __sys;
};

} // end namespace ibex
#endif // __IBEX_DEFAULT_OPTIMIZER_H__
