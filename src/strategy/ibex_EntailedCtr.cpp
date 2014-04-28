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

EntailedCtr::EntailedCtr() : orig_sys(NULL), norm_sys(NULL) {

}

void EntailedCtr::init_root(const System& user_sys, const NormalizedSystem& sys) {
	orig_sys = &user_sys;
	norm_sys = &sys;

	orig_entailed = new bool[orig_sys->nb_ctr];
	norm_entailed = new bool[norm_sys->nb_ctr];

	for (int i=0; i<orig_sys->nb_ctr; i++) {
		orig_entailed[i]=false;
	}
	for (int i=0; i<norm_sys->nb_ctr; i++) {
		norm_entailed[i]=false;
	}
}

EntailedCtr::EntailedCtr(const EntailedCtr& e) : orig_sys(e.orig_sys), norm_sys(e.norm_sys) {
	orig_entailed = new bool[orig_sys->nb_ctr];
	norm_entailed = new bool[norm_sys->nb_ctr];

	for (int i=0; i<orig_sys->nb_ctr; i++) {
		orig_entailed[i]=e.orig_entailed[i];
	}
	for (int i=0; i<norm_sys->nb_ctr; i++) {
		norm_entailed[i]=e.norm_entailed[i];
	}
}

std::pair<Backtrackable*,Backtrackable*> EntailedCtr::down() {
	return std::pair<Backtrackable*,Backtrackable*>(new EntailedCtr(*this),new EntailedCtr(*this));
}


void EntailedCtr::set_normalized_entailed(int i) {
	norm_entailed[i] = true;
	int j=norm_sys->original_index(i);
	if (orig_sys->ctrs[j].op!=EQ)
		orig_entailed[j] = true;
}

EntailedCtr::~EntailedCtr() {
	 delete[] orig_entailed;
	 delete[] norm_entailed;
}

std::ostream& operator<<(std::ostream& os, const EntailedCtr& e) {
	os << "original sytem: (";
	for (int i=0; i<e.orig_sys->nb_ctr; i++) {
		os << e.original(i) << (i<e.orig_sys->nb_ctr-1?" ":"");
	}
	os << '\n';
	os << "normalized sytem: (";
	for (int i=0; i<e.norm_sys->nb_ctr; i++) {
		os << e.normalized(i) << (i<e.norm_sys->nb_ctr-1?" ":"");
	}

	return os << ')';
}

} // end namespace ibex
