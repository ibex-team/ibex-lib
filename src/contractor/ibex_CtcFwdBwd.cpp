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
#include "ibex_BxpActiveCtr.h"
#include "ibex_BxpSystemCache.h"
#include "ibex_ExprCopy.h"

namespace ibex {

namespace {

NumConstraint& build_ctr(const Function& f, const Domain& y) {
	Array<const ExprSymbol> x(f.nb_arg());
	varcopy(f.args(),x);
	return *new NumConstraint(x,f((Array<const ExprNode>&) x)=ExprConstant::new_(y));
}

}

CtcFwdBwd::CtcFwdBwd(const Function& f, const Domain& y) : Ctc(f.nb_var()), ctr(build_ctr(f,y)), d(ctr.right_hand_side()), ctr_num(-1), active_prop_id(BxpActiveCtr::get_id(ctr)), system_cache_id(-1), own_ctr(true) {
	assert(f.expr().dim==y.dim);

	init();
}

CtcFwdBwd::CtcFwdBwd(const Function& f, const Interval& y) : Ctc(f.nb_var()), ctr(build_ctr(f,Domain((Interval&) y))), d(ctr.right_hand_side()), ctr_num(-1), active_prop_id(BxpActiveCtr::get_id(ctr)), system_cache_id(-1), own_ctr(true) {
	assert(f.expr().dim==Dim::scalar());

	init();
}

CtcFwdBwd::CtcFwdBwd(const Function& f, const IntervalVector& y) : Ctc(f.nb_var()), ctr(build_ctr(f,Domain((IntervalVector&) y,f.expr().dim.type()==Dim::ROW_VECTOR))), d(ctr.right_hand_side()), ctr_num(-1), active_prop_id(BxpActiveCtr::get_id(ctr)), system_cache_id(-1), own_ctr(true) {
	assert(f.expr().dim.is_vector() && f.expr().dim.vec_size()==y.size());

	init();
}

CtcFwdBwd::CtcFwdBwd(const Function& f, const IntervalMatrix& y) : Ctc(f.nb_var()), ctr(build_ctr(f,Domain((IntervalMatrix&) y))), d(ctr.right_hand_side()), ctr_num(-1), active_prop_id(BxpActiveCtr::get_id(ctr)), system_cache_id(-1), own_ctr(true) {
	assert(f.expr().dim==Dim::matrix(y.nb_rows(),y.nb_cols()));

	init();
}

CtcFwdBwd::CtcFwdBwd(const Function& f, CmpOp op) : Ctc(f.nb_var()), ctr(*new NumConstraint(f,op)), d(ctr.right_hand_side()), ctr_num(-1), active_prop_id(BxpActiveCtr::get_id(ctr)), system_cache_id(-1), own_ctr(true) {
	init();
}

CtcFwdBwd::CtcFwdBwd(const NumConstraint& ctr) : Ctc(ctr.f.nb_var()), ctr(ctr), d(ctr.right_hand_side()), ctr_num(-1), active_prop_id(BxpActiveCtr::get_id(ctr)), system_cache_id(-1), own_ctr(false) {
	init();
}

CtcFwdBwd::CtcFwdBwd(const System& sys, int i) : Ctc(sys.nb_var), ctr(sys.ctrs[i]), d(ctr.right_hand_side()), ctr_num(i), active_prop_id(-1), system_cache_id(BxpSystemCache::get_id(sys)), own_ctr(false) {
	init();
}

CtcFwdBwd::~CtcFwdBwd() {
	delete input;
	delete output;
	if (own_ctr) delete &ctr;
}

void CtcFwdBwd::init() {
	input = new BitSet(ctr.f.used_vars);
	output = new BitSet(ctr.f.used_vars);
}

void CtcFwdBwd::add_property(const IntervalVector& init_box, BoxProperties& map) {
	if (ctr_num==-1) {
		if (ctr.op!=EQ && !map[active_prop_id])
		map.add(new BxpActiveCtr(init_box, ctr));
	} else {
		// the system cache is added at higher level
	}
}

void CtcFwdBwd::contract(IntervalVector& box) {
	CtcContext context;
	contract(box,context);
}

void CtcFwdBwd::contract(IntervalVector& box, CtcContext& context) {

	assert(box.size()==ctr.f.nb_var());

	BxpActiveCtr* p=NULL;
	BxpSystemCache* sp=NULL;

	if (context.data()) {
		if (ctr_num==-1)
			p=(BxpActiveCtr*) (*context.data())[active_prop_id];
		else
			sp=(BxpSystemCache*) (*context.data())[system_cache_id];
	}

	if ((p && !p->active()) || (sp && !(sp->active_ctrs()[ctr_num]))) {
		context.set_flag(CtcContext::INACTIVE);
		context.set_flag(CtcContext::FIXPOINT);
		return;
	}

	//std::cout << " hc4 of " << f << "=" << d << " with box=" << box << std::endl;
	if (ctr.f.backward(d,box)) {
		if (p) p->set_inactive();
		if (sp) sp->active_ctrs().remove(ctr_num);
		context.set_flag(CtcContext::INACTIVE);
		context.set_flag(CtcContext::FIXPOINT);
		return;
	}
	//std::cout << " ---> " << box << std::endl;

	if (box.is_empty()) {
		context.set_flag(CtcContext::FIXPOINT);
	}

	// Note: setting the FIXPOINT flag is incorrect when there
	// is no multiple occurrence because some operators
	// may be non-optimal in backward mode.

	if (context.data()) {
		//TODO: set used_vars in impact
		context.data()->update(BoxEvent(box,BoxEvent::CONTRACT));
	}
}


} // namespace ibex
