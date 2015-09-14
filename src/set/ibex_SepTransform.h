//============================================================================
//                                  I B E X                                   
// File        : ibex_SepTransform.h
// Author      : Benoit Desrochers, Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Mar 3, 2015
// Last Update : Mar 3, 2015
//============================================================================

#ifndef __IBEX_SEP_TRANSFORM_H__
#define __IBEX_SEP_TRANSFORM_H__

#include "ibex_Sep.h"
#include "ibex_Function.h"


namespace ibex {
/**
* \ingroup set
*
* \brief Transformation of a separator with an inversible transformation
*	T(S)(X) = { Tinv*Sin*T(X), Tinv*Sout*T(X)}
*/

class SepTransform : public Sep {
public:
    SepTransform(Sep& s, Function& t, Function &tinv);
    ~SepTransform();
    virtual void separate(IntervalVector& xin, IntervalVector& xout);

    /** Separator */
    Sep& s;
    /** transformation */
    Function& t;
    /** Inverse transformation */
    Function& tinv;

private:	
    IntervalVector yin, yout;

};

} // end namespace ibex
#endif // __IBEX_SEP_TRANSFORM_H__
