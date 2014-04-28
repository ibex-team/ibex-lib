//============================================================================
//                                  I B E X                                   
// File        : ibex_CtcIn.h
// Author      : Jordan Ninin
// License     : See the LICENSE file
// Created     : Jan 29, 2014
// Last Update : Jan 29, 2014
//============================================================================

#ifndef __IBEX_CTC_IN_H__
#define __IBEX_CTC_IN_H__

#include "ibex_Ctc.h"
#include "ibex_Function.h"

namespace ibex {

/**
 * \ingroup contractor wrt f(x) not-in [y]
 *
 */
class CtcIn : public Ctc {
public:
	/**
	 * \brief Build the contractor for "f(x) in [y]".
	 */
	CtcIn(Function& f, const Domain& y);
	CtcIn(Function& f, const Interval& y);
	CtcIn(Function& f, const IntervalVector& y);
	CtcIn(Function& f, const IntervalMatrix& y);

	/**
	 * \brief Contract the box.
	 */
	virtual void contract(IntervalVector& box);

protected:
	Function& _f;
	Domain _d; //
};

} // end namespace ibex
#endif // __IBEX_CTC_IN_H__
