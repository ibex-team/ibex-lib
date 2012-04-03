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

void HC4ReviseAlgo::vector_bwd(const ExprVector& v, Domain** compL, const Domain& y) {
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

void HC4ReviseAlgo::apply_bwd(const ExprApply& a, Domain** argL, const Domain& y) {
	// download data
	Eval& fevl=((EvalApplyLabel&) y).fevl;
	//fevl.apply_fwd(p)
	fevl.symbolLabels = ((EvalApplyLabel&) y).args_doms;  // upload data (in the function arguments' box).
}

void HC4ReviseAlgo::contract(IntervalVector& box) {
	eval.forward(box);

	const ExprNode& root=eval.f.expr;
	Domain& root_label=(Domain&) *(root.deco);
	Interval right_cst(equality? 0: NEG_INFINITY, 0);

	switch(root.type()) {
	case Dim::SCALAR:       root_label.i()=right_cst; break;
	case Dim::ROW_VECTOR:   root_label.v()=IntervalVector(root.dim.dim3,right_cst); break;
	case Dim::COL_VECTOR:   root_label.v()=IntervalVector(root.dim.dim2,right_cst); break;
	case Dim::MATRIX:       root_label.m()=IntervalMatrix(root.dim.dim2,root.dim.dim3,right_cst); break;
	case Dim::MATRIX_ARRAY: assert(false); /* impossible */ break;
	}

	eval.f.backward(*this);

	box = eval.symbolLabels;
}

} /* namespace ibex */
