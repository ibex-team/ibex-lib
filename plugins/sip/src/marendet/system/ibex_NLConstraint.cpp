/* ============================================================================
 * I B E X - ibex_NLConstraint.cpp
 * ============================================================================
 * Copyright   : IMT Atlantique (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Antoine Marendet, Gilles Chabert
 * Created     : May 4, 2018
 * ---------------------------------------------------------------------------- */
 
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
