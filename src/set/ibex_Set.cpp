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

Set::Set(const IntervalVector& bounding_box, double eps, SetType status) : root(new SetLeaf(status)), eps(eps), bounding_box(bounding_box) {

}

bool Set::is_empty() const {
	return root==NULL;
}

void Set::sync(Bracket& br) {
	try {
		root = root->sync(bounding_box, br, eps);
	} catch(NoSet& e) {
		delete root;
		root = NULL;
		throw e;
	}
}

void Set::contract(Bracket& br) {
	root->set_in_tmp();
	root = root->inter(bounding_box, br, eps);
	root->unset_in_tmp();
}

Set& Set::operator&=(const Set& set) {
	root->set_in_tmp();
	root = root->inter(bounding_box, set.root, set.bounding_box, eps);
	root->unset_in_tmp();
	return *this;
}


void Set::to_vibes(SetNode::color_code color_func) const {
	root->to_vibes(color_func, bounding_box);
}

std::ostream& operator<<(std::ostream& os, const Set& set) {
	set.root->print(os,set.bounding_box, 0);
	return os;
}

Set::~Set() {
	delete root;
}

} // namespace ibex
