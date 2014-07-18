//============================================================================
//                                  I B E X                                   
// File        : ibex_SetNode.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : 13 juil. 2014
//============================================================================

#include "ibex_SetNode.h"

using namespace std;

namespace ibex {

SetNode* diff(const IntervalVector& x, const IntervalVector& y, BoolInterval x_status, BoolInterval y_status, double eps) {

	if (y.is_empty()) {
		return new SetLeaf(x_status);
	}

	const int nn=x.size();

	typedef struct {
		int var;
		double pt;
		bool y_left; // is the part that belongs to y on the left (=true) or right (=false) side?
	} bisection;

	bisection *tmp = new bisection[2*nn]; // in the worst case, there is 2n bisections
	Interval c1, c2;
	int b=0;

	for (int var=0; var<nn; var++) {

		x[var].diff(y[var],c1,c2);

		if (!c1.is_empty() && c1.diam()>=eps) {
			tmp[b].var = var;
			if (c1.lb()==x.lb()) {
				tmp[b].pt = c1.ub();
				tmp[b].y_left = false;
			} else {
				tmp[b].pt = c1.lb();
				tmp[b].y_left = true;
			}
			b++;
		}

		if (!c2.is_empty() && c2.diam()>=eps) {
			tmp[b].var = var;
			tmp[b].pt = c2.lb();
			tmp[b].y_left = true;
			b++;
		}
	}

	SetNode* root;

	if (b==0) {
		root = new SetLeaf(x_status);
	} else {
		SetBisect* bisect=new SetLeaf(y_status);

		for (int i=b-1; i>=0; i--) {
			if (tmp[i].y_left) {
				bisect=new SetBisect(tmp[i].var,tmp[i].pt, bisect, new SetLeaf(x_status));
			} else {
				bisect=new SetBisect(tmp[i].var,tmp[i].pt, new SetLeaf(x_status), bisect);
			}
		}
		root = bisect;
	}
	delete[] tmp;

	return root;
}

// result of a contraction represented as a SetNode
SetNode* contract_set(const IntervalVector& box1, Ctc& c, BoolInterval c_status, double eps) {
	if (c_status==MAYBE) return new SetLeaf(MAYBE);

	SetNode* root2;
	IntervalVector box2(box1);

	try {
		c.contract(box2);
		root2 = diff(box1, box2, c_status, MAYBE, eps);
	} catch(EmptyBoxException&) {
		root2 = new SetLeaf(c_status);
	}

	return root2;
}


SetNode* SetNode::contract(const IntervalVector& nodebox, Ctc& c, BoolInterval c_status, double eps) {
	// perform contraction
	SetNode* this2 = inter(nodebox, contract_set(nodebox,c,c_status,eps), nodebox, eps);

	this2->contract_rec(nodebox, c, c_status, eps);

	if (this2)

	// warning: cannot use this anymore (use this2 instead)
	if (!this2->is_leaf()) {

		SetBisect* bisect_node = (SetBisect*) this2;

		// call recursively
		left = bisect_node->contract(nodebox, c, c_status, eps);
		right = bisect_node->contract(nodebox, c, c_status, eps);

		// try compaction
		BoolInterval left_status=bisect_node->left->status();
		if (left_status!=MAYBE && left_status==bisect_node->right->status()) {
			delete this2; // warning: suicide again
			return new SetLeaf(left_status);
		} else
			return this;
	}
	else {
		return this;
	}
}

SetNode* SetNode::inter(const IntervalVector& nodebox, const SetNode* other, const IntervalVector& otherbox, double eps) {

	if (other->is_leaf()) {
		return inter(nodebox, otherbox, other->status(), eps);
	} else {

		SetBisect* bisect_node = (SetBisect*) other;
		SetNode* this2 = inter(nodebox, bisect_node->left, bisect_node->left_box(otherbox), eps);
		// warning: cannot use this anymore (use this2 instead)
		return this2->inter(nodebox, bisect_node->right, bisect_node->right_box(otherbox), eps);
	}
}


} // namespace ibex
