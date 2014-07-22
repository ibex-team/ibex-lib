//============================================================================
//                                  I B E X                                   
// File        : ibex_Bracket.h
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jul 21, 2014
//============================================================================

#ifndef __IBEX_BRACKET_H__
#define __IBEX_BRACKET_H__

#include "ibex_Ctc.h"
#include "ibex_Function.h"

namespace ibex {

class Bracket {
public:
	// constraint w.r.t. f(x)<=0
	Bracket(Function& f);

	~Bracket();

	Bracket(Ctc& ctc_in, Ctc& ctc_out);

	Ctc& ctc_in;
	Ctc& ctc_out;
};

inline Bracket::Bracket(Ctc& _ctc_in, Ctc& _ctc_out) : ctc_in(_ctc_in), ctc_out(_ctc_out) {

}

} // namespace ibex

#endif // __IBEX_BRACKET_H__
