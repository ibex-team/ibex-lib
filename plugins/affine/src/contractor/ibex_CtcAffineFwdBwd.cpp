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


void CtcAffineFwdBwd::init() {


	input = new BitSet(0,nb_var-1,BitSet::empt);
	output = new BitSet(0,nb_var-1,BitSet::empt);

	int v;
	for (int i=0; i<f.nb_used_vars(); i++) {
		v=f.used_var(i);
		output->add(v);
		input->add(v);
	}

	_affine_evaluator = new Affine2Eval(f);
	_hc4revise = new HC4Revise(*_affine_evaluator);
}

CtcAffineFwdBwd::~CtcAffineFwdBwd(){
	delete _affine_evaluator;
	delete _hc4revise;
}


void CtcAffineFwdBwd::contract(IntervalVector& box) {

	assert(box.size()==f.nb_var());

	//std::cout << " AffineHC4 of " << f << "=" << d << " with box=" << box << std::endl;
	if (_hc4revise->proj(d,box)) {
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
