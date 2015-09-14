//============================================================================
//                                  I B E X                                   
// File        : ibex_SepInverse.cpp
// Author      : Benoit Desrochers, Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Mar 3, 2015
// Last Update : Mar 3, 2015
//============================================================================

#include "ibex_SepQInter.h"
#include "ibex_QInter.h"

namespace ibex {

SepQInterProjF::SepQInterProjF(const Array<Sep>& list) : Sep(), list(list), q(0) { }


void SepQInterProjF::setq(int q){ this->q = q; }

int SepQInterProjF::getq(){ return q; }

void SepQInterProjF::separate(IntervalVector& xin, IntervalVector& xout) {
	IntervalMatrix boxes_in(list.size(), xin.size());
	IntervalMatrix boxes_out(list.size(), xin.size());
	Array<IntervalVector> refs_in(list.size());
	Array<IntervalVector> refs_out(list.size());


	for (int i=0; i<list.size(); i++) {
		boxes_in[i]=xin;
		boxes_out[i]=xout;

		list[i].separate(boxes_in[i], boxes_out[i]);

		refs_in.set_ref(i,boxes_in[i]);
		refs_out.set_ref(i,boxes_out[i]);
	}
	
	xin &= qinter_projf(refs_in,q+1);
    xout &= qinter_projf(refs_out, list.size() - q);
	
}


} // end namespace ibex
