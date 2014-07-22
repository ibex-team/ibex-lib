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

#include "/home/gilles/apps/VIBES-0.2.0/client-api/C++/src/vibes.h"

using namespace std;

namespace ibex {

SetLeaf::SetLeaf(SetType status) : SetNode(status) {
	if (status>UNK && status!=IN_TMP) {
		ibex_error("cannot set multiple status to SetLeaf");
	}
}
SetLeaf::~SetLeaf() {

}

bool SetLeaf::is_leaf() const {
	return true;
}

//SetNode* SetLeaf::sync(const IntervalVector& nodebox, const IntervalVector& x, SetType xstatus, double eps, Mode mode) {
//	switch (mode) {
//	case SYNC: return sync(nodebox, x, xstatus, eps);
//	case INTER: return inter(nodebox, x, xstatus, eps);
//	case UNION: not_implemented("SetLeaf::sync with union"); return this; break;
//	default : ibex_error("SetLeaf::sync: unknown mode"); return this;
//	}
//}

SetNode* SetLeaf::sync(const IntervalVector& nodebox, const IntervalVector& x, SetType xstatus, double eps) {
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

SetNode* SetLeaf::sync_rec(const IntervalVector& nodebox, Bracket& br, double eps) {

	if (status<UNK || nodebox.max_diam()<=eps) {
		return this;
	} else {
		int var=nodebox.extr_diam_index(false);
		pair<IntervalVector,IntervalVector> p=nodebox.bisect(var);
		double pt=p.first[var].ub();
		SetBisect* bis = new SetBisect(var, pt, new SetLeaf(UNK), new SetLeaf(UNK));
		delete this;
		return bis->sync_rec(nodebox, br, eps);
	}
}



SetNode* SetLeaf::inter(const IntervalVector& nodebox, const IntervalVector& x, SetType xstatus, double eps) {
	//cout << nodebox << " " << to_string(status)  << " inter " << x << " ";
	assert(xstatus<=UNK);

	if (status<UNK || xstatus==UNK) {
		//cout << "this\n";
		return this;
	} else if (nodebox.is_subset(x)) {
		if (xstatus==IN && status==IN_TMP) status=IN; // if status==UNK, it remains UNK.
		else if (xstatus==OUT ) status=OUT;
		 //cout << "this\n";
		return this;
	} else {
		// status=(UNK | IN_TMP), xstatus=(IN | OUT).
		SetNode* new_node=diff(nodebox, x, status, xstatus, eps);
		delete this; // warning: suicide, don't move it before previous line
		//cout << "gives "; new_node->print(cout,nodebox,0);
		return new_node;
	}
}

SetNode* SetLeaf::inter_rec(const IntervalVector& nodebox, Bracket& br, double eps) {

	if (status<UNK)
		return this;
	else if (nodebox.max_diam()<=eps) {
		return this;  // status is IN_TMP and stays like this.
	} else {
		int var=nodebox.extr_diam_index(false);
		pair<IntervalVector,IntervalVector> p=nodebox.bisect(var);
		double pt=p.first[var].ub();
		SetBisect* bis = new SetBisect(var, pt, new SetLeaf(status), new SetLeaf(status));
		delete this;
		return bis->inter_rec(nodebox, br, eps);
	}
}

void SetLeaf::to_vibes(color_code color_func, const IntervalVector& nodebox) const {
	if (nodebox.min_diam()<0.02) return;
	double dec=0; //0.01;
	vibes::drawBox(nodebox[0].lb()+dec, nodebox[0].ub()-dec, nodebox[1].lb()+dec, nodebox[1].ub()-dec, color_func(status));
}

void SetLeaf::print(ostream& os, const IntervalVector& nodebox, int shift) const {
	for (int i=0; i<shift; i++) os << ' ';
	os  << nodebox << " " << to_string(status) << endl;
}

void SetLeaf::set_in_tmp() {
	if (status==IN) status=IN_TMP;
}

void SetLeaf::unset_in_tmp() {
	if (status==IN_TMP) status=UNK;
}

} // namespace ibex
