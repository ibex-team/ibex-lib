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

LPSolver TestLinearSolver::create_kleemin(int n) {
	LPSolver lp(n);
	Vector v(n);

	for (int j=1;j<=n;j++) {
		v[j-1]= ::pow(10,n-j);
	}
	lp.set_obj(-v);

	IntervalVector bound (n, Interval::pos_reals());
	lp.set_bounds(bound);

	for (int i=1;i<=n;i++) {
		v=Vector::zeros(n);
		for (int j=1;j<=i-1;j++) {
			v[j-1]= 2*(::pow(10,i-j));
		}
		v[i-1] =1;
		lp.add_constraint(v,LEQ, ::pow(10,i-1));
	}

	return lp;
}

void TestLinearSolver::kleemin( int n) {
	LPSolver lp(create_kleemin(n));

	LPSolver::Status res = lp.optimize_proved();
	CPPUNIT_ASSERT(res==LPSolver::Status::OptimalProved);
	double eps = lp.tolerance();
	lp.write_to_file("coucou" + std::to_string(n) + ".lp");

	Vector dualsol = lp.uncertified_dual_sol();
	Vector primalsol = lp.uncertified_primal_sol();
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

	IntervalVector bound (n, Interval::pos_reals());
	lp.set_bounds(bound);

	for (int i=1;i<=n;i++) {
		v=Vector::zeros(n);
		for (int j=1;j<=i-1;j++) {
			v[j-1]= 2*(::pow(10,i-j));
		}
		v[i-1] =1;
		lp.add_constraint(v,LEQ, ::pow(10,i-1));
	}
	LPSolver::Status res;
	CPPUNIT_ASSERT_ASSERTION_PASS(
	 res = lp.optimize_proved()
	);

	CPPUNIT_ASSERT(res!=LPSolver::Status::InfeasibleProved);

	CPPUNIT_ASSERT_ASSERTION_PASS(
	lp.write_to_file("coucou.lp")
	);

	CPPUNIT_ASSERT_ASSERTION_PASS(
	double eps = lp.tolerance()
	);
	switch (res) {
		case (LPSolver::Status::Infeasible):
		case (LPSolver::Status::Unknown): {
			CPPUNIT_ASSERT_THROW(
			Vector dualsol = lp.uncertified_dual_sol(),
			LPException
			);
			CPPUNIT_ASSERT_THROW(
			Vector primalsol = lp.uncertified_primal_sol(),
			LPException
			);
			break;
		}
		case (LPSolver::Status::Optimal):
		case (LPSolver::Status::OptimalProved): {
			CPPUNIT_ASSERT_ASSERTION_PASS(
			Vector dualsol = lp.uncertified_dual_sol()
			);
			Vector primalsol(n);
			CPPUNIT_ASSERT_ASSERTION_PASS(
			primalsol = lp.uncertified_primal_sol()
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

void TestLinearSolver::add_column() {
/*	LPSolver lp_ref(create_kleemin(8));
	LPSolver::Status res_ref = lp_ref.optimize();


	LPSolver lp(create_kleemin(8));
	Vector v(lp.nb_rows());
	lp.add_column(1, v, Interval(-1, 1));
	int n = lp.get_nb_vars();
	lp.set_bounds_var(n-1, Interval(-2, 2));
	LPSolver::Status res = lp.solve_proved();
	CPPUNIT_ASSERT(res==LPSolver::Status::Optimal_PROVED);

	//lp.write_file("coucou.lp");
	//lp_ref.write_file("coucou_ref.lp");
	Vector primalsol = lp.get_primal_sol();
	Vector primalsol_ref(n);
	primalsol_ref.put(0, lp.get_primal_sol());
	primalsol_ref[n-1] = primalsol[n-1];
	CPPUNIT_ASSERT(primalsol_ref == primalsol);*/
}

} // end namespace
