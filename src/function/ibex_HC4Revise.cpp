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
#include "ibex_Eval.h"
#include "ibex_Affine2Eval.h"

namespace ibex {

const double HC4Revise::RATIO = 0.1;

HC4Revise::HC4Revise(FwdMode mode) : fwd_mode(mode) {

}

#define EVAL(f,x) if (fwd_mode==HC4Revise::INTERVAL_MODE) Eval().eval(f,x); else Affine2Eval().eval(f,x);

void HC4Revise::proj(const Function& f, const Domain& y, IntervalVector& x) {
	EVAL(f,x);
	*f.expr().deco.d &= y;
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
	EVAL(f,x);
	*f.expr().deco.d &= y;
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
