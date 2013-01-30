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

bool newton(const Function& f, IntervalVector& box, double prec, double ratio_gauss_seidel) {
	assert(f.expr().dim.is_vector());
	int n=f.nb_var();
	int m=f.expr().dim.vec_size();

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

} // end namespace ibex
