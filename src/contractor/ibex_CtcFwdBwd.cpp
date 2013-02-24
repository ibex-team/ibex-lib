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
#include "ibex_HC4Revise.h"

namespace ibex {

CtcFwdBwd::CtcFwdBwd(Function& f, CmpOp op) : Ctc(f.nb_var()), ctr(f,op) {
	for (int v=0; v<ctr.f.nb_var(); v++)
		output[v]=input[v]=ctr.f.used(v);

}

CtcFwdBwd::CtcFwdBwd(const NumConstraint& ctr) : Ctc(ctr.f.nb_var()), ctr(ctr.f,ctr.op) {
	for (int v=0; v<ctr.f.nb_var(); v++)
		output[v]=input[v]=ctr.f.used(v);
}

void CtcFwdBwd::contract(IntervalVector& box) {
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
		HC4Revise().proj(ctr.f,root_label,box);
	} catch (EmptyBoxException& e) {
		box.set_empty();
		throw e;
	}
}

} // namespace ibex
