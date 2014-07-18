//============================================================================
//                                  I B E X                                   
// File        : ibex_SetBisect.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : 13 juil. 2014
//============================================================================

#include "ibex_SetBisect.h"
#include "ibex_SetLeaf.h"
#include <stack>
#include <utility>

using namespace std;

namespace ibex {

SetBisect::SetBisect(int var, double pt, SetNode* left, SetNode* right) : var(var), pt(pt), left(left), right(right) {

}

SetBisect::~SetBisect() {
	delete &left;
	delete &right;
}


BoolInterval SetBisect::status() const {
	// a bisectNode with two subnodes of same status should not exist
	// (automatically compacted as a leaf node)
	assert(left->status()==MAYBE || left->status()!=right->status());

	return MAYBE;
}

bool SetBisect::is_leaf() const {
	return false;
}

SetNode* SetBisect::inter(const IntervalVector& nodebox, const IntervalVector& x, BoolInterval x_status, double eps) {
	if (x_status==MAYBE) {
		return this;
	}
	else if (nodebox.is_subset(x)) {
		delete this; // warning: suicide
		return new SetLeaf(x_status);
	} else {
		left = left->inter(left_box(nodebox),x,x_status,eps);
		right = right->inter(right_box(nodebox),x,x_status,eps);
		// status of children may have changed --> try merge
		return try_merge();
	}
}

SetNode* SetBisect::contract_rec(const IntervalVector& nodebox, Ctc& ctc_in, Ctc& ctc_out, double eps) {
	left = left->contract(nodebox, ctc_in, ctc_out, eps);
	right = right->contract(nodebox, ctc_in, ctc_out, eps);
	// status of children may have changed --> try merge
	return try_merge();
}

void SetBisect::to_vibes(color_code color_func, const IntervalVector& nodebox) const {
	left->to_vibes(color_func, left_box(nodebox));
	right->to_vibes(color_func, right_box(nodebox));
}

IntervalVector SetBisect::left_box(const IntervalVector& nodebox) const {
	IntervalVector leftbox(nodebox);
	leftbox[var] =Interval(nodebox[var].lb(),pt);
	return leftbox;
}

IntervalVector SetBisect::right_box(const IntervalVector& nodebox) const {
	IntervalVector rightbox(nodebox);
	rightbox[var]=Interval(pt,nodebox[var].ub());
	return rightbox;
}

SetNode* SetBisect::try_merge() {
	if (left->status()!=MAYBE && left->status()==right->status()) {
		BoolInterval s=left->status();
		delete this;
		return new SetLeaf(s);
	} else
		return this;
}

} // namespace ibex
