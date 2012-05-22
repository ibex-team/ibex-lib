/* ============================================================================
 * I B E X - ibex_Gradient.cpp
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Jan 27, 2012
 * ---------------------------------------------------------------------------- */

#include "ibex_Gradient.h"

namespace ibex {

GradLabel::GradLabel(const Dim& dim) : Domain(dim), g(dim) { }

ApplyGradLabel::ApplyGradLabel(const Dim& dim, Function& f) : Domain(dim), GradLabel(dim), BasicApplyLabel(dim, f), fgrad(f) { }


void GradDecorator::decorate(const Function& f) const {
	if (f.expr().deco!=NULL) {
		return; //throw NonRecoverableException("Cannot re-decorate a function");
	}
	((GradDecorator&) *this).visit(f.expr()); // // cast -> we know *this will not be modified
}

void GradDecorator::visit(const ExprNode& n) {
	n.acceptVisitor(*this);
}

void GradDecorator::visit(const ExprIndex& idx) {
	idx.deco = new GradLabel(idx.dim);
}

void GradDecorator::visit(const ExprSymbol& s) {
	s.deco = new GradLabel(s.dim);
}

void GradDecorator::visit(const ExprConstant& c) {
	c.deco = new GradLabel(c.dim);
}

void GradDecorator::visit(const ExprNAryOp& a) {
	a.deco = new GradLabel(a.dim);
}

void GradDecorator::visit(const ExprBinaryOp& b) {
	b.deco = new GradLabel(b.dim);
}

void GradDecorator::visit(const ExprUnaryOp& u) {
	u.deco = new GradLabel(u.dim);
}

void GradDecorator::visit(const ExprVector& v) {
	v.deco = new GradLabel(v.dim);
}

void GradDecorator::visit(const ExprApply&) {

}

Gradient::Gradient(const Function& f) : eval(f) {

	f.decorate(GradDecorator());
}

void Gradient::calculate(const IntervalVector& box, IntervalVector& g) const {
	eval.eval(box);
	eval.f.backward<Gradient,GradLabel>(*this);
	//for (int i=0; i<)
	// TO COMPLETE
}

void Gradient::jacobian(const IntervalVector& box, IntervalMatrix& J) const {

}


void Gradient::index_bwd (const ExprIndex& idx, GradLabel& exprL, const GradLabel& result) {
	exprL.v()[idx.index] += result.i();

	switch (idx.type()) {
	case Dim::SCALAR:       exprL.g.v()[idx.index]+=result.g.i(); break;
	case Dim::ROW_VECTOR:   exprL.g.m()[idx.index]+=result.g.v(); break;
	case Dim::COL_VECTOR:   assert(false); break;
	case Dim::MATRIX:       exprL.g.ma()[idx.index]+=result.g.m(); break;
	case Dim::MATRIX_ARRAY: assert(false); /* impossible */ break;
	}
}

void Gradient::vector_bwd(const ExprVector& v, GradLabel** compL, const GradLabel& y) {
	if (v.dim.is_vector()) {
		for (int i=0; i<v.length(); i++) compL[i]->g.i()+=y.g.v()[i];
	}
	else {
		if (v.row_vector())
			for (int i=0; i<v.length(); i++) compL[i]->g.v()+=y.g.m()[i];
		else
			for (int i=0; i<v.length(); i++) compL[i]->g.v()+=y.g.m().col(i);
	}
}

void Gradient::apply_bwd (const ExprApply&, GradLabel** x, const GradLabel& y) {
	IntervalMatrix J=
}

void Gradient::sign_bwd  (const ExprSign& e,  GradLabel& exprL, const GradLabel& result) {
	if (exprL.i().contains(0)) exprL.g.i() += result.g.i()*Interval::POS_REALS;
	else ; // nothing to do: derivative is zero
}

void Gradient::abs_bwd (const ExprAbs& e, GradLabel& exprL, const GradLabel& result) {
	if (exprL.i().lb()>=0) exprL.g.i() += 1.0*result.g.i();
	else if (exprL.i().ub()<=0) exprL.g.i() += -1.0*result.g.i();
	else exprL.g.i() += Interval(-1,1)*result.g.i();
}

} // namespace ibex
