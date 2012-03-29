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

void EvalDecorator::decorate(const Function& f) const {
	undecorating = false;
	if (f.expr().deco!=NULL) {
		throw NonRecoverableException("Cannot re-decorate a function");
	}
	((EvalDecorator&) *this).visit(f.expr()); // // cast -> we know *this will not be modified
}

void EvalDecorator::undecorate(const Function& f) const {
	undecorating = true;
	if (f.expr().deco==NULL) {
		throw NonRecoverableException("Cannot un-decorate a function without decoration");
	}
	((EvalDecorator&) *this).visit(f.expr()); // // cast -> we know *this will not be modified
}

void EvalDecorator::visit(const ExprNode& n) {
	n.acceptVisitor(*this);
}

void EvalDecorator::visit(const ExprIndex& idx) {
	if (undecorating) delete idx.deco;
	else idx.deco = new EvalLabel(); // domain of this node is a reference
}

void EvalDecorator::visit(const ExprSymbol& s) {
	if (undecorating) delete s.deco;
	else s.deco = new EvalLabel(); // domain of this node is a reference
}

void EvalDecorator::visit(const ExprConstant& c) {
	if (undecorating) { ((EvalLabel*) c.deco)->cleanup(c.dim); delete c.deco; }
	else c.deco = new EvalLabel(c.dim);
}

void EvalDecorator::visit(const ExprNAryOp& e) {
	e.acceptVisitor(*this);
}

void EvalDecorator::visit(const ExprBinaryOp& b) {
	if (undecorating) { ((EvalLabel*) b.deco)->cleanup(b.dim); delete b.deco; }
	b.deco = new EvalLabel(b.dim);
}

void EvalDecorator::visit(const ExprUnaryOp& u) {
	if (undecorating) { ((EvalLabel*) u.deco)->cleanup(u.dim); delete u.deco; }
	else u.deco = new EvalLabel(u.dim);
}

void EvalDecorator::visit(const ExprVector& v) {
	if (undecorating) { ((EvalLabel*) v.deco)->cleanup(v.dim); delete v.deco; }
	else v.deco = new EvalLabel(v.dim);
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


Eval::Eval(const Function& f) : f(*new CompiledFunction<EvalLabel>(f,EvalDecorator())), proper_compiled_func(true) { }

Eval::Eval(const CompiledFunction<EvalLabel>& f) : f(f), proper_compiled_func(false) { }

Eval::~Eval() {
	if (proper_compiled_func) delete (CompiledFunction<EvalLabel>*) &f;
}

void Eval::Eval::vector_fwd(const ExprVector& v, const EvalLabel** compL, EvalLabel& y) {

	assert(v.type()!=Dim::SCALAR);
	assert(v.type()!=Dim::MATRIX_ARRAY);

	if (v.dim.is_vector()) {
		for (int i=0; i<v.length(); i++) y.v()[i]=compL[i]->i();
	}
	else {
		if (v.in_rows())
			for (int i=0; i<v.length(); i++) y.m().set_row(i,compL[i]->v());
		else
			for (int i=0; i<v.length(); i++) y.m().set_col(i,compL[i]->v());
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
