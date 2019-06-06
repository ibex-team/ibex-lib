/* ============================================================================
 * I B E X - System Tests
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Aug 23, 2012
 * ---------------------------------------------------------------------------- */

#include "TestAmpl.h"
#include "ibex_AmplInterface.h"
#include "ibex_ExtendedSystem.h"
#include "ibex_NormalizedSystem.h"

#include <sstream>

using namespace std;

namespace ibex {

static System* sysex1() {
	SystemFactory fac;
	Variable x(3,"x");
	Variable A(3,3,"A");
	Variable y("y");

	fac.add_var(x);
	fac.add_var(A);
	fac.add_var(y,Interval(-1,1));
	fac.add_goal(y-cos(x[1]));
	IntervalVector a(3,0);
	fac.add_ctr(A*x=a);
	fac.add_ctr(y>=x[0]);

	return new System(fac);
}

static System* ampl_sysex1() {
	AmplInterface interface(SRCDIR_TESTS "/../plugins/ampl/tests/ex_ampl/ex1.nl");
	return new System(interface);
}

static System* sysex2() {
	SystemFactory fac;
	Variable x(3,"x");
	Variable y("y");

	fac.add_var(x);
	fac.add_var(y);
	fac.add_goal(y-cos(x[1]));
	IntervalVector a(3,0);
	fac.add_ctr(x[0]+x[1]>=0);
	fac.add_ctr(x[1]+x[2]<=y);
	fac.add_ctr(y>=x[0]);

	return new System(fac);
}


static System* ampl_sysex2() {
	AmplInterface interface(SRCDIR_TESTS "/../plugins/ampl/tests/ex_ampl/ex2.nl");
	return new System(interface);
}


static System* sysex3() {
	SystemFactory fac;
	Variable x("x");
	Variable y("y");

	fac.add_var(x);
	fac.add_var(y);
	fac.add_ctr(x+y=Interval(-1,1));
	fac.add_ctr(x-y<=1);
	fac.add_ctr(x-y>=-1);
	fac.add_ctr(x=y);

	return new System(fac);
}


static System* ampl_sysex3() {
	AmplInterface interface(SRCDIR_TESTS "/../plugins/ampl/tests/ex_ampl/ex3.nl");
	return new System(interface);
}


void TestAmpl::factory01() {

	System& sys(*ampl_sysex1());
	CPPUNIT_ASSERT(sys.nb_ctr==4);
	CPPUNIT_ASSERT(sys.nb_var==13);
	CPPUNIT_ASSERT(sys.args.size()==13);
	for (int i; i <13; i++)
		CPPUNIT_ASSERT(sys.args[i].dim==Dim::scalar());
	check(sys.goal->eval(IntervalVector(13,Interval(0))),Interval(-1));
	//CPPUNIT_ASSERT(sameExpr(sys.goal->expr(),"(y-cos(_svar[2]))"));

	CPPUNIT_ASSERT(sys.box.size()==13);
	CPPUNIT_ASSERT(sys.box[12]==Interval(-1,1));

	CPPUNIT_ASSERT(sys.ctrs.size()==4);
	CPPUNIT_ASSERT(sys.f_ctrs.nb_arg()==13);
	CPPUNIT_ASSERT(sys.f_ctrs.nb_var()==13);
	CPPUNIT_ASSERT(sys.f_ctrs.image_dim()==4);
	CPPUNIT_ASSERT(sameExpr(sys.f_ctrs[0].expr(),"((((_svar[4]*_svar[2])+(_svar[5]*_svar[1]))+(_svar[6]*_svar[3]))-10)"));
	CPPUNIT_ASSERT(sameExpr(sys.f_ctrs[1].expr(),"((((_svar[7]*_svar[2])+(_svar[8]*_svar[1]))+(_svar[9]*_svar[3]))-11)"));
	CPPUNIT_ASSERT(sameExpr(sys.f_ctrs[2].expr(),"((((_svar[10]*_svar[2])+(_svar[11]*_svar[1]))+(_svar[12]*_svar[3]))-12)"));
	CPPUNIT_ASSERT(sys.ops[0]==EQ);
	CPPUNIT_ASSERT(sys.ops[1]==EQ);
	CPPUNIT_ASSERT(sys.ops[2]==EQ);
	CPPUNIT_ASSERT(sameExpr(sys.f_ctrs[3].expr(),"((-_svar[2])+_svar[13])"));
	CPPUNIT_ASSERT(sys.ops[3]==GEQ);
	delete &sys;
}


void TestAmpl::factory02() {
	System& sys(*ampl_sysex3());

	CPPUNIT_ASSERT(sys.nb_ctr==5);
	CPPUNIT_ASSERT(sys.nb_var==2);
	CPPUNIT_ASSERT(sys.goal==NULL);
	CPPUNIT_ASSERT(sys.ctrs.size()==5);
	CPPUNIT_ASSERT(sys.f_ctrs.nb_arg()==2);
	CPPUNIT_ASSERT(sys.f_ctrs.nb_var()==2);
	CPPUNIT_ASSERT(sys.f_ctrs.image_dim()==5);

	CPPUNIT_ASSERT(sameExpr(sys.ctrs[0].f.expr(),"((_svar[1]+_svar[2])-1)"));
	CPPUNIT_ASSERT(sameExpr(sys.ctrs[1].f.expr(),"((_svar[1]+_svar[2])+1)"));
	CPPUNIT_ASSERT(sys.ctrs[0].op==LEQ);
	CPPUNIT_ASSERT(sys.ctrs[1].op==GEQ);
	delete &sys;

}



void TestAmpl::extend() {
	System& _sys(*ampl_sysex2());
	ExtendedSystem sys(_sys);
	delete &_sys;

	CPPUNIT_ASSERT(sys.nb_ctr==4);
	CPPUNIT_ASSERT(sys.nb_var==5);
	CPPUNIT_ASSERT(sys.args.size()==5);
	for (int i; i <5; i++)
		CPPUNIT_ASSERT(sys.args[i].dim==Dim::scalar());
	//CPPUNIT_ASSERT(sys.args[5].dim==Dim::scalar());
	CPPUNIT_ASSERT(sameExpr(sys.goal->expr(),"__goal__"));
	//CPPUNIT_ASSERT(sys.goal==NULL);

	CPPUNIT_ASSERT(sys.box.size()==5);

	CPPUNIT_ASSERT(sys.ctrs.size()==4);
	CPPUNIT_ASSERT(sys.f_ctrs.nb_arg()==5);
	CPPUNIT_ASSERT(sys.f_ctrs.nb_var()==5);
	CPPUNIT_ASSERT(sys.f_ctrs.image_dim()==4);
	CPPUNIT_ASSERT(sameExpr(sys.ctrs[0].f.expr(),"(((-cos(_svar[1]))+_svar[4])-__goal__)"));
	CPPUNIT_ASSERT(sys.ctrs[0].op==EQ);
	CPPUNIT_ASSERT(sameExpr(sys.ctrs[1].f.expr(),"(-(_svar[1]+_svar[2]))"));
	CPPUNIT_ASSERT(sys.ctrs[1].op==LEQ);
	CPPUNIT_ASSERT(sameExpr(sys.ctrs[2].f.expr(),"((_svar[1]+_svar[3])-_svar[4])"));
	CPPUNIT_ASSERT(sys.ctrs[2].op==LEQ);
	CPPUNIT_ASSERT(sameExpr(sys.ctrs[3].f.expr(),"(-((-_svar[2])+_svar[4]))"));
	CPPUNIT_ASSERT(sys.ctrs[3].op==LEQ);

}

void TestAmpl::normalize() {
	System& _sys(*ampl_sysex3());
	NormalizedSystem sys(_sys,0.5);
	delete &_sys;

	CPPUNIT_ASSERT(sys.nb_ctr==6);
	CPPUNIT_ASSERT(sys.nb_var==2);
	CPPUNIT_ASSERT(sys.goal==NULL);
	CPPUNIT_ASSERT(sys.ctrs.size()==6);
	CPPUNIT_ASSERT(sys.f_ctrs.nb_arg()==2);
	CPPUNIT_ASSERT(sys.f_ctrs.nb_var()==2);
	CPPUNIT_ASSERT(sys.f_ctrs.image_dim()==6);

	CPPUNIT_ASSERT(sameExpr(sys.f_ctrs[0].expr(),"((_svar[1]+_svar[2])-1)"));
	CPPUNIT_ASSERT(sameExpr(sys.f_ctrs[1].expr(),"(-((_svar[1]+_svar[2])+1))"));
	CPPUNIT_ASSERT(sameExpr(sys.f_ctrs[2].expr(),"((_svar[1]-_svar[2])-1)"));
	CPPUNIT_ASSERT(sameExpr(sys.f_ctrs[3].expr(),"(-((_svar[1]-_svar[2])+1))"));
	CPPUNIT_ASSERT(sameExpr(sys.f_ctrs[4].expr(),"((_svar[1]-_svar[2])-0.5)"));
	CPPUNIT_ASSERT(sameExpr(sys.f_ctrs[5].expr(),"((-(_svar[1]-_svar[2]))+-0.5)") ||
				   sameExpr(sys.f_ctrs[5].expr(),"((-(_svar[1]-_svar[2]))-0.5)")   );
	CPPUNIT_ASSERT(sys.ctrs[0].op==LEQ);
	CPPUNIT_ASSERT(sys.ctrs[1].op==LEQ);
	CPPUNIT_ASSERT(sys.ctrs[2].op==LEQ);
	CPPUNIT_ASSERT(sys.ctrs[3].op==LEQ);
	CPPUNIT_ASSERT(sys.ctrs[4].op==LEQ);
	CPPUNIT_ASSERT(sys.ctrs[5].op==LEQ);
}

void TestAmpl::merge() {

	AmplInterface i1(SRCDIR_TESTS "/../plugins/ampl/tests/ex_ampl/bearing.nl");
	AmplInterface i2(SRCDIR_TESTS "/../plugins/ampl/tests/ex_ampl/I5.nl");
	System sys1(i1);
	System sys2(i2);
	System sys3(sys1,sys2);

	for (int i=0; i<sys1.nb_ctr; i++)
		CPPUNIT_ASSERT(sameExpr(sys3.f_ctrs[i].expr(),sys1.f_ctrs[i].expr()));
	for (int i=0; i<sys2.nb_ctr; i++)
		CPPUNIT_ASSERT(sameExpr(sys3.f_ctrs[sys1.nb_ctr+i].expr(),sys2.f_ctrs[i].expr()));
}

void TestAmpl::variable1() {
	AmplInterface inter(SRCDIR_TESTS "/../plugins/ampl/tests/ex_ampl/ex4.nl" );
	System sys(inter);
	CPPUNIT_ASSERT(sameExpr(sys.goal->expr(),"(_svar[1]+_svar[2])"));
	CPPUNIT_ASSERT(sys.ctrs.size()==3);
	CPPUNIT_ASSERT(sys.f_ctrs.nb_arg()==2);
	CPPUNIT_ASSERT(sys.f_ctrs.nb_var()==2);
	CPPUNIT_ASSERT(sameExpr(sys.f_ctrs[0].expr(),"(_svar[1]+_svar[2])"));
	CPPUNIT_ASSERT(sameExpr(sys.f_ctrs[1].expr(),"_svar[1]"));
	CPPUNIT_ASSERT(sameExpr(sys.f_ctrs[2].expr(),"(-_svar[2])"));
	CPPUNIT_ASSERT(sys.ops[0]==EQ);
	CPPUNIT_ASSERT(sys.ops[1]==LEQ);
	CPPUNIT_ASSERT(sys.ops[2]==GEQ);
}


void TestAmpl::variable2() {
	AmplInterface inter(SRCDIR_TESTS "/../plugins/ampl/tests/ex_ampl/ex5.nl" );
	System sys(inter);
	CPPUNIT_ASSERT(sameExpr(sys.goal->expr(),"(((_svar[1]+_svar[2])*(_svar[1]+_svar[2]))+_svar[2])"));
	CPPUNIT_ASSERT(sys.ctrs.size()==3);
	CPPUNIT_ASSERT(sys.f_ctrs.nb_arg()==2);
	CPPUNIT_ASSERT(sys.f_ctrs.nb_var()==2);
	CPPUNIT_ASSERT(sameExpr(sys.f_ctrs[0].expr(),"((_svar[1]+_svar[2])*(_svar[1]+_svar[2]))"));
	CPPUNIT_ASSERT(sameExpr(sys.f_ctrs[1].expr(),"(_svar[1]+_svar[2])"));
	CPPUNIT_ASSERT(sameExpr(sys.f_ctrs[2].expr(),"(-_svar[2])"));
	CPPUNIT_ASSERT(sys.ops[0]==LEQ);
	CPPUNIT_ASSERT(sys.ops[1]==EQ);
	CPPUNIT_ASSERT(sys.ops[2]==GEQ);
}

void TestAmpl::variable3() {
	AmplInterface inter(SRCDIR_TESTS "/../plugins/ampl/tests/ex_ampl/ex6.nl" );
	System sys(inter);
	CPPUNIT_ASSERT(sameExpr(sys.goal->expr(),"((((_svar[1]+_svar[2])*(((((_svar[1]+_svar[2])*(_svar[1]+_svar[2]))+((_svar[1]+_svar[2])*(_svar[1]+_svar[2])))-_svar[1])+_svar[2]))+((((((_svar[1]+_svar[2])*(_svar[1]+_svar[2]))+((_svar[1]+_svar[2])*(_svar[1]+_svar[2])))-_svar[1])+_svar[2])*(((_svar[1]+_svar[2])*(_svar[1]+_svar[2]))+_svar[2])))-_svar[1])"));
	CPPUNIT_ASSERT(sys.ctrs.size()==3);
	CPPUNIT_ASSERT(sys.f_ctrs.nb_arg()==2);
	CPPUNIT_ASSERT(sys.f_ctrs.nb_var()==2);
	CPPUNIT_ASSERT(sameExpr(sys.f_ctrs[0].expr(),"((_svar[1]+_svar[2])*(_svar[1]+_svar[2]))"));
	CPPUNIT_ASSERT(sameExpr(sys.f_ctrs[1].expr(),"(((-(((_svar[1]+_svar[2])*(_svar[1]+_svar[2]))+((_svar[1]+_svar[2])*(_svar[1]+_svar[2]))))+(2*_svar[1]))-_svar[2])"));
	CPPUNIT_ASSERT(sameExpr(sys.f_ctrs[2].expr(),"(_svar[1]+_svar[2])"));
	CPPUNIT_ASSERT(sys.ops[0]==LEQ);
	CPPUNIT_ASSERT(sys.ops[1]==GEQ);
	CPPUNIT_ASSERT(sys.ops[2]==LEQ);
}
} // end namespace
