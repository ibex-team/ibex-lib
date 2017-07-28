/* ============================================================================
 * I B E X - Continuation Tests
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes and CNRS
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert, Alexandre Goldsztejn
 * Created     : Sep 06, 2016
 * ---------------------------------------------------------------------------- */

#include "TestParametricProof.h"
#include "ibex_ParametricProof.h"
#include "ibex_ContCell.h"
#include "ibex_LinearException.h"
#include "ibex_Newton.h"

using namespace std;

namespace ibex {

void TestParametricProof::get_vars01() {

	Function f("x","y","z","x^2+y^2+z^2-1");

	double __x[]={::sqrt(1/3.),::sqrt(1/3.),::sqrt(1/3.)};
	Vector x(3,__x);

	BitSet forced(BitSet::empty(3));

	VarSet v=get_newton_vars(f,x,forced);
	CPPUNIT_ASSERT(v.nb_var==1);

	//cout << "with forced parameter 0: ";
	forced.add(0);
	v=get_newton_vars(f,x,forced);
	CPPUNIT_ASSERT(v.nb_var==1 && !v.is_var[0]);

	//cout << "with forced parameter 1: ";
	forced.remove(0);
	forced.add(1);
	v=get_newton_vars(f,x,forced);
	CPPUNIT_ASSERT(v.nb_var==1 && !v.is_var[1]);

	//cout << "with forced parameter 2: ";
	forced.remove(1);
	forced.add(2);
	v=get_newton_vars(f,x,forced);
	CPPUNIT_ASSERT(v.nb_var==1 && !v.is_var[2]);

	//cout << "with forced parameter 0 and 1: ";
	forced.remove(2);
	forced.add(0);
	forced.add(1);
	v=get_newton_vars(f,x,forced);
	CPPUNIT_ASSERT(v.nb_var==1 && v.is_var[2]);

	//cout << "with forced parameter 0 and 2: ";
	forced.remove(1);
	forced.add(2);
	v=get_newton_vars(f,x,forced);
	CPPUNIT_ASSERT(v.nb_var==1 && v.is_var[1]);

	//cout << "with forced parameter 1 and 2: ";
	forced.remove(0);
	forced.add(1);
	v=get_newton_vars(f,x,forced);
	CPPUNIT_ASSERT(v.nb_var==1 && v.is_var[0]);

	//cout << "with forced parameters O, 1 and 2: ";
	forced.add(0);
	CPPUNIT_ASSERT_THROW(get_newton_vars(f,x,forced),SingularMatrixException);

}

void TestParametricProof::find_sol() {
	Variable x,y,z;
    Function f(x,y,z,sqr(x)+sqr(y)+sqr(z)-1);
    IntervalVector domain(3);

	VarSet vars(f,z);
	double _box[][2]={{-0.1,0.1},{-0.1,0.1},{0.9,1.1}};
	IntervalVector box(3,_box);
	//cout << "Try to find a point on the unit sphere on each facet of the box:\n" << box << endl;

	ContCell cell(box,box,domain,vars);

	//cout << "\nResult:\n" << endl;
	while (!cell.empty_facets()) {
		IntervalVector facet=cell.pop_front_facet();
		//cout << "========================" << endl;
		//cout << "facet=" << facet << endl;
		IntervalVector sol=find_solution(f,facet,vars);
		//cout << "x*=" << sol << endl;
		//cout << "f(x*)=" << f.eval(sol) << endl;
		CPPUNIT_ASSERT(facet.is_superset(sol));
		CPPUNIT_ASSERT(sol.max_diam()<1e-10);
		CPPUNIT_ASSERT(f.eval(sol).contains(0.0));
	}

}

void TestParametricProof::half_ball_1D() {

	IntervalVector p(1,Interval(-1,1));
	IntervalVector ginf(1,Interval(-1));
	IntervalMatrix Dg(1,1);
	Dg[0][0]=1;
	CPPUNIT_ASSERT(is_homeomorph_half_ball(ginf,Dg,p));

	Dg[0][0]=Interval(0.51,10);
	CPPUNIT_ASSERT(is_homeomorph_half_ball(ginf,Dg,p));

	Dg[0][0]=Interval(0.49,10);
	CPPUNIT_ASSERT(!is_homeomorph_half_ball(ginf,Dg,p));

}

void TestParametricProof::half_ball_2D() {

	IntervalVector p(2,Interval(-1,1));
	IntervalVector ginf(2,Interval(-2));
	IntervalMatrix Dg(1,2);
	Dg[0][0]=1;
	Dg[0][1]=1;
	CPPUNIT_ASSERT(is_homeomorph_half_ball(ginf,Dg,p));

	Dg[0][0]=0.51;
	Dg[0][1]=0.51;
	CPPUNIT_ASSERT(is_homeomorph_half_ball(ginf,Dg,p));

	Dg[0][0]=0.49;
	Dg[0][1]=0.49;
	CPPUNIT_ASSERT(!is_homeomorph_half_ball(ginf,Dg,p));
}

} // end namespace
