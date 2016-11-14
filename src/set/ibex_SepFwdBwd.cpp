//============================================================================
//                                  I B E X
// File        : Separator for constraint "f(x) op 0" or "f(x) in y"
// Authors     : Gilles Chabert, Benoit Desrochers
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Mar 26, 2015
// Last Update : Mar 26, 2015
//============================================================================

#include "ibex_SepFwdBwd.h"
#include "ibex_CtcHC4.h"
#include "ibex_CtcUnion.h"
#include "ibex_CtcIdentity.h"

namespace ibex {

namespace {

Ctc* _neg(Function& f, CmpOp& op) {

	if (op==EQ) return new CtcIdentity(f.nb_var());

	Interval x;

	// since the "diff" does not calculate open bounds, we can
	// take (-oo,0] even if op=LT.
	if (op==LT || op==LEQ)  x=Interval::NEG_REALS;
	else x=Interval::POS_REALS;

	Domain d(f.expr().dim);

	switch(f.expr().dim.type()) {
	case Dim::SCALAR:       d.i()=x; break;
	case Dim::ROW_VECTOR:
	case Dim::COL_VECTOR:   d.v().init(x); break;
	case Dim::MATRIX:       d.m().init(x); break;
	}

	return new CtcNotIn(f,d);
}

}

SepFwdBwd::SepFwdBwd(Function& f, CmpOp op) : SepCtcPair(*_neg(f,op), *new CtcFwdBwd(f,op)) { }

SepFwdBwd::SepFwdBwd(const System& sys) : SepCtcPair(*new CtcUnion(sys),*new CtcHC4(sys)) { }

} // end namespace
