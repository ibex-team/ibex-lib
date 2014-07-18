//============================================================================
//                                  I B E X                                   
// File        : ibex_StcEqual.h
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jul 15, 2014
//============================================================================

#ifndef __IBEX_STC_EQUAL_H__
#define __IBEX_STC_EQUAL_H__

#include "ibex_Stc.h"
#include "ibex_Ctc.h"

namespace ibex {

class StcEqual : public Stc {
public:
	StcEqual(Ctc& c_in, Ctc& c_out, double eps);

	void contract(Set& set);

	virtual ~StcEqual();

	Ctc& c_in;
	Ctc& c_out;
	double eps;
};

} // namespace ibex

#endif // __IBEX_STC_EQUAL_H__
