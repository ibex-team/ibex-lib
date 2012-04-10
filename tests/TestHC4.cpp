//============================================================================
//                                  I B E X                                   
// File        : TestHC4.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Apr 10, 2012
// Last Update : Apr 10, 2012
//============================================================================

#include "TestHC4.h"
#include "Ponts30.h"
#include "ibex_HC4Revise.h"
#include "ibex_Propagation.h"

namespace ibex {

void TestHC4::ponts30() {
	Ponts30 p30;
	Function* ctr=p30.f.separate();
	cout << "firt function:" << ctr[29] << endl;
	IntervalVector box = p30.init_box;

	HC4Revise c0(NumConstraint(ctr[29],true));
	cout << "before="<< box << endl;
	c0.contract(box);

	cout << "after="<< box << endl;
}

} // end namespace ibex
