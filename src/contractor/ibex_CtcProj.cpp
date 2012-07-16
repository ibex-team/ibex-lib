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

#include "ibex_CtcProj.h"

namespace ibex {

CtcProj::CtcProj(Function& f, NumConstraint::CompOp op) : Ctc(f.input_size()), ctr(f,op) {
	for (int v=0; v<ctr.f.input_size(); v++)
		output[v]=input[v]=ctr.f.used(v);

}

CtcProj::CtcProj(const NumConstraint& ctr) : Ctc(ctr.f.input_size()), ctr(ctr.f,ctr.op) {
	for (int v=0; v<ctr.f.input_size(); v++)
		output[v]=input[v]=ctr.f.used(v);
}

void CtcProj::contract(IntervalVector& box) {
	const Dim& d=ctr.f.expr().dim;
	Domain root_label(d);
	Interval right_cst;

	switch (ctr.op) {
	case NumConstraint::LT :
	case NumConstraint::LEQ : right_cst=Interval(NEG_INFINITY, 0); break;
	case NumConstraint::EQ  : right_cst=Interval(0,0); break;
	case NumConstraint::GEQ :
	case NumConstraint::GT : right_cst=Interval(0,POS_INFINITY); break;
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
