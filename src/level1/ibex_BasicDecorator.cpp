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
		((BasicDecorator*) this)->visit(f.expr());
	}
}

void BasicDecorator::visit(const ExprNode& e) {
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

void BasicDecorator::visit(const ExprNAryOp& e) {
	for (int i=0; i<e.nb_args; i++)
		visit(e.arg(i));

	e.acceptVisitor(*this);
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
	v.deco = new Domain(v.dim);
}

void BasicDecorator::visit(const ExprApply& a) {
	a.deco = new BasicApplyLabel(a.dim, a.func);

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

BasicApplyLabel::BasicApplyLabel(const Dim& dim, const Function& f) :
		Domain(dim), args_doms(f.nb_symbols()), fevl(NumConstraint(f,true)) {

}

} // end namespace ibex
