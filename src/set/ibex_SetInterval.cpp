//============================================================================
//                                  I B E X                                   
// File        : ibex_Set.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : 13 juil. 2014
//============================================================================

#include "ibex_SetInterval.h"
#include "ibex_SetLeaf.h"

using namespace std;

namespace ibex {

SetInterval::SetInterval(const IntervalVector& bounding_box, double eps, bool inner) : root(new SetLeaf(inner? __IBEX_IN__: __IBEX_UNK__)), eps(eps), bounding_box(bounding_box) {

}

bool SetInterval::is_empty() const {
	return root==NULL;
}

void SetInterval::sync(Separator& sep) {
	try {
		root = root->sync(bounding_box, sep, eps);
	} catch(NoSet& e) {
		delete root;
		root = NULL;
		throw e;
	}
}

void SetInterval::contract(Separator& sep) {
	root->set_in_tmp();
	root = root->inter(bounding_box, sep, eps);
	root->unset_in_tmp();
}

SetInterval& SetInterval::operator&=(const SetInterval& set) {
	root->set_in_tmp();
	root = root->inter(bounding_box, set.root, set.bounding_box, eps);
	root->unset_in_tmp();
	return *this;
}

SetInterval& SetInterval::operator|=(const SetInterval& set) {
	root = root->union_(bounding_box, set.root, set.bounding_box, eps);
	return *this;
}


void SetInterval::visit_leaves(SetNode::leaf_func func) const {
	root->visit_leaves(func, bounding_box);
}

std::ostream& operator<<(std::ostream& os, const SetInterval& set) {
	set.root->print(os,set.bounding_box, 0);
	return os;
}

SetInterval::~SetInterval() {
	delete root;
}

} // namespace ibex
