/* ============================================================================
 * I B E X - ibex_CtcFwdBwdNLC.cpp
 * ============================================================================
 * Copyright   : IMT Atlantique (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Antoine Marendet, Gilles Chabert
 * Created     : May 4, 2018
 * ---------------------------------------------------------------------------- */
 
#include "ibex_CtcFwdBwdNLC.h"

#include "ibex_BitSet.h"
#include "ibex_Function.h"
#include "ibex_SIPSystem.h"

using namespace std;

namespace ibex {

CtcFwdBwdNLC::CtcFwdBwdNLC(const NLConstraint& constraint, const SIPSystem& system)
: Ctc(constraint.function_->nb_var()), function_(constraint.function_),
backward_domain_(Interval::NEG_REALS), system_(system)
{
	init();
}

CtcFwdBwdNLC::~CtcFwdBwdNLC() {
    delete input;
    delete output;
}

void CtcFwdBwdNLC::init() {
	input = new BitSet(nb_var);
	output = new BitSet(nb_var);
	
	for (vector<int>::const_iterator it=function_->used_vars.begin(); it!=function_->used_vars.end(); it++) {
		output->add(*it);
		input->add(*it);
	}
}

void CtcFwdBwdNLC::add_property(const IntervalVector& init_box, BoxProperties& map) {
    if(map[BxpNodeData::id] == nullptr) {
        map.add(new BxpNodeData(system_.getInitialNodeCaches()));
        ((BxpNodeData*)map[BxpNodeData::id])->init_box = init_box;
    }
}

void CtcFwdBwdNLC::contract(IntervalVector& box) {
    ContractContext context(box);
	contract(box,context);
}

void CtcFwdBwdNLC::contract(IntervalVector& box, ContractContext& context) {
    if(function_->backward(backward_domain_, box)) {
        context.output_flags.add(INACTIVE);
        context.output_flags.add(FIXPOINT);
    }

    
    if(box.is_empty()) {
        context.output_flags.add(FIXPOINT);
    }

}
} // end namespace ibex
