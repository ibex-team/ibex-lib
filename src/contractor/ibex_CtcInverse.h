//============================================================================
//                                  I B E X                                   
// File        : ibex_CtcInverse.h
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jun 5, 2013
// Last Update : Jun 5, 2013
//============================================================================

#ifndef __IBEX_CTC_INVERSE_H__
#define __IBEX_CTC_INVERSE_H__

#include "ibex_Ctc.h"
#include "ibex_Function.h"

namespace ibex {

class CtcInverse : public Ctc {
public:
	CtcInverse(Ctc& c, Function& f);
	~CtcInverse();

	/**
	 * \brief Contract a box.
	 */
	void contract(IntervalVector& box);

	virtual void contract(IntervalVector& box, ContractContext& context);

	Ctc& c;
	Function& f;

private:
	Function *id;
	IntervalVector y;
};

} // end namespace ibex
#endif // __IBEX_CTC_INVERSE_H__
