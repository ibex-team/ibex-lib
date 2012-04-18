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
#include "ibex_BasicDecorator.h"

namespace ibex {

HC4ReviseAlgo::HC4ReviseAlgo(const NumConstraint& ctr) :
		eval(ctr.f), equality(ctr.equality) { }

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
	HC4ReviseAlgo& fevl=((BasicApplyLabel&) y).fevl;
	fevl.backward(y);
}

void HC4ReviseAlgo::contract(IntervalVector& box) {
	eval.eval(box);

	const ExprNode& root=eval.f.expr();
	Domain& root_label=(Domain&) *(root.deco);
	Interval right_cst(equality? 0: NEG_INFINITY, 0);

	switch(root.type()) {
	case Dim::SCALAR:       root_label.i()=right_cst; break;
	case Dim::ROW_VECTOR:   root_label.v()=IntervalVector(root.dim.dim3,right_cst); break;
	case Dim::COL_VECTOR:   root_label.v()=IntervalVector(root.dim.dim2,right_cst); break;
	case Dim::MATRIX:       root_label.m()=IntervalMatrix(root.dim.dim2,root.dim.dim3,right_cst); break;
	case Dim::MATRIX_ARRAY: assert(false); /* impossible */ break;
	}

	eval.f.backward<HC4ReviseAlgo,Domain>(*this);

	box = eval.symbolLabels;
}

} /* namespace ibex */
