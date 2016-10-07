//============================================================================
//                                  I B E X                                   
// File        : ibex_SetLeaf.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : 13 juil. 2014
//============================================================================

#include <stdlib.h>
#include "ibex_SetLeaf.h"
#include "ibex_SetBisect.h"
#include "ibex_SetInterval.h"
#include "ibex_Sep.h"

using namespace std;

namespace ibex {


pair<SetNode*,SetLeaf*> diff(const IntervalVector& x, const IntervalVector& y, BoolInterval x_status, BoolInterval y_status, double eps) {

	assert(!x.is_empty());

	assert(x_status!=y_status);

	if (y.is_empty()) {
		return pair<SetNode*,SetLeaf*>(new SetLeaf(x_status),NULL);
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
				return pair<SetNode*,SetLeaf*>(new SetLeaf(x_status),NULL);
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

	SetLeaf* y_node = new SetLeaf(y_status);
	SetNode* bisect=y_node;

	// both x and y with status MAYBE -> skip
	if (y_status == x_status) {
		delete[] tmp;
		return pair<SetNode*,SetLeaf*>(y_node,y_node);
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

	delete[] tmp;
	return pair<SetNode*,SetLeaf*>(bisect,y_node);
}


SetLeaf::SetLeaf(BoolInterval status) : status(status) {

}

SetLeaf::~SetLeaf() {

}

bool SetLeaf::is_leaf() const {
	return true;
}

SetNode* SetLeaf::inter(bool iset, const IntervalVector& nodebox, Sep& sep, double eps) {

	if (status==NO || (iset && status==YES))
		return this;

	IntervalVector box1(nodebox);
	IntervalVector box2(nodebox);

	sep.separate(box1,box2);

	if (nodebox.max_diam()<=eps) {
		if (box1.is_empty()) status=sep.status1();
		else if (box2.is_empty()) status=sep.status2();
		else if (!iset) status = MAYBE;
		return this;
	}

	BoolInterval tmp_leaf2_status = sep.status1()==YES? MAYBE : YES;
	pair<SetNode*,SetLeaf*> new_nodes2=diff(nodebox, box1, sep.status1(), tmp_leaf2_status, 0);

	SetNode* root2=new_nodes2.first;
	SetLeaf* leaf2=new_nodes2.second;

	if (leaf2!=NULL) {

		BoolInterval tmp_leaf3_status = sep.status2()==YES? MAYBE : YES;
		pair<SetNode*,SetLeaf*> new_nodes3=diff(box1, box2, sep.status2(), tmp_leaf3_status, 0);

		SetNode* root3=new_nodes3.first;
		SetLeaf* leaf3=new_nodes3.second;

		if (leaf2==root2) {
			delete root2;
			root2=root3;
		} else {
			leaf2->replace_with(root3);
		}

		if (leaf3!=NULL) {

			IntervalVector box=box1 & box2;
			SetNode* root4;

			if (box.max_diam()>eps) {
				int var=box.extr_diam_index(false);
				pair<IntervalVector,IntervalVector> p=box.bisect(var);
				double pt=p.first[var].ub();
				assert(box[var].interior_contains(pt));

				SetNode* left = new SetLeaf(status);
				SetNode* right = new SetLeaf(status);

				SetBisect* bis = new SetBisect(var, pt);
				bis->left = left->inter(iset, p.first, sep, eps);
				bis->left->father = bis;
				bis->right = right->inter(iset, p.second, sep, eps);
				bis->right->father = bis;
				root4=bis->try_merge();
			} else {
				root4=new SetLeaf(status);
				root4=root4->inter(iset, box, sep, eps);
			}

			//TODO : we may have two sons with same status!

			if (leaf3==root2) {
				delete root2;
				root2=root4;
			} else {
				leaf3->replace_with(root4);
			}
		}
	}
	delete this;
	return root2;
}

SetNode* SetLeaf::inter(bool iset, const IntervalVector& nodebox, const IntervalVector& x, BoolInterval xstatus) {
	//cout << nodebox << " " << to_string(status)  << " inter " << x << " ";

	if (iset) {
		if (xstatus==MAYBE)
			return this;

		// TODO assert((xstatus==NO && !contains_in) || (xstatus==YES && !contains_out));
		if ((xstatus==NO && status==YES) || (xstatus==NO && status==YES))
			throw SetInterval::NoSet();

		// xstatus is either YES or NO
		if (nodebox.is_subset(x)) {
			status=xstatus;
			return this;
		}
	} else {
		if (status==NO || xstatus==YES)
			return this;

		// xstatus is either NO or MAYBE
		// and status is either YES or MAYBE
		if (nodebox.is_subset(x)) {
			status=xstatus;
			return this;
		}
	}

	if (status==xstatus)
		return this;
	else if (!nodebox.intersects(x))
		return this;
	else {

		// iset  : (status,xstatus) = (MAYBE,YES) or (MAYBE,NO)
		// !iset : (status,xstatus) = (YES,NO) or (YES,MAYBE) or (MAYBE,NO)

		// note: what is outside of x is considered to be "YES" if iset=false and "MAYBE" if iset=true.
		SetNode* new_node=diff(nodebox, x, status, xstatus, 0).first; //eps);
//		cout << "[diff] this=" << nodebox << " ("<<(status==YES?"Y":(status==NO?"N":"?")) << ") x=" <<
//				x << " (" << (xstatus==YES?"Y":(xstatus==NO?"N":"?")) << ") gives:";
//		new_node->print(cout,nodebox,0);

		delete this; // warning: suicide, don't move it before previous line

		return new_node;
	}
}

SetNode* SetLeaf::contract_no_diff(BoolInterval _status, const IntervalVector& nodebox, const IntervalVector& box) {
	if (!nodebox.intersects(box)) status=_status;
	return this;
}

SetNode* SetLeaf::union_(const IntervalVector& nodebox, const IntervalVector& x, BoolInterval xstatus) {
	//cout << nodebox << " " << to_string(status)  << " union << x << " ";

	if (status==YES || xstatus==NO || status==xstatus) {
		//cout << "this\n";
		return this;
	} else if (nodebox.is_subset(x)) {
		status=xstatus; // NO -> MAYBE or MAYBE -> IN
		 //cout << "this\n";
		return this;
	} else if (!nodebox.intersects(x))
		return this;
	else {
		// status=(MAYBE | NO), xstatus=(YES).
		SetNode* new_node=diff(nodebox, x, status, xstatus, 0).first; //eps);
		delete this; // warning: suicide, don't move it before previous line
		//cout << "gives "; new_node->print(cout,nodebox,0);
		return new_node;
	}
}

BoolInterval SetLeaf::is_superset(const IntervalVector& nodebox, const IntervalVector& box) const {
	if (!nodebox.intersects(box)) return YES;
	else return status;
}


void SetLeaf::visit(const IntervalVector& nodebox, SetVisitor& visitor) const {
	visitor.visit_leaf(nodebox, status);
}

void SetLeaf::print(ostream& os, const IntervalVector& nodebox, int shift) const {
	for (int i=0; i<shift; i++) os << ' ';
	os  << nodebox << " " << (status==YES?"Y":(status==NO?"N":"?")) << endl;
}

void SetLeaf::replace_with(SetNode* node) {
	if (this==node) return;
	if (father!=NULL) {
		if (this==father->left)
			father->left=node;
		else
			father->right=node;
		node->father=father;
	}
	delete this;
}
} // namespace ibex
