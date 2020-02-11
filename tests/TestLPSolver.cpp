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
	lp.set_cost(-v);

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
	LPSolver lp(n, LPSolver::Mode::Certified);
	Vector v(n);

	for (int j=1;j<=n;j++) {
		v[j-1]= ::pow(10,n-j);
	}
	lp.set_cost(-v);

	IntervalVector bound (n, Interval(0, 1e200));
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

	LPSolver::Status res = lp.minimize();
	CPPUNIT_ASSERT(res==LPSolver::Status::OptimalProved);

	Vector dualsol = lp.not_proved_dual_sol();
	Vector primalsol = lp.not_proved_primal_sol();
	Vector vrai(n);
	vrai[n-1] = ::pow(10,n-1);
	check_relatif(vrai,primalsol,1.e-9);
}

void TestLinearSolver::kleemin30() {
	int n=30;
	LPSolver lp(n, LPSolver::Mode::Certified);
	Vector v(n);

	for (int j=1;j<=n;j++) {
		v[j-1]= ::pow(10,n-j);
	}
	lp.set_cost(-v);

	IntervalVector bound (n, Interval(0, 1e200));
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
	 res = lp.minimize()
	);

	CPPUNIT_ASSERT(res!=LPSolver::Status::InfeasibleProved);

	switch (res) {
		case (LPSolver::Status::Infeasible):
		case (LPSolver::Status::Unbounded):
		case (LPSolver::Status::Unknown): {
// ---> error
//			CPPUNIT_ASSERT_THROW(
//			Vector dualsol = lp.not_proved_dual_sol(),
//			LPException
//			);
//			CPPUNIT_ASSERT_THROW(
//			Vector primalsol = lp.not_proved_primal_sol(),
//			LPException
//			);
			break;
		}
		case (LPSolver::Status::Optimal):
		case (LPSolver::Status::OptimalProved): {
			CPPUNIT_ASSERT_ASSERTION_PASS(
			Vector dualsol = lp.not_proved_dual_sol()
			);
			Vector primalsol(n);
			CPPUNIT_ASSERT_ASSERTION_PASS(
			primalsol = lp.not_proved_primal_sol()
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

void TestLinearSolver::reset() {
	LPSolver lp(create_kleemin(8));
	int n = 3;
	lp.reset(n);

	// copy-past kleemin to
	Vector v(n);

	for (int j=1;j<=n;j++) {
		v[j-1]= ::pow(10,n-j);
	}
	lp.set_cost(-v);

	IntervalVector bound (n, Interval(0, 1e200));
	lp.set_bounds(bound);

	for (int i=1;i<=n;i++) {
		v=Vector::zeros(n);
		for (int j=1;j<=i-1;j++) {
			v[j-1]= 2*(::pow(10,i-j));
		}
		v[i-1] =1;
		lp.add_constraint(v,LEQ, ::pow(10,i-1));
	}

	LPSolver::Status res = lp.minimize();
	CPPUNIT_ASSERT(res==LPSolver::Status::OptimalProved);

	Vector dualsol = lp.not_proved_dual_sol();
	Vector primalsol = lp.not_proved_primal_sol();
	Vector vrai(n);
	vrai[n-1] = ::pow(10,n-1);
	check_relatif(vrai,primalsol,1.e-9);
}

void TestLinearSolver::test_known_problem(std::string filename, double optimal) {
	LPSolver lp_ref(filename);
	LPSolver lp(lp_ref.nb_vars(), LPSolver::Mode::NotCertified);
	double scaling=1;
	lp.add_constraints(lp_ref.lhs(), lp_ref.rows(), lp_ref.rhs());
	lp.set_bounds(lp_ref.bounds());
	lp.set_cost(scaling*lp_ref.cost());
	lp.set_tolerance(1e-9);
	lp.set_max_iter(-1);
	lp.minimize();
	CPPUNIT_ASSERT(lp.status() == LPSolver::Status::Optimal);
	double obj = lp.minimum().lb();
	check_relatif(obj, scaling*optimal, 1e-9);
}

/*
 * The problem is
 * min x
 * s.t. y <= ax + a
 *      y >= 0
 *      x <= 0
 *
 * a = 1e-7 is the smallest value where the optimum is found under SoPlex 3.1.1
 */
void TestLinearSolver::nearly_parallel_constraints() {
	LPSolver lp(2, LPSolver::Mode::Certified);
	lp.set_bounds(0, Interval(-1e200, 0));
	lp.set_bounds(1, Interval(0, 1e200));
	double a = 1e-7;
	lp.add_constraint({-a, 1}, CmpOp::LEQ, a);
	lp.set_cost({1, 0});
	LPSolver::Status status = lp.minimize();
	CPPUNIT_ASSERT(status == LPSolver::Status::OptimalProved);
	double obj = lp.minimum().lb();
	check_relatif(obj, -1, 1e-9);
}

void TestLinearSolver::cost_parallel_to_constraint() {
	LPSolver lp(2, LPSolver::Mode::Certified);
	lp.set_bounds(0, Interval(-1e200, 0));
	lp.set_bounds(1, Interval(0, 1e200));
	double a = 1e-12; // a: 1 -> 0
	double b = 1/a; // b: 0 -> 1/a
	lp.add_constraint({-a, 1}, CmpOp::LEQ, a);
	lp.set_cost({1, -b});
	LPSolver::Status status = lp.minimize();
	CPPUNIT_ASSERT(status == LPSolver::Status::OptimalProved);
	double obj = lp.minimum().lb();
	check_relatif(obj, -1, 1e-9);
}

/*
 * x <= 0
 * y >= 0
 * y <= a x + eps
 * a: 1 -> 0
 * critere: tester cycling, scaling
 * c = x - beta y + beta
 * beta: 0 -> 1/alpha
 */
} // end namespace
