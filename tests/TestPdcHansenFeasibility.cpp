//============================================================================
//                                  I B E X                                   
// File        : TestPdcHansenFeasibility.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Nov 28, 2013
//============================================================================

#include "TestPdcHansenFeasibility.h"
#include "ibex_PdcHansenFeasibility.h"
#include "ibex_Function.h"

using namespace std;

namespace ibex {

void TestPdcHansenFeasibility::test01() {

	Variable x,y;
	Function f(x,y,y-sqr(x));

	PdcHansenFeasibility p(f,true);

	double _box[][2] = { {1,3},{1,3} };
	IntervalVector box(2,_box);

	BoolInterval r=p.test(box);

	CPPUNIT_ASSERT(r==YES);

	IntervalVector sol=p.solution();

	// test that the fixed dimension is "y"
	CPPUNIT_ASSERT(sol[1].is_degenerated());
	//cout << "sol=" << sol << endl;
	CPPUNIT_ASSERT(!(sol[0] & sqrt(sol[1])).is_empty());

	CPPUNIT_ASSERT(sol[0].diam() <=1e-08);

}
} // end namespace

