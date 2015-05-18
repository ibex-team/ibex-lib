//============================================================================
//                                  I B E X                                   
// File        : ibex_SetLeaf.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : 13 juil. 2014
//============================================================================

#include "ibex_SetLeaf.h"
#include "ibex_SetBisect.h"

using namespace std;

namespace ibex {

SetLeaf::SetLeaf(BoolInterval status) : status(status) {

}

SetLeaf::~SetLeaf() {

}

bool SetLeaf::is_leaf() const {
	return true;
}

// note: we know  that targetbox is inside nodebox (see SetNode::inter(,...,Sep&,...)
SetNode* SetLeaf::inter_rec(bool sync, const IntervalVector& nodebox, Sep& sep, const IntervalVector& targetbox, double eps) {

	if (status==NO || (sync && status==YES) || !nodebox.intersects(targetbox))
		return this;
	else if (nodebox.max_diam()<=eps) {
		if (!sync) status = MAYBE;
		return this;
	} else {
		int var=nodebox.extr_diam_index(false);
		pair<IntervalVector,IntervalVector> p=nodebox.bisect(var);
		double pt=p.first[var].ub();
		assert(nodebox[var].interior_contains(pt));

		SetNode* left = new SetLeaf(status);
		SetNode* right = new SetLeaf(status);

		SetBisect* bis = new SetBisect(var, pt);
		bis->left = left->inter(sync, p.first, sep, p.first & targetbox, eps);
		bis->right = right->inter(sync, p.second, sep, p.second & targetbox, eps);
		delete this;
		return bis->try_merge();
	}
}

SetNode* SetLeaf::inter(bool sync, const IntervalVector& nodebox, const IntervalVector& x, BoolInterval xstatus, double eps) {
	//cout << nodebox << " " << to_string(status)  << " inter " << x << " ";

	if (sync) {
		if (xstatus==MAYBE)
			return this;

		// TODO assert((xstatus==NO && !contains_in) || (xstatus==YES && !contains_out));
		assert((xstatus==NO && status!=YES) || (xstatus==YES && status!=NO));

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

		// sync  : (status,xstatus) = (MAYBE,YES) or (MAYBE,NO)
		// !sync : (status,xstatus) = (YES,NO) or (YES,MAYBE) or (MAYBE,NO)

		// note: what is outside of x is considered to be "YES" if sync=false and "MAYBE" if sync=true.
		SetNode* new_node=diff(nodebox, x, status, xstatus, eps);
//		cout << "[diff] this=" << nodebox << " ("<<(status==YES?"Y":(status==NO?"N":"?")) << ") x=" <<
//				x << " (" << (xstatus==YES?"Y":(xstatus==NO?"N":"?")) << ") gives:";
//		new_node->print(cout,nodebox,0);

		delete this; // warning: suicide, don't move it before previous line

		return new_node;
	}
}

SetNode* SetLeaf::union_(const IntervalVector& nodebox, const IntervalVector& x, BoolInterval xstatus, double eps) {
	//cout << nodebox << " " << to_string(status)  << " union << x << " ";

	if (status==YES || xstatus>YES) {
		//cout << "this\n";
		return this;
	} else if (nodebox.is_subset(x)) {
		status=YES;
		 //cout << "this\n";
		return this;
	} else {
		// status=(MAYBE | NO), xstatus=(YES).
		SetNode* new_node=diff(nodebox, x, status, YES, eps);
		delete this; // warning: suicide, don't move it before previous line
		//cout << "gives "; new_node->print(cout,nodebox,0);
		return new_node;
	}
}

void SetLeaf::visit_leaves(leaf_func func, const IntervalVector& nodebox) const {
	func(nodebox, status);
}

void SetLeaf::print(ostream& os, const IntervalVector& nodebox, int shift) const {
	for (int i=0; i<shift; i++) os << ' ';
	os  << nodebox << " " << (status==YES?"Y":(status==NO?"N":"?")) << endl;
}

} // namespace ibex
