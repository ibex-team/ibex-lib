/* ============================================================================
 * I B E X - ibex_LLP_Factory.h
 * ============================================================================
 * Copyright   : IMT Atlantique (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Sep 20, 2017
 * ---------------------------------------------------------------------------- */

#ifndef __IBEX_LLP_FACTORY_H__
#define __IBEX_LLP_FACTORY_H__

#include "ibex_SystemFactory.h"
#include "ibex_MitsosSIP.h"

namespace ibex {

/**
 * \ingroup sip
 *
 * \brief Factory for both LLP problems
 */
class LLP_Factory : public SystemFactory {
public:

	class ParameterFreeConstraint { };

	/**
	 * Build the LLP problem for the constraint c.
	 *
	 * \param sip  - The SIP problem
	 * \param c    - The constraint number (in the original system)
	 * \param xopt - Either x_LBD or x_UBD
	 */
	LLP_Factory(const MitsosSIP& sip, int c, const Vector& xopt);

	virtual ~LLP_Factory();

	Array<const ExprSymbol> new_vars_y;
	// The domain filled with x*, the optimum
	// at a given step
	Array<Domain> x_domain;

	// Tells which parameter is a variable in the LLP!
	BitSet param_LLP_var;
};

} /* namespace ibex */

#endif /* __IBEX_LLP_FACTORY_H__ */
