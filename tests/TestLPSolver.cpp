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

#include "TestLPSolver.h"
#include "ibex_LPSolver.h"

using namespace std;

namespace ibex {

void TestLinearSolver::test01() {
	int n=3;
	LPSolver lp(n);
	Vector v(n);

	for (int j=1;j<=n;j++) {
		v[j-1]= ::pow(10,n-j);
	}
	lp.set_obj(-v);

	IntervalVector bound (n, Interval(-30,30));
	lp.set_bounds(bound);

	for (int i=1;i<=n;i++) {
		v=Vector::zeros(n);
		for (int j=1;j<=i-1;j++) {
			v[j-1]= 2*(::pow(10,i-j));
		}
		v[i-1] =1;
		lp.add_constraint(v,LEQ, ::pow(10,i-1));
	}

}


void TestLinearSolver::kleemin( int n) {
	LPSolver lp(n);
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

	LPSolver::Status_Sol res = lp.solve_proved();
	CPPUNIT_ASSERT(res==LPSolver::OPTIMAL_PROVED);

	double eps = lp.get_epsilon();
	lp.write_file("coucou.lp");

	Vector dualsol = lp.get_dual_sol();
	Vector primalsol = lp.get_primal_sol();
	Vector vrai(n);
	vrai[n-1] = ::pow(10,n-1);
	check_relatif(vrai,primalsol,1.e-9);

}

void TestLinearSolver::kleemin30() {
	int n=30;
	LPSolver lp(n);
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
	LPSolver::Status_Sol res;
	CPPUNIT_ASSERT_ASSERTION_PASS(
	 res = lp.solve_proved()
	);
	CPPUNIT_ASSERT(res!=LPSolver::INFEASIBLE_PROVED);

	CPPUNIT_ASSERT_ASSERTION_PASS(
	lp.write_file("coucou.lp")
	);

	CPPUNIT_ASSERT_ASSERTION_PASS(
	double eps = lp.get_epsilon()
	);
	switch (res) {
		case (LPSolver::INFEASIBLE):
		case (LPSolver::UNKNOWN): {
			CPPUNIT_ASSERT_THROW(
			Vector dualsol = lp.get_dual_sol(),
			LPException
			);
			CPPUNIT_ASSERT_THROW(
			Vector primalsol = lp.get_primal_sol(),
			LPException
			);
			break;
		}
		case (LPSolver::OPTIMAL):
		case (LPSolver::OPTIMAL_PROVED): {
			CPPUNIT_ASSERT_ASSERTION_PASS(
			Vector dualsol = lp.get_dual_sol()
			);
			Vector primalsol(n);
			CPPUNIT_ASSERT_ASSERTION_PASS(
			primalsol = lp.get_primal_sol()
			);
			Vector vrai(n);
			vrai[n-1] = ::pow(10,n-1);
			check_relatif(vrai,primalsol,1.e-9);
			break;
		}
		default:
			CPPUNIT_ASSERT(false);
	}

}

} // end namespace
