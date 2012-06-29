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
#include "ibex_NumConstraint.h"

namespace ibex {

/**
 * \ingroup contractor
 * \brief HC4Revise contractor.
 *
 */
class CtcProj: public Ctc {

public:
	/**
	 * \brief HC4Revise on f(x)=0 or f(x)<=0.
	 *
	 * \param equality If true: f(x)=0, otherwise: f(x)<=0.
	 */
	CtcProj(Function& f, bool equality=true);

	CtcProj(const NumConstraint& ctr);

	/**
	 * \brief Contract the box.
	 */
	virtual void contract(IntervalVector& box);

	/**
	 * The constraint.
	 */
	const NumConstraint ctr;

	/**
	 * HC4Revise algorithm.
	 */
	HC4Revise hc4r;
private:

};

} // namespace ibex
#endif // __IBEX_CTC_HC4_REVISE_H__
