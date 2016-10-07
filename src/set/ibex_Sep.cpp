//============================================================================
//                                  I B E X                                   
// File        : ibex_Sep.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jun 12, 2015
//============================================================================


#include "ibex_Sep.h"

using namespace std;

namespace ibex {

void Sep::contract(Set& set, double eps) {
	set.root = set.root->inter(false, set.Rn, *this, eps);
}

void Sep::contract(SetInterval& iset, double eps, BoolInterval status1, BoolInterval status2) {
	_status1=status1;
	_status2=status2;

	try {
		iset.root = iset.root->inter(true, iset.Rn, *this, eps);
	} catch(SetInterval::NoSet& e) {
		delete iset.root;
		iset.root = NULL;
	}

	// Restore default values
	_status1 = YES;
	_status2 = NO;
}

} // namespace ibex

