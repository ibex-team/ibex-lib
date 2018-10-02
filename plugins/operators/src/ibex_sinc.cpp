//============================================================================
//                                  I B E X
// File        : ibex_sinc.cpp
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Oct 02, 2018
//============================================================================

#include "ibex_Expr.h"
#include "ibex_Domain.h"

namespace ibex {

Domain sinc(const Domain& x) {
	return sin(x)/x;
}

void sinc_bwd(Domain& x, const Domain& y) {
	x=sin(x)/y; // pessimistic...
}

Domain dsinc(const Domain& x) {
	return cos(x)-sin(x)/sqr(x);
}

const ExprNode& dsinc_symb(const ExprNode& expr) {
	return cos(expr)-sin(expr)/sqr(expr);
}

} // end namespace
