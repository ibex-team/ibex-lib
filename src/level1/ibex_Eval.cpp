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
