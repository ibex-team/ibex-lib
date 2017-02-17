/* ============================================================================
 * I B E X - OptimMinMax Tests
 * ============================================================================
 * Copyright   : ENSTA Bretagne (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Jordan Ninin
 * Created     : Oct 2, 2016
 * ---------------------------------------------------------------------------- */

#include "TestOptimMinMax.h"
#include "ibex_OptimMinMax.h"
#include "ibex_SystemFactory.h"
#include "ibex_CtcIdentity.h"
#include "ibex_CtcHC4.h"
#include "ibex_CtcFwdBwd.h"

using namespace std;

namespace ibex {



//******** messine GOW conf*********
void TestOptimMinMax::ex_messine() {

	double x_prec = 1e-6;
	double y_prec = 1e-4;
	double stop_prec = 1e-2;

	Variable x(2),y(2);
	IntervalVector x_ini(2,Interval(-10,10));
	IntervalVector y_ini(2,Interval(-0.1,0.1));
	Function func(x,y,(pow((x[0]+y[0]-2),6)+0.2)*log(1+pow(x[0]+y[0],2)) + (pow((x[1]+y[1]-2),6)+0.2)*log(1+pow(x[1]+y[1],2)));

	SystemFactory x_fac;
	x_fac.add_var(x, x_ini);
	NormalizedSystem x_sys(x_fac);
	CtcIdentity x_ctc(x_ini.size());

	SystemFactory xy_fac;
	xy_fac.add_var(x, x_ini);
	xy_fac.add_var(y, y_ini);
	xy_fac.add_goal(func);
	NormalizedSystem xy_sys(xy_fac);
	CtcIdentity xy_ctc(x_ini.size()+y_ini.size());

	OptimMinMax oo(x_sys, xy_sys,x_ctc,xy_ctc,x_prec, y_prec,stop_prec);
	//oo.trace=1;
    oo.timeout=100;
	Optim::Status res = oo.optimize();
	oo.report();

	CPPUNIT_ASSERT(res==Optim::SUCCESS);
	CPPUNIT_ASSERT(oo.loup<=0.5286);
	CPPUNIT_ASSERT(oo.nb_cells<=9000);

}

//  *********** Problems from "A differential evolution approach for solving constrained min–max optimization problems",Segundo, 2012

void TestOptimMinMax::ex_segundo1() {

    double x_prec = 1e-6;
    double y_prec = 1e-3;
    double stop_prec = 1e-3;

    Variable x(1),y(1);
    IntervalVector x_ini(1,Interval(-3.14,3.14));
    IntervalVector y_ini(1,Interval(-3.14,3.14));
    Function func(x,y,pow(cos(y)+cos(2*y+x),2));

    Function c1f(x,y,y-x*(x+6.28) );
    NumConstraint c1(c1f,LEQ);
    Function c2f(x,y,y-x*(x-6.28));
    NumConstraint c2(c2f,LEQ);

    SystemFactory x_fac;
    x_fac.add_var(x, x_ini);
    NormalizedSystem x_sys(x_fac);
    CtcIdentity x_ctc(x_ini.size());

    SystemFactory xy_fac;
    xy_fac.add_var(x, x_ini);
    xy_fac.add_var(y, y_ini);
    xy_fac.add_goal(func);
    xy_fac.add_ctr(c1);
    xy_fac.add_ctr(c2);
    NormalizedSystem xy_sys(xy_fac);
    CtcHC4 xy_ctc(xy_sys);


    OptimMinMax oo(x_sys, xy_sys,x_ctc,xy_ctc,x_prec, y_prec,stop_prec);
    //oo.trace=1;
    oo.timeout=120;

    Optim::Status res = oo.optimize();
    oo.report();

    CPPUNIT_ASSERT(res==Optim::SUCCESS);
    CPPUNIT_ASSERT(oo.loup<=0.009);
    CPPUNIT_ASSERT(oo.nb_cells<=40);
}

void TestOptimMinMax::ex_segundo2() {

    double x_prec = 1e-6;
    double y_prec = 1e-6;
    double stop_prec = 1e-3;

    Variable x(1),y(1);
    IntervalVector x_ini(1,Interval(0,6));
    IntervalVector y_ini(1,Interval(2,8));
    Function func(x,y,pow(x,2)+pow(y,2)+2*x*y-20*x-20*y+100);

    Function c1f(x,y,-pow(x-5,2)-pow(y-3,2)+4);
    NumConstraint c1(c1f,LEQ);
    Function c2f(x,y,pow(x-5,2)+pow(y-3,2)-16);
    NumConstraint c2(c2f,LEQ);

    SystemFactory x_fac;
    x_fac.add_var(x, x_ini);
    NormalizedSystem x_sys(x_fac);
    CtcIdentity x_ctc(x_ini.size());

    SystemFactory xy_fac;
    xy_fac.add_var(x, x_ini);
    xy_fac.add_var(y, y_ini);
    xy_fac.add_goal(func);
    xy_fac.add_ctr(c1);
    xy_fac.add_ctr(c2);
    NormalizedSystem xy_sys(xy_fac);
    CtcHC4 xy_ctc(xy_sys);

    OptimMinMax oo(x_sys, xy_sys,x_ctc,xy_ctc,x_prec, y_prec,stop_prec);
    //oo.trace=1;
    oo.timeout=100;
    Optim::Status res = oo.optimize();
    oo.report();

    CPPUNIT_ASSERT(res==Optim::SUCCESS);
    CPPUNIT_ASSERT(oo.loup<=1.103);
    CPPUNIT_ASSERT(oo.nb_cells<=1400);

}

void TestOptimMinMax::ex_segundo3() {

    double x_prec = 1e-6;
    double y_prec = 1e-6;
    double stop_prec = 1e-5;

    Variable x(1),y(1);
    IntervalVector x_ini(1,Interval(-5,5));
    IntervalVector y_ini(1,Interval(-5,5));
    Function func(x,y,pow(sin(x),2)-x*cos(y)+2*sin(x)-pow(cos(y),2)+y-1 );

    Function c1f(x,y,pow(x,2)+pow(y,2)-25);
    NumConstraint c1(c1f,GEQ);

    SystemFactory x_fac;
    x_fac.add_var(x, x_ini);
    NormalizedSystem x_sys(x_fac);
    CtcIdentity x_ctc(x_ini.size());

    SystemFactory xy_fac;
    xy_fac.add_var(x, x_ini);
    xy_fac.add_var(y, y_ini);
    xy_fac.add_goal(func);
    xy_fac.add_ctr(c1);
    NormalizedSystem xy_sys(xy_fac);
    CtcFwdBwd xy_ctc(c1);

    OptimMinMax oo(x_sys, xy_sys,x_ctc,xy_ctc,x_prec, y_prec,stop_prec);
    //oo.trace=1;
    oo.timeout=100;
    Optim::Status res = oo.optimize();
    oo.report();

    CPPUNIT_ASSERT(res==Optim::SUCCESS);
    CPPUNIT_ASSERT(oo.loup<=-6.6415);
    CPPUNIT_ASSERT(oo.nb_cells<=75);

}

void TestOptimMinMax::ex_segundo4() {

    double x_prec = 1e-1;
    double y_prec = 1e-1;
    double stop_prec = 1e-3;

    Variable x(2),y(2);
    IntervalVector x_ini(2,Interval(0,10));
    IntervalVector y_ini(2,Interval(0,10));
    Function func(x,y, -((x[0]+y[0])*(x[1]+y[1]))/(1+x[0]*y[0]+x[1]*y[1]) );

    Function c1f(x,y,pow(x[0],2)+pow(x[1],2)-100);
    Function c2f(x,y,y[0]-x[0]);
    Function c3f(x,y,y[1]-x[1]);
    NumConstraint c1(c1f,LEQ);
    NumConstraint c2(c2f,LEQ);
    NumConstraint c3(c3f,LEQ);

    SystemFactory x_fac;
    x_fac.add_var(x, x_ini);
    NormalizedSystem x_sys(x_fac);
    CtcIdentity x_ctc(x_ini.size());

    SystemFactory xy_fac;
    xy_fac.add_var(x, x_ini);
    xy_fac.add_var(y, y_ini);
    xy_fac.add_goal(func);
    xy_fac.add_ctr(c1);
    xy_fac.add_ctr(c2);
    xy_fac.add_ctr(c3);
    NormalizedSystem xy_sys(xy_fac);
    CtcHC4 xy_ctc(xy_sys);


    OptimMinMax oo(x_sys, xy_sys,x_ctc,xy_ctc,x_prec, y_prec,stop_prec);
    //oo.trace=1;
    oo.timeout=10;
    Optim::Status res = oo.optimize();
    oo.report();

    CPPUNIT_ASSERT(res==Optim::TIME_OUT);
    CPPUNIT_ASSERT(oo.loup<=-1.9);
    //CPPUNIT_ASSERT(oo.nb_cells>=950);
}


} // end namespace
