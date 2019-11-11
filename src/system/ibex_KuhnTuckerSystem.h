//============================================================================
//                                  I B E X                                   
// File        : ibex_KuhnTuckerSystem.h
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : May 31, 2013
// Last Update : Jul 31, 2019
//============================================================================

#ifndef __IBEX_KUHN_TUCKER_SYSTEM_H__
#define __IBEX_KUHN_TUCKER_SYSTEM_H__

#include "ibex_System.h"

namespace ibex {

/**
 * \ingroup system
 * \brief System of KKT conditions
 *
 * The system contains n+M+R+K+1 variables ( "+1" because of the multiplier of the objective)
 *
 */
class KuhnTuckerSystem : public System {

public:

	KuhnTuckerSystem(const System& sys, bool copy_ineq);

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

#endif // __IBEX_KUHN_TUCKER_SYSTEM_H__
