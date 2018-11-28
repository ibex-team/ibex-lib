//============================================================================
//                                  I B E X
// File        : ibex_SepInverse.cpp
// Author      : Benoit Desrochers, Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Mar 3, 2015
// Last Update : Mar 3, 2015
//============================================================================

#include "ibex_SepInverse.h"

namespace ibex {

SepInverse::SepInverse(Sep& s, Function& f) : Sep(f.nb_var()), s(s), f(f), yin(f.image_dim()), yout(f.image_dim()) {
	const ExprSymbol& y=ExprSymbol::new_(f.expr().dim);
	id = new Function(y,y);
}

SepInverse::~SepInverse() {
	delete id;
}
void SepInverse::separate(IntervalVector& xin, IntervalVector& xout){

	assert(xin.size()==f.nb_var() && xout.size() == f.nb_var());


	xin &= xout;
	Domain tmp=f.eval_domain(xin);
	yin.init(Interval::ALL_REALS);
	yout.init(Interval::ALL_REALS);
	id->backward(tmp, yin);
	id->backward(tmp, yout);

	s.separate(yin, yout);

	if( yin.is_empty())
		xin.set_empty();
	else {
		tmp = id->eval_domain(yin);
		f.backward(tmp, xin);
	}

	if( yout.is_empty())
		xout.set_empty();
	else {
		tmp = id->eval_domain(yout);
		f.backward(tmp, xout);
	}
}

} // end namespace ibex
