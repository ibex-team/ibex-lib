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

// =========== shortcuts ==================
#define IN         __IBEX_IN__
#define OUT        __IBEX_OUT__
#define UNK        __IBEX_UNK__
#define UNK_IN     __IBEX_UNK_IN__
#define UNK_OUT    __IBEX_UNK_OUT__
#define UNK_IN_OUT __IBEX_UNK_IN_OUT__
#define IN_TMP     __IBEX_IN_TMP__
// ========================================

namespace ibex {

SetNode* diff(const IntervalVector& x, const IntervalVector& y, NodeType x_status, NodeType y_status, double eps) {

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
		//cout << "x[" << var << "]=" << x[var] << " y[" << var << "]=" << y[var] << " c1=" << c1 << " c2=" << c2 << endl;

		if (c1.is_empty()) continue;

		// we discard the x-part if it is not significant
		// but this is safe only if the status of y is UNK.
		if (y_status==UNK && c1.diam()<eps) continue;

		// when the set difference is close to x itself
		// we return x. But this is safe only if the
		// status of x is UNK.
		if (x[var]==c1 || (x_status==UNK && x[var].delta(c1)<eps)) {
			// no (significant) difference at all
			delete[] tmp;
			return new SetLeaf(x_status);
		}

		tmp[b].var = var;
		if (c1.lb()==x[var].lb()) {
			tmp[b].pt = c1.ub();
			assert(x[var].interior_contains(tmp[b].pt));
			tmp[b].y_left = false;
		} else {
			tmp[b].pt = c1.lb();
			assert(x[var].interior_contains(tmp[b].pt));
			tmp[b].y_left = true;
		}
		b++;

		if (c2.is_empty()) continue;
		if (y_status==UNK && c2.diam()<eps)  continue;
		if (x[var]==c2 || (x_status==UNK && x[var].delta(c2)<eps)) {
			// no (significant) difference at all
			delete[] tmp;
			return new SetLeaf(x_status);
		}

		tmp[b].var = var;
		tmp[b].pt = c2.lb();
		assert(x[var].interior_contains(tmp[b].pt));
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
				assert(x[tmp[i].var].interior_contains(tmp[i].pt));
				bisect=new SetBisect(tmp[i].var,tmp[i].pt, bisect, new SetLeaf(x_status));
			} else {
				assert(x[tmp[i].var].interior_contains(tmp[i].pt));
				bisect=new SetBisect(tmp[i].var,tmp[i].pt, new SetLeaf(x_status), bisect);
			}
		}
		root = bisect;
	}
	delete[] tmp;

	return root;
}

// result of a contraction represented as a SetNode
SetNode* contract_set(const IntervalVector& box, Sep& sep, double eps) {

	IntervalVector box1(box);
	IntervalVector box2(box);

	sep.separate(box1,box2);

	SetNode* root1 = diff(box, box1, IN, box1.max_diam()<=eps? UNK : UNK_IN_OUT, eps);
	//cout << "set obtained with inner contraction:" << endl; root1->print(cout,box,0);
	SetNode* root2 = diff(box, box2, OUT, box2.max_diam()<=eps? UNK : UNK_IN_OUT, eps);
	//cout << "set obtained with outer contraction:" << endl;	root2->print(cout,box,0);

	// TODO: pb: sync fails
	// check "false" (skip_other_maybe is an old parameter not used anymore)
	SetNode* root3 = root1->inter(box,root2,box,eps); //,false);

	delete root2;

	//cout << "final set:" << endl; root3->print(cout,box,0);

	return root3;
}

char to_string(const NodeType& status) {
	switch(status) {
	case IN : return 'Y'; break;
	case OUT : return 'N'; break;
	default : return '?';
	}
}

SetNode::SetNode(NodeType status) : status(status) {

}

SetNode::~SetNode() {

}

