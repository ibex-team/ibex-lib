//============================================================================
//                                  I B E X                                   
// File        : ibex_BasicDecorator.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Apr 4, 2012
// Last Update : Apr 4, 2012
//============================================================================

#include "ibex_BasicDecorator.h"

namespace ibex {

void BasicDecorator::decorate(const Function& f) const {
	if (f.expr().deco!=NULL) {
		return; //throw NonRecoverableException("Cannot re-decorate a function");
	} else {
		int n=f.nb_nodes();
		visited = new bool[n];
		for (int i=0; i<n; i++) visited[i]=false;

		// we cannot just call visit(f.expr()) because
		// some symbols may not appear in the expression
		// and they have to be decorated. So we first
		// decorate all the symbols.
		for (int i=0; i<f.nb_symbols(); i++)
			((BasicDecorator*) this)->visit(f.symbol(i));

		((BasicDecorator*) this)->visit(f.expr());
		delete[] visited;
	}
}

void BasicDecorator::visit(const ExprNode& e) {
	if (!visited[e.id]) {
		visited[e.id]=true;
		e.acceptVisitor(*this);
	}
}

void BasicDecorator::visit(const ExprNAryOp& e) {
	e.acceptVisitor(*this);
}

void BasicDecorator::visit(const ExprIndex& idx) {

	visit(idx.expr);

	Domain& l=(Domain&) *idx.expr.deco;

	switch (idx.expr.type()) {
	case Dim::SCALAR:       idx.deco = new Domain(l.v()[idx.index]); break;
	case Dim::ROW_VECTOR:   idx.deco = new Domain(l.m()[idx.index],idx.type()==Dim::ROW_VECTOR); break;
	case Dim::COL_VECTOR:   assert(false); /* see comment in ExprVector */ break;
	case Dim::MATRIX:       idx.deco = new Domain(l.ma()[idx.index]); break;
	case Dim::MATRIX_ARRAY: assert(false); /* impossible */ break;
	}
}

void BasicDecorator::visit(const ExprSymbol& e) {
	e.deco = new Domain(e.dim);
}

void BasicDecorator::visit(const ExprConstant& e) {
	e.deco = new Domain(e.dim);
}

void BasicDecorator::visit(const ExprBinaryOp& b) {
	visit(b.left);
	visit(b.right);
	b.deco = new Domain(b.dim);
}

void BasicDecorator::visit(const ExprUnaryOp& u) {
	visit(u.expr);
	u.deco = new Domain(u.dim);
}

void BasicDecorator::visit(const ExprVector& v) {
	for (int i=0; i<v.nb_args; i++)
			visit(v.arg(i));

	v.deco = new Domain(v.dim);
}

void BasicDecorator::visit(const ExprApply& a) {
	for (int i=0; i<a.nb_args; i++)
			visit(a.arg(i));

	a.deco = new BasicApplyLabel(a.dim, (Function&) a.func);

	/* we could also be more efficient by making symbolLabels of a.deco->fevl
	 * direct references to the arguments' domain.
	 * However if the same function (hence the same decoration) is
	 * used at different places, the references would have to be set
	 * dynamically to the actual arguments domains, which is not very clean
	 * (would probably require a "set_reference" function in the class Domain).
	 * The advantage:
	 * - there would be no "args_doms" field in BasicApplyLabel
	 * - there would be no copy in fwd_apply and bwd_apply.
	 * The last point is crucial in case of symbols that are IntervalMatrixArray
	 * (i.e., that have potentially big domains w.r.t. memory).
	 */
	for (int i=0; i<a.nb_args; i++) {
		((BasicApplyLabel*) a.deco)->args_doms.set(i,(Domain&) (*a.arg(i).deco));
	}
}

BasicApplyLabel::BasicApplyLabel(const Dim& dim, Function& f) :
		/* same comment as above. We could make the root of the sub-function f
		 * being a reference (set dynamically) to the corresponding node in the
		 * DAG of the function.
		 */
		Domain(dim), args_doms(f.nb_symbols()), feq(f,true), fevl(feq) {

}

} // end namespace ibex
