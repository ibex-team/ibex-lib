//============================================================================
//                                  I B E X                                   
// File        : TestCtcHC4.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Apr 10, 2012
// Last Update : Apr 10, 2012
//============================================================================

#include "TestCtcHC4.h"
#include "Ponts30.h"
#include "ibex_CtcFwdBwd.h"
#include "ibex_CtcHC4.h"
#include "ibex_Array.h"

namespace ibex {

void TestCtcHC4::ponts30() {
	Ponts30 p30;
	IntervalVector box = p30.init_box;

	NumConstraint* ctr[30];
	CtcFwdBwd* c[30];

	for (int i=0; i<30; i++) {
		Function* fi=dynamic_cast<Function*>(&((*p30.f)[i]));
		CPPUNIT_ASSERT(fi!=NULL);
		ctr[i]=new NumConstraint(*fi,EQ);
		c[i]=new CtcFwdBwd(*ctr[i]);
	}
//	cout << "before="<< box << endl;

	for (int i=0; i<30; i++) {
		c[i]->contract(box);
		//cout << box << endl;
		//cout << p30.hc4r_box[i] << endl;
		//c[i]->hc4r.eval.f.cf.print<Domain>();
		CPPUNIT_ASSERT(almost_eq(box, p30.hc4r_box[i],1e-02));
	}
	//cout << "after="<< box << endl;
	box = p30.init_box;

	Array<NumConstraint> a(ctr,30);
	CtcHC4 hc4(a,0.1);
	hc4.accumulate=true;
	box=p30.init_box;
	hc4.contract(box);

	CPPUNIT_ASSERT(almost_eq(box, p30.hc4_box,1e-04));

	for (int i=0; i<30; i++) {
		delete c[i];
		delete ctr[i];
	}
}

} // end namespace ibex
