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

#ifndef __IBEX_HC4REVISE_H__
#define __IBEX_HC4REVISE_H__

#include "ibex_Contractor.h"
#include "ibex_HC4ReviseAlgo.h"

namespace ibex {

/**
 * \ingroup level2
 * \brief HC4Revise contractor.
 *
 */
class HC4Revise: public ibex::Contractor {

public:

	HC4Revise(const NumConstraint& ctr);

	virtual void contract(IntervalVector& box);

	virtual bool can_contract(int v);

	const NumConstraint& ctr;

	HC4ReviseAlgo hc4r;
private:

};

} // namespace ibex
#endif // IBEX_HC4REVISE_H_
