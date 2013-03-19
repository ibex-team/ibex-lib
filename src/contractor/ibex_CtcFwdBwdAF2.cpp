/* ============================================================================
 * I B E X - HC4 Revise (forward-backward algorithm) using Affine2 Arithmetic
 * ============================================================================
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Jordan Ninin
 * Created     : March 18, 2013
 * ---------------------------------------------------------------------------- */

#include "ibex_CtcFwdBwdAF2.h"
#include "ibex_Affine2Eval.h"

namespace ibex {

CtcFwdBwdAF2::CtcFwdBwdAF2(Function& f, CmpOp op) : Ctc(f.nb_var()), ctr(f,op) {
	for (int v=0; v<ctr.f.nb_var(); v++)
		output[v]=input[v]=ctr.f.used(v);

}

CtcFwdBwdAF2::CtcFwdBwdAF2(const NumConstraint& ctr) : Ctc(ctr.f.nb_var()), ctr(ctr.f,ctr.op) {
	for (int v=0; v<ctr.f.nb_var(); v++)
		output[v]=input[v]=ctr.f.used(v);
}

void CtcFwdBwdAF2::contract(IntervalVector& box) {
	const Dim& d=ctr.f.expr().dim;
	Affine2Domain root_label(d);
	Interval right_cst;

	switch (ctr.op) {
	case LT :
	case LEQ : right_cst=Interval(NEG_INFINITY, 0); break;
	case EQ  : right_cst=Interval(0,0); break;
	case GEQ :
	case GT : right_cst=Interval(0,POS_INFINITY); break;
	}

	Affine2 right_cst_af2 (0,right_cst);
	right_cst_af2.set_actif(false);

	switch(d.type()) {
	case Dim::SCALAR:       root_label.i()=right_cst_af2; break;
	case Dim::ROW_VECTOR:   root_label.v()=Affine2Vector(d.dim3,right_cst_af2); break;
	case Dim::COL_VECTOR:   root_label.v()=Affine2Vector(d.dim2,right_cst_af2); break;
	case Dim::MATRIX:       root_label.m()=Affine2Matrix(d.dim2,d.dim3,right_cst_af2); break;
	case Dim::MATRIX_ARRAY: assert(false); /* impossible */ break;
	}

	try {
		Affine2Eval().proj_affine2(ctr.f,root_label,box);
	} catch (EmptyBoxException& e) {
		box.set_empty();
		throw e;
	}
}

} // namespace ibex
