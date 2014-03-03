//============================================================================
//                                  I B E X                                   
// File        : ibex_Newton.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : May 24, 2012
// Last Update : May 24, 2012
//============================================================================

#include "ibex_Newton.h"
#include "ibex_Linear.h"
#include "ibex_LinearException.h"
#include "ibex_EmptyBoxException.h"
#include <cassert>

using namespace std;

namespace ibex {

double default_newton_prec=1e-07;
double default_gauss_seidel_ratio=1e-04;


namespace {
//
//inline bool newton_step(const Fnc& f, IntervalVector& box,
//		IntervalVector& mid, IntervalVector& Fmid, IntervalMatrix& J) {
//
//	f.hansen_matrix(box,J); //may throw EmptyBoxException?
//	if (J.is_empty()) { return false; }
//	mid = box.mid();
//	Fmid=f.eval_vector(mid);
//
}

bool newton(const Fnc& f, IntervalVector& box, double prec, double ratio_gauss_seidel) {
	int n=f.nb_var();
	int m=f.image_dim();
	assert(box.size()==n);

	IntervalMatrix J(m, n);
	IntervalVector y(n);
	IntervalVector y1(n);
	IntervalVector mid(n);
	IntervalVector Fmid(m);
	bool reducted=false;
	double gain;
	y1= box.mid();

	do {
//		cout.precision(20);
//		cout << box << endl << endl << endl;
		f.hansen_matrix(box,J); //may throw EmptyBoxException?
//		f.jacobian(box,J); //may throw EmptyBoxException
		if (J.is_empty()) { return false; }
//		for (int i=0; i<m; i++)
//			for (int j=0; j<n; j++)
//				if (J[i][j].mag()>1e-10) cout << "(" << i << "," << j << ")=" << J[i][j] << "  ";

		/* remove this block
		 *
		 for (int i=0; i<m; i++)
			for (int j=0; j<n; j++)
				if (J[i][j].is_unbounded()) return false;
		 */

		mid = box.mid();

		Fmid=f.eval_vector(mid);

		y = mid-box;
		if (y==y1) break;
		y1=y;

		try {
			precond(J, Fmid);

			gauss_seidel(J, Fmid, y, ratio_gauss_seidel);

			if (y.is_empty()) { box.set_empty(); throw EmptyBoxException(); }
		} catch (LinearException& ) {
			return reducted; // should be false
		}

		IntervalVector box2=mid-y;

		if ((box2 &= box).is_empty()) { box.set_empty(); throw EmptyBoxException(); }

		gain = box.maxdelta(box2);

		if (gain >= prec) reducted = true;

		box=box2;

	}
	while (gain >= prec);
	return reducted;
}

bool inflating_newton(const Fnc& f, IntervalVector& box, int k_max, double mu_max, double delta, double chi) {
	int n=f.nb_var();
	int m=f.image_dim();
	assert(box.size()==n);

	int k=0;
	bool success=false;

	IntervalMatrix J(m, n);
	IntervalVector y(n);
	IntervalVector y1(n);
	IntervalVector mid(n);
	IntervalVector Fmid(m);

	y1= box.mid();

	while (k<k_max) {

		f.hansen_matrix(box,J);

		if (J.is_empty()) { return false; }

		mid = box.mid();
		Fmid=f.eval_vector(mid);

		y = mid-box;
		if (y==y1) break;
		y1=y;

		precond(J, Fmid);

		// Note: giving mu_max to gauss-seidel (GS) is slightly different from checking the condition "mu<mu_max" in the
		// Newton procedure itself. If GS transforms x0 to x1 in n iterations, and then x1 to x2 in n other iterations
		// it is possible that each of these 2n iterations satisfies mu<mu_max, whereas the two global Newton iterations
		// do not, i.e., d(x2,x1) > mu_max d(x1,x0).
		if (!inflating_gauss_seidel(J, Fmid, y, 1e-12, mu_max)) // TODO: replace hardcoded value 1e-12
			// when k~kmax, "divergence" may also mean "cannot contract more" (d/dold~1)
			return success;

		IntervalVector box2=mid-y;

		if (box2.is_subset(box)) {
			success=true;  // we don't return now, to let the box being contracted more
		}

		box=box2;
		k++;
	}
	return success;
}

} // end namespace ibex
