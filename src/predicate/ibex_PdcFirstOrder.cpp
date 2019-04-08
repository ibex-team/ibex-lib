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

using namespace std;

namespace ibex {

PdcFirstOrder::PdcFirstOrder(const System& sys, const IntervalVector& init_box) :
		Pdc(sys.nb_var), sys(sys), init_box(init_box), multiplier_sign_test(true) {

	if (!sys.goal) {
		ibex_error("[PdcFirstOrder] not an optimization problem.");
	}

	for (int i=0; i<sys.nb_ctr; i++) {
		if (sys.ops[i]==EQ) multiplier_sign_test=false;
	}

}

BoolInterval PdcFirstOrder::test(const IntervalVector& box) {

	BoolInterval res;
	int n=sys.nb_var;
	// note: calling directly sys.active_ctrs_jacobian(box)
	// fails if there is no active constraint
	BitSet bitset=sys.active_ctrs(box);

	if (bitset.empty()) {
		if (!sys.goal->gradient(box).contains(Vector::zeros(n)))
			return NO;
		else
			return MAYBE;
	}

	// count the number of active constraints
	// in the system
	int M=bitset.size();

	if (M>n) {
		return MAYBE;
	} // cannot be full rank

	IntervalMatrix* J=new IntervalMatrix(M+1,n); // +1 because we add the gradient of f

	sys.goal->gradient(box,J->row(0));

	J->put(1,0,sys.f_ctrs.jacobian(box,bitset));

	int N=sys.nb_var; // final number of variables

	// check the active bounding constraints
	for (int i=0; i<sys.nb_var; i++) {
		// if the ith bounding constraint is active
		// we will remove the ith column in the matrix J2
		if (!box[i].is_interior_subset(init_box[i])) {
			if (box[i].is_superset(init_box[i])) {
				delete J;
				// two linearly dependent bound constraints
				// activated at the same time --> MAYBE
				return MAYBE; // cannot be full rank
			}
			else {
				N--;
			}
		}
	}

	IntervalMatrix* J2;
	if (N==n) J2=J; // useless to build J a second time.
	else if (M+1>N) { // cannot be full rank
		delete J;
		return MAYBE;
	}
	else {
		J2 = new IntervalMatrix(M+1,N);
		int i2=0;
		for (int i=0; i<sys.nb_var; i++) {
			if (box[i].is_interior_subset(init_box[i]))
				J2->set_col(i2++,J->col(i));
		}
		delete J;
		assert(i2==N);
	}

	// multiplier sign test
	if (multiplier_sign_test) {
		for (int j=0; j<N; j++) {
			bool sign;

			if ((*J2)[0][j].lb()>0)
				sign=true;
			else if ((*J2)[0][j].ub()<0)
				sign=false;
			else
				continue;

			int i=0;
			for (; i<M+1; i++) {
				if (sign) {
					if ((*J2)[i][j].lb()<=0) break;
				} else {
					if ((*J2)[i][j].ub()>=0) break;
				}
			}
			if (i==M+1) // the whole column has the same sign
				return NO;
		}
	}

	// check for invertibility
	int *p=new int[M+1]; // will not be used
	int *q=new int[n];   // will not be used
	IntervalMatrix LU(M+1,N); // will not be used
	try {
		// note: seems worse with complete pivoting
		interval_LU(*J2, LU, p, q); //, q);
		// the matrix is rank M+1
		res = NO;
	} catch(SingularMatrixException&) {
		res = MAYBE;
	}
	delete J2;
	delete [] p;
	delete [] q;
	return res;
}



} // end namespace ibex
