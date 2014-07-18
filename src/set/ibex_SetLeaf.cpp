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

SetLeaf::SetLeaf(BoolInterval status) : _status(status) {

}

BoolInterval SetLeaf::status() const {
	return _status;
}

bool SetLeaf::is_leaf() const {
	return true;
}

SetNode* SetLeaf::inter(const IntervalVector& nodebox, const IntervalVector& x, BoolInterval x_status, double eps) {
	if (x_status==MAYBE) {
		return this;
	} else if (nodebox.is_subset(x)) {
		assert(_status==MAYBE || _status==x_status);
		_status=x_status;
		return this;
	} else {
		SetNode* new_node=diff(nodebox, x, _status, x_status, eps);
		delete this; // warning: suicide, don't move it before previous line
		return new_node;
	}
}

SetNode* SetLeaf::contract_rec(const IntervalVector& nodebox, Ctc& ctc_in, Ctc& ctc_out, double eps) {

	if (_status!=MAYBE || nodebox.max_diam()<=eps)
		return this;
	else {
		int var=nodebox.extr_diam_index(false);
		pair<IntervalVector,IntervalVector> p=nodebox.bisect(var);
		double pt=p.first[var].ub();
		SetBisect* bis = new SetBisect(var, pt, new SetLeaf(MAYBE), new SetLeaf(MAYBE));
		delete this;
		return bis->contract_rec(nodebox, ctc_in, ctc_out, eps);
	}

}

void SetLeaf::to_vibes(color_code color_func, const IntervalVector& nodebox) const {
	vibes::drawBox(nodebox[0].lb(), nodebox[0].ub(), nodebox[1].lb(), nodebox[1].ub(), color_func(_status));
}


} // namespace ibex
