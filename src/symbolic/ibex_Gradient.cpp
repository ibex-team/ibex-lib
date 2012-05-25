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
#include "ibex_Eval.h"

namespace ibex {

void GradDecorator::decorate(const Function& f) const {
	((GradDecorator&) *this).visit(f.expr()); // // cast -> we know *this will not be modified
}

void GradDecorator::visit(const ExprNode& n) {
	n.acceptVisitor(*this);
}

void GradDecorator::visit(const ExprIndex& idx) {
	idx.deco.g = new Domain(idx.dim);
}

void GradDecorator::visit(const ExprSymbol& s) {
	s.deco.g = new Domain(s.dim);
}

void GradDecorator::visit(const ExprConstant& c) {
	c.deco.g = new Domain(c.dim);
}

void GradDecorator::visit(const ExprNAryOp& a) {
	a.deco.g = new Domain(a.dim);
}

void GradDecorator::visit(const ExprBinaryOp& b) {
	b.deco.g = new Domain(b.dim);
}

void GradDecorator::visit(const ExprUnaryOp& u) {
	u.deco.g = new Domain(u.dim);
}

void GradDecorator::visit(const ExprVector& v) {
	v.deco.g = new Domain(v.dim);
}

void GradDecorator::visit(const ExprApply&) {

}


void Gradient::gradient(const Function& f, const IntervalVector& box, IntervalVector& g) const {
	assert(f.expr().dim.is_scalar());
	assert(f.expr().deco.d);
	assert(f.expr().deco.g);


	f.eval(box);
	f.forward<Gradient>(*this);

	f.expr().deco.g->i()=1.0;
	f.backward<Gradient>(*this);
	for (int i=0; i<f.nb_symbols(); i++) {
		//g[i]=f.symbol_deriv
	}
}


void Gradient::jacobian(const Function& f, const IntervalVector& box, IntervalMatrix& J) const {
	assert(f.expr().dim.is_vector());
	assert(f.expr().deco.d);
	assert(f.expr().deco.g);

	int m=f.expr().dim.vec_size();

	f.eval(box);

	// calculate the gradient of each component of f
	for (int i=0; i<m; i++) {

		f.forward<Gradient>(*this);

		for (int i2=0; i2<m; i2++) {
			f.expr().deco.g->v()[i2]=(i2==i?1.0:0.0);
		}

		f.backward<Gradient>(*this);

		if (f.all_symbols_scalar())
			for (int j=0; j<f.nb_symbols(); j++) {
				J[i][j]=f.symbol_deriv[j].i();
			}
		else
			load(J[i],f.symbol_deriv);
	}
}


void Gradient::symbol_fwd(const ExprSymbol& s, ExprLabel& y) {
	switch (s.type()) {
		case Dim::SCALAR:       y.g->i()=0; break;
		case Dim::ROW_VECTOR:
		case Dim::COL_VECTOR:   y.g->v().clear(); break;
		case Dim::MATRIX:       y.g->m().clear(); break;
		case Dim::MATRIX_ARRAY: for (int i=0; i<s.dim.dim1; i++) y.g->ma()[i].clear(); break;
		}
}

void Gradient::vector_fwd(const ExprVector& v, const ExprLabel** x, ExprLabel& y) {
	if (v.dim.is_vector())
		y.g->v().clear();
	else
		y.g->m().clear();
}

void Gradient::apply_fwd(const ExprApply& a, ExprLabel**, ExprLabel& y) {
	switch (a.type()) {
		case Dim::SCALAR:       y.g->i()=0; break;
		case Dim::ROW_VECTOR:
		case Dim::COL_VECTOR:   y.g->v().clear(); break;
		case Dim::MATRIX:       y.g->m().clear(); break;
		case Dim::MATRIX_ARRAY: assert(false); break;
		}
}

void Gradient::index_bwd (const ExprIndex& idx, ExprLabel& exprL, const ExprLabel& result) {
	exprL.d->v()[idx.index] += result.d->i();

	switch (idx.type()) {
	case Dim::SCALAR:       exprL.g->v()[idx.index]+=result.g->i(); break;
	case Dim::ROW_VECTOR:   exprL.g->m()[idx.index]+=result.g->v(); break;
	case Dim::COL_VECTOR:   assert(false); break;
	case Dim::MATRIX:       exprL.g->ma()[idx.index]+=result.g->m(); break;
	case Dim::MATRIX_ARRAY: assert(false); /* impossible */ break;
	}
}

void Gradient::vector_bwd(const ExprVector& v, ExprLabel** compL, const ExprLabel& y) {
	if (v.dim.is_vector()) {
		for (int i=0; i<v.length(); i++) compL[i]->g->i()+=y.g->v()[i];
	}
	else {
		if (v.row_vector())
			for (int i=0; i<v.length(); i++) compL[i]->g->v()+=y.g->m()[i];
		else
			for (int i=0; i<v.length(); i++) compL[i]->g->v()+=y.g->m().col(i);
	}
}

void Gradient::apply_bwd (const ExprApply&, ExprLabel** x, const ExprLabel& y) {
// TO DO
}

void Gradient::sign_bwd  (const ExprSign& e,  ExprLabel& exprL, const ExprLabel& result) {
	if (exprL.d->i().contains(0)) exprL.g->i() += result.g->i()*Interval::POS_REALS;
	else ; // nothing to do: derivative is zero
}

void Gradient::abs_bwd (const ExprAbs& e, ExprLabel& exprL, const ExprLabel& result) {
	if (exprL.d->i().lb()>=0) exprL.g->i() += 1.0*result.g->i();
	else if (exprL.d->i().ub()<=0) exprL.g->i() += -1.0*result.g->i();
	else exprL.g->i() += Interval(-1,1)*result.g->i();
}

} // namespace ibex
