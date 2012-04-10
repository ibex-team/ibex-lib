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
#include <typeinfo>
Eval::Eval(const Function& f) : f(f), symbolLabels(f.nb_symbols()) {

	f.decorate(BasicDecorator());

	for (int i=0; i<f.nb_symbols(); i++) {
		symbolLabels.set(i,((Domain&) *f.symbol(i).deco));
	}
}

Eval::~Eval() {

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
	BasicApplyLabel& l=(BasicApplyLabel&) y;
	//cout << "args Doms=";
	//for (int i=0; i<l.args_doms.size(); i++)
	//	cout << l.args_doms[i] << " ";
	//	cout << endl;
	((Domain&) l) = l.fevl.eval.eval(l.args_doms);
}

} // namespace ibex
