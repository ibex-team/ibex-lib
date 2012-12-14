//============================================================================
//                                  I B E X                                   
// File        : HC4Revise Algorithm
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Dec 31, 2011
// Last Update : 
//============================================================================

#include "ibex_HC4Revise.h"

namespace ibex {

void HC4Revise::proj(const Function& f, const Domain& y, Array<Domain>& x) {
	Eval().eval(f,x);
	*f.expr().deco.d = y;
	f.backward<HC4Revise>(*this);
	// note: not very clean.
	// the box x is not emptied if an EmptyBoxException is thrown
	// before (this is done by the contractor).
	load(x,f.arg_domains,f.nb_used_vars,f.used_var);
}

void HC4Revise::proj(const Function& f, const Domain& y, IntervalVector& x) {
	Eval().eval(f,x);
	*f.expr().deco.d = y;
	f.backward<HC4Revise>(*this);

	if (f.all_args_scalar()) {
		int j;
		for (int i=0; i<f.nb_used_vars; i++) {
			j=f.used_var[i];
			x[j]=f.arg_domains[j].i();
		}
	}
	else
		load(x,f.arg_domains,f.nb_used_vars,f.used_var);
}

void HC4Revise::proj(const Function& f, const Domain& y, ExprLabel** x) {
	Eval().eval(f,x);
	*f.expr().deco.d = y;
	f.backward<HC4Revise>(*this);

	Array<Domain> argD(f.nb_arg());

	for (int i=0; i<f.nb_arg(); i++) {
		argD.set_ref(i,*(x[i]->d));
	}

	load(argD,f.arg_domains,f.nb_used_vars,f.used_var);
}

void HC4Revise::vector_bwd(const ExprVector& v, ExprLabel** compL, const ExprLabel& y) {
	if (v.dim.is_vector()) {
		for (int i=0; i<v.length(); i++)
			if ((compL[i]->d->i() &= y.d->v()[i]).is_empty()) throw EmptyBoxException();
	}
	else {
		if (v.row_vector())
			for (int i=0; i<v.length(); i++) {
				if ((compL[i]->d->v()&=y.d->m().col(i)).is_empty()) throw EmptyBoxException();
			}
		else
			for (int i=0; i<v.length(); i++) {
				if ((compL[i]->d->v()&=y.d->m().row(i)).is_empty()) throw EmptyBoxException();
			}
	}
}

} /* namespace ibex */
