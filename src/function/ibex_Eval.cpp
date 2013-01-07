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
#include <typeinfo>


Domain& Eval::eval(const Function& f, ExprLabel** args) const {
	assert(f.expr().deco.d);

	Array<const Domain> argD(f.nb_arg());

	for (int i=0; i<f.nb_arg(); i++) {
		argD.set_ref(i,*(args[i]->d));
	}

	load(f.arg_domains,argD,f.nb_used_vars,f.used_var);

	//------------- for debug
	//	cout << "Function " << f.name << ", domains before eval:" << endl;
	//	for (int i=0; i<f.nb_arg(); i++) {
	//		cout << "arg[" << i << "]=" << f.arg_domains[i] << endl;
	//	}

	return *f.forward<Eval>(*this).d;
}

Domain& Eval::eval(const Function& f, const Array<const Domain>& d) const {
	assert(f.expr().deco.d);

	load(f.arg_domains,d,f.nb_used_vars,f.used_var);

	return *f.forward<Eval>(*this).d;
}

Domain& Eval::eval(const Function& f, const Array<Domain>& d) const {
	assert(f.expr().deco.d);

	load(f.arg_domains,d,f.nb_used_vars,f.used_var);

	return *f.forward<Eval>(*this).d;
}

Domain& Eval::eval(const Function &f, const IntervalVector& box) const {
	assert(f.expr().deco.d);

	if (f.all_args_scalar()) {
		int j;
		for (int i=0; i<f.nb_used_vars; i++) {
			j=f.used_var[i];
			f.arg_domains[j].i()=box[j];
		}
	}
	else
		load(f.arg_domains,box,f.nb_used_vars,f.used_var); // load the domains of all the symbols

	return *f.forward<Eval>(*this).d;
}

void Eval::vector_fwd(const ExprVector& v, const ExprLabel** compL, ExprLabel& y) {

	assert(v.type()!=Dim::SCALAR);
	assert(v.type()!=Dim::MATRIX_ARRAY);

	if (v.dim.is_vector()) {
		for (int i=0; i<v.length(); i++) y.d->v()[i]=compL[i]->d->i();
	}
	else {
		if (v.row_vector())
			for (int i=0; i<v.length(); i++) y.d->m().set_col(i,compL[i]->d->v());
		else
			for (int i=0; i<v.length(); i++) y.d->m().set_row(i,compL[i]->d->v());
	}
}

} // namespace ibex
