//============================================================================
//                                  I B E X                                   
// File        : ibex_EntailedCtr.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Apr 15, 2013
// Last Update : Apr 15, 2013
//============================================================================

#include "ibex_EntailedCtr.h"
#include <stdlib.h>

namespace ibex {

EntailedCtr::EntailedCtr() : n(-1), entailed(NULL) {

}

void EntailedCtr::init_root(int n) {
	(int&) this->n=n;
	entailed = new bool[n];
	for (int i=0; i<n; i++) {
		entailed[i]=false;
	}
}


EntailedCtr::EntailedCtr(const EntailedCtr& e) : n(e.n) {
	entailed = new bool[n];
	for (int i=0; i<n; i++) {
		entailed[i]=e.entailed[i];
	}
}

std::pair<Backtrackable*,Backtrackable*> EntailedCtr::down() {
	return std::pair<Backtrackable*,Backtrackable*>(new EntailedCtr(*this),new EntailedCtr(*this));
}

EntailedCtr::~EntailedCtr() {
	 delete[] entailed;
}

} // end namespace ibex
