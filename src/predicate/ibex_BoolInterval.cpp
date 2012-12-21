//============================================================================
//                                  I B E X                                   
// File        : ibex_BoolInterval.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Nov 27, 2012
// Last Update : Nov 27, 2012
//============================================================================


#include "ibex_BoolInterval.h"

namespace ibex {

BoolInterval operator&(const BoolInterval& x, const BoolInterval& y) {
	if (x==NO) return NO;
	else if (y==NO) return NO;
	else if (x==MAYBE) return MAYBE;
	else return y;
}

BoolInterval operator|(const BoolInterval& x, const BoolInterval& y) {
	if (x==YES) return YES;
	else if (y==YES) return YES;
	else if (x==MAYBE) return MAYBE;
	else return y;
}

BoolInterval operator&=(BoolInterval& x, const BoolInterval& y) {
	x = (x&y);
	return x;
}

BoolInterval operator|=(BoolInterval& x, const BoolInterval& y) {
	x = (x|y);
	return x;
}

BoolInterval operator!(const BoolInterval& x) {
	if (x==YES) return NO;
	else if (x==NO) return YES;
	else return x;
}

} // end namespace
