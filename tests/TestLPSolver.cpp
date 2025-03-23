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

LPSolver* TestLinearSolver::create_kleemin(int n) {
	LPSolver* lp = new LPSolver(n, LPSolver::Mode::Certified);
	Vector v(n);

	for (int j=1;j<=n;j++) {
		v[j-1]= ::pow(10,n-j);
	}
	lp->set_cost(-v);

	IntervalVector bound (n, Interval(0,1e200));
	lp->set_bounds(bound);

	for (int i=1;i<=n;i++) {
		v=Vector::zeros(n);
		for (int j=1;j<=i-1;j++) {
			v[j-1]= 2*(::pow(10,i-j));
		}
		v[i-1] =1;
		lp->add_constraint(v,LEQ, ::pow(10,i-1));
	}

	return lp;
}

void TestLinearSolver::kleemin( int n) {
	LPSolver* lp = create_kleemin(n);

	LPSolver::Status res = lp->minimize();
	CPPUNIT_ASSERT(res==LPSolver::Status::OptimalProved);

	Vector dualsol = lp->not_proved_dual_sol();
	Vector primalsol = lp->not_proved_primal_sol();
	Vector vrai(n);
	vrai[n-1] = ::pow(10,n-1);
	check_relatif(vrai,primalsol,1.e-9);
	delete lp;
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
	LPSolver* lp = create_kleemin(8);
	int n = 3;
	lp->reset(n);

	// copy-past kleemin to
	Vector v(n);

	for (int j=1;j<=n;j++) {
		v[j-1]= ::pow(10,n-j);
	}
	lp->set_cost(-v);

	IntervalVector bound (n, Interval(0, 1e200));
	lp->set_bounds(bound);

	for (int i=1;i<=n;i++) {
		v=Vector::zeros(n);
		for (int j=1;j<=i-1;j++) {
			v[j-1]= 2*(::pow(10,i-j));
		}
		v[i-1] =1;
		lp->add_constraint(v,LEQ, ::pow(10,i-1));
	}

	LPSolver::Status res = lp->minimize();
	CPPUNIT_ASSERT(res==LPSolver::Status::OptimalProved);

	Vector dualsol = lp->not_proved_dual_sol();
	Vector primalsol = lp->not_proved_primal_sol();
	Vector vrai(n);
	vrai[n-1] = ::pow(10,n-1);
	check_relatif(vrai,primalsol,1.e-9);
	delete lp;
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


void TestLinearSolver::test_easy_feasible() {
    /**
     maximize 3*x1 + 2*x2
     st:
        x1 + 2*x2 <= 4
        x1 - x2 <= 1
        x1,x2 >= 0
     
     Minimum at (2,1) with objective of 8
     **/
    LPSolver lp(2,LPSolver::Mode::NotCertified);
    
    IntervalVector x(2);
    for(int i=0;i<x.size();i++){
        x[i]=Interval::pos_reals();
    }
    
    lp.set_bounds(x);
    
    Vector row1(2);row1[0]=1;row1[1]=2;
    lp.add_constraint(NEG_INFINITY,row1,4);
    Vector row2(2);row2[0]=1;row2[1]=-1;
    lp.add_constraint(NEG_INFINITY,row2,1);

    Vector obj(2);obj[0]=-3;obj[1]=-2;
    lp.set_cost(obj);
    
    lp.minimize();

    CPPUNIT_ASSERT(lp.status() == LPSolver::Status::Optimal);
    //cout<<lp.minimum()<<endl;
    check_relatif(lp.minimum().mid(),-8,1.e-9);
}

void TestLinearSolver::test_easy_feasible_certified() {
    /**
     maximize 3*x1 + 2*x2
     st:
        x1 + 2*x2 <= 4
        x1 - x2 <= 1
        x1,x2 >= 0
     
     Minimum at (2,1) with objective of 8
     **/
    LPSolver lp_cert(2,LPSolver::Mode::Certified);
    
    IntervalVector x(2);
    
    // NOTE: next instruction allows to have the
    // same behaviour using Filib as using Gaol.
    // For this test, the side effects of rounding 
    // to nearest in the simplex eventually leads to
    // a failure. 
    fpu_round_up();
    
    for(int i=0;i<x.size();i++){
        x[i]=Interval::pos_reals();
    }
    
    lp_cert.set_bounds(x);
    
    Vector row1(2);row1[0]=1;row1[1]=2;
    lp_cert.add_constraint(NEG_INFINITY,row1,4);
    Vector row2(2);row2[0]=1;row2[1]=-1;
    lp_cert.add_constraint(NEG_INFINITY,row2,1);

    Vector obj(2);obj[0]=-3;obj[1]=-2;
    lp_cert.set_cost(obj);
    
    lp_cert.minimize();

    CPPUNIT_ASSERT(lp_cert.status() == LPSolver::Status::OptimalProved);
    //cout<<lp_cert.minimum()<<endl;
    check_relatif(lp_cert.minimum().lb(),-8,1.e-9);
}

void TestLinearSolver::test_unbounded() {
    /**
     maximize 3*x1 + 2*x2
     st:
        x1 + 2*x2 >= 4
        x1 - x2 >= 1
        x1,x2 >= 0
     
     Unbounded
     **/
    LPSolver lp(2,LPSolver::Mode::NotCertified);
    
    IntervalVector x(2);
    for(int i=0;i<x.size();i++){
        x[i]=Interval::pos_reals();
    }
    
    lp.set_bounds(x);
    
    Vector row1(2);row1[0]=1;row1[1]=2;
    lp.add_constraint(4,row1,POS_INFINITY);
    Vector row2(2);row2[0]=1;row2[1]=-1;
    lp.add_constraint(1,row2,POS_INFINITY);

    Vector obj(2);obj[0]=-3;obj[1]=-2;
    lp.set_cost(obj);
    
    lp.minimize();
    CPPUNIT_ASSERT(lp.status() == LPSolver::Status::Unbounded);
}

void TestLinearSolver::test_unbounded_certified() {
    LPSolver lp_cert(2,LPSolver::Mode::Certified);
    
    IntervalVector x(2);
    for(int i=0;i<x.size();i++){
        x[i]=Interval::pos_reals();
    }
    
    lp_cert.set_bounds(x);
    
    Vector row1(2);row1[0]=1;row1[1]=2;
    lp_cert.add_constraint(4,row1,POS_INFINITY);
    Vector row2(2);row2[0]=1;row2[1]=-1;
    lp_cert.add_constraint(1,row2,POS_INFINITY);

    Vector obj(2);obj[0]=-3;obj[1]=-2;
    lp_cert.set_cost(obj);
    
    lp_cert.minimize();
    
    CPPUNIT_ASSERT(lp_cert.status() == LPSolver::Status::Unbounded);
}

void TestLinearSolver::test_infeasible() {
    /**
     maximize 3*x1 + 2*x2
     st:
        x1 + 2*x2 <= -4
        x1 - x2 >= 1
        x1,x2 >= 0
     
     Minimum at (2,1) with objective of 8
     **/
    
    LPSolver lp(2,LPSolver::Mode::NotCertified);
    
    IntervalVector x(2);
    for(int i=0;i<x.size();i++){
        x[i]=Interval::pos_reals();
    }
    
    lp.set_bounds(x);
    
    Vector row1(2);row1[0]=1;row1[1]=2;
    lp.add_constraint(NEG_INFINITY,row1,-4);
    Vector row2(2);row2[0]=1;row2[1]=-1;
    lp.add_constraint(1,row2,POS_INFINITY);

    Vector obj(2);obj[0]=-3;obj[1]=-2;
    lp.set_cost(obj);
    
    lp.minimize();
    CPPUNIT_ASSERT(lp.status() == LPSolver::Status::Infeasible);
}

void TestLinearSolver::test_infeasible_certified() {
    /**
     maximize 3*x1 + 2*x2
     st:
        x1 + 2*x2 <= -4
        x1 - x2 >= 1
        x1,x2 >= 0
     
     Minimum at (2,1) with objective of 8
     **/
    
    LPSolver lp_cert(2,LPSolver::Mode::Certified);
    
    IntervalVector x(2);
    for(int i=0;i<x.size();i++){
        x[i]=Interval::pos_reals();
    }
    
    lp_cert.set_bounds(x);
    
    Vector row1(2);row1[0]=1;row1[1]=2;
    lp_cert.add_constraint(NEG_INFINITY,row1,-4);
    Vector row2(2);row2[0]=1;row2[1]=-1;
    lp_cert.add_constraint(1,row2,POS_INFINITY);

    Vector obj(2);obj[0]=-3;obj[1]=-2;
    lp_cert.set_cost(obj);
    
    lp_cert.minimize();
    CPPUNIT_ASSERT(lp_cert.status() == LPSolver::Status::InfeasibleProved);
}


void TestLinearSolver::test_model(){
    /**
     maximize 3*x1 + 2*x2
     st:
        x1 + 2*x2 <= 4
        x1 - x2 <= 1
        x1,x2 >= 0
     
     Minimum at (2,1) with objective of 8
     **/
    LPSolver lp(2,LPSolver::Mode::Certified);
    
    IntervalVector x(2);
    for(int i=0;i<x.size();i++){
        x[i]=Interval::pos_reals();
    }
    
    lp.set_bounds(x);
    
    Vector row1(2);row1[0]=1;row1[1]=2;
    lp.add_constraint(NEG_INFINITY,row1,4);
    Vector row2(2);row2[0]=1;row2[1]=-1;
    lp.add_constraint(NEG_INFINITY,row2,1);

    Vector obj(2);obj[0]=-3;obj[1]=-2;
    lp.set_cost(obj);
    
    lp.minimize();
    
    Matrix A = lp.rows();
    for (int i=0;i<lp.nb_vars();i++){
        CPPUNIT_ASSERT(A[i][i] == 1);
    }
    CPPUNIT_ASSERT(A[lp.nb_vars()][0] == 1);
    CPPUNIT_ASSERT(A[lp.nb_vars()][1] == 2);
    CPPUNIT_ASSERT(A[lp.nb_vars()+1][0] == 1);
    CPPUNIT_ASSERT(A[lp.nb_vars()+1][1] == -1);
    
    CPPUNIT_ASSERT(lp.row(0)[0] == 1);
    Vector row_0 = lp.row(lp.nb_vars());
    Vector row_1 = lp.row(lp.nb_vars()+1);
    CPPUNIT_ASSERT(row_0[0] == 1);
    CPPUNIT_ASSERT(row_0[1] == 2);
    CPPUNIT_ASSERT(row_1[0] == 1);
    CPPUNIT_ASSERT(row_1[1] == -1);
    
    Vector col_0 = lp.col(0);
    CPPUNIT_ASSERT(col_0[0] == 1);
    CPPUNIT_ASSERT(col_0[1] == 0);
    CPPUNIT_ASSERT(col_0[2] == 1);
    CPPUNIT_ASSERT(col_0[3] == 1);
    
    Vector col_1 = lp.col(1);
    CPPUNIT_ASSERT(col_1[0] == 0);
    CPPUNIT_ASSERT(col_1[1] == 1);
    CPPUNIT_ASSERT(col_1[2] == 2);
    CPPUNIT_ASSERT(col_1[3] == -1);
    
    
    Matrix AT = lp.rows_transposed();
    for (int i=0;i<lp.nb_vars();i++){
        CPPUNIT_ASSERT(AT[i][i] == 1);
    }
    CPPUNIT_ASSERT(AT[0][lp.nb_vars()] == 1);
    CPPUNIT_ASSERT(AT[1][lp.nb_vars()] == 2);
    CPPUNIT_ASSERT(AT[0][lp.nb_vars()+1] == 1);
    CPPUNIT_ASSERT(AT[1][lp.nb_vars()+1] == -1);
    
    Vector lhs = lp.lhs();
    CPPUNIT_ASSERT(lhs[0] == 0);
    CPPUNIT_ASSERT(lhs[1] == 0);
    CPPUNIT_ASSERT(lhs[2] <= -1e308);
    CPPUNIT_ASSERT(lhs[3] <= -1e308);
    
    Vector rhs = lp.rhs();
    CPPUNIT_ASSERT(rhs[0] >= 1e308);
    CPPUNIT_ASSERT(rhs[1] >= 1e308);
    CPPUNIT_ASSERT(rhs[2] == 4);
    CPPUNIT_ASSERT(rhs[3] == 1);
    
    IntervalVector lhs_rhs = lp.lhs_rhs();
    CPPUNIT_ASSERT(lhs_rhs[0].lb() == 0);
    CPPUNIT_ASSERT(lhs_rhs[1].lb() == 0);
    CPPUNIT_ASSERT(lhs_rhs[2].lb() <= -1e308);
    CPPUNIT_ASSERT(lhs_rhs[3].lb() <= -1e308);
    CPPUNIT_ASSERT(lhs_rhs[0].ub() >= 1e308);
    CPPUNIT_ASSERT(lhs_rhs[1].ub() >= 1e308);
    CPPUNIT_ASSERT(lhs_rhs[2].ub() == 4);
    CPPUNIT_ASSERT(lhs_rhs[3].ub() == 1);
    
    Vector cost = lp.cost();
    CPPUNIT_ASSERT(cost[0] == -3);
    CPPUNIT_ASSERT(cost[1] == -2);
    
    IntervalVector bnds = lp.bounds();
    CPPUNIT_ASSERT(bnds[0].lb() == 0);
    CPPUNIT_ASSERT(bnds[1].lb() == 0);
    CPPUNIT_ASSERT(bnds[0].ub() == POS_INFINITY);
    CPPUNIT_ASSERT(bnds[1].ub() == POS_INFINITY);
    
    Interval bnd_0 = lp.bounds(0);
    CPPUNIT_ASSERT(bnd_0.lb() == 0);
    CPPUNIT_ASSERT(bnd_0.ub() == POS_INFINITY);
    
    lp.clear_constraints();
    CPPUNIT_ASSERT(lp.nb_rows() == lp.nb_vars());
    
    lp.clear_bounds();
    CPPUNIT_ASSERT(lp.bounds()[0] == Interval::ALL_REALS);
    CPPUNIT_ASSERT(lp.bounds()[1] == Interval::ALL_REALS);
    
    lp.reset(lp.nb_vars());
    CPPUNIT_ASSERT(lp.status() == LPSolver::Status::Unknown);
    CPPUNIT_ASSERT(lp.cost()[0] == 0);
    CPPUNIT_ASSERT(lp.cost()[1] == 0);
    CPPUNIT_ASSERT(lp.nb_rows() == lp.nb_vars());
    CPPUNIT_ASSERT(lp.bounds()[0] == Interval::ALL_REALS);
    CPPUNIT_ASSERT(lp.bounds()[1] == Interval::ALL_REALS);
    
}


} // end namespace
