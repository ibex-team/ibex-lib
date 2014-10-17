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

CtcNotIn::CtcNotIn(Function& f, const Interval& y) : Ctc(f.nb_var()), f(f), d1(f.expr().dim), d2(f.expr().dim) {
	assert(f.expr().dim.is_scalar());
	d1.i()=Interval(NEG_INFINITY,y.lb());
	d2.i()=Interval(y.ub(),POS_INFINITY);
}

CtcNotIn::CtcNotIn(Function& f, const IntervalVector& y) : Ctc(f.nb_var()), f(f), d1(f.expr().dim), d2(f.expr().dim) {
	assert(f.expr().dim.is_vector());
	d1.i()=Interval(NEG_INFINITY,y.lb());
	d2.i()=Interval(y.ub(),POS_INFINITY);
}

CtcNotIn::CtcNotIn(Function& f, const IntervalMatrix& y) : Ctc(f.nb_var()), f(f), d1(f.expr().dim), d2(f.expr().dim) {
	assert(f.expr().dim.is_matrix());
	d1.i()=Interval(NEG_INFINITY,y.lb());
	d2.i()=Interval(y.ub(),POS_INFINITY);
}


void CtcNotIn::contract(IntervalVector& box) {

	// it's simpler here to use direct computation, but
	// we could also have used CtCunion of two CtcFwdBwd

	IntervalVector savebox(box);
	try {
		HC4Revise().proj(f,d1,box);
	} catch (EmptyBoxException& ) {box.set_empty(); }
	try {
		HC4Revise().proj(f,d2,savebox);
	} catch (EmptyBoxException& ) {savebox.set_empty(); }

	box |= savebox;
	if (box.is_empty()) throw EmptyBoxException();

}

} // end namespace ibex
