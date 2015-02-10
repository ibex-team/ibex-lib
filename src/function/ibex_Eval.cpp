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

#include <typeinfo>
namespace ibex {

Domain& Eval::eval(const Function& f, ExprLabel** args) const {
	assert(f.expr().deco.d);

	Array<const Domain> argD(f.nb_arg());

	for (int i=0; i<f.nb_arg(); i++) {
		argD.set_ref(i,*(args[i]->d));
	}

	f.write_arg_domains(argD);

	//------------- for debug
	//	cout << "Function " << f.name << ", domains before eval:" << endl;
	//	for (int i=0; i<f.nb_arg(); i++) {
	//		cout << "arg[" << i << "]=" << f.arg_domains[i] << endl;
	//	}

	try {
		f.forward<Eval>(*this);
	} catch(EmptyBoxException&) {
		f.expr().deco.d->set_empty();
	}
	return *f.expr().deco.d;
}

Domain& Eval::eval(const Function& f, const Array<const Domain>& d) const {
	assert(f.expr().deco.d);

	f.write_arg_domains(d);

	try {
		f.forward<Eval>(*this);
	} catch(EmptyBoxException&) {
		f.expr().deco.d->set_empty();
	}
	return *f.expr().deco.d;
}

Domain& Eval::eval(const Function& f, const Array<Domain>& d) const {
	assert(f.expr().deco.d);

	f.write_arg_domains(d);

	try {
		f.forward<Eval>(*this);
	} catch(EmptyBoxException&) {
		f.expr().deco.d->set_empty();
	}
	return *f.expr().deco.d;
}

Domain& Eval::eval(const Function &f, const IntervalVector& box) const {
	assert(f.expr().deco.d);

	f.write_arg_domains(box);

	try {
		f.forward<Eval>(*this);
	} catch(EmptyBoxException&) {
		f.expr().deco.d->set_empty();
	}
	return *f.expr().deco.d;
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
