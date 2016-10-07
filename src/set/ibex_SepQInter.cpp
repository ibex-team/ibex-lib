//============================================================================
//                                  I B E X
// File        : ibex_SepQInter.cpp
// Author      : Benoit Desrochers, Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Mar 3, 2015
// Last Update : Jul 05, 2016
//============================================================================

#include "ibex_SepQInter.h"
#include "ibex_QInter.h"

namespace ibex {

SepQInter::SepQInter(const Array<Sep>& list, int q) :
	Sep(list[0].nb_var),
	list(list),
	boxes_in(list.size(), list[0].nb_var),
	boxes_out(list.size(), list[0].nb_var)
	{ this->set_q(q); }


void SepQInter::separate(IntervalVector& xin, IntervalVector& xout) {
	Array<IntervalVector> refs_in(list.size());
	Array<IntervalVector> refs_out(list.size());


	for (int i=0; i<list.size(); i++) {
		boxes_in[i]=xin;
		boxes_out[i]=xout;

		list[i].separate(boxes_in[i], boxes_out[i]);

		refs_in.set_ref(i,boxes_in[i]);
		refs_out.set_ref(i,boxes_out[i]);
	}

	xin &= qinter(refs_in,q+1);
  xout &= qinter(refs_out, list.size() - q);

}


} // end namespace ibex