SetNode* SetNode::sync(const IntervalVector& nodebox, Sep& sep, double eps) {
	// perform contraction
	//cout << "=== contract with ctc_in  =======" << endl;

	// we skip other UNK-box if this node is not a leaf. This makes no difference
	// if we are in SYNC mode but if we are in INTER mode, this prevents from
	// this node to be "absorbed" by a temporary UNK box resulting from contraction.
	SetNode* this2 = this->sync(nodebox, contract_set(nodebox, sep, eps), nodebox, eps, !is_leaf());

	//cout << " sep gives: "; this2->print(cout,nodebox,0);
	//cout << endl;


	SetNode* this3 = this2->sync_rec(nodebox, sep, eps);

	return this3;
}

SetNode* SetNode::sync(const IntervalVector& nodebox, const SetNode* other, const IntervalVector& otherbox, double eps, bool skip_other_maybe) {

	if (nodebox.is_disjoint(otherbox))
		return this;
	else if (other->is_leaf()) {
		//if (other->status<UNK) // || !skip_other_maybe)
			return sync(nodebox, otherbox, other->status, eps);
		//else
		//	return this;
	} else {

		SetBisect* bisect_node = (SetBisect*) other;
		SetNode* this2 = sync(nodebox, bisect_node->left, bisect_node->left_box(otherbox), eps, skip_other_maybe);
		// warning: cannot use this anymore (use this2 instead)
		return this2->sync(nodebox, bisect_node->right, bisect_node->right_box(otherbox), eps, skip_other_maybe);
	}
}

SetNode* SetNode::inter(const IntervalVector& nodebox, Sep& sep, double eps) {
	// we skip other UNK-box if this node is not a leaf. This makes no difference
	// if we are in SYNC mode but if we are in INTER mode, this prevents from
	// this node to be "absorbed" by a temporary UNK box resulting from contraction.
	SetNode* this2 = this->inter(nodebox, contract_set(nodebox, sep, eps), nodebox, eps);
	//cout << " sep gives: "; this2->print(cout,nodebox,0);

	SetNode* this4 = this2->inter_rec(nodebox, sep, eps);

	return this4;
}


SetNode* SetNode::inter(const IntervalVector& nodebox, const SetNode* other, const IntervalVector& otherbox, double eps) {

	if (nodebox.is_disjoint(otherbox))
		return this;
	else if (other->is_leaf()) {
		// we consider IN_TMP to be "UNK" until the end. Otherwise, there
		// would be problems when other->status==UNK. Indeed, if we set the status
		// of this node to UNK, we lose information ("other" may be an intermediate
		// node in the process, although it is a SetLeaf, and some sub-nodes of other
		// could be IN. But once a node is UNK it cannot be set to IN anymore...).
		// If we impose this node to be a leaf, it does not fix the problem (this node may also be an
		// intermediate node in the process: consider a plain box (a leaf) to be
		// contracted).
			return inter(nodebox, otherbox, other->status, eps);
	} else {

		SetBisect* bisect_node = (SetBisect*) other;
		SetNode* this2 = inter(nodebox, bisect_node->left, bisect_node->left_box(otherbox), eps);
		// warning: cannot use this anymore (use this2 instead)
		return this2->inter(nodebox, bisect_node->right, bisect_node->right_box(otherbox), eps);
	}
}

SetNode* SetNode::union_(const IntervalVector& nodebox, const SetNode* other, const IntervalVector& otherbox, double eps) {

	if (nodebox.is_disjoint(otherbox))
		return this;
	else if (other->is_leaf()) {
		return union_(nodebox, otherbox, other->status, eps);
	} else {

		SetBisect* bisect_node = (SetBisect*) other;
		SetNode* this2 = union_(nodebox, bisect_node->left, bisect_node->left_box(otherbox), eps);
		// warning: cannot use this anymore (use this2 instead)
		return this2->union_(nodebox, bisect_node->right, bisect_node->right_box(otherbox), eps);
	}
}

} // namespace ibex
