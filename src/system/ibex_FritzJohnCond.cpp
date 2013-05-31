//============================================================================
//                                  I B E X                                   
// File        : ibex_FritzJohnCond.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : May 31, 2013
// Last Update : May 31, 2013
//============================================================================

#include "ibex_FritzJohnCond.h"
#include "ibex_SystemFactory.h"
#include "ibex_FritzJohnFactory.cpp_"

namespace ibex {

FritzJohnCond::FritzJohnCond(const System& sys) : n(0), M(0), R(0), K(0) /* TMP */ {

	FritzJohnFactory fac(sys);

	init(fac);

	(int&) n = fac.n;
	(int&) M = fac.M;
	(int&) R = fac.R;
	(int&) K = fac.K;
}

} // end namespace ibex
