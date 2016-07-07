//============================================================================
//                                  I B E X                                   
// File        : ibex_SepInverse.h
// Author      : Benoit Desrochers, Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Mar 3, 2015
// Last Update : Mar 3, 2015
//============================================================================

#ifndef __IBEX_SEP_INVERSE_H__
#define __IBEX_SEP_INVERSE_H__

#include "ibex_Sep.h"
#include "ibex_Function.h"

namespace ibex {
/**
 * \ingroup set
 *
 * \brief Image of a separator by a function in a forward-backaward manner
 *
 */

class SepInverse : public Sep {
public:
    SepInverse(Sep& s, Function& f);
    ~SepInverse();
    virtual void separate(IntervalVector& xin, IntervalVector& xout);

    Sep& s;
    Function& f;

private:
    Function *id;
    IntervalVector yin, yout;

};

} // end namespace ibex
#endif // __IBEX_SEP_INVERSE_H__
