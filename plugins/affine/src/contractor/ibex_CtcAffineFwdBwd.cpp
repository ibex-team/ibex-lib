/* ============================================================================
 * I B E X - HC4 Revise with Affine arithmetic (forward-backward algorithm)
 * ============================================================================
 * Copyright   : ENSTA Bretagne (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Jordan Ninin
 * Created     : Jan 20, 2017
 * ---------------------------------------------------------------------------- */

#include "ibex_CtcAffineFwdBwd.h"

namespace ibex {


CtcAffineFwdBwd::CtcAffineFwdBwd(Function& f, CmpOp op): CtcFwdBwd(f,op),
		_affine_evaluator(f), _hc4revise(_affine_evaluator) { }

CtcAffineFwdBwd::CtcAffineFwdBwd(Function& f, const Domain& y): CtcFwdBwd(f,y),
		_affine_evaluator(f), _hc4revise(_affine_evaluator) { }

CtcAffineFwdBwd::CtcAffineFwdBwd(Function& f, const Interval& y): CtcFwdBwd(f,y),
		_affine_evaluator(f), _hc4revise(_affine_evaluator) { }

CtcAffineFwdBwd::CtcAffineFwdBwd(Function& f, const IntervalVector& y): CtcFwdBwd(f,y),
		_affine_evaluator(f), _hc4revise(_affine_evaluator) { }

CtcAffineFwdBwd::CtcAffineFwdBwd(Function& f, const IntervalMatrix& y): CtcFwdBwd(f,y),
		_affine_evaluator(f), _hc4revise(_affine_evaluator) { }

CtcAffineFwdBwd::CtcAffineFwdBwd(const NumConstraint& ctr): CtcFwdBwd(ctr),
		_affine_evaluator(ctr.f), _hc4revise(_affine_evaluator) { }


void CtcAffineFwdBwd::contract(IntervalVector& box) {

	assert(box.size()==f.nb_var());

	//std::cout << " AffineHC4 of " << f << "=" << d << " with box=" << box << std::endl;
	if (_hc4revise.proj(d,box)) {
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
