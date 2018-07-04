/* ============================================================================
 * I B E X - ibex_SIP.h
 * ============================================================================
 * Copyright   : IMT Atlantique (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Sep 18, 2017
 * ---------------------------------------------------------------------------- */

#ifndef __IBEX_SIP_H__
#define __IBEX_SIP_H__

#include <vector>
#include <list>

#include "ibex_System.h"
#include "ibex_BitSet.h"

namespace ibex {

/**
 * \defgroup sip Semi-infinite Programming
 */

/**
 * \ingroup sip
 *
 * \brief Class represeting a SIP.
 */

class SIP {
public:

	/**
	 * Build a SIP instance.
	 *
	 * \param sys    - The system "min f(x) s.t. g(x,y)<=0"
	 * \param vars   - Symbols corresponding to x (variables)
	 * \param params - Symbols corresponding to y (parameters)
	 */
	SIP(System& sys, const Array<const ExprSymbol>& vars, const Array<const ExprSymbol>& params, const BitSet& is_param);

	virtual ~SIP();

	/**
	 * Does the constraint n°c uses argument n°K?
	 */
	bool used_arg(int c, int K) const {
		return nodes[c]->found(sys.ctrs[c].f.arg(K));
	}

	//=================================================================================================

	const System& sys;                             // original IBEX system
	const Array<const ExprSymbol>& vars;           // variable symbols ("x")
	const Array<const ExprSymbol>& params;         // parameter symbols ("y")
	const BitSet& is_param;                        // whether the ith symbol is a parameter
	const int n_arg;                               // number of variable symbols "x"
	const int p_arg;                               // number of parameter symbols "y"

	const VarSet varset;                           // which components in a box are parameter
	const int n;                                   // number of components corresponding to variables
	const int p;                                   // number of components corresponding to parameters
	const IntervalVector var_init_domain;          // domain of variables
	const IntervalVector param_init_domain;        // domain of parameters


	ExprSubNodes** nodes;
};

} // namespace ibex

#endif /* __IBEX_SIP_H__ */
