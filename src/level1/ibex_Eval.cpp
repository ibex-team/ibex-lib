/* ============================================================================
 * I B E X - Function basic evaluation
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Jan 14, 2012
 * ---------------------------------------------------------------------------- */

#include "ibex_Eval.h"
#include "ibex_Expr.h"

namespace ibex {

std::ostream& operator<<(std::ostream& os,const Domain& d) {
	switch (d.dim.type()) {
		case Dim::SCALAR:       os << d.i(); break;
		case Dim::ROW_VECTOR:
		case Dim::COL_VECTOR:   os << d.v(); break;
		case Dim::MATRIX:       os << d.m(); break;
		case Dim::MATRIX_ARRAY: os << d.ma(); break;
		}
	return os;
}


void EvalDecorator::decorate(const Function& f) const {
	if (f.expr().deco!=NULL) {
		return; //throw NonRecoverableException("Cannot re-decorate a function");
	} else {
		((EvalDecorator*) this)->visit(f.expr());
	}
}

void EvalDecorator::visit(const ExprNode& e) {
	e.acceptVisitor(*this);
}

void EvalDecorator::visit(const ExprIndex& idx) {

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

void EvalDecorator::visit(const ExprSymbol& e) {
	e.deco = new Domain(e.dim);
}

void EvalDecorator::visit(const ExprConstant& e) {
	e.deco = new Domain(e.dim);
}

void EvalDecorator::visit(const ExprNAryOp& e) {
	for (int i=0; i<e.nb_args; i++)
		visit(e.arg(i));

	e.acceptVisitor(*this);
}

void EvalDecorator::visit(const ExprBinaryOp& b) {
	visit(b.left);
	visit(b.right);
	b.deco = new Domain(b.dim);
}

void EvalDecorator::visit(const ExprUnaryOp& u) {
	visit(u.expr);
	u.deco = new Domain(u.dim);
}

void EvalDecorator::visit(const ExprVector& v) {
	v.deco = new Domain(v.dim);
}

void EvalDecorator::visit(const ExprApply& a) {
	a.deco = new EvalApplyLabel(a.dim, a.func);
	for (int i=0; i<a.nb_args; i++) {
		((EvalApplyLabel*) a.deco)->args_doms.set(i,(Domain&) (*a.arg(i).deco));
	}
}

EvalApplyLabel::EvalApplyLabel(const Dim& dim, const Function& f) : Domain(dim), args_doms(f.nb_symbols()), fevl(f) {

}

/*==========================================================================================*/

Eval::Eval(const Function& f) : f(*new CompiledFunction<Domain>(f,EvalDecorator())),
		symbolLabels(f.nb_symbols()), proper_compiled_func(true) {

	for (int i=0; i<f.nb_symbols(); i++) {
		symbolLabels.set(i,((Domain&) *f.symbol(i).deco));
	}
}

Eval::Eval(const CompiledFunction<Domain>& f) : f(f), symbolLabels(f.f.nb_symbols()), proper_compiled_func(false) {
	for (int i=0; i<f.f.nb_symbols(); i++) {
		symbolLabels.set(i,((Domain&) *f.f.symbol(i).deco));
	}
}

Eval::~Eval() {
	if (proper_compiled_func) delete (CompiledFunction<Domain>*) &f;
}

void Eval::Eval::vector_fwd(const ExprVector& v, const Domain** compL, Domain& y) {

	assert(v.type()!=Dim::SCALAR);
	assert(v.type()!=Dim::MATRIX_ARRAY);

	if (v.dim.is_vector()) {
		for (int i=0; i<v.length(); i++) y.v()[i]=compL[i]->i();
	}
	else {
		if (v.row_vector())
			for (int i=0; i<v.length(); i++) y.m().set_col(i,compL[i]->v());
		else
			for (int i=0; i<v.length(); i++) y.m().set_row(i,compL[i]->v());
	}
}

void Eval::apply_fwd(const ExprApply& a, const Domain** argL, Domain& y) {
	Eval& fevl=((EvalApplyLabel&) y).fevl;
	fevl.symbolLabels = ((EvalApplyLabel&) y).args_doms;  // upload data (in the function arguments' box).
	fevl.f.forward(fevl);
}

} // namespace ibex
