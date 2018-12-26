/* ============================================================================
 * I B E X - ibex_Constraint.h
 * ============================================================================
 * Copyright   : IMT Atlantique (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Antoine Marendet, Gilles Chabert
 * Created     : May 4, 2018
 * ---------------------------------------------------------------------------- */
 
#ifndef __SIP_IBEX_CONSTRAINT_H__
#define __SIP_IBEX_CONSTRAINT_H__

#include "ibex_IntervalVector.h"


namespace ibex {
class Constraint {
    
public:
	virtual ~Constraint() { };
    //virtual Interval evaluate(const IntervalVector& box) const =0;
    //virtual IntervalVector gradient(const IntervalVector& box) const =0;
    //virtual bool isSatisfied(const IntervalVector& box) const =0;
};

} // end namespace ibex

#endif // __SIP_IBEX_CONSTRAINT_H__
