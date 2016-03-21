//============================================================================
//                                  I B E X                                   
// File        : ExFunction.h
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Apr 17, 2012
// Last Update : Apr 17, 2012
//============================================================================

#ifndef EXFUNCTION_H_
#define EXFUNCTION_H_

#include "ibex_Function.h"
#include "ibex_Domain.h"

namespace ibex {

class ExFunction {
public:

	CPPUNIT_TEST_SUITE(ExFunction);
	ExFunction(const ExprSymbol& in, const ExprNode& out, const IntervalVector& x, const Dim& dimY, const IntervalVector& xBwd) : f(in,out), x(x), yFwd(dimY), xBwd(xBwd) { }

	Function f;

	IntervalVector x;
	Domain yFwd;
	IntervalVector xBwd;
};

CPPUNIT_TEST_SUITE_REGISTRATION(ExFunction);


void init_FUNC();

extern ExFunction* FUNC[10];

} // end namespace ibex
#endif // EXFUNCTION_H_
