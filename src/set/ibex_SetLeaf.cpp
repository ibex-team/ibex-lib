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
SetNode* SetLeaf::inter_rec(bool sync, const IntervalVector& nodebox, Sep& sep, double eps) {

	if (status==NO || (sync && status==YES))
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
		bis->left = left->inter(sync, p.first, sep, eps);
		bis->left->father = bis;
		bis->right = right->inter(sync, p.second, sep, eps);
		bis->right->father = bis;
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
		if ((xstatus==NO && status==YES) || (xstatus==NO && status==YES))
			throw NoSet();

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
		SetNode* new_node=diff(nodebox, x, status, xstatus, 0); //eps);
//		cout << "[diff] this=" << nodebox << " ("<<(status==YES?"Y":(status==NO?"N":"?")) << ") x=" <<
//				x << " (" << (xstatus==YES?"Y":(xstatus==NO?"N":"?")) << ") gives:";
//		new_node->print(cout,nodebox,0);

		delete this; // warning: suicide, don't move it before previous line

		return new_node;
	}
}

SetNode* SetLeaf::contract_no_diff(const IntervalVector& nodebox, const IntervalVector& box) {
	if (!nodebox.intersects(box)) status=NO;
	return this;
}

SetNode* SetLeaf::contract_diff(const IntervalVector& nodebox, const IntervalVector& box) {
	if (status==NO)
		return this;
	else if (!nodebox.intersects(box)) {
		status=NO;
		return this;
	} else {
		SetNode* new_node=diff(nodebox, box, NO, status, 0);
		delete this;
		return new_node;
	}
}

SetNode* SetLeaf::union_(const IntervalVector& nodebox, const IntervalVector& x, BoolInterval xstatus, double eps) {
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
		SetNode* new_node=diff(nodebox, x, status, xstatus, 0); //eps);
		delete this; // warning: suicide, don't move it before previous line
		//cout << "gives "; new_node->print(cout,nodebox,0);
		return new_node;
	}
}

BoolInterval SetLeaf::is_superset(const IntervalVector& nodebox, const IntervalVector& box) const {
	if (!nodebox.intersects(box)) return YES;
	else return status;
}


void SetLeaf::visit_leaves(leaf_func func, const IntervalVector& nodebox) const {
	func(nodebox, status);
}

void SetLeaf::print(ostream& os, const IntervalVector& nodebox, int shift) const {
	for (int i=0; i<shift; i++) os << ' ';
	os  << nodebox << " " << (status==YES?"Y":(status==NO?"N":"?")) << endl;
}

} // namespace ibex
