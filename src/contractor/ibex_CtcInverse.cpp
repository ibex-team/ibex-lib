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
	id = new Function(y,y);
	//id = new Function("x","x");
}

CtcInverse::~CtcInverse() {
	delete id;
}

void CtcInverse::contract(IntervalVector& box) {

	assert(box.size()==f.nb_var());

	Domain fx=f.eval_domain(box);
	y.init(Interval::ALL_REALS);
	id->backward(fx,y);

	try {
	//	BitSet flags(BitSet::empty(Ctc::NB_OUTPUT_FLAGS));
	//	BitSet impact(BitSet::all(nb_var));
	//	c.contract(y,impact,flags);
		c.contract(y);

	} catch(EmptyBoxException& e) {
		box.set_empty();
		throw e;
	}
	//	if (c.is_inactive()) set_flag(INACTIVE);
	fx=id->eval_domain(y);
	f.backward(fx,box);
}

} // end namespace ibex
