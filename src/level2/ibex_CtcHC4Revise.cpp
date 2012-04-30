/* ============================================================================
 * I B E X - HC4Revise contractor
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Feb 27, 2012
 * ---------------------------------------------------------------------------- */

#include "ibex_CtcHC4Revise.h"

namespace ibex {

CtcHC4Revise::CtcHC4Revise(Function& f, bool equality) : Ctc(f.input_size()), ctr(f,equality), hc4r(f) {

}

CtcHC4Revise::CtcHC4Revise(const NumConstraint& ctr) : Ctc(ctr.f.nb_symbols()), ctr(ctr), hc4r(ctr.f) {

}

void CtcHC4Revise::contract(IntervalVector& box) {
	const Dim& d=ctr.f.expr().dim;
	Domain root_label(d);
	Interval right_cst(ctr.equality? 0: NEG_INFINITY, 0);

	switch(d.type()) {
	case Dim::SCALAR:       root_label.i()=right_cst; break;
	case Dim::ROW_VECTOR:   root_label.v()=IntervalVector(d.dim3,right_cst); break;
	case Dim::COL_VECTOR:   root_label.v()=IntervalVector(d.dim2,right_cst); break;
	case Dim::MATRIX:       root_label.m()=IntervalMatrix(d.dim2,d.dim3,right_cst); break;
	case Dim::MATRIX_ARRAY: assert(false); /* impossible */ break;
	}
	hc4r.contract(box,root_label);
	//hc4r.eval.f.cf.print<Domain>();
}

bool CtcHC4Revise::can_contract(int v) const {
	return ctr.f.used(v);
}

} // namespace ibex
