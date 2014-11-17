//============================================================================
//                                  I B E X                                   
// File        : ibex_Multipliers.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Apr 18, 2013
//============================================================================

#include "ibex_Multipliers.h"
#include <stdlib.h>

namespace ibex {

Multipliers::Multipliers(): lambda(1) {

}

void Multipliers::init_root(int M, int R, int K) {

	lambda.resize(1+M+R+K);

	lambda[0]=Interval(0,1);

	for (int m=0; m<M; m++)
		lambda[1+m]=Interval(0,1);

	for (int r=0; r<R; r++)
		lambda[1+M+r]=Interval(-1,1);

	for (int k=0; k<K; k++)
		lambda[1+M+R+k]=Interval(0,1);
}


Multipliers::Multipliers(const Multipliers& e) : lambda(e.lambda) {

}

std::pair<Backtrackable*,Backtrackable*> Multipliers::down() {
	return std::pair<Backtrackable*,Backtrackable*>(new Multipliers(*this),new Multipliers(*this));
}

Multipliers::~Multipliers() {

}

} // end namespace ibex
