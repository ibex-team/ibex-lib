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

void InHC4Revise::iproj(const Function& f, const Domain& y, IntervalVector& x) {

	for (int i=0; i<f.nb_nodes(); i++)
		f.node(i).deco.p->set_empty();

	Eval().eval(f,x);

	*f.expr().deco.d = y;

	try {
		f.backward<InHC4Revise>(*this);

		if (f.all_symbols_scalar())
			for (int i=0; i<f.nb_symbols(); i++) {
				x[i]=f.symbol_domains[i].i();
			}
		else
			load(x,f.symbol_domains);

	} catch(EmptyBoxException&) {
		x.set_empty();
	}
}

void InHC4Revise::iproj(const Function& f, const Domain& y, IntervalVector& x, const IntervalVector& xin) {

	Eval e;

	if (!xin.is_empty()) {
		e.eval(f,xin);
		for (int i=0; i<f.nb_nodes(); i++)
			*f.node(i).deco.p = *f.node(i).deco.d;
	}
	else {
		for (int i=0; i<f.nb_nodes(); i++)
			f.node(i).deco.p->set_empty();
	}

	e.eval(f,x);

	*f.expr().deco.d = y;

	try {

		f.backward<InHC4Revise>(*this);

		if (f.all_symbols_scalar())
			for (int i=0; i<f.nb_symbols(); i++) {
				x[i]=f.symbol_domains[i].i();
			}
		else
			load(x,f.symbol_domains);

	} catch(EmptyBoxException&) {
		x.set_empty();
	}
}

} // end namespace ibex
