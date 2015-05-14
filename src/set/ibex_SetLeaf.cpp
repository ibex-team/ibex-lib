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

// =========== shortcuts ==================
#define IN         __IBEX_IN__
#define OUT        __IBEX_OUT__
#define UNK        __IBEX_UNK__
#define UNK_IN     __IBEX_UNK_IN__
#define UNK_OUT    __IBEX_UNK_OUT__
#define UNK_IN_OUT __IBEX_UNK_IN_OUT__
// ========================================

namespace ibex {

SetLeaf::SetLeaf(NodeType status) : SetNode(status) { }

SetLeaf::~SetLeaf() {

}

bool SetLeaf::is_leaf() const {
	return true;
}

//SetNode* SetLeaf::sync(const IntervalVector& nodebox, const IntervalVector& x, NodeType xstatus, double eps, Mode mode) {
//	switch (mode) {
//	case SYNC: return sync(nodebox, x, xstatus, eps);
//	case INTER: return inter(nodebox, x, xstatus, eps);
//	case UNION: not_implemented("SetLeaf::sync with union"); return this; break;
//	default : ibex_error("SetLeaf::sync: unknown mode"); return this;
//	}
//}

SetNode* SetLeaf::sync(const IntervalVector& nodebox, const IntervalVector& x, NodeType xstatus, double eps) {
	//cout << nodebox << " " << to_string(status)  << " sync " << x << " ";

	if (!nodebox.intersects(x) || status<UNK) {
		//cout << "this\n";
		return this;
	} else if (nodebox.is_subset(x)) {
		if (xstatus<UNK || nodebox==x) status=status & xstatus; // may throw Noset.
		//cout << "this\n";
		return this;
	} else {
		// if x is not included in nodebox, the "IN" or the "OUT"
		// part of x may be outside or inside this.
		if (xstatus>UNK && !nodebox.is_superset(x))
			xstatus = UNK;

		if (xstatus==UNK) { // => useles to go further
			//cout << "this\n";
			return this; // there may be no more IN or OUT anymore.
		}

		// nodebox is not included in x so the "IN" or the "OUT" part of this node may be outside or inside x.
		// => We set status to "UNK" before calculating the diff.
		if (status>=UNK)
			status = UNK;

		SetNode* new_node=diff(nodebox, x, UNK, xstatus, eps);
		delete this; // warning: suicide, don't move it before previous line
		//cout << "gives "; new_node->print(cout,nodebox,0);
		return new_node;
	}
}

SetNode* SetLeaf::sync_rec(const IntervalVector& nodebox, Sep& sep, const IntervalVector& targetbox, double eps) {

	if (status<UNK || nodebox.max_diam()<=eps) {
		return this;
	} else {
		int var=nodebox.extr_diam_index(false);
		pair<IntervalVector,IntervalVector> p=nodebox.bisect(var);
		double pt=p.first[var].ub();
		assert(nodebox[var].interior_contains(pt));
		SetBisect* bis = new SetBisect(var, pt, new SetLeaf(UNK), new SetLeaf(UNK));
		delete this;
		return bis->sync_rec(nodebox, sep, targetbox, eps);
	}
}



SetNode* SetLeaf::inter(const IntervalVector& nodebox, const IntervalVector& x, NodeType xstatus, double eps) {
	//cout << nodebox << " " << to_string(status)  << " inter " << x << " ";

	NodeType sub_xstatus=subset(xstatus);

	if (status==OUT || xstatus==IN || !nodebox.intersects(x)) {
		//cout << "this\n";
		return this;
	} else if (nodebox.is_subset(x)) {
		if (sub_xstatus!=xstatus && nodebox==x) status=ibex::inter(status,xstatus);
		else status=ibex::inter(status,sub_xstatus);
		return this;
	} else {
		// if x is not included in nodebox, the "IN" or the "OUT"
		// part of x may be outside or inside this.
		if (sub_xstatus!=xstatus && !nodebox.is_superset(x))
			xstatus = sub_xstatus;

		NodeType sub_status=subset(status);

		// nodebox is not included in x so the "IN" or the "OUT" part of this node may be outside or inside x.
		xstatus = ibex::inter(sub_status,xstatus);

		// ex: status==UNK_OUT and xstatus==UNK
		// in this case there is no need calculating the diff
		// and then merging the result (we would lose the OUT information)
		if (sub_status==xstatus) {
			// ex: status=UNK_IN and xstatus=UNK
			// then status at the end is UNK.
			status = ibex::inter(status, ibex::_union(sub_status,xstatus));
			return this;
		}

		// at this point (sub_status,xstatus) can be:
		// (UNK, OUT), (UNK,UNK_OUT), (IN,>=UNK) or (IN, OUT) (the last should not happen in practice)
		// note: what is outside of x is considered to be "IN" (not "OUT")
		SetNode* new_node=diff(nodebox, x, sub_status, xstatus, eps);
		delete this; // warning: suicide, don't move it before previous line
		//cout << "gives "; new_node->print(cout,nodebox,0);
		return new_node;
	}
}

SetNode* SetLeaf::inter_rec(const IntervalVector& nodebox, Sep& sep, const IntervalVector& targetbox, double eps) {

	if (status==OUT)
		return this;
	else if (nodebox.max_diam()<=eps) {
		// we know that targetbox is inside nodebox (see SetNode::inter(,...,Sep&,...)
		status = UNK;
		return this;
	} else {
		int var=nodebox.extr_diam_index(false);
		pair<IntervalVector,IntervalVector> p=nodebox.bisect(var);
		double pt=p.first[var].ub();
		assert(nodebox[var].interior_contains(pt));
		SetBisect* bis = new SetBisect(var, pt, new SetLeaf(status), new SetLeaf(status));
		delete this;
		return bis->inter_rec(nodebox, sep, targetbox, eps);
	}
}

SetNode* SetLeaf::union_(const IntervalVector& nodebox, const IntervalVector& x, NodeType xstatus, double eps) {
	//cout << nodebox << " " << to_string(status)  << " union << x << " ";
	assert(xstatus<=UNK);

	if (status==IN || xstatus>IN) {
		//cout << "this\n";
		return this;
	} else if (nodebox.is_subset(x)) {
		status=IN;
		 //cout << "this\n";
		return this;
	} else {
		// status=(UNK | OUT), xstatus=(IN).
		SetNode* new_node=diff(nodebox, x, status, IN, eps);
		delete this; // warning: suicide, don't move it before previous line
		//cout << "gives "; new_node->print(cout,nodebox,0);
		return new_node;
	}
}

void SetLeaf::visit_leaves(leaf_func func, const IntervalVector& nodebox) const {
	func(nodebox, status==IN? YES : (status==OUT? NO : MAYBE));
}

void SetLeaf::print(ostream& os, const IntervalVector& nodebox, int shift) const {
	for (int i=0; i<shift; i++) os << ' ';
	os  << nodebox << " " << to_string(status) << endl;
}

} // namespace ibex
