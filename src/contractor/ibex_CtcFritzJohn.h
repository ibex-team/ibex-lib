//============================================================================
//                                  I B E X                                   
// File        : ibex_CtcFritzJohn.h
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Apr 13, 2013
// Last Update : Apr 13, 2013
//============================================================================

#ifndef __IBEX_CTC_FRITZ_JOHN_H__
#define __IBEX_CTC_FRITZ_JOHN_H__

#include "ibex_Ctc.h"
#include "ibex_System.h"

namespace ibex {

/**
 * \brief Contractor based on Fritz-John conditions.
 *
 */
class CtcFritzJohn : public Ctc {
public:
	/**
	 * \brief Build the contarctor for an optimization problem.
	 *
	 */
	CtcFritzJohn(const System& sys);

	~CtcFritzJohn();

	virtual void contract(IntervalVector& box);

//	Function* dg;
//	Function df;
	System fritz;
	const int m; // number of constraints
};

} // end namespace ibex
#endif // __IBEX_CTC_FRITZ_JOHN_H__
