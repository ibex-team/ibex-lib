//============================================================================
//                                  I B E X                                   
// File        : ibex_SetNode.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : 13 juil. 2014
//============================================================================

#include "ibex_SetNode.h"
#include "ibex_SetLeaf.h"
#include "ibex_SetBisect.h"
 #include <unistd.h>

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
		//cout << "x[var]=" << x[var] << " y[var]=" << y[var] << " c1=" << c1 << " c2=" << c2 << endl;

		if (c1.is_empty()) continue;

		// we discard the y-part if it is not significant
		// but this is safe only if the status of y is MAYBE.
		if (y_status==MAYBE && c1.diam()<eps) continue;

		// when the set difference is close to x itself
		// we return x. But this is safe only if the
		// status of x is MAYBE.
		if (x_status==MAYBE && x[var].delta(c1)<eps) {
			// no (significant) difference at all
			delete[] tmp;
			return new SetLeaf(x_status);
		}

		tmp[b].var = var;
		if (c1.lb()==x[var].lb()) {
			tmp[b].pt = c1.ub();
			tmp[b].y_left = false;
		} else {
			tmp[b].pt = c1.lb();
			tmp[b].y_left = true;
		}
		b++;

		if (c2.is_empty()) continue;
		if (y_status==MAYBE && c2.diam()<eps)  continue;
		if (x_status==MAYBE && x[var].delta(c2)<eps) {
			// no (significant) difference at all
			delete[] tmp;
			return new SetLeaf(x_status);
		}

		tmp[b].var = var;
		tmp[b].pt = c2.lb();
		tmp[b].y_left = true;
		b++;
	}

	SetNode* root;

	if (b==0) {
		root = new SetLeaf(y_status);
	} else {
		SetNode* bisect=new SetLeaf(y_status);

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
	//cout << "contract set:" << endl;
	//root2->print(cout,box1,0);

	return root2;
}

char to_string(const BoolInterval& status) {
	switch(status) {
	case YES : return 'Y'; break;
	case NO : return 'N'; break;
	case MAYBE : return '?'; break;
	}
}

const char* color(BoolInterval status) {
	switch (status) {
	case YES: return "g";
	case NO:  return "r";
	case MAYBE : return "b";
	}
}

SetNode::~SetNode() {

}

SetNode* SetNode::contract(const IntervalVector& nodebox, Ctc& ctc_in, Ctc& ctc_out, double eps) {
	// perform contraction
	//cout << "=== contract with ctc_in  =======" << endl;
	SetNode* this2 = this->inter(nodebox, contract_set(nodebox, ctc_in, YES, eps), nodebox, eps);
	//cout << " ctc_in gives: ";
	//this2->print(cout,nodebox,0);
	//cout << endl;

	//cout << "=== contract with ctc_out =======" << endl;
	SetNode* this3 = this2->inter(nodebox, contract_set(nodebox, ctc_out, NO, eps), nodebox, eps);
	//cout << " ctc_out gives: ";
	//this3->print(cout,nodebox,0);
	//cout << endl;

	SetNode* this4 = this3->contract_rec(nodebox, ctc_in, ctc_out, eps);

	return this4;
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
