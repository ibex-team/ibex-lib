/* ============================================================================
 * I B E X - HC4 Revise (forward-backward algorithm)
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Feb 27, 2012
 * ---------------------------------------------------------------------------- */

#ifndef __IBEX_CTC_HC4_REVISE_H__
#define __IBEX_CTC_HC4_REVISE_H__

#include "ibex_Ctc.h"
#include "ibex_HC4Revise.h"

namespace ibex {

/**
 * \ingroup level2
 * \brief HC4Revise contractor.
 *
 */
class CtcHC4Revise: public Ctc {

public:

	CtcHC4Revise(const NumConstraint& ctr);

	virtual void contract(IntervalVector& box);

	virtual bool can_contract(int v) const;

	const NumConstraint& ctr;

	HC4ReviseAlgo hc4r;
private:

};

} // namespace ibex
#endif // __IBEX_CTC_HC4_REVISE_H__
