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
#include "ibex_CtcEmpty.h"
#include "ibex_CtcFwdBwd.h"
#include "ibex_CtcUnion.h"

namespace ibex {

CtcNotIn::CtcNotIn(const Function& f, const Domain& y) : Ctc(f.nb_var()), f(f) {
	switch(y.dim.type()) {
	case Dim::SCALAR:       init(y.i()); break;
	case Dim::ROW_VECTOR:
	case Dim::COL_VECTOR:   init (y.v()); break;
	case Dim::MATRIX:       init(y.m()); break;
	}
}

CtcNotIn::CtcNotIn(const Function& f, const Interval& y) : Ctc(f.nb_var()), f(f) {
	init(y);
}

CtcNotIn::CtcNotIn(const Function& f, const IntervalVector& y) : Ctc(f.nb_var()), f(f) {
	init(y);
}

CtcNotIn::CtcNotIn(const Function& f, const IntervalMatrix& y) : Ctc(f.nb_var()), f(f) {
	init(y);
}

void CtcNotIn::init(const Interval& y) {
	assert(f.expr().dim.is_scalar());

	Interval c1,c2;
	y.complementary(c1,c2);

	if (c1.is_empty()) {
		diff_size=0;
		_union = new CtcEmpty(f.nb_var());
	} else {
		if (c2.is_empty()) {
			diff_size=1;
			_union = new CtcFwdBwd(f,c1);
		} else {
			diff_size=2;
			_union = new CtcUnion(*new CtcFwdBwd(f,c1), *new CtcFwdBwd(f,c2));
		}
	}
}

void CtcNotIn::init(const IntervalVector& y) {

	assert(f.expr().dim.is_vector());

	IntervalVector* result;
	diff_size = y.complementary(result);

	if (diff_size==0) {
		_union = new CtcEmpty(f.nb_var());
	} else {
		if (diff_size==1) {
			_union = new CtcFwdBwd(f,result[0]);
		} else {
			Array<Ctc> ctc(diff_size);
			for (int i=0; i<diff_size; i++) {
				ctc.set_ref(i,*new CtcFwdBwd(f,result[i]));
			}
			_union = new CtcUnion(ctc);
		}
	}
	delete[] result;
}

void CtcNotIn::init(const IntervalMatrix& y) {
	assert(f.expr().dim.is_matrix());
	_union = NULL;
	diff_size=0;
	// TODO: requires third-order tensor "diff"
	not_implemented("CtcNotIn with matrix-valued functions");
}


CtcNotIn::~CtcNotIn() {
	if (diff_size>1) {
		for (int i=0; i<diff_size; i++)
			delete &((CtcUnion*) _union)->list[i];
	}
	delete _union;
}

void CtcNotIn::contract(IntervalVector& box) {
	_union->contract(box);
}

} // end namespace ibex
