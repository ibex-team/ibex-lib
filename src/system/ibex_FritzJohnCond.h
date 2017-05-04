//============================================================================
//                                  I B E X                                   
// File        : ibex_FritzJohnCond.h
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : May 31, 2013
// Last Update : May 31, 2013
//============================================================================

#ifndef __IBEX_FRITZ_JOHN_COND_H__
#define __IBEX_FRITZ_JOHN_COND_H__

#include "ibex_System.h"

namespace ibex {

/**
 * \ingroup system
 * \brief System of Fritz-John conditions
 *
 * The system contains n+M+R+K+1 variables ( "+1" because of the multiplier of the objective)
 *
 */
class FritzJohnCond : public System {

public:

	FritzJohnCond(const System& sys);

	/** Number of basic variables */
	const int n;

	/** Number of inequalities */
	const int M;

	/** Number of equalities */
	const int R;

	/** Number of bound constraints */
	const int K;

};

} // end namespace ibex

#endif // __IBEX_FRITZ_JOHN_COND_H__
