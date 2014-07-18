//============================================================================
//                                  I B E X                                   
// File        : ibex_Set.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : 13 juil. 2014
//============================================================================

#include "ibex_Set.h"

using namespace std;

namespace ibex {

Set::Set(const IntervalVector& bounding_box) : root(new SetLeaf(MAYBE)), bounding_box(bounding_box) {

}

void Set::contract(Ctc& ctc, BoolInterval c_status, double eps) {
	root = root->contract(bounding_box, ctc, c_status, eps);
}

void Set::to_vibes() const {
	root->to_vibes(bounding_box);
}

Set::~Set() {
	delete root;
}

} // namespace ibex
