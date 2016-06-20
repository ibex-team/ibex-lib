//============================================================================
//                                  I B E X                                   
// File        : ibex_InHC4Revise.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jul 12, 2012
// Last Update : Jul 12, 2012
//============================================================================

#include "ibex_Function.h"
#include "ibex_InHC4Revise.h"

namespace ibex {

InHC4Revise::InHC4Revise(Eval& e) : f(e.f), eval(e), d(e.d), p_eval(f), p(p_eval.d) {

}

void InHC4Revise::iproj(const Domain& y, IntervalVector& x) {
	iproj(y,x,IntervalVector::empty(x.size()));
}

void InHC4Revise::iproj(const Domain& y, IntervalVector& x, const IntervalVector& xin) {

	if (!xin.is_empty()) {
		p_eval.eval(xin);

		assert(!p.top->is_empty());
	}
	else {
		for (int i=0; i<f.expr().size; i++)
			p[i].set_empty();
	}

	eval.eval(x);

	if (d.top->is_empty()) {
		assert(xin.is_empty());
		x.set_empty();
		return;
	}

	*d.top = y;

	try {
		f.backward<InHC4Revise>(*this);

		d.read_arg_domains(x);

	} catch(EmptyBoxException&) {
		assert(xin.is_empty());
		x.set_empty();
	}
}

void InHC4Revise::iproj(const Domain& y, Array<Domain>& x, const Array<Domain>& argP) {
	if (!argP[0].is_empty()) { // if the first domain is empty, so they all are
		p_eval.eval(argP);
	} else {
		for (int i=0; i<f.expr().size; i++)
			p[i].set_empty();
	}

	eval.eval(x);

	assert(argP[0].is_empty() || !d.top->is_empty());

	*d.top = y;

	// may throw EmptyBoxException&) {
	f.backward<InHC4Revise>(*this);

	d.read_arg_domains(x);
}

void InHC4Revise::idx_cp_bwd(int x, int y) {
	assert(dynamic_cast<const ExprIndex*> (&f.node(y)));

	const ExprIndex& e = (const ExprIndex&) f.node(y);

	d[x].put(e.index.first_row(), e.index.first_col(), d[y]);
}

void InHC4Revise::apply_bwd(int* x, int y) {

	assert(dynamic_cast<const ExprApply*> (&f.node(y)));

	const ExprApply& a = (const ExprApply&) f.node(y);

	assert(&a.func!=&f); // recursive calls not allowed

	Array<Domain> d2(a.func.nb_arg());
	Array<Domain> p2(a.func.nb_arg());

	for (int i=0; i<a.func.nb_arg(); i++) {
		d2.set_ref(i,d[x[i]]);
		p2.set_ref(i,p[x[i]]);
	}

	// if next instruction throws an EmptyBoxException,
	// it will be caught by iproj(...,IntervalVector& x).
	// (it is a protected function, not called outside of the class
	// so there is no risk)
	a.func.inhc4revise().iproj(d[y],d2,p2);
}

} // end namespace ibex
