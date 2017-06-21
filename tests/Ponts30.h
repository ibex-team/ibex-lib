//============================================================================
//                                  I B E X                                   
// File        : Ponts30.h
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Apr 10, 2012
// Last Update : Apr 10, 2012
//============================================================================

#ifndef __PONTS_30_H__
#define __PONTS_30_H__

#include "ibex_Function.h"
#include "ibex_IntervalVector.h"

namespace ibex {

class Ponts30 {
public:
	Ponts30();
	~Ponts30() { delete f;}

	Function* f;
	IntervalVector init_box;
	// hc4r_box[i] is the box obtained after applying
	// HC4Revise once on each ctr (in increasing order)
	IntervalMatrix hc4r_box;

	// box obtained after HC4 propagation
	IntervalVector hc4_box;
private:
	void build_box();
	void build_equ();
};

} // end namespace ibex

#endif // __PONTS_30_H__
