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
#include "ibex_Exception.h"

#include <sstream>

namespace ibex {

void Decorator::decorate(const Function& f) {

	if (f.expr().deco.d) return; // already decorated

	// we cannot just call visit(f.expr()) because:
	//
	// 1- some symbols may not appear in the expression
	// and they have to be decorated. So we first
	// decorate all the symbols.
	//
	// 2- we can check, in this way, that all the
	// symbols appearing in the expression are arguments
	// of the function (since they have to be already decorated)
	for (int i=0; i<f.nb_arg(); i++) {
		const ExprSymbol& x=f.arg(i);
		//visit((const ExprNode&) x); // don't (because of case 2- above)
		map.insert(x,true);
		x.deco.d = new Domain(x.dim);
		x.deco.g = new Domain(x.dim);
		x.deco.p = new Domain(x.dim);
		x.deco.af2 = new Affine2Domain(x.dim);
	}

	visit(f.expr()); // cast -> we know *this will not be modified
}

void Decorator::visit(const ExprNode& e) {
	if (!map.found(e)) {
		map.insert(e,true);
		e.acceptVisitor(*this);
	}
}

void Decorator::visit(const ExprIndex& idx) {

	visit(idx.expr);

	Domain& d=(Domain&) *idx.expr.deco.d;
	Domain& g=(Domain&) *idx.expr.deco.g;
	Domain& di=(Domain&) *idx.expr.deco.p;
	Affine2Domain& af2=(Affine2Domain&) *idx.expr.deco.af2;

	switch (idx.expr.type()) {
	case Dim::SCALAR:
		idx.deco.d = new Domain(d.i());
		idx.deco.g = new Domain(g.i());
		idx.deco.p = new Domain(di.i());
		idx.deco.af2 = new Affine2Domain(af2.i());
		break;
	case Dim::ROW_VECTOR:
	case Dim::COL_VECTOR:
		idx.deco.d = new Domain(d.v()[idx.index]);
		idx.deco.g = new Domain(g.v()[idx.index]);
		idx.deco.p = new Domain(di.v()[idx.index]);
		idx.deco.af2 = new Affine2Domain(af2.v()[idx.index]);
		break;
	case Dim::MATRIX:
		idx.deco.d = new Domain(d.m()[idx.index],true);
		idx.deco.g = new Domain(g.m()[idx.index],true);
		idx.deco.p = new Domain(di.m()[idx.index],true);
		idx.deco.af2 = new Affine2Domain(af2.m()[idx.index],true);
		break;
	case Dim::MATRIX_ARRAY:
		idx.deco.d = new Domain(d.ma()[idx.index]);
		idx.deco.g = new Domain(g.ma()[idx.index]);
		idx.deco.p = new Domain(di.ma()[idx.index]);
		idx.deco.af2 = new Affine2Domain(af2.ma()[idx.index]);
		break;
	}

}

void Decorator::visit(const ExprLeaf& e) {
	assert(false);
}

void Decorator::visit(const ExprConstant& e) {
	e.deco.d = new Domain(e.dim);
	e.deco.g = new Domain(e.dim);
	e.deco.p = new Domain(e.dim);
	e.deco.af2 = new Affine2Domain(e.dim);
}

void Decorator::visit(const ExprSymbol& e) {
	std::stringstream s;
	s << "Symbol\"" << e.name << "\" is not an argument of the function";
	ibex_error(s.str().c_str());
}

void Decorator::visit(const ExprBinaryOp& b) {
	visit(b.left);
	visit(b.right);
	b.deco.d = new Domain(b.dim);
	b.deco.g = new Domain(b.dim);
	b.deco.p = new Domain(b.dim);
	b.deco.af2 = new Affine2Domain(b.dim);
}

void Decorator::visit(const ExprUnaryOp& u) {
	visit(u.expr);
	const ExprTrans* t=dynamic_cast<const ExprTrans*>(&u);

	if (t && u.dim.is_vector()) {
		u.deco.d = new Domain(*u.expr.deco.d,true);
		u.deco.g = new Domain(*u.expr.deco.g,true);
		u.deco.p = new Domain(*u.expr.deco.p,true);
		u.deco.af2 = new Affine2Domain(*u.expr.deco.af2,true);
	} else {
		/* TODO: seems impossible to have references
		 in case of matrices... */
		u.deco.d = new Domain(u.dim);
		u.deco.g = new Domain(u.dim);
		u.deco.p = new Domain(u.dim);
		u.deco.af2 = new Affine2Domain(u.dim);
	}
}

void Decorator::visit(const ExprNAryOp& a) {
	for (int i=0; i<a.nb_args; i++)
		visit(a.arg(i));
	a.deco.d = new Domain(a.dim);
	a.deco.g = new Domain(a.dim);
	a.deco.p = new Domain(a.dim);
	a.deco.af2 = new Affine2Domain(a.dim);

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
