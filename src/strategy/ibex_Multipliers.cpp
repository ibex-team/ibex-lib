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

Multipliers::Multipliers() {

}

void Multipliers::init_root(int n) {
	lambda.resize(n);

	for (int i=0; i<n; i++) {
		lambda[i]=Interval(0,1);
	}
}


Multipliers::Multipliers(const Multipliers& e) : lambda(e.lambda) {

}

std::pair<Backtrackable*,Backtrackable*> Multipliers::down() {
	return std::pair<Backtrackable*,Backtrackable*>(new Multipliers(*this),new Multipliers(*this));
}

Multipliers::~Multipliers() {

}

} // end namespace ibex
