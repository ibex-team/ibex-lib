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

namespace ibex {

CtcNotIn::CtcNotIn(Function& f, const Interval& y) : Ctc(f.nb_var()), f(f), d1(f.expr().dim), d2(f.expr().dim), hc4r() {
	assert(f.expr().dim.is_scalar());
	d1.i()=Interval(NEG_INFINITY,y.lb());
	d2.i()=Interval(y.ub(),POS_INFINITY);
}

CtcNotIn::CtcNotIn(Function& f, const IntervalVector& y) : Ctc(f.nb_var()), f(f), d1(f.expr().dim), d2(f.expr().dim), hc4r() {
	assert(f.expr().dim.is_vector());
	d1.v()=y.lb()+IntervalVector(y.size(),Interval::NEG_REALS);
	d2.v()=y.ub()+IntervalVector(y.size(),Interval::POS_REALS);
}

CtcNotIn::CtcNotIn(Function& f, const IntervalMatrix& y) : Ctc(f.nb_var()), f(f), d1(f.expr().dim), d2(f.expr().dim), hc4r() {
	assert(f.expr().dim.is_matrix());
	d1.m()=y.lb()+IntervalMatrix(y.nb_rows(),y.nb_cols(),Interval::NEG_REALS);
	d2.m()=y.ub()+IntervalMatrix(y.nb_rows(),y.nb_cols(),Interval::POS_REALS);
}


void CtcNotIn::contract(IntervalVector& box) {

	// it's simpler here to use direct computation, but
	// we could also have used CtCunion of two CtcFwdBwd

	IntervalVector savebox(box);
	bool is_inactive=false;
	try {
		if (hc4r.proj(f,d1,box)) {
			set_flag(INACTIVE); // TODO: incorrect in general
			set_flag(FIXPOINT); // TODO: incorrect if multiple occurrences
			is_inactive =true;

		}
	} catch (EmptyBoxException& ) {box.set_empty(); }

	if (!is_inactive) {
		try {
			if (hc4r.proj(f,d2,savebox)){
				set_flag(INACTIVE); // TODO: incorrect in general
				set_flag(FIXPOINT); // TODO: incorrect if multiple occurrences
				is_inactive =true;
			}
		} catch (EmptyBoxException& ) {savebox.set_empty(); }

		box |= savebox;
		if (box.is_empty()) throw EmptyBoxException();
	}

}

} // end namespace ibex
