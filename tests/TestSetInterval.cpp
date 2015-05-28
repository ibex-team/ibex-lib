//============================================================================
//                                  I B E X                                   
// File        : TestSetInterval.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Aug 18, 2014
//============================================================================

#include "TestSetInterval.h"
#include "ibex_SetInterval.h"
#include "ibex_SetLeaf.h"

using namespace std;

namespace ibex {

void TestSetInterval::diff01() {
	IntervalVector x(2);
	x[0]=Interval(5,10);
	x[1]=Interval(-10,10);
	IntervalVector y(2);
	y[0]=Interval(-5,5.09);
	y[1]=Interval(5,9.91);

	SetNode* node=diff(x,y,__IBEX_UNK__,__IBEX_OUT__,0.1);

	SetLeaf* leaf=dynamic_cast<SetLeaf*>(node);
	TEST_ASSERT(leaf && leaf->status==__IBEX_UNK__);
}

void TestSetInterval::diff02() {
	IntervalVector x(3,Interval(-2,2));
	IntervalVector y(3,Interval(-1,1));

	SetNode* node=diff(x,y,__IBEX_UNK__,__IBEX_OUT__,0.1);

	node->print(cout,x,0); //cout << *node << endl;

//	SetLeaf* leaf=dynamic_cast<SetLeaf*>(node);
//	TEST_ASSERT(leaf && leaf->status==__IBEX_UNK__);
}

} // end namespace ibex
