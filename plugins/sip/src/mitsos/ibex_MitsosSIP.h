/* ============================================================================
 * I B E X - ibex_MitsosSIP.h
 * ============================================================================
 * Copyright   : IMT Atlantique (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Sep 18, 2017
 * ---------------------------------------------------------------------------- */

#ifndef __IBEX_MITSOS_SIP_H__
#define __IBEX_MITSOS_SIP_H__

#include "ibex_SIP.h"

namespace ibex {

/**
 * \ingroup sip
 */
class MitsosSIP : public SIP {
public:

	/**
	 * Build a SIP instance.
	 *
	 * \param sys    - The system "min f(x) s.t. g(x,y)<=0"
	 * \param vars   - Symbols corresponding to x (variables)
	 * \param params - Symbols corresponding to y (parameters)
	 */
	MitsosSIP(System& sys, const std::vector<const ExprSymbol*>& vars,  const std::vector<const ExprSymbol*>& params, const BitSet& is_param);

	virtual ~MitsosSIP();

	// A temporary domain that will be filled with sampled values.
	// These values change with duplications of the same constraint
	mutable Array<Domain> p_domain;

	mutable std::vector<double>* LBD_samples;      // sampled values for each parameter, for the LBD problem
	mutable std::vector<double>* UBD_samples;      // sampled values for each parameter, for the UBD problem
};

} // namespace ibex

#endif /* __IBEX_MITSOS_SIP_H__ */
