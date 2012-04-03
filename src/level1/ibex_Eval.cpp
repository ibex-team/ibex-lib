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
}

/*==========================================================================================*/

Eval::Eval(const Function& f) : f(*new CompiledFunction<Domain>(f,EvalDecorator())),
		proper_compiled_func(true) {

	symbolLabels = new Domain*[f.nb_symbols()];

	for (int i=0; i<f.nb_symbols(); i++) {
		symbolLabels[i] = (Domain*) f.symbol(i).deco;
	}
}

Eval::Eval(const CompiledFunction<Domain>& f) : f(f), proper_compiled_func(false) { }

Eval::~Eval() {
	if (proper_compiled_func) delete (CompiledFunction<Domain>*) &f;
	delete[] symbolLabels;
}

void Eval::read(const IntervalVector& x) const {
	int i=0;

	for (int s=0; s<f.f.nb_symbols(); s++) {
		const Dim& dim=f.f.symbol(s).dim;
		switch (dim.type()) {
		case Dim::SCALAR:
			symbolLabels[s]->i()=x[i++];
			break;
		case Dim::ROW_VECTOR:
		{
			IntervalVector& v=symbolLabels[s]->v();
			for (int j=0; j<dim.dim3; j++)
				v[j]=x[i++];
		}
		break;
		case Dim::COL_VECTOR:
		{
			IntervalVector& v=symbolLabels[s]->v();
			for (int j=0; j<dim.dim2; j++)
				v[j]=x[i++];
		}
		break;

		case Dim::MATRIX:
		{
			IntervalMatrix& M=symbolLabels[s]->m();
			for (int k=0; k<dim.dim2; k++)
				for (int j=0; j<dim.dim3; j++)
					M[k][j]=x[i++];
		}
		break;
		case Dim::MATRIX_ARRAY:
		{
			IntervalMatrixArray& A=symbolLabels[s]->ma();
			for (int l=0; l<dim.dim1; l++)
				for (int k=0; k<dim.dim2; k++)
					for (int j=0; j<dim.dim3; j++)
						A[l][k][j]=x[i++];
		}
		break;
		}
	}
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
	// upload data (in the function arguments' box).
	Eval& fevl=((EvalApplyLabel&) y).fevl;
	for (int i=0; i<a.nb_args; i++) {
		switch(a.args[i]->type()) {
		case Dim::SCALAR:       fevl.domain(i).i()=argL[i]->i(); break;
		case Dim::ROW_VECTOR:
		case Dim::COL_VECTOR:   fevl.domain(i).v()=argL[i]->v(); break;
		case Dim::MATRIX:       fevl.domain(i).m()=argL[i]->m(); break;
		case Dim::MATRIX_ARRAY: assert(false); /* impossible */ break;
		}
	}
	fevl.f.forward(fevl);
}

} // namespace ibex
