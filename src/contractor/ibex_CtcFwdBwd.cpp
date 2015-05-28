/* ============================================================================
 * I B E X - HC4 Revise (forward-backward algorithm)
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Feb 27, 2012
 * ---------------------------------------------------------------------------- */

#include "ibex_CtcFwdBwd.h"


namespace ibex {

namespace {

/**
 * Initialize the domain "d" such that a constraint "f op 0" is equivalent to "f in d".
 */
void int_ctr_domain(Domain& d, CmpOp op) {

	Interval right_cst;

	switch (op) {
	case LT :
	case LEQ : right_cst=Interval::NEG_REALS; break;
	case EQ  : right_cst=Interval::ZERO;      break;
	case GEQ :
	case GT : right_cst=Interval::POS_REALS;  break;
	}

	switch(d.dim.type()) {
	case Dim::SCALAR:       d.i()=right_cst; break;
	case Dim::ROW_VECTOR:   d.v()=IntervalVector(d.dim.dim3,right_cst); break;
	case Dim::COL_VECTOR:   d.v()=IntervalVector(d.dim.dim2,right_cst); break;
	case Dim::MATRIX:       d.m()=IntervalMatrix(d.dim.dim2,d.dim.dim3,right_cst); break;
	case Dim::MATRIX_ARRAY: assert(false); /* impossible */ break;
	}
}

} // end anonymous namespace

CtcFwdBwd::CtcFwdBwd(Function& f, const Domain& y, FwdMode mode) : Ctc(f.nb_var()), f(f), d(f.expr().dim), hc4r(mode) {
	assert(f.expr().dim==y.dim);
	d = y;

	init();
}

CtcFwdBwd::CtcFwdBwd(Function& f, const Interval& y, FwdMode mode) : Ctc(f.nb_var()), f(f), d(Dim()), hc4r(mode) {
	assert(f.expr().dim==d.dim);
	d.i() = y;

	init();
}

CtcFwdBwd::CtcFwdBwd(Function& f, const IntervalVector& y, FwdMode mode) : Ctc(f.nb_var()), f(f), d(f.expr().dim), hc4r(mode) {
	assert(f.expr().dim.is_vector() && f.expr().dim.vec_size()==y.size());
	d.v() = y;

	init();
}

CtcFwdBwd::CtcFwdBwd(Function& f, const IntervalMatrix& y, FwdMode mode) : Ctc(f.nb_var()), f(f), d(f.expr().dim), hc4r(mode) {
	assert(f.expr().dim==Dim::matrix(y.nb_rows(),y.nb_cols()));
	d.m() = y;

	init();
}

CtcFwdBwd::CtcFwdBwd(Function& f, CmpOp op, FwdMode mode) : Ctc(f.nb_var()), f(f), d(f.expr().dim), hc4r(mode)  {
	int_ctr_domain(d,op);

	init();
}

CtcFwdBwd::CtcFwdBwd(const NumConstraint& ctr, FwdMode mode) : Ctc(ctr.f.nb_var()), f(ctr.f), d(ctr.f.expr().dim), hc4r(mode) {
	int_ctr_domain(d,ctr.op);

	init();
}

CtcFwdBwd::~CtcFwdBwd() {
	delete input;
	delete output;
}

void CtcFwdBwd::init() {

	input = new BitSet(0,nb_var-1,BitSet::empt);
	output = new BitSet(0,nb_var-1,BitSet::empt);

	int v;
	for (int i=0; i<f.nb_used_vars(); i++) {
		v=f.used_var(i);
		output->add(v);
		input->add(v);
	}
}

void CtcFwdBwd::contract(IntervalVector& box) {

	assert(box.size()==f.nb_var());

	//std::cout << " hc4 of " << f << "=" << d << " with box=" << box << std::endl;
	if (hc4r.proj(f,d,box)) {
		set_flag(INACTIVE);
		set_flag(FIXPOINT);
	}
	//std::cout << " ---> " << box << std::endl;

	if (box.is_empty()) {
		set_flag(FIXPOINT);
	}

	// Note: setting the FIXPOINT flag is incorrect when there
	// is no multiple occurrence because some operators
	// may be non-optimal in backward mode.
}

} // namespace ibex
