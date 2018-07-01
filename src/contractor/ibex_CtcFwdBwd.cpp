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
#include "ibex_BoxPropActiveCtr.h"

namespace ibex {

CtcFwdBwd::CtcFwdBwd(const Function& f, const Domain& y) : Ctc(f.nb_var()), f(f), d(f.expr().dim), active_prop_id(next_id()) {
	assert(f.expr().dim==y.dim);
	d = y;

	init();
}

CtcFwdBwd::CtcFwdBwd(const Function& f, const Interval& y) : Ctc(f.nb_var()), f(f), d(Dim()), active_prop_id(next_id()) {
	assert(f.expr().dim==d.dim);
	d.i() = y;

	init();
}

CtcFwdBwd::CtcFwdBwd(const Function& f, const IntervalVector& y) : Ctc(f.nb_var()), f(f), d(f.expr().dim), active_prop_id(next_id()) {
	assert(f.expr().dim.is_vector() && f.expr().dim.vec_size()==y.size());
	d.v() = y;

	init();
}

CtcFwdBwd::CtcFwdBwd(const Function& f, const IntervalMatrix& y) : Ctc(f.nb_var()), f(f), d(f.expr().dim), active_prop_id(next_id()) {
	assert(f.expr().dim==Dim::matrix(y.nb_rows(),y.nb_cols()));
	d.m() = y;

	init();
}

CtcFwdBwd::CtcFwdBwd(const Function& f, CmpOp op) : Ctc(f.nb_var()), f(f), d(NumConstraint(f,op).right_hand_side()), active_prop_id(next_id()) {
	init();
}

CtcFwdBwd::CtcFwdBwd(const NumConstraint& ctr) : Ctc(ctr.f.nb_var()), f(ctr.f), d(ctr.right_hand_side()), active_prop_id(ctr.active_prop_id) {
	init();
}

CtcFwdBwd::~CtcFwdBwd() {
	delete input;
	delete output;
}

void CtcFwdBwd::init() {

	input = new BitSet(nb_var);
	output = new BitSet(nb_var);

	int v;
	for (int i=0; i<f.nb_used_vars(); i++) {
		v=f.used_var(i);
		output->add(v);
		input->add(v);
	}
}


void CtcFwdBwd::contract(IntervalVector& box) {
	CtcContext context;
	contract(box,context);
}

void CtcFwdBwd::contract(IntervalVector& box, CtcContext& context) {

	assert(box.size()==f.nb_var());

	BxpActiveCtr* p=context.data() ? (BxpActiveCtr*) (*context.data())[active_prop_id] : NULL;

	if (p && !p->active) {
		context.set_flag(CtcContext::INACTIVE);
		context.set_flag(CtcContext::FIXPOINT);
		return;
	}

	//std::cout << " hc4 of " << f << "=" << d << " with box=" << box << std::endl;
	if (f.backward(d,box)) {
		if (p) p->active=false;
		context.set_flag(CtcContext::INACTIVE);
		context.set_flag(CtcContext::FIXPOINT);
	}
	//std::cout << " ---> " << box << std::endl;

	if (box.is_empty()) {
		context.set_flag(CtcContext::FIXPOINT);
	}

	// Note: setting the FIXPOINT flag is incorrect when there
	// is no multiple occurrence because some operators
	// may be non-optimal in backward mode.
}

void CtcFwdBwd::add_property(Map<Bxp>& map) {
	if (!map.found(active_prop_id))
		map.insert_new(active_prop_id, new BxpActiveCtr());
}

} // namespace ibex
