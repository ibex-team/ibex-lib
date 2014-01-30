//============================================================================
//                                  I B E X                                   
// File        : ibex_CtcIn.cpp
// Author      : Jordan Ninin
// License     : See the LICENSE file
// Created     : Jan 29, 2014
// Last Update : Jan 29, 2014
//============================================================================


#include "ibex_CtcIn.h"
#include "ibex_HC4Revise.h"

namespace ibex {

CtcIn::CtcIn(Function& f, const Domain& y) :
		Ctc(f.nb_var()), _f(f), _d(_f.expr().dim) {
	assert(_f.expr().dim==y.dim);
	_d = y;
}

CtcIn::CtcIn(Function& f, const Interval& y) :
		Ctc(f.nb_var()), _f(f), _d(Dim()) {
	assert(_f.expr().dim==_d.dim);
	_d.i() = y;
}

CtcIn::CtcIn(Function& f, const IntervalVector& y) :
		Ctc(f.nb_var()), _f(f), _d(_f.expr().dim) {
	assert(_f.expr().dim.is_vector());
	_d.v() = y;
}

CtcIn::CtcIn(Function& f, const IntervalMatrix& y) :
		Ctc(f.nb_var()), _f(f), _d(_f.expr().dim) {
	assert(_f.expr().dim==Dim(1,y.nb_rows(),y.nb_cols()));
	_d.m() = y;
}



void CtcIn::contract(IntervalVector& box) {

	// it's simpler here to use direct computation, but
	// we could also have used CtcFwdBwd

	try {
		HC4Revise().proj(_f,_d,box);
	} catch (EmptyBoxException& e) {
		box.set_empty();
		throw e;
	}
}

} // end namespace ibex
