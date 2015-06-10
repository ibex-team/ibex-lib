//============================================================================
//                                  I B E X                                   
// File        : ibex_SetInterval.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jun 10, 2015
//============================================================================

#include "ibex_SetInterval.h"

using namespace std;

namespace ibex {
SetInterval::SetInterval(const IntervalVector& box, BoolInterval status) : Set(box,status) {

}

SetInterval::SetInterval(const char* filename) : Set(filename) {

}

SetInterval::~SetInterval() {

}

SetInterval&  SetInterval::inter(Sep& sep, double eps) {
	try {
		root = root->inter(true, bounding_box, sep, eps);
	} catch(NoSet& e) {
		delete root;
		root = NULL;
	}
	return *this;
}

bool SetInterval::is_empty() const {
	return root==NULL;
}

void SetInterval::save(const char* filename) {
	Set::save(filename);
}

void SetInterval::visit_leaves(SetNode::leaf_func func) const {
	Set::visit_leaves(func);
}

} // namespace ibex
