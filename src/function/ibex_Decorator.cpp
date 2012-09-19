//============================================================================
//                                  I B E X                                   
// File        : ibex_Decorator.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Apr 4, 2012
// Last Update : Jul 16, 2012
//============================================================================

#include "ibex_Decorator.h"
#include "ibex_Function.h"
#include "ibex_Expr.h"

namespace ibex {

void Decorator::decorate(const Function& f) {

	if (f.expr().deco.d) return; // already decorated

	f.expr().reset_visited();


	// we cannot just call visit(f.expr()) because
	// some symbols may not appear in the expression
	// and they have to be decorated. So we first
	// decorate all the symbols.
	for (int i=0; i<f.nb_symbols(); i++) {
		f.symbol(i).deco.visited=false;
		visit((const ExprNode&) f.symbol(i)); // cast to ExprNode, in order to set "visited" to true.
	}

	visit(f.expr()); // cast -> we know *this will not be modified
}

void Decorator::visit(const ExprNode& e) {
	if (!e.deco.visited) {
		e.deco.visited=true;
		e.acceptVisitor(*this);
	}
}

void Decorator::visit(const ExprIndex& idx) {

	visit(idx.expr);

	Domain& d=(Domain&) *idx.expr.deco.d;
	Domain& g=(Domain&) *idx.expr.deco.g;
	Domain& di=(Domain&) *idx.expr.deco.p;

	switch (idx.expr.type()) {
	case Dim::SCALAR:
		idx.deco.d = new Domain(d.i());
		break;
	case Dim::ROW_VECTOR:
	case Dim::COL_VECTOR:
		idx.deco.d = new Domain(d.v()[idx.index]);
		idx.deco.g = new Domain(g.v()[idx.index]);
		idx.deco.p = new Domain(g.v()[idx.index]);
		break;
	case Dim::MATRIX:
		idx.deco.d = new Domain(d.m()[idx.index],true);
		idx.deco.g = new Domain(g.m()[idx.index],true);
		idx.deco.p = new Domain(di.m()[idx.index],true);
		break;
	case Dim::MATRIX_ARRAY:
		idx.deco.d = new Domain(d.ma()[idx.index]);
		idx.deco.g = new Domain(g.ma()[idx.index]);
		idx.deco.p = new Domain(di.ma()[idx.index]);
		break;
	}
}

void Decorator::visit(const ExprLeaf& e) {
	e.deco.d = new Domain(e.dim);
	e.deco.g = new Domain(e.dim);
	e.deco.p = new Domain(e.dim);
}

void Decorator::visit(const ExprBinaryOp& b) {
	visit(b.left);
	visit(b.right);
	b.deco.d = new Domain(b.dim);
	b.deco.g = new Domain(b.dim);
	b.deco.p = new Domain(b.dim);
}

void Decorator::visit(const ExprUnaryOp& u) {
	visit(u.expr);
	const ExprTrans* t=dynamic_cast<const ExprTrans*>(&u);

	if (t && u.dim.is_vector()) {
		u.deco.d = new Domain(*u.expr.deco.d,true);
		u.deco.g = new Domain(*u.expr.deco.g,true);
		u.deco.p = new Domain(*u.expr.deco.p,true);
	} else {
		/* TODO: seems impossible to have references
		 in case of matrices... */
		u.deco.d = new Domain(u.dim);
		u.deco.g = new Domain(u.dim);
		u.deco.p = new Domain(u.dim);
	}
}

void Decorator::visit(const ExprNAryOp& a) {
	for (int i=0; i<a.nb_args; i++)
		visit(a.arg(i));
	a.deco.d = new Domain(a.dim);
	a.deco.g = new Domain(a.dim);
	a.deco.p = new Domain(a.dim);

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
	//	for (int i=0; i<a.nb_args; i++) {
	//		((BasicApplyLabel*) a.deco)->args_doms.set(i,(Domain&) (*a.arg(i).deco));
	//	}
}

/*
BasicApplyLabel::BasicApplyLabel(const Dim& dim, Function& f) :
		// same comment as above. We could make the root of the sub-function f
		// being a reference (set dynamically) to the corresponding node in the
		// DAG of the function.
		//
		Domain(dim), args_doms(f.nb_symbols()), f(f), fevl(f) {

}*/

} // end namespace ibex
