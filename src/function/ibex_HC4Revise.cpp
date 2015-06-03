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
#include "ibex_AffineLinEval.h"

namespace ibex {

const double HC4Revise::RATIO = 0.1;

HC4Revise::HC4Revise(FwdMode mode) : fwd_mode(mode) {

}

//void HC4Revise::proj(const Function& f, const Domain& y, Array<Domain>& x) {
//	Eval().eval(f,x);
//	*f.expr().deco.d &= y; // "&" for the case of a function x->x
//	f.backward<HC4Revise>(*this);
//	// note: not very clean.
//	// the box x is not emptied if an EmptyBoxException is thrown
//	// before (this is done by the contractor).
//	load(x,f.arg_domains,f.nb_used_vars,f.used_var);
//}

#define EVAL(f,x) if (fwd_mode==INTERVAL_MODE) Eval().eval(f,x); else AffineLinEval().eval(f,x);

bool HC4Revise::proj(const Function& f, const Domain& y, IntervalVector& x) {
	EVAL(f,x);

	//std::cout << "forward:" << std::endl; f.cf.print();

	Domain& root=*f.expr().deco.d;

	if (root.is_empty()) { x.set_empty(); return false; }

	switch(y.dim.type()) {
	case Dim::SCALAR:       if (root.i().is_subset(y.i())) return true; break;
	case Dim::ROW_VECTOR:
	case Dim::COL_VECTOR:   if (root.v().is_subset(y.v())) return true; break;
	case Dim::MATRIX:       if (root.m().is_subset(y.m())) return true; break;
	case Dim::MATRIX_ARRAY: assert(false); /* impossible */ break;
	}

	root &= y;

	try {
		f.backward<HC4Revise>(*this);
	} catch (EmptyBoxException& e) {
		x.set_empty();
		return false;
	}
	//std::cout << "backward:" << std::endl; f.cf.print();

	f.read_arg_domains(x);

	return false;
}

void HC4Revise::proj(const Function& f, const Domain& y, ExprLabel** x) {
	EVAL(f,x);
	*f.expr().deco.d &= y;

	// if next instruction throws an EmptyBoxException,
	// it will be caught by proj(...,IntervalVector& x).
	// (it is a protected function, not called outside of the class
	// so there is no risk)
	f.backward<HC4Revise>(*this);

	Array<Domain> argD(f.nb_arg());

	for (int i=0; i<f.nb_arg(); i++) {
		argD.set_ref(i,*(x[i]->d));
	}

	f.read_arg_domains(argD);
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
