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
	CtcInteger(int nb_var, const BitSet& is_int);

	/**
	 * \brief Delete this.
	 */
	~CtcInteger();

	/**
	 * \brief Contract a box.
	 */
	void contract(IntervalVector& box);

	virtual void contract(IntervalVector& box, ContractContext& context);

//	virtual void contract(IntervalVector& box, const BitSet& impact);

	virtual bool idempotent();

	/**
	 * The mask.
	 */
	BitSet is_int;
};

} // end namespace ibex
#endif // __IBEX_CTC_INTEGER_H__
