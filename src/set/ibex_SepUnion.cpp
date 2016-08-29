//============================================================================
//                                  I B E X
// File        : ibex_SepUnion.cpp
// Author      : Benoit Desrochers
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : May 14, 2012
// Last Update : May 14, 2012
//============================================================================

#include "ibex_SepUnion.h"
#include <cassert>

namespace ibex {

SepUnion::SepUnion(const Array<Sep>& list) : Sep(list[0].nb_var), list(list) {

}

SepUnion::SepUnion(Sep& s1, Sep& s2) : Sep(s1.nb_var), list(Array<Sep>(s1,s2)) {

}

SepUnion::SepUnion(Sep& s1, Sep& s2, Sep& s3) : Sep(s1.nb_var), list(Array<Sep>(s1,s2,s3)) {

}

void SepUnion::separate(IntervalVector &x_in, IntervalVector &x_out){
	assert(x_in==x_out);
	IntervalVector result_x_out(IntervalVector::empty(x_out.size()));
    IntervalVector savebox(x_out);

    for (int i=0; i<list.size(); i++) {
    	if (i>0) x_out=savebox;
    	x_out &= x_in;
    	list[i].separate(x_in,x_out);
    	result_x_out |= x_out;
    }
    x_out = result_x_out;

}

} // end namespace ibex
