//============================================================================
//                                  I B E X                                   
// File        : ibex_PdcHansenFeasibility.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Aug 29, 2013
// Last Update : Aug 29, 2013
//============================================================================

#include "ibex_PdcHansenFeasibility.h"
#include "ibex_Newton.h" // just for default_gauss_seidel_ratio!
#include "ibex_Linear.h" // just for default_gauss_seidel_ratio!

namespace ibex {

PdcHansenFeasibility::PdcHansenFeasibility(Function& f) : Pdc(f.nb_var()), f(f) {

}

BoolInterval PdcHansenFeasibility::test(const IntervalVector& box) {

	assert(f.expr().dim.is_vector());
	int n=f.nb_var();
	int m=f.expr().dim.vec_size();
	IntervalVector mid=box.mid();

	/* Determine the "most influencing" variable thanks to
	 * the pivoting of Gauss elimination */
	// ==============================================================
	Matrix A=f.jacobian(mid).mid();
	Matrix LU(m,n);
	int pr[m];
	int pc[n]; // the interesting output: the variables permutation
	real_LU(A,LU,pr,pc);
	// ==============================================================

	/* Fix the n-m last coordinates to their midpoint */
	IntervalVector x(n);
	for (int i=0; i<n; i++) {
		if (pc[i]>m) x[i]=box[i].mid();
		else x[i]=box[i];
	}

	/* Calculate the Jacobi matrix and image of this subbox */
	// TODO: we should give the permutation to hansen_matrix?
	IntervalMatrix J(m,n);
	f.hansen_matrix(x,J);
	IntervalVector Fmid=f.eval_vector(x);

	/* Build the square subsystem */
	IntervalMatrix J2(m,m);
	for (int i=0; i<m; i++) {
		for (int j=0; j<m; j++) J2[i][j]=J[i][pc[j]];
	}
	IntervalVector box2(m);
	for (int j=0; j<m; j++) box2[j]=box[pc[j]];

	IntervalVector mid2(m);
	for (int j=0; j<m; j++) mid2[j]=mid[pc[j]];

	IntervalVector y = mid-box;

	try {
		// What Hansen suggests apparently is to reuse the
		// LU decomposition above for calculating an approximate
		// inverse of J2's midpoint. Seems complicated to implement
		precond(J2, Fmid);

		gauss_seidel(J2, Fmid, y, default_gauss_seidel_ratio);

		if (y.is_empty()) { return MAYBE; }
	} catch (LinearException& ) {
		return MAYBE;
	}

	IntervalVector box3=mid-y;

	if ((box3 &= box).is_empty()) { return MAYBE; }

	if (box3.is_strict_subset(box)) return YES;

	return MAYBE;
}

} // end namespace ibex
