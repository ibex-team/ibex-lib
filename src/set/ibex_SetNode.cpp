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

	assert(!x.is_empty());

	if (y.is_empty()) {
		return new SetLeaf(x_status);
	}

	if (y_status>=UNK && !x.is_superset(y)) y_status = UNK; // we don't know if the IN/OUT part of y is inside x...

	const int nn=x.size();

	typedef struct {
		int var;
		double pt;
		bool x_left;      // is the part that belongs to x is on the left (=true) or right (=false) side?
		bool x_enlarged;  // the x-part is enlarged by eps and therefore has both x_status and y_status
	} bisection;

	// used to determine the status of the final box once the "slices" has been cut off.
	// It may either be y_status or (x_status | y_status) if at some point
	// the y-part has been enlarged.
	bool y_enlarged=false; // false by default

	bisection *tmp = new bisection[2*nn]; // in the worst case, there is 2n bisections

	int b=0; // count bisections

	Interval* c = new Interval[2];

	for (int var=0; var<nn; var++) {

		x[var].diff(y[var],c[0],c[1]);
		//cout << "x[" << var << "]=" << x[var] << " y[" << var << "]=" << y[var] << " c1=" << c[0] << " c2=" << c[1] << endl;

		for (int i=0; i<2; i++) {

			// x-part is empty: no bisection on this variable
			if (c[i].is_empty()) continue;

			// y-part is empty: no bisection at all
			if (c[i]==x[var]) {
				delete[] tmp;
				delete[] c;
				return new SetLeaf(x_status);
			}

			// x is too small
			if (x[var].ub() <= x[var].lb()+eps) {
				y_enlarged = true;
				continue;
			}

			tmp[b].var = var;
			tmp[b].x_enlarged = false; // by default

			// c[i] is on the left side
			if (c[i].lb()==x[var].lb()) {

				tmp[b].x_left = true;

				// we enlarge the x-part if it is too small
				if (c[i].diam()<eps) {
					c[i] = Interval(x[var].lb(), x[var].lb()+eps);
					tmp[b].x_enlarged = true;
				}
				// we enlarge the y-part if it is too small
				else if (x[var].delta(c[i])<eps) {
					c[i] = Interval(x[var].lb(), x[var].ub()-eps);
					y_enlarged = true;
				}

				tmp[b].pt = c[i].ub();
				assert(x[var].interior_contains(tmp[b].pt));
			}

			// c[i] is on the right side
			else  {

				tmp[b].x_left = false;

				// we enlarge the x-part if it is too small
				if (c[i].diam()<eps) {
					c[i] = Interval(x[var].ub()-eps, x[var].ub());
					tmp[b].x_enlarged = true;
				}
				// enlarge the y-part if it is too small
				else if (x[var].delta(c[i])<eps) {
					c[i] = Interval(x[var].lb()+eps, x[var].ub());
					y_enlarged = true;
				}

				tmp[b].pt = c[i].lb();
				assert(x[var].interior_contains(tmp[b].pt));
			}

			b++;
		}
	}

	delete[] c;

	SetNode* root = new SetLeaf(y_enlarged? x_status | y_status : y_status);

	SetNode* bisect=root;

	// we first proceed with the eps-enlarged slices (this order
	// corresponds to the reverse order of the bisections)
	for (int i=b-1; i>=0; i--) {
		if (!tmp[i].x_enlarged) continue; // postponed

		if (tmp[i].x_left) {
			assert(x[tmp[i].var].interior_contains(tmp[i].pt));
			bisect=new SetBisect(tmp[i].var,tmp[i].pt, new SetLeaf(x_status | y_status), bisect);
		} else {
			assert(x[tmp[i].var].interior_contains(tmp[i].pt));
			bisect=new SetBisect(tmp[i].var,tmp[i].pt, bisect, new SetLeaf(x_status | y_status));
		}

	}

	// we then proceed with the exact bisections (x_status is not lost for the slices)
	for (int i=b-1; i>=0; i--) {
		if (tmp[i].x_enlarged) continue; // already done

		if (tmp[i].x_left) {
			assert(x[tmp[i].var].interior_contains(tmp[i].pt));
			bisect=new SetBisect(tmp[i].var,tmp[i].pt, new SetLeaf(x_status), bisect);
		} else {
			assert(x[tmp[i].var].interior_contains(tmp[i].pt));
			bisect=new SetBisect(tmp[i].var,tmp[i].pt, bisect, new SetLeaf(x_status));
		}
	}

	root = bisect;
	delete[] tmp;
	return root;
}

SetNode::SetNode(NodeType status) : status(status) {

}

SetNode::~SetNode() {

}

SetNode* SetNode::sync(const IntervalVector& nodebox, Sep& sep, const IntervalVector& targetbox, double eps) {

	IntervalVector box=targetbox & nodebox;

	if (box.is_empty()) return this;


	IntervalVector box1(box);
	IntervalVector box2(box);

	sep.separate(box1,box2);

	SetNode* root1 = diff(box, box1, IN, UNK, eps);
	//cout << "set obtained with inner contraction:" << endl; root1->print(cout,box,0);
	SetNode* root2 = diff(box, box2, OUT, UNK, eps);
	//cout << "set obtained with outer contraction:" << endl; root2->print(cout,box,0);

	SetNode* root3 = root1->sync(box,root2,box,eps);
	delete root2;

	SetNode* this2 = this->sync(nodebox, root3, targetbox, eps);

	SetNode* this3 = this2->sync_rec(nodebox, sep, targetbox, eps);
	//cout << " inter rec gives: "; this3->print(cout,nodebox,0);

	return this3;
}

SetNode* SetNode::inter(const IntervalVector& nodebox, Sep& sep, const IntervalVector& targetbox, double eps) {

	IntervalVector box=targetbox & nodebox;

	if (box.is_empty()) return this;

	IntervalVector box1(box);
	IntervalVector box2(box);

	sep.separate(box1,box2);

	SetNode* root1 = diff(box, box2, OUT, IN, eps);

	SetNode* this2 = this->inter(nodebox, root1, box, eps);
	//cout << " sep gives: "; this2->print(cout,nodebox,0);

	if (box1.is_empty())
		return this2;
	else {
		SetNode* this3 = this2->inter_rec(nodebox, sep, box1, eps);
		//cout << " inter rec gives: "; this4->print(cout,nodebox,0);

		return this3;
	}
}

SetNode* SetNode::sync(const IntervalVector& nodebox, const SetNode* other, const IntervalVector& otherbox, double eps) {

	if (nodebox.is_disjoint(otherbox))
		return this;
	else if (other->is_leaf()) {
		return sync(nodebox, otherbox, other->status, eps);
	} else {

		SetBisect* bisect_node = (SetBisect*) other;
		SetNode* this2 = sync(nodebox, bisect_node->left, bisect_node->left_box(otherbox), eps);
		// warning: cannot use this anymore (use this2 instead)
		return this2->sync(nodebox, bisect_node->right, bisect_node->right_box(otherbox), eps);
	}
}

SetNode* SetNode::inter(const IntervalVector& nodebox, const SetNode* other, const IntervalVector& otherbox, double eps) {

	if (nodebox.is_disjoint(otherbox))
		return this;
	else if (other->is_leaf()) {
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
