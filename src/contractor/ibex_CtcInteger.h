//============================================================================
//                                  I B E X                                   
// File        : ibex_CtcInteger.h
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jul 20, 2012
// Last Update : Jul 20, 2012
//============================================================================

#ifndef __IBEX_CTC_INTEGER_H__
#define __IBEX_CTC_INTEGER_H__

#include "ibex_Ctc.h"

namespace ibex {

/**
 * \ingroup contractor
 *
 * \brief Integral contractor
 *
 * Contracts the domain of integral variables to their largest
 * integer subintervals.
 */
class CtcInteger : public Ctc {
public:
	/**
	 * \param is_int A \a nb-var sized mask with is_int[i] <=> the ith variable is integer.
	 */
	CtcInteger(const BoolMask& is_int);

	/**
	 * \brief Delete this.
	 */
	~CtcInteger();

	virtual void contract(IntervalVector& box);

	virtual void contract(IntervalVector& box, const BoolMask& impact);

	virtual bool idempotent();

	/**
	 * The mask.
	 */
	BoolMask is_int;
};

} // end namespace ibex
#endif // __IBEX_CTC_INTEGER_H__
