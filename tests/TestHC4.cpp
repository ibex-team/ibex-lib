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
	Function* fs=p30.f.separate();
	IntervalVector box = p30.init_box;

	NumConstraint* ctr[30];
	HC4Revise* c[30];
	for (int k=0; k<10; k++)

	for (int i=0; i<30; i++) {
		ctr[i]=new NumConstraint(fs[i],true);
		c[i]=new HC4Revise(*ctr[i]);
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



	for (int i=0; i<30; i++) {
		delete c[i];
		delete ctr[i];
	}
	//cout << "goal=" << p30.hc4r_box << endl;
}

} // end namespace ibex
