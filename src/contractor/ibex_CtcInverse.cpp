//============================================================================
//                                  I B E X                                   
// File        : ibex_CtcInverse.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jun 5, 2013
// Last Update : Jun 5, 2013
//============================================================================

#include "ibex_CtcInverse.h"

namespace ibex {

CtcInverse::CtcInverse(Ctc& c, Function& f) : Ctc(f.nb_var()), c(c), f(f), y(f.image_dim()) {
	const ExprSymbol& y=ExprSymbol::new_(f.expr().dim);
	id.init(y,y);
}

void CtcInverse::contract(IntervalVector& box) {
	Domain fx=f.eval_domain(box);
	y.init(Interval::ALL_REALS);
	id.backward(fx,y);

	try {
		c.contract(y);
	} catch(EmptyBoxException& e) {
		box.set_empty();
		throw e;
	}

	fx=id.eval_domain(y);
	f.backward(fx,box);
}

} // end namespace ibex
