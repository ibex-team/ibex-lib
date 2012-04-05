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
#include "ibex_BasicDecorator.h"

namespace ibex {

Eval::Eval(const Function& f) : f(*new CompiledFunction<Domain>(f,BasicDecorator())),
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

void Eval::apply_fwd(const ExprApply& a, const Domain**, Domain& y) {
	EvalApplyLabel& l=(EvalApplyLabel&) y;
	l.fevl.eval(l.args_doms);
}

} // namespace ibex
