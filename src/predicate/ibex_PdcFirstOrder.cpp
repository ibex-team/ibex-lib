//============================================================================
//                                  I B E X                                   
// File        : ibex_PdcFirstOrder.h
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Oct 02, 2013
// Last Update : Oct 02, 2013
//============================================================================

#include "ibex_PdcFirstOrder.h"
#include "ibex_Linear.h"

namespace ibex {

PdcFirstOrder::PdcFirstOrder(const System& sys) : Pdc(sys.nb_var), sys(sys), e(NULL) { }

BoolInterval PdcFirstOrder::test(const IntervalVector& box) {

	int n=sys.nb_var;
	int M=sys.nb_ctr;
	// count the number of active constraints
	// in the original system
	for (int j=0; j<sys.nb_ctr; j++) {
		if (e && e->original(j)) M--;
	}

	if (M>n) return MAYBE; // cannote be full rank

	int j2=0;
	IntervalMatrix J(M+1,n); // +1 because we add the gradient of f

	sys.goal->gradient(box,J.row(j2++));

	for (int j=0; j<sys.nb_ctr; j++) {
		if (e && e->original(j)) {
			continue;
		}
		sys.f[j].gradient(box,J.row(j2++));
	}

	// check for invertibility
	int p[M+1]; // will not be used
	//int q[n];   // will not be used
	IntervalMatrix LU(M+1,n); // will not be used
	try {
		// note: seems worse with complete pivoting
		interval_LU(J, LU, p); //, q);
		return NO;
	} catch(SingularMatrixException&) {
		return MAYBE;
	}
}



} // end namespace ibex
