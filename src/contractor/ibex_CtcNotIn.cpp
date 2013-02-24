//============================================================================
//                                  I B E X                                   
// File        : ibex_CtcNotIn.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Feb 24, 2013
// Last Update : Feb 24, 2013
//============================================================================

#include "ibex_CtcNotIn.h"
#include "ibex_HC4Revise.h"

namespace ibex {

CtcNotIn::CtcNotIn(Function& f, const Interval& y) :
		Ctc(f.nb_var()), f(f), d1(Dim()), d2(Dim()) {

	d1.i()=Interval(NEG_INFINITY,y.lb());
	d2.i()=Interval(y.ub(),POS_INFINITY);
}


void CtcNotIn::contract(IntervalVector& box) {

	// it's simpler here to use direct computation, but
	// we could also have used CtCunion of two CtcFwdBwd

	try {
		IntervalVector savebox(box);

		HC4Revise().proj(f,d1,box);
		HC4Revise().proj(f,d2,savebox);
		box |= savebox;
	} catch (EmptyBoxException& e) {
		box.set_empty();
		throw e;
	}
}

} // end namespace ibex
