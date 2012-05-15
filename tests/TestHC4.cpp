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
#include "ibex_CtcHC4Revise.h"
#include "ibex_CtcHC4.h"
#include "ibex_Array.h"

namespace ibex {

void TestHC4::ponts30() {
	Ponts30 p30;
	Function* fs=p30.f.separate();
	IntervalVector box = p30.init_box;

	NumConstraint* ctr[30];
	CtcHC4Revise* c[30];

	//cout << fs[0] << endl;

	for (int i=0; i<30; i++) {
		ctr[i]=new NumConstraint(fs[i],true);
		c[i]=new CtcHC4Revise(*ctr[i]);
	}
//	cout << "before="<< box << endl;

	for (int i=0; i<30; i++) {
		c[i]->contract(box);
		//cout << box << endl;
		//cout << p30.hc4r_box[i] << endl;
		//c[i]->hc4r.eval.f.cf.print<Domain>();
		TEST_ASSERT(almost_eq(box, p30.hc4r_box[i],1e-02));
	}
	//cout << "after="<< box << endl;
	box = p30.init_box;

	Array<NumConstraint> a(ctr,30);
	CtcHC4 hc4(a);
	box=p30.init_box;
	hc4.contract(box);

	TEST_ASSERT(almost_eq(box, p30.hc4_box,1e-05));

	for (int i=0; i<30; i++) {
		delete c[i];
		delete ctr[i];
	}
}

} // end namespace ibex
