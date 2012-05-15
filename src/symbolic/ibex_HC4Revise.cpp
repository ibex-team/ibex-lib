//============================================================================
//                                  I B E X                                   
// File        : HC4Revise Algorithm
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Dec 31, 2011
// Last Update : 
//============================================================================

#include "ibex_HC4Revise.h"
#include "ibex_BasicDecorator.h"

namespace ibex {

HC4Revise::HC4Revise(const Function& f) : eval(f) {
	current_box = NULL;
}

void HC4Revise::vector_bwd(const ExprVector& v, Domain** compL, const Domain& y) {
	if (v.dim.is_vector()) {
		for (int i=0; i<v.length(); i++) compL[i]->i() &= y.v()[i];
	}
	else {
		if (v.row_vector())
			for (int i=0; i<v.length(); i++) compL[i]->v()=y.m().col(i);
		else
			for (int i=0; i<v.length(); i++) compL[i]->v()=y.m().row(i);
	}
}

void HC4Revise::apply_bwd(const ExprApply& a, Domain** argL, const Domain& y) {
	BasicApplyLabel& l=(BasicApplyLabel&) y;
	l.fevl.contract(l.args_doms,l);
}


} /* namespace ibex */
