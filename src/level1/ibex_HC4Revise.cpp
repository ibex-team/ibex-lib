//============================================================================
//                                  I B E X                                   
// File        : ibex_HC4Revise.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Dec 31, 2011
// Last Update : 
//============================================================================

#include "ibex_HC4Revise.h"

namespace ibex {

void HC4Revise::vector_bwd(const ExprVector& v, EvalLabel** compL, const EvalLabel& y) {
	if (v.type()==Dim::VECTOR) {
		assert(v.in_rows); // a vector is necessarily a column vector
		for (int i=0; i<v.size(); i++) compL[i]->i() &= y.v()[i];
	}
	else {
		if (v.in_rows)
			for (int i=0; i<v.size(); i++) compL[i]->v()=y.m().row(i);
		else
			for (int i=0; i<v.size(); i++) compL[i]->v()=y.m().col(i);
	}
}

void HC4Revise::apply_bwd(const ExprApply& a, EvalLabel** argL, const EvalLabel& y) {
	// download data
	const Domain& b=((const EvalApplyLabel&) y).fbox;
	for (int i=0; i<a.nb_args(); i++) {
		switch(a.args[i]->type()) {
		case Dim::SCALAR:       argL[i]->i() &=b.get(i);    break;
		case Dim::VECTOR:       argL[i]->v() &=b.vector(i); break;
		case Dim::MATRIX:       argL[i]->m() &=b.matrix(i);  break;
		case Dim::MATRIX_ARRAY: assert(false); /* impossible */ break;
		}
	}
}

void HC4Revise::contract(Domain& box) {
	eval.forward(box);
	eval.f.backward(*this);
}

} /* namespace ibex */
