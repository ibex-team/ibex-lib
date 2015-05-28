//============================================================================
//                                  I B E X                                   
// File        : ibex_InHC4Revise.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jul 12, 2012
// Last Update : Jul 12, 2012
//============================================================================

#include "ibex_InHC4Revise.h"
#include "ibex_Eval.h"

namespace ibex {

void InHC4Revise::ibwd(const Function& f, const Domain& y, IntervalVector& x) {

	for (int i=0; i<f.nb_nodes(); i++)
		f.node(i).deco.p->set_empty();

	if (f.eval_domain(x).is_empty()) { x.set_empty(); return; }

	*f.expr().deco.d = y;

	try {
		f.backward<InHC4Revise>(*this);

		f.read_arg_domains(x);

	} catch(EmptyBoxException&) {
		x.set_empty();
	}
}

void InHC4Revise::ibwd(const Function& f, const Domain& y, IntervalVector& x, const IntervalVector& xin) {

	Eval e;

	if (!xin.is_empty()) {
		e.eval(f,xin);

		assert(!f.expr().deco.d->is_empty());

		for (int i=0; i<f.nb_nodes(); i++)
			*f.node(i).deco.p = *f.node(i).deco.d;
	}
	else {
		for (int i=0; i<f.nb_nodes(); i++)
			f.node(i).deco.p->set_empty();
	}

	e.eval(f,x);

	assert(!f.expr().deco.d->is_empty());

	*f.expr().deco.d = y;

	try {

		f.backward<InHC4Revise>(*this);

		f.read_arg_domains(x);

	} catch(EmptyBoxException&) {
		x.set_empty();
	}
}

bool InHC4Revise::ibwd(const Function& f, const Domain& y, ExprLabel** x) {

	Eval e;

	// the box to be inflated is found
	// in the "p" field of the x labels:
	Array<Domain> argP(f.nb_arg());
	for (int i=0; i<f.nb_arg(); i++) {
		argP.set_ref(i,*x[i]->p);
	}

	if (!argP[0].is_empty()) { // if the first domain is empty, so they all are
		e.eval(f,argP);

		assert(!f.expr().deco.d->is_empty());

		for (int i=0; i<f.nb_nodes(); i++)
			*f.node(i).deco.p = *f.node(i).deco.d;
	}
	else {
		for (int i=0; i<f.nb_nodes(); i++)
			f.node(i).deco.p->set_empty();
	}

	e.eval(f,x);

	assert(!f.expr().deco.d->is_empty());

	*f.expr().deco.d = y;

	Array<Domain> argD(f.nb_arg());

	for (int i=0; i<f.nb_arg(); i++) {
		argD.set_ref(i,*(x[i]->d));
	}

	try {

		f.backward<InHC4Revise>(*this);

		f.read_arg_domains(argD);

	} catch(EmptyBoxException&) {
		// should we force argD to be the empty set here?
		// (probably no, as long as this function is called
		// inside InHC4Revise)
		return false;
	}

	return true;
}
} // end namespace ibex
