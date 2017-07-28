/* ============================================================================
 * I B E X - LinearSolver Tests
 * ============================================================================
 * Copyright   : ENSTA-Bretagne (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Jordan Ninin
 * Created     : Mar 2, 2017
 * ---------------------------------------------------------------------------- */

#include "TestLinearSolver.h"
#include "ibex_LinearSolver.h"

using namespace std;

namespace ibex {



void TestLinearSolver::kleemin3() {

	int n=3;
	LinearSolver lp(n);
	Vector v(n);

	for (int j=1;j<=n;j++) {
		v[j-1]= ::pow(10,n-j);
	}
	lp.set_obj(-v);

	IntervalVector bound (n, Interval::POS_REALS);
	lp.set_bounds(bound);

	for (int i=1;i<=n;i++) {
		v=Vector::zeros(n);
		for (int j=1;j<=i-1;j++) {
			v[j-1]= 2*(::pow(10,i-j));
		}
		v[i-1] =1;
		lp.add_constraint(v,LEQ, ::pow(10,i-1));
	}

	LinearSolver::Status_Sol res = lp.solve_proved();

	lp.write_file("coucou.lp");

	Vector dualsol = lp.get_dual_sol();
	Vector primalsol = lp.get_primal_sol();

	double vrai1[3]= {0,0,100};
	Vector vrai2(3,vrai1);
	IntervalVector vrai_sol(vrai2);
	almost_eq(vrai_sol,primalsol,1.e-12);
}

} // end namespace
