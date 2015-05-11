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
	assert(xstatus<=UNK);

	if (xstatus==UNK || status==xstatus) {
		//cout << "this\n";
		return this;
	} else if (nodebox.is_subset(x)) {
		if (status!=UNK) throw NoSet();
		status=xstatus;
		//cout << "this\n";
		return this;
	} else {
		if ((nodebox & x).is_flat()) {
			//cout << "this\n";
			return this;
		}
		SetNode* new_node=diff(nodebox, x, status, xstatus, eps);
		delete this; // warning: suicide, don't move it before previous line
		//cout << "gives "; new_node->print(cout,nodebox,0);
		return new_node;
	}
}

SetNode* SetLeaf::sync_rec(const IntervalVector& nodebox, Sep& sep, double eps) {

	if (status<UNK || nodebox.max_diam()<=eps) {
		return this;
	} else {
		int var=nodebox.extr_diam_index(false);
		pair<IntervalVector,IntervalVector> p=nodebox.bisect(var);
		double pt=p.first[var].ub();
		assert(nodebox[var].interior_contains(pt));
		SetBisect* bis = new SetBisect(var, pt, new SetLeaf(UNK), new SetLeaf(UNK));
		delete this;
		return bis->sync_rec(nodebox, sep, eps);
	}
}



SetNode* SetLeaf::inter(const IntervalVector& nodebox, const IntervalVector& x, NodeType xstatus, double eps) {
	//cout << nodebox << " " << to_string(status)  << " inter " << x << " ";

	if (status==OUT || xstatus==IN || xstatus==UNK_IN_OUT) {
		//cout << "this\n";
		return this;
	} else if (nodebox.is_subset(x)) {
		status=xstatus; // status is either IN or UNK, xstatus is either OUT or UNK
		 //cout << "this\n";
		return this;
	} else if (status==UNK && xstatus==UNK) {
		 //cout << "this\n";
		return this;
	} else {
		// (status,xstatus)=(IN, OUT) or (IN, UNK) or (UNK, OUT).
		// note: (IN,OUT) should not occur if there is a boundary
		SetNode* new_node=diff(nodebox, x, status, xstatus, eps);
		delete this; // warning: suicide, don't move it before previous line
		//cout << "gives "; new_node->print(cout,nodebox,0);
		return new_node;
	}
}

SetNode* SetLeaf::inter_rec(const IntervalVector& nodebox, Sep& sep, double eps) {

	if (status<UNK)
		return this;
	else if (nodebox.max_diam()<=eps) {
		return this;  // status is IN_TMP and stays like this.
	} else {
		int var=nodebox.extr_diam_index(false);
		pair<IntervalVector,IntervalVector> p=nodebox.bisect(var);
		double pt=p.first[var].ub();
		assert(nodebox[var].interior_contains(pt));
		SetBisect* bis = new SetBisect(var, pt, new SetLeaf(status), new SetLeaf(status));
		delete this;
		return bis->inter_rec(nodebox, sep, eps);
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
