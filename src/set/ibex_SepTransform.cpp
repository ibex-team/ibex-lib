//============================================================================
//                                  I B E X                                   
// File        : ibex_SepTransform.cpp
// Author      : Benoit Desrochers, Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Mar 3, 2015
// Last Update : Mar 3, 2015
//============================================================================

#include "ibex_SepTransform.h"

namespace ibex {

SepTransform::SepTransform(Sep& s, Function& t, Function& tinv) : s(s), t(t), tinv(tinv), yin(t.image_dim()), yout(t.image_dim()) {
}

SepTransform::~SepTransform() {
	
}
void SepTransform::separate(IntervalVector& xin, IntervalVector& xout){

	assert(xin.size()==t.nb_var() && xout.size() == t.nb_var());


	xin &= xout;
	yin = t.eval_vector(xin);
	yout = yin;

	s.separate(yin, yout);
	
	if( yin.is_empty() )
		xin.set_empty();
	else
		xin &= tinv.eval_vector(yin);

	if( yout.is_empty() )
		xout.set_empty();
	else
		xout &= tinv.eval_vector(yout);
		
}

} // end namespace ibex
