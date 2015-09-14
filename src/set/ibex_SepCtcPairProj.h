//============================================================================
//                                  I B E X                                   
// File        : ibex_SepCtcPairProj.h
// Author      : Benoit Desrochers
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : May 04, 2015
//============================================================================

#ifndef __IBEX_SEP_CTCPAIR_PROJ_H__
#define __IBEX_SEP_CTCPAIR_PROJ_H__


#include <ibex_IntervalVector.h>
#include <ibex_SepCtcPair.h>
#include <ibex_CtcForAll.h>
#include <ibex_CtcExist.h>

namespace ibex {



class SepCtcPairProj : public SepCtcPair {

public:
    SepCtcPairProj(Ctc& ctc_in, Ctc& ctc_out, const IntervalVector& y_init, double prec);
    ~SepCtcPairProj();

    void separate(IntervalVector &x_in, IntervalVector &x_out);
    
protected:
    CtcExist *ctcExist;
    CtcForAll *ctcForAll;

    Bitset<unsigned int, float> vars;

};

} // namespace ibex

#endif // __IBEX_SEP_CTCPAIR_PROJ_H__

