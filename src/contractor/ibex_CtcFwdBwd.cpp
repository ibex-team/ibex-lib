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

CtcFwdBwd::CtcFwdBwd(Function& f, CmpOp op, FwdMode mode) : ctr(f,op), hc4r(mode) {

	int nb_var = f.nb_var();
	input = new BoolMask(nb_var);
	output = new BoolMask(nb_var);

	int v;
	for (int i=0; i<f.nb_used_vars(); i++) {
		v=f.used_var(i);
		(*output)[v]=(*input)[v]=true;
	}
}

CtcFwdBwd::CtcFwdBwd(const NumConstraint& ctr, FwdMode mode) : ctr(ctr.f,ctr.op), hc4r(mode) {

	int nb_var = ctr.f.nb_var();

	input = new BoolMask(nb_var);
	output = new BoolMask(nb_var);

	int v;
	for (int i=0; i<ctr.f.nb_used_vars(); i++) {
		v=ctr.f.used_var(i);
		(*output)[v]=(*input)[v]=true;
	}
}

CtcFwdBwd::~CtcFwdBwd() {
	delete input;
	delete output;
}

void CtcFwdBwd::contract(IntervalVector& box) {

	assert(box.size()==ctr.f.nb_var());

	const Dim& d=ctr.f.expr().dim;
	Domain root_label(d);
	Interval right_cst;

	switch (ctr.op) {
	case LT :
	case LEQ : right_cst=Interval(NEG_INFINITY, 0); break;
	case EQ  : right_cst=Interval(0,0); break;
	case GEQ :
	case GT : right_cst=Interval(0,POS_INFINITY); break;
	}

	switch(d.type()) {
	case Dim::SCALAR:       root_label.i()=right_cst; break;
	case Dim::ROW_VECTOR:   root_label.v()=IntervalVector(d.dim3,right_cst); break;
	case Dim::COL_VECTOR:   root_label.v()=IntervalVector(d.dim2,right_cst); break;
	case Dim::MATRIX:       root_label.m()=IntervalMatrix(d.dim2,d.dim3,right_cst); break;
	case Dim::MATRIX_ARRAY: assert(false); /* impossible */ break;
	}

	try {
		if (hc4r.proj(ctr.f,root_label,box)) {
			set_flag(INACTIVE); // TODO: incorrect in general
			set_flag(FIXPOINT); // TODO: incorrect if multiple occurrences
		}
	} catch (EmptyBoxException& e) {
		box.set_empty();
		throw e;
	}
}

} // namespace ibex
