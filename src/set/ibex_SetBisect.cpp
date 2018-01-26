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
#include "ibex_Sep.h"
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

SetNode* SetBisect::inter(bool iset, const IntervalVector& nodebox, const IntervalVector& x, BoolInterval xstatus) {

	if (iset) {
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
		left = left->inter(iset, left_box(nodebox), x, xstatus);
		left->father = this;
		right = right->inter(iset, right_box(nodebox), x, xstatus);
		right->father = this;
		// status of children may have changed --> try merge
		return try_merge();
	}

}

/**
 * It is not a good idea to keep track of the result of a contraction at one node
 * for processing the subnodes. First, if we mix box1 and box2 (the result of the
 * inner and outer contraction) then, by default, what is outside (box1 & box2) is
 * supposed to be "IN" and we lose the "OUT" information.
 * If we keep only "box2" and replace nodebox by "nodebox & box2", the result of
 * the contraction is not better in general because contractors are monotonous.
 * Furthermore, trying to abort contraction in the case where nodebox is a superset
 * of box2 is not a good idea because
 *    C(x) \subseteq y \subseteq x does not imply C(x) \subseteq C(y)
 *
 *
 * A first variant could be to build for each node a temporary set that corresponds to the
 * contraction (with "diff") of the node box, and recursively performs and intersection
 * between this node and this temporary set, which may cause some part of the set
 * to be destroyed (which is good) but other part to be restructured (some leaves
 * may be split by the effect of the "diff").
 *
 * We chose another variant which is faster because:
 * - We don't create a temporary set.
 * - We directly call a "contract" function on this node with the box that results
 *   from the outer contraction.
 * - This contract function can only destroy node, not create new ones (whence the
 *   "_no_diff"), except if this node is a leaf and if we are in "irregular" mode.
 * - The approach allows to manage the "regular" mode easily.
 */
SetNode* SetBisect::inter(bool iset, const IntervalVector& nodebox, Sep& sep, double eps) {

	IntervalVector box1(nodebox);
	IntervalVector box2(nodebox);

	sep.separate(box1,box2);

	SetNode* this2=this;

	if ((iset && sep.status1()!=MAYBE) || sep.status1()==NO)
		this2=this2->contract_no_diff(sep.status1(), nodebox, box1);

	if ((iset && sep.status2()!=MAYBE) || sep.status2()==NO)
		this2=this2->contract_no_diff(sep.status2(), nodebox, box2);

	if (box1.is_empty() || box2.is_empty() || this2->is_leaf()) return this2;

	SetBisect* bis = (SetBisect*) this2;

	bis->left = bis->left->inter(iset, left_box(nodebox), sep, eps);
	bis->left->father = bis;
	bis->right = bis->right->inter(iset,right_box(nodebox), sep, eps);
	bis->right->father = bis;

	// status of children may have changed --> try merge or update status
	return bis->try_merge();
}

SetNode* SetBisect::union_(const IntervalVector& nodebox, const IntervalVector& x, BoolInterval x_status) {
	if (x_status==NO) {
		return this;
	}
    if (x_status==YES && nodebox.is_subset(x)) {
		delete this; // warning: suicide
		return new SetLeaf(YES);
	} else {
		left = left->union_(left_box(nodebox), x, x_status);
		left->father = this;
		right = right->union_(right_box(nodebox), x, x_status);
		right->father = this;
		// status of children may have changed --> try merge
		return try_merge();
	}
}

BoolInterval SetBisect::is_superset(const IntervalVector& nodebox, const IntervalVector& box) const {
	if (!nodebox.intersects(box)) return YES;
	else {
		// maybe compilers do the following optimization (avoiding to call
		// superset on the right side if the answer with the left side is "NO") when
		// we write:
		//
		// left->is_superset(left_box(nodebox),box) & right->is_superset(right_box(nodebox),box);

		BoolInterval l_res=left->is_superset(left_box(nodebox),box);
		if (l_res==NO) return NO;
		else return l_res & right->is_superset(right_box(nodebox),box);
	}
}

SetNode* SetBisect::contract_no_diff(BoolInterval status, const IntervalVector& nodebox, const IntervalVector& box) {
	if (nodebox.is_subset(box)) {
		return this;
	} else if (!nodebox.intersects(box)) {
		delete this;
		return new SetLeaf(status);
	} else {
		left = left->contract_no_diff(status, left_box(nodebox), box);
		left->father = this;
		right = right->contract_no_diff(status, right_box(nodebox), box);
		right->father = this;
		return try_merge();
	}
}

void SetBisect::visit(const IntervalVector& nodebox, SetVisitor& visitor) const {
	if(visitor.visit_node(nodebox)){
		left->visit(left_box(nodebox),visitor);
		right->visit(right_box(nodebox),visitor);
	}
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
