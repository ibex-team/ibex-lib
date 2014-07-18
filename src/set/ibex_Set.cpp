//============================================================================
//                                  I B E X                                   
// File        : ibex_Set.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : 13 juil. 2014
//============================================================================

#include "ibex_Set.h"
#include "ibex_SetLeaf.h"

using namespace std;

namespace ibex {

Set::Set(const IntervalVector& bounding_box) : root(new SetLeaf(MAYBE)), bounding_box(bounding_box) {

}

void Set::contract(Ctc& ctc_in, Ctc& ctc_out, double eps) {
	root = root->contract(bounding_box, ctc_in, ctc_out, eps);
}

void Set::to_vibes(SetNode::color_code color_func) const {
	root->to_vibes(color_func, bounding_box);
}

Set::~Set() {
	delete root;
}

} // namespace ibex
