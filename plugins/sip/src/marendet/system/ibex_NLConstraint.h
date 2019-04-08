/* ============================================================================
 * I B E X - ibex_NLConstraint.h
 * ============================================================================
 * Copyright   : IMT Atlantique (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Antoine Marendet, Gilles Chabert
 * Created     : May 4, 2018
 * ---------------------------------------------------------------------------- */
 
#ifndef __SIP_IBEX_NLCONSTRAINT_H__
#define __SIP_IBEX_NLCONSTRAINT_H__

#include "ibex_Constraint.h"
#include "ibex_Function.h"
#include "ibex_Interval.h"
#include "ibex_IntervalVector.h"

namespace ibex {
class NLConstraint : public Constraint {
public:
    NLConstraint(const Function* function);
    virtual ~NLConstraint();
    Interval evaluate(const IntervalVector &box) const;
    IntervalVector gradient(const IntervalVector &box) const;
    bool isSatisfied(const IntervalVector &box) const;
    
    const Function* function_;
};

} // end namespace ibex

#endif // __SIP_IBEX_NLCONSTRAINT_H__
