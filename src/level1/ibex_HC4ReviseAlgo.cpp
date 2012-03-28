//============================================================================
//                                  I B E X                                   
// File        : HC4Revise Algorithm
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Dec 31, 2011
// Last Update : 
//============================================================================

#include "ibex_HC4ReviseAlgo.h"

namespace ibex {

void HC4ReviseAlgo::vector_bwd(const ExprVector& v, EvalLabel** compL, const EvalLabel& y) {
	if (v.dim.is_vector()) {
		for (int i=0; i<v.length(); i++) compL[i]->i() &= y.v()[i];
	}
	else {
		if (v.in_rows())
			for (int i=0; i<v.length(); i++) compL[i]->v()=y.m().row(i);
		else
			for (int i=0; i<v.length(); i++) compL[i]->v()=y.m().col(i);
	}
}

void HC4ReviseAlgo::apply_bwd(const ExprApply& a, EvalLabel** argL, const EvalLabel& y) {
	// download data
	const Domain& b=((const EvalApplyLabel&) y).fbox;
	for (int i=0; i<a.nb_args; i++) {
		switch(a.args[i]->type()) {
		case Dim::SCALAR:       argL[i]->i() &=b.get(i);    break;
		case Dim::ROW_VECTOR:
		case Dim::COL_VECTOR:   argL[i]->v() &=b.vector(i); break;
		case Dim::MATRIX:       argL[i]->m() &=b.matrix(i);  break;
		case Dim::MATRIX_ARRAY: assert(false); /* impossible */ break;
		}
	}
}

void HC4ReviseAlgo::contract(Domain& box) {
	eval.forward(box);

	const ExprNode& root=eval.f.expr;
	EvalLabel& root_label=(EvalLabel&) *(root.deco);
	Interval right_cst(equality? 0: NEG_INFINITY, 0);

	switch(root.type()) {
	case Dim::SCALAR:       root_label.set_domain(right_cst); break;
	case Dim::ROW_VECTOR:   root_label.set_domain(IntervalVector(root.dim.dim2,right_cst)); break;
	case Dim::COL_VECTOR:   root_label.set_domain(IntervalVector(root.dim.dim3,right_cst)); break;
	case Dim::MATRIX:       root_label.set_domain(IntervalMatrix(root.dim.dim2,root.dim.dim3,right_cst)); break;
	case Dim::MATRIX_ARRAY: assert(false); /* impossible */ break;
	}

	eval.f.backward(*this);
}

} /* namespace ibex */
