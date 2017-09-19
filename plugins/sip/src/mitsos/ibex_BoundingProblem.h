/* ============================================================================
 * I B E X - ibex_BoundingProblem.h
 * ============================================================================
 * Copyright   : IMT Atlantique (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Sep 18, 2017
 * ---------------------------------------------------------------------------- */

#ifndef __IBEX_BOUNDING_PROBLEM_H__
#define __IBEX_BOUNDING_PROBLEM_H__

#include "ibex_SystemFactory.h"
#include "ibex_MitsosSIP.h"

namespace ibex {

/**
 * \ingroup sip
 *
 * \brief Factory for either the LBD or UBD problem
 */
class BoundingProblemFactory  : public SystemFactory {
public:
	/**
		 * \param sip   - The SIP problem
		 * \param eps_g - if eps_g>0 ---> UBD
		 *                   eps_g=0 ---> LBD
		 */
	BoundingProblemFactory(const MitsosSIP& sip, double eps_g);

	virtual ~BoundingProblemFactory();

	Array<const ExprSymbol> new_vars;
};

} // namespace ibex

#endif /* __IBEX_BOUNDING_PROBLEM_H__ */
