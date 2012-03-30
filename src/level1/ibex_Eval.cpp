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

void EvalDecorator::decorate(const Function& f) {
	undecorating = false;
	if (f.expr().deco!=NULL) {
		throw NonRecoverableException("Cannot re-decorate a function");
	} else {
		visit(f.expr());
	}
}

void EvalDecorator::undecorate(const Function& f) {
	undecorating = true;
	if (f.expr().deco==NULL) {
		throw NonRecoverableException("Cannot un-decorate a function without decoration");
	}
	visit(f.expr());
}

void EvalDecorator::visit(const ExprNode& e) {

		is_index=false;         // by default "e" is not an IndexExpr

		e.acceptVisitor(*this); // may set is_index to true

		if (!is_index) {        // an IndexExpr has a special treatment
			if (undecorating) {
				switch(e.type()) {
				case Dim::SCALAR:       delete &((EvalLabel*) e.deco)->i();  break;
				case Dim::ROW_VECTOR:
				case Dim::COL_VECTOR:   delete &((EvalLabel*) e.deco)->v();  break;
				case Dim::MATRIX:       delete &((EvalLabel*) e.deco)->m();  break;
				case Dim::MATRIX_ARRAY: delete &((EvalLabel*) e.deco)->ma(); break;
				}
				delete e.deco;
			}
			else {
				switch(e.type()) {
				case Dim::SCALAR:       e.deco = new EvalLabel(new Interval()); break;
				case Dim::ROW_VECTOR:   e.deco = new EvalLabel(new IntervalVector(e.dim.dim3)); break;
				case Dim::COL_VECTOR:   e.deco = new EvalLabel(new IntervalVector(e.dim.dim2)); break;
				case Dim::MATRIX:       e.deco = new EvalLabel(new IntervalMatrix(e.dim.dim2,e.dim.dim3)); break;
				case Dim::MATRIX_ARRAY: e.deco = new EvalLabel(new IntervalMatrixArray(e.dim.dim1,e.dim.dim2,e.dim.dim3)); break;
				}
			}
		}
}

void EvalDecorator::visit(const ExprIndex& idx) {

	visit(idx.expr);

	if (undecorating)
		delete idx.deco;
	else {
		void* domain;
		switch (idx.expr.type()) {
		case Dim::SCALAR:       domain = &((EvalLabel*) idx.expr.deco)->v()[idx.index]; break;
		case Dim::ROW_VECTOR:   domain = &((EvalLabel*) idx.expr.deco)->m()[idx.index]; break;
		case Dim::COL_VECTOR:   assert(false); /* see comment in ExprVector */ break;
		case Dim::MATRIX:       domain = &((EvalLabel*) idx.expr.deco)->ma()[idx.index]; break;
		case Dim::MATRIX_ARRAY: assert(false); /* impossible */ break;
		}
		idx.deco = new EvalLabel(domain);
	}

	is_index=true;
}

void EvalDecorator::visit(const ExprSymbol& s) {

}

void EvalDecorator::visit(const ExprConstant& c) {

}

void EvalDecorator::visit(const ExprNAryOp& e) {
	e.acceptVisitor(*this);
}

void EvalDecorator::visit(const ExprBinaryOp& b) {
	visit(b.left);
	visit(b.right);
}

void EvalDecorator::visit(const ExprUnaryOp& u) {
	visit(u.expr);
}

void EvalDecorator::visit(const ExprVector& v) {
	for (int i=0; i<v.nb_args; v++)
		visit(v.arg(i));
}

void EvalDecorator::visit(const ExprApply& a) {
	if (undecorating) {
		EvalApplyLabel* l =(EvalApplyLabel*) a.deco;
		l->cleanup(a.dim);
		delete &l->fbox;
		delete &l->fevl;
		delete l;
	}
	else {
		Domain* d = new Domain();
		for (int i=0; i<a.nb_args; i++)
			d->add(a.arg(i).dim);
		a.deco = new EvalApplyLabel(a.dim, *new Eval(a.func),*d);
	}

}

/*==========================================================================================*/

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

Eval::Eval(const Function& f) : f(*new CompiledFunction<EvalLabel>(f,EvalDecorator())),
		proper_compiled_func(true) {

	symbolLabels = new EvalLabel*[f.nb_symbols()];

	for (int i=0; i<f.nb_symbols(); i++) {
		symbolLabels[i] = f.symbol(i).deco;
	}
}

Eval::Eval(const CompiledFunction<EvalLabel>& f) : f(f), proper_compiled_func(false) { }

Eval::~Eval() {
	if (proper_compiled_func) delete (CompiledFunction<EvalLabel>*) &f;
	delete[] symbolLabels;
}

void Eval::Eval::vector_fwd(const ExprVector& v, const EvalLabel** compL, EvalLabel& y) {

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

void Eval::apply_fwd(const ExprApply& a, const EvalLabel** argL, EvalLabel& y) {
	// upload data (in the function arguments' box).
	Domain& b=((EvalApplyLabel&) y).fbox;
	for (int i=0; i<a.nb_args; i++) {
		switch(a.args[i]->type()) {
		case Dim::SCALAR:       b.get(i)=argL[i]->i(); break;
		case Dim::ROW_VECTOR:
		case Dim::COL_VECTOR:   b.vector(i)=argL[i]->v(); break;
		case Dim::MATRIX:       b.matrix(i)=argL[i]->m(); break;
		case Dim::MATRIX_ARRAY: assert(false); /* impossible */ break;
		}
	}
	((EvalApplyLabel&) y).fevl.eval(b);
}

} // namespace ibex
