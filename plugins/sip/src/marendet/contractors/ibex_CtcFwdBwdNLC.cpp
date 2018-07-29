//============================================================================
//                                  I B E X                                   
// File        : ibex_CtcFwdBwdNLC.cpp
// Author      : Antoine Marendet, Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : May 4, 2018
// Last Update : May 4, 2018
//============================================================================

#include "ibex_CtcFwdBwdNLC.h"

#include "ibex_BitSet.h"
#include "ibex_Function.h"

namespace ibex {

CtcFwdBwdNLC::CtcFwdBwdNLC(const NLConstraint& constraint)
: Ctc(constraint.function_->nb_var()), function_(constraint.function_),
backward_domain_(Interval::NEG_REALS)
{
	init();
}

CtcFwdBwdNLC::~CtcFwdBwdNLC() {
    delete input;
    delete output;
}

void CtcFwdBwdNLC::init() {
	input = new BitSet(function_->used_vars);
	output = new BitSet(function_->used_vars);
}

void CtcFwdBwdNLC::contract(IntervalVector &box) {
    if(function_->backward(backward_domain_, box)) {
        set_flag(INACTIVE);
        set_flag(FIXPOINT);
    }

    
    if(box.is_empty()) {
        set_flag(FIXPOINT);
    }

}
} // end namespace ibex
