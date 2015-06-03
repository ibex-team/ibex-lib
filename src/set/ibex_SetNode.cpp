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

	assert(!x.is_empty());

	assert(x_status!=y_status);

	if (y.is_empty()) {
		return new SetLeaf(x_status);
	}

	const int nn=x.size();

	typedef struct {
		int var;
		double pt;
		bool x_left;      // is the part that belongs to x is on the left (=true) or right (=false) side?
		bool x_enlarged;  // the x-part is enlarged by eps and therefore has both x_status and y_status
	} bisection;

	// used to determine the status of the final box once the "slices" has been cut off.
	// It may either be y_status or MAYBE if at some point
	// the y-part has been enlarged.
	bool y_enlarged=false; // false by default

	bisection *tmp = new bisection[2*nn]; // in the worst case, there is 2n bisections

	int b=0; // count bisections

	Interval* c = new Interval[2];

	for (int var=0; var<nn; var++) {

		Interval _y = y[var]; // copy that may be modified

		// x is too small
		if (x[var].ub() <= x[var].lb()+eps) {
			y_enlarged = true;
			continue;
		}

		x[var].diff(_y,c[0],c[1]);
		//cout << "x[" << var << "]=" << x[var] << " y[" << var << "]=" << _y << " c1=" << c[0] << " c2=" << c[1] << endl;

		for (int i=0; i<2; i++) {

			// x-part is empty: no bisection on this variable
			if (c[i].is_empty()) continue;

			// y-part is empty: no bisection at all
			if (c[i]==x[var]) {
				delete[] tmp;
				delete[] c;
				return new SetLeaf(x_status);
			}

			tmp[b].var = var;
			tmp[b].x_enlarged = false; // by default

			// c[i] is on the left side
			if (c[i].lb()==x[var].lb()) {

				tmp[b].x_left = true;

				// we enlarge the x-part if it is too small
				if (c[i].diam()<eps) {
					if (_y.ub() <= x[var].lb()+eps) {
						// In this case the y part is entirely absorbed
						// by the x part. We switch the role and exit
						// the loop (no more bisection)
						c[i] = Interval(x[var].lb(), _y.ub());
						y_enlarged = true;
						tmp[b].x_left = false;
						break;
					} else {
						c[i] = Interval(x[var].lb(), x[var].lb()+eps);
						tmp[b].x_enlarged = true;

						// if x is enlarged, y is narrowed. So we update y.
						// This is useful to avoid crossings if the
						// x-part is also enlarged for i=1 (when diameter of y is < eps).
						_y=Interval(x[var].lb()+eps,_y.ub());

					}
				}
				// we enlarge the y-part if it is too small
				else if (x[var].delta(c[i])<eps) {
					c[i] = Interval(x[var].lb(), x[var].ub()-eps);
					_y=Interval(x[var].ub()-eps,_y.ub());
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
					if (x[var].ub()-eps <= _y.lb()) {
						assert(i==1);
						y_enlarged = true; // the y-part is absorbed by the x-part.
						break;             // no more bisection
					}
					else {
						c[i] = Interval(x[var].ub()-eps, x[var].ub());
						tmp[b].x_enlarged = true;
					}
				}
				// enlarge the y-part if it is too small
				else if (x[var].delta(c[i])<eps) {
					c[i] = Interval(x[var].lb()+eps, x[var].ub());
					y_enlarged = true;
				}

				tmp[b].pt = c[i].lb();
				assert(x[var].interior_contains(tmp[b].pt));
			}

			//cout << "var=" << tmp[b].var << " pt=" << tmp[b].pt << " left?=" << tmp[b].x_left << " x_enlarged?=" << tmp[b].x_enlarged << endl;
			b++;
		}
	}

	delete[] c;

	if (y_enlarged) y_status = MAYBE;

	SetNode* root = new SetLeaf(y_status);
	SetNode* bisect=root;

	// both x and y with status MAYBE -> skip
	if (y_status == x_status) {
		delete[] tmp;
		return root;
	}

	// we first proceed with the eps-enlarged slices (this order
	// corresponds to the reverse order of the bisections)
	// but if y is also enlarged, we have both x and y with status
	// MAYBE -> skip bisection

	if (y_status!=MAYBE) {
		for (int i=b-1; i>=0; i--) {
			if (!tmp[i].x_enlarged) continue; // postponed

			if (tmp[i].x_left) {
				assert(x[tmp[i].var].interior_contains(tmp[i].pt));
				bisect=new SetBisect(tmp[i].var,tmp[i].pt, new SetLeaf(MAYBE), bisect);

			} else {
				assert(x[tmp[i].var].interior_contains(tmp[i].pt));
				bisect=new SetBisect(tmp[i].var,tmp[i].pt, bisect, new SetLeaf(MAYBE));

			}
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

SetNode::SetNode() : father(NULL) {

}

SetNode::~SetNode() {

}


SetNode* SetNode::inter(bool sync, const IntervalVector& nodebox, Sep& sep, const IntervalVector& targetbox, double eps) {

	IntervalVector box=targetbox & nodebox;

	if (box.is_empty()) return this;

	//if (box==targetbox) return inter_rec(sync, nodebox, sep, targetbox, eps);

	IntervalVector box1(box);
	IntervalVector box2(box);

	sep.separate(box1,box2);

	SetNode* root3;

	if (sync)  {

		SetNode* root1 = diff(box, box1, YES, MAYBE, eps);
		//cout << "set obtained with inner contraction:" << endl; root1->print(cout,box,0);
		SetNode* root2 = diff(box, box2, NO, MAYBE, eps);
		//cout << "set obtained with outer contraction:" << endl; root2->print(cout,box,0);

		root3 = root1->inter(true, box, root2, box, eps);
		//root3 = root1->inter2(true, box, pair<SetNode*,IntervalVector>(root2, box), eps);
		delete root2;


	} else {

		root3 = diff(box, box2, NO, YES, eps);

		//		cout << " before contraction % NO gives: "; this->print(cout,nodebox,0);
		//		cout << "     root3="; root3->print(cout,box,0);
		//

	}

	SetNode* this2 = this->inter(sync, nodebox, root3, box, eps);
	//SetNode* this2 = this->inter2(sync, nodebox, pair<SetNode*,IntervalVector>(root3, box), eps);
	delete root3;
	//		cout << " after contraction % NO gives: "; this2->print(cout,nodebox,0);

	//cout << " sep gives: "; this2->print(cout,nodebox,0);

	box1 &= box2;
	if (box1.is_empty()) return this2;

	SetNode* this3 = this2->inter_rec(sync, nodebox, sep, box1, eps);
	//cout << " inter rec gives: "; this3->print(cout,nodebox,0);

	return this3;
}

// TODO: merge this code with union_

SetNode* SetNode::inter(bool sync, const IntervalVector& nodebox, const SetNode* other, const IntervalVector& otherbox, double eps) {

	if (nodebox.is_disjoint(otherbox))
		return this;
	else if (other->is_leaf()) {
		//cout << "this1 bef="; this->print(cout,nodebox,0);
		//cout << "otherbox=" << otherbox << " status=" << ((SetLeaf*) other)->status << endl;
		SetNode* this1=inter(sync, nodebox, otherbox, ((SetLeaf*) other)->status, eps);
		//cout << "this1: "; this1->print(cout,nodebox,0);
		return this1;
	} else {
		// ******************************** balancing **************************************************
		if (nodebox.max_diam()>otherbox.max_diam()) {

			SetBisect* bis;

			if (is_leaf()) {
				int var=nodebox.extr_diam_index(false);
				pair<IntervalVector,IntervalVector> p=nodebox.bisect(var);
				double pt=p.first[var].ub();
				assert(nodebox[var].interior_contains(pt));

				bis = new SetBisect(var, pt);
				bis->left  = new SetLeaf(((SetLeaf *) this)->status);
				bis->right = new SetLeaf(((SetLeaf *) this)->status);
				bis->left  = bis->left->inter(sync, p.first, other, otherbox, eps);
				bis->right = bis->right->inter(sync, p.second, other, otherbox, eps);
				delete this;
			}
			else {
				bis = (SetBisect*) this;
				bis->left  = bis->left->inter(sync,bis->left_box(nodebox), other, otherbox, eps);
				bis->right = bis->right->inter(sync,bis->right_box(nodebox), other, otherbox, eps);
			}

			bis->left->father = bis;
			bis->right->father = bis;
			return bis->try_merge();

		} else {
			SetBisect* bisect_node = (SetBisect*) other;
			SetNode* this2 = inter(sync, nodebox, bisect_node->left, bisect_node->left_box(otherbox), eps);
			//cout << "this2: "; this2->print(cout,nodebox,0);
			// warning: cannot use this anymore (use this2 instead)
			SetNode* this3 = this2->inter(sync, nodebox, bisect_node->right, bisect_node->right_box(otherbox), eps);
			//cout << "this3: "; this3->print(cout,nodebox,0);
			return this3;
		}
	}
}

SetNode* SetNode::union_(const IntervalVector& nodebox, const SetNode* other, const IntervalVector& otherbox, double eps) {

	if (nodebox.is_disjoint(otherbox))
		return this;
	else if (other->is_leaf()) {
		return union_(nodebox, otherbox, ((SetLeaf*) other)->status, eps);
	} else {

		// ******************************** balancing **************************************************
		if (nodebox.max_diam()>otherbox.max_diam()) {

			SetBisect* bis;

			if (is_leaf()) {
				int var=nodebox.extr_diam_index(false);
				pair<IntervalVector,IntervalVector> p=nodebox.bisect(var);
				double pt=p.first[var].ub();
				assert(nodebox[var].interior_contains(pt));

				bis = new SetBisect(var, pt);
				bis->left  = new SetLeaf(((SetLeaf *) this)->status);
				bis->right = new SetLeaf(((SetLeaf *) this)->status);
				bis->left  = bis->left->union_(p.first, other, otherbox, eps);
				bis->right = bis->right->union_(p.second, other, otherbox, eps);
				delete this;
			}
			else {
				bis = (SetBisect*) this;
				bis->left  = bis->left->union_(bis->left_box(nodebox), other, otherbox, eps);
				bis->right = bis->right->union_(bis->right_box(nodebox), other, otherbox, eps);
			}

			bis->left->father = bis;
			bis->right->father = bis;
			return bis->try_merge();

		} else {
			SetBisect* bisect_node = (SetBisect*) other;
			SetNode* this2 = this->union_(nodebox, bisect_node->left, bisect_node->left_box(otherbox), eps);
			// warning: cannot use this anymore (use this2 instead)
			return this2->union_(nodebox, bisect_node->right, bisect_node->right_box(otherbox), eps);
		}

		// *********************************************************************************************

//		SetBisect* bisect_node = (SetBisect*) other;
//		SetNode* this2 = this->union_(nodebox, bisect_node->left, bisect_node->left_box(otherbox), eps);
//		return this2->union_(nodebox, bisect_node->right, bisect_node->right_box(otherbox), eps);
	}
}

} // namespace ibex
