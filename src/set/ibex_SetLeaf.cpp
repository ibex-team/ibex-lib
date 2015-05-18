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

// note: we know  that targetbox is inside nodebox (see SetNode::inter(,...,Sep&,...)
SetNode* SetLeaf::inter_rec(bool sync, const IntervalVector& nodebox, Sep& sep, const IntervalVector& targetbox, double eps) {

	if (status==OUT || (sync && status==IN) || !nodebox.intersects(targetbox))
		return this;
	else if (nodebox.max_diam()<=eps) {
		// by simplicity, we consider nodebox ~ targetbox:
		status = inter(sync,__IBEX_SET_EQ__,status,UNK);
		//other (finer) possibility: return inter(nodebox,targetbox,UNK,eps);
		return this;
	} else {
		int var=nodebox.extr_diam_index(false);
		pair<IntervalVector,IntervalVector> p=nodebox.bisect(var);
		double pt=p.first[var].ub();
		assert(nodebox[var].interior_contains(pt));
		// note: bisection loses some information
		SetBisect* bis = new SetBisect(var, pt, new SetLeaf(subset(status)), new SetLeaf(subset(status)));
		delete this;
		return bis->inter_rec(sync, nodebox, sep, targetbox, eps);
		// we could also intersect the status of the result with the initial status
		// like we do in inter(...) (bisection loses information)
	}
}

SetNode* SetLeaf::inter(bool sync, const IntervalVector& nodebox, const IntervalVector& x, NodeType xstatus, double eps) {
	//cout << nodebox << " " << to_string(status)  << " inter " << x << " ";

	NodeType sub_xstatus=subset(xstatus);

	if (sync && xstatus==UNK)
		return this;
	else if (!sync && (status==OUT || xstatus==IN))
		return this;
	else if (!nodebox.intersects(x))
		return this;
	else if (nodebox.is_subset(x)) {
		// note: we avoid to check  set relation (nodebox==x?) if the sub-status is the same
		if (sub_xstatus!=xstatus && nodebox==x)
			status=inter(sync,__IBEX_SET_EQ__, status, xstatus);
		else
			status=inter(sync,__IBEX_SUBSET__, status, xstatus);
		return this;
	} else {

		// if x is not included in nodebox, the "IN" or the "OUT"
		// part of x may be outside or inside this.
		// note: we avoid to check set relation if the sub-status of x is the same as x
		if (sub_xstatus!=xstatus && nodebox.is_superset(x))
			xstatus = inter(sync,__IBEX_SUPSET__, status, xstatus);
		else
			xstatus = inter(sync,__IBEX_OVERLAP__, status, xstatus);

		NodeType sub_status=subset(status);

		// ex: status==UNK_OUT and xstatus==UNK
		// in this case there is no need calculating the diff
		// and then merging the result (we would lose the OUT information)
		if (sub_status==xstatus) {
			// ex: status=UNK_IN and xstatus=UNK
			// then status at the end is UNK.
			status = inter(sync,__IBEX_SET_EQ__, status, sub_status | xstatus);
			return this;
		}

		// at this point (sub_status,xstatus) can be:
		// (UNK, OUT), (UNK,UNK_OUT), (IN,>=UNK) or (IN, OUT) (the last should not happen in practice)
		// note: what is outside of x is considered to be "IN" (not "OUT")

		SetNode* new_node=diff(nodebox, x, sub_status, xstatus, eps);
		delete this; // warning: suicide, don't move it before previous line

		return new_node;
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
