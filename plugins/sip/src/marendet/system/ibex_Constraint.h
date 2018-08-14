//============================================================================
//                                  I B E X                                   
// File        : ibex_Constraint.h
// Author      : Antoine Marendet, Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : May 4, 2018
// Last Update : May 4, 2018
//============================================================================

#ifndef __SIP_IBEX_CONSTRAINT_H__
#define __SIP_IBEX_CONSTRAINT_H__

#include "ibex_IntervalVector.h"


namespace ibex {
class Constraint {
    
public:
	virtual ~Constraint() { };
    virtual Interval evaluate(const IntervalVector& box) const =0;
    virtual IntervalVector gradient(const IntervalVector& box) const =0;
    virtual bool isSatisfied(const IntervalVector& box) const =0;
};

} // end namespace ibex

#endif // __SIP_IBEX_CONSTRAINT_H__
