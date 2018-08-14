//============================================================================
//                                  I B E X                                   
// File        : ibex_NLConstraint.cpp
// Author      : Antoine Marendet, Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : May 4, 2018
// Last Update : May 4, 2018
//============================================================================

#include "ibex_NLConstraint.h"

#include "ibex_Function.h"

namespace ibex {

NLConstraint::NLConstraint(const Function* function)
: function_(function) {
}

NLConstraint::~NLConstraint() {
}

Interval NLConstraint::evaluate(const IntervalVector &box) const {
    // TODO centered form
    return function_->eval(box);
}

IntervalVector NLConstraint::gradient(const IntervalVector &box) const {
    return function_->gradient(box);
}

bool NLConstraint::isSatisfied(const IntervalVector &box) const {
    return evaluate(box).ub() <= 0;
}
} // end namespace ibex
