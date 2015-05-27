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
	// a bisectNode with two subnodes of same status IN or OUT should not exist
	assert(!dynamic_cast<SetLeaf*>(left) ||
			!dynamic_cast<SetLeaf*>(right) ||
			dynamic_cast<SetLeaf*>(left)->status !=dynamic_cast<SetLeaf*>(right)->status);

	left->father = this;
	right->father = this;
}

SetBisect::SetBisect(int var, double pt) : var(var), pt(pt), left(NULL), right(NULL) {

}

SetBisect::~SetBisect() {
	delete left;
	delete right;
}

bool SetBisect::is_leaf() const {
	return false;
}

SetNode* SetBisect::inter(bool sync, const IntervalVector& nodebox, const IntervalVector& x, BoolInterval xstatus, double eps) {

	if (sync) {
		if (xstatus==MAYBE)
			return this;

		// TODO assert((xstatus==NO && !contains_in) || (xstatus==YES && !contains_out));

		// xstatus is either YES or NO
		if (nodebox.is_subset(x)) {
			delete this;
			return new SetLeaf(xstatus);
		}
	} else {
		if (xstatus==YES)
			return this;

		// if xstatus is MAYBE we don't replace
		// this node by a leaf MAYBE (we lose information)
		if (xstatus==NO && nodebox.is_subset(x)) {
			delete this;
			return new SetLeaf(xstatus);
		}
	}

	if (!nodebox.intersects(x))
		return this;
	else {
		left = left->inter(sync, left_box(nodebox), x, xstatus, eps);
		left->father = this;
		right = right->inter(sync, right_box(nodebox), x, xstatus, eps);
		right->father = this;
		// status of children may have changed --> try merge
		return try_merge();
	}

}

SetNode* SetBisect::inter2(bool sync, const IntervalVector& nodebox, const pair<SetNode*,IntervalVector>& other, double eps) {
	if (!nodebox.intersects(other.second)) return this;

	if (other.first->is_leaf()) return inter(sync, nodebox, other.second, ((SetLeaf*) other.first)->status, eps);

	const IntervalVector lbox=left_box(nodebox);
	const IntervalVector rbox=right_box(nodebox);

	if (lbox.intersects(other.second)) {
		left = left->inter2(sync, lbox, other.first->subset(other.second,lbox), eps);
		left->father = this;
	}
	if (rbox.intersects(other.second)) {
		right = right->inter2(sync, rbox, other.first->subset(other.second,rbox), eps);
		right->father = this;
	}
	return try_merge();
}

pair<SetNode*,IntervalVector> SetBisect::subset(const IntervalVector& nodebox, const IntervalVector& box) {
	const IntervalVector lbox=left_box(nodebox);
	const IntervalVector rbox=right_box(nodebox);

	if (lbox.intersects(box)) {
		if (rbox.intersects(box)) return pair<SetNode*,IntervalVector>(this,nodebox);
		else return left->subset(lbox,box);
	}

	assert(rbox.intersects(box));
	return right->subset(rbox,box);
}

SetNode* SetBisect::inter_rec(bool sync, const IntervalVector& nodebox, Sep& sep, const IntervalVector& targetbox, double eps) {
	left = left->inter(sync, left_box(nodebox), sep, targetbox, eps);
	left->father = this;
	right = right->inter(sync,right_box(nodebox), sep, targetbox, eps);
	right->father = this;

	//cout << "left="; left->print(cout,left_box(nodebox),0);
	//cout << "right="; right->print(cout,right_box(nodebox),0);
	// status of children may have changed --> try merge or update status
	return try_merge();
}

SetNode* SetBisect::union_(const IntervalVector& nodebox, const IntervalVector& x, BoolInterval x_status, double eps) {
	assert(x_status<=MAYBE);

	if (x_status>YES) {
		return this;
	}

    if (nodebox.is_subset(x)) {
		delete this; // warning: suicide
		return new SetLeaf(YES);
	} else {
		left = left->union_(left_box(nodebox), x, x_status, eps);
		left->father = this;
		right = right->union_(right_box(nodebox), x, x_status, eps);
		right->father = this;
		// status of children may have changed --> try merge
		return try_merge();
	}
}

void SetBisect::visit_leaves(leaf_func func, const IntervalVector& nodebox) const {
	left->visit_leaves(func, left_box(nodebox));
	right->visit_leaves(func, right_box(nodebox));
}

void SetBisect::print(ostream& os, const IntervalVector& nodebox, int shift) const {
	for (int i=0; i<shift; i++) os << ' ';
	os << "* " << nodebox << endl;
	left->print(os, left_box(nodebox), shift+2);
	right->print(os, right_box(nodebox), shift+2);
}

IntervalVector SetBisect::left_box(const IntervalVector& nodebox) const {
	IntervalVector leftbox(nodebox);
	assert (nodebox[var].contains(pt));
	leftbox[var] =Interval(nodebox[var].lb(),pt);
	return leftbox;
}

IntervalVector SetBisect::right_box(const IntervalVector& nodebox) const {
	IntervalVector rightbox(nodebox);
	assert (nodebox[var].contains(pt));
	rightbox[var]=Interval(pt,nodebox[var].ub());
	return rightbox;
}

SetNode* SetBisect::try_merge() {
	if (left->is_leaf() && right->is_leaf()) {
		BoolInterval left_status=((SetLeaf*) left)->status;
		if (left_status == ((SetLeaf*) right)->status) {
			delete this; // warning: left does not exist anymore
			return new SetLeaf(left_status);
		}
	}
	return this;
}

} // namespace ibex
