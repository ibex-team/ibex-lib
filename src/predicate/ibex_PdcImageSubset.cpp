//============================================================================
//                                  I B E X                                   
// File        : ibex_PdcImageSubset.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Dec 21, 2012
// Last Update : Dec 21, 2012
//============================================================================

#include "ibex_PdcImageSubset.h"
#include "ibex_LinearException.h"
#include "ibex_Linear.h"
#include <cassert>

namespace ibex {
namespace {

IntervalMatrix inv_diag(const IntervalMatrix& m) {
	int n=m.nb_rows();
	IntervalMatrix minv(n,n,Interval::ZERO);
	for (int i=0; i<n; i++) {
		if (m[i][i].contains(0)) throw SingularMatrixException();
		minv[i][i]=1/m[i][i];
	}
	return minv;
}

IntervalMatrix off_diag(const IntervalMatrix& m) {
	int n=m.nb_rows();
	IntervalMatrix m2(n,n);
	for (int i=0; i<n; i++) {
		for (int j=0; j<n; j++) {
			if (i==j) m2[i][j]=Interval::ZERO;
			else m2[i][j]=m[i][j];
		}
	}
	return m2;
}

}

PdcImageSubset::PdcImageSubset(Function& f, const IntervalVector& x0, Pdc& p_in) : Pdc(2*f.nb_var()), f(f), x0(x0), p_in(p_in) {
	assert(x0.size()==f.nb_var());
	assert(f.image_dim()==f.nb_var()); // only square functions accepted
}


BoolInterval PdcImageSubset::test(const IntervalVector& xy) {

	int n=f.nb_var();
	assert(xy.size()==2*n);

	IntervalVector xtilde=xy.subvector(0,n-1);
	IntervalVector ytilde=xy.subvector(n,2*n-1);

	Vector xmid=xtilde.mid();
	Matrix Jxt=f.jacobian(xmid).mid(); // other option: take J.mid() directly (see below)
	Matrix C(n,n);
	try {
		real_inverse(Jxt,C);
	} catch(SingularMatrixException& ) {
		return MAYBE;
	}

	double tau=1.01;
	double mu=0.9;
	IntervalMatrix J(n,n);
	IntervalVector b=C*ytilde-C*f.eval_vector(xmid);
	double dk=POS_INFINITY;
	double dk1=POS_INFINITY;

	IntervalVector x2(n);
	while (dk<=mu*dk1 && xtilde.is_subset(x0) && p_in.test(xtilde)==YES) {
		f.jacobian(xtilde,J);
		x2=xtilde-xmid;

		// sol 1
		//gauss_seidel(C*J,b,x2);
		// --> wrong

		// sol 2
		J=C*J;
		try {
			x2=inv_diag(J)*(b-(off_diag(J)*x2));
		} catch (SingularMatrixException& ) {
			return MAYBE;
		}

		if (x2.is_empty()) return MAYBE; // MAYBE or NO ?
		if ((xmid+x2).is_subset(xtilde)) return YES;
		dk1=dk;
		x2=xmid+tau*x2;
		dk=distance(xtilde,x2);
		xtilde=x2;
	}

	return MAYBE;
}

} // end namespace ibex
