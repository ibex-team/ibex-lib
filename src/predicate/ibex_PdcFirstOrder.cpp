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

PdcFirstOrder::PdcFirstOrder(const System& sys, const IntervalVector& init_box) : Pdc(sys.nb_var), sys(sys), init_box(init_box), e(NULL) { }

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
	IntervalMatrix* J=new IntervalMatrix(M+1,n); // +1 because we add the gradient of f

	sys.goal->gradient(box,J->row(j2++));

	for (int j=0; j<sys.nb_ctr; j++) {
		if (e && e->original(j)) {
			continue;
		}
		sys.f[j].gradient(box,J->row(j2++));
	}

	int N=sys.nb_var; // final number of variables
	// check the active bouding constraints
	for (int i=0; i<sys.nb_var; i++) {
		// if the ith bounding constraint is active
		// we will remove the ith column in the matrix J2
		if (!box[i].is_strict_subset(init_box[i])) N--;
	}

	IntervalMatrix* J2;
	if (N==n) J2=J; // useless to build J a second time.
	else {
		J2 = new IntervalMatrix(M+1,N);
		int i2=0;
		for (int i=0; i<sys.nb_var; i++) {
			if (box[i].is_strict_subset(init_box[i]))
				J2->set_col(i2++,J->col(i));
		}
		delete J;
		assert(i2==N);
	}

	// check for invertibility
	int p[M+1]; // will not be used
	//int q[n];   // will not be used
	IntervalMatrix LU(M+1,N); // will not be used
	try {
		// note: seems worse with complete pivoting
		interval_LU(*J2, LU, p); //, q);
		return NO;
	} catch(SingularMatrixException&) {
		return MAYBE;
	}

	delete J2;
}



} // end namespace ibex
