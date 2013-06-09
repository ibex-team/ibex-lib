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

#include "TestSystem.h"
#include "ibex_System.h"
#include "ibex_SystemFactory.h"
#include "ibex_SyntaxError.h"

using namespace std;

namespace ibex {

static System* sysex1() {
	SystemFactory fac;
	Variable x(3,"x");
	Variable A(3,3,"A");
	Variable y("y");

	fac.add_var(x);
	fac.add_var(A);
	fac.add_var(y);
	fac.add_goal(y-cos(x[1]));
	IntervalVector a(3,0);
	fac.add_ctr(A*x=a);
	fac.add_ctr(y>=x[0]);

	return new System(fac);
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

static System* sysex3() {
	SystemFactory fac;
	Variable x("x");
	Variable y("y");

	fac.add_var(x);
	fac.add_var(y);
	fac.add_ctr(x+y=Interval(-1,1));
	fac.add_ctr(x-y<=1);
	fac.add_ctr(x-y>=-1);

	return new System(fac);
}

void TestSystem::factory01() {

	System& sys(*sysex1());

	TEST_ASSERT(sys.nb_ctr==2);
	TEST_ASSERT(sys.nb_var==13);
	TEST_ASSERT(sys.args.size()==3);
	TEST_ASSERT(sys.args[0].dim==Dim::col_vec(3));
	TEST_ASSERT(sys.args[1].dim==Dim::matrix(3,3));
	TEST_ASSERT(sys.args[2].dim==Dim::scalar());
	TEST_ASSERT(sameExpr(sys.goal->expr(),"(y-cos(x[1]))"));
	TEST_ASSERT(strcmp(sys.goal_name,System::goal_name)==0);

	TEST_ASSERT(sys.box.size()==13);

	TEST_ASSERT(sys.ctrs.size()==2);
	TEST_ASSERT(sys.f.nb_arg()==3);
	TEST_ASSERT(sys.f.nb_var()==13);
	TEST_ASSERT(sys.f.image_dim()==4);
	TEST_ASSERT(sameExpr(sys.ctrs[0].f.expr(),"(A*x)"));
	TEST_ASSERT(sys.ctrs[0].op==EQ);
	TEST_ASSERT(sameExpr(sys.ctrs[1].f.expr(),"(y-x[0])"));
	TEST_ASSERT(sys.ctrs[1].op==GEQ);

	delete &sys;
}


void TestSystem::factory02() {
	System sys("quimper/unconstrained.qpr");

	TEST_ASSERT(sys.nb_ctr==0);
	TEST_ASSERT(sys.nb_var==2);
	TEST_ASSERT(sys.args.size()==2);
	TEST_ASSERT(sameExpr(sys.goal->expr(),"(x+y)"));
	TEST_ASSERT(strcmp(sys.goal_name,System::goal_name)==0);
	TEST_ASSERT(sys.ctrs.size()==0);
}

void TestSystem::copy01() {
	System& _sys(*sysex1());
	System sys(_sys,System::COPY);
	//delete &_sys;

	// all the lines below are copy-pasted from factory01...
	TEST_ASSERT(sys.nb_ctr==2);
	TEST_ASSERT(sys.nb_var==13);
	TEST_ASSERT(sys.args.size()==3);
	TEST_ASSERT(sys.args[0].dim==Dim::col_vec(3));
	TEST_ASSERT(sys.args[1].dim==Dim::matrix(3,3));
	TEST_ASSERT(sys.args[2].dim==Dim::scalar());
	TEST_ASSERT(sameExpr(sys.goal->expr(),"(y-cos(x[1]))"));
	TEST_ASSERT(strcmp(sys.goal_name,System::goal_name)==0);

	TEST_ASSERT(sys.box.size()==13);

	TEST_ASSERT(sys.ctrs.size()==2);
	TEST_ASSERT(sys.f.nb_arg()==3);
	TEST_ASSERT(sys.f.nb_var()==13);
	TEST_ASSERT(sys.f.image_dim()==4);
	TEST_ASSERT(sameExpr(sys.ctrs[0].f.expr(),"(A*x)"));
	TEST_ASSERT(sys.ctrs[0].op==EQ);
	TEST_ASSERT(sameExpr(sys.ctrs[1].f.expr(),"(y-x[0])"));
	TEST_ASSERT(sys.ctrs[1].op==GEQ);
}

void TestSystem::copy02() {
	System _sys("quimper/unconstrained.qpr");
	System sys(_sys, System::COPY);
	TEST_ASSERT(sys.nb_ctr==0);
	TEST_ASSERT(sys.nb_var==2);
	TEST_ASSERT(sameExpr(sys.goal->expr(),"(x+y)"));
	TEST_ASSERT(sys.ctrs.size()==0);
}

void TestSystem::extend01() {
	System& _sys(*sysex2());
	System sys(_sys,System::EXTEND);
	delete &_sys;

	TEST_ASSERT(sys.nb_ctr==4);
	TEST_ASSERT(sys.nb_var==5);
	TEST_ASSERT(sys.args.size()==3);
	TEST_ASSERT(sys.args[0].dim==Dim::col_vec(3));
	TEST_ASSERT(sys.args[1].dim==Dim::scalar());
	TEST_ASSERT(sys.args[2].dim==Dim::scalar());
	TEST_ASSERT(sys.goal==NULL);

	TEST_ASSERT(sys.box.size()==5);

	TEST_ASSERT(sys.ctrs.size()==4);
	TEST_ASSERT(sys.f.nb_arg()==3);
	TEST_ASSERT(sys.f.nb_var()==5);
	TEST_ASSERT(sys.f.image_dim()==4);
	TEST_ASSERT(sameExpr(sys.ctrs[0].f.expr(),"(__goal__-(y-cos(x[1])))"));
	TEST_ASSERT(sys.ctrs[0].op==EQ);
	TEST_ASSERT(sameExpr(sys.ctrs[1].f.expr(),"(-(x[0]+x[1]))"));
	TEST_ASSERT(sys.ctrs[1].op==LEQ);
	TEST_ASSERT(sameExpr(sys.ctrs[2].f.expr(),"((x[1]+x[2])-y)"));
	TEST_ASSERT(sys.ctrs[2].op==LEQ);
	TEST_ASSERT(sameExpr(sys.ctrs[3].f.expr(),"(-(y-x[0]))"));
	TEST_ASSERT(sys.ctrs[3].op==LEQ);
}

void TestSystem::extend02() {
	System _sys("quimper/unconstrained.qpr");
	System sys(_sys, System::EXTEND);
	TEST_ASSERT(sys.nb_ctr==1);
	TEST_ASSERT(sys.nb_var==3);
	TEST_ASSERT(sys.goal==NULL);
	TEST_ASSERT(sys.ctrs.size()==1);
	TEST_ASSERT(sys.f.nb_arg()==3);
	TEST_ASSERT(sys.f.nb_var()==3);
	TEST_ASSERT(sys.f.image_dim()==1);
	TEST_ASSERT(sameExpr(sys.ctrs[0].f.expr(),"(__goal__-(x+y))"));
	TEST_ASSERT(sys.ctrs[0].op==EQ);
}

void TestSystem::normalize01() {
	System& _sys(*sysex3());
	System sys(_sys,System::NORMALIZE);
	delete &_sys;

	TEST_ASSERT(sys.nb_ctr==4);
	TEST_ASSERT(sys.nb_var==2);
	TEST_ASSERT(sys.goal==NULL);
	TEST_ASSERT(sys.ctrs.size()==4);
	TEST_ASSERT(sys.f.nb_arg()==2);
	TEST_ASSERT(sys.f.nb_var()==2);
	TEST_ASSERT(sys.f.image_dim()==4)

	TEST_ASSERT(sameExpr(sys.ctrs[0].f.expr(),"((x+y)-1)"));
	TEST_ASSERT(sameExpr(sys.ctrs[1].f.expr(),"((-(x+y))-1)"));
	TEST_ASSERT(sameExpr(sys.ctrs[2].f.expr(),"((x-y)-1)"));
	TEST_ASSERT(sameExpr(sys.ctrs[3].f.expr(),"(-((x-y)--1))"));
	TEST_ASSERT(sys.ctrs[0].op==LEQ);
	TEST_ASSERT(sys.ctrs[1].op==LEQ);
	TEST_ASSERT(sys.ctrs[2].op==LEQ);
	TEST_ASSERT(sys.ctrs[3].op==LEQ);
}

void TestSystem::merge01() {
	SystemFactory fac1;
	{
		Variable x("x");
		Variable y(2,"y");
		fac1.add_var(x);
		fac1.add_var(y);
		fac1.add_ctr(x=0);
	}
	System sys1(fac1);

	SystemFactory fac2;
	{
		Variable x("x");
		Variable y(2,"y");
		fac2.add_var(x);
		fac2.add_var(y);
		fac2.add_ctr(y=IntervalVector(2,Interval::ZERO));
	}
	System sys2(fac2);

	System sys3(sys1,sys2);
	TEST_ASSERT(sys3.args.size()==2);
	TEST_ASSERT(strcmp(sys3.args[0].name,"x")==0);
	TEST_ASSERT(strcmp(sys3.args[1].name,"y")==0);
	TEST_ASSERT(sys3.nb_ctr==2);
	TEST_ASSERT(sameExpr(sys1.ctrs[0].f.expr(),sys3.ctrs[0].f.expr()));
	TEST_ASSERT(sameExpr(sys2.ctrs[0].f.expr(),sys3.ctrs[1].f.expr()));
}

void TestSystem::merge02() {
	SystemFactory fac1;
	{
		Variable x("x");
		fac1.add_var(x);
		fac1.add_goal(x);
	}
	System sys1(fac1);

	SystemFactory fac2;
	{
		Variable x("x");
		Variable y(2,"y");
		fac2.add_var(x);
		fac2.add_var(y);
		fac2.add_ctr(y=IntervalVector(2,Interval::ZERO));
	}
	System sys2(fac2);

	System sys3(sys1,sys2);

	TEST_ASSERT(sys3.args.size()==2);
	TEST_ASSERT(strcmp(sys3.args[0].name,"x")==0);
	TEST_ASSERT(strcmp(sys3.args[1].name,"y")==0);
	TEST_ASSERT(sys3.nb_ctr==1);
	TEST_ASSERT(sameExpr(sys1.goal->expr(),sys3.goal->expr()));
	TEST_ASSERT(sameExpr(sys2.ctrs[0].f.expr(),sys3.ctrs[0].f.expr()));

	System sys4(sys2,sys1);
	TEST_ASSERT(sys4.args.size()==2);
	TEST_ASSERT(strcmp(sys4.args[0].name,"x")==0);
	TEST_ASSERT(strcmp(sys4.args[1].name,"y")==0);
	TEST_ASSERT(sys4.nb_ctr==1);
	TEST_ASSERT(sameExpr(sys1.goal->expr(),sys4.goal->expr()));
	TEST_ASSERT(sameExpr(sys2.ctrs[0].f.expr(),sys4.ctrs[0].f.expr()));

}

void TestSystem::merge03() {
	SystemFactory fac1;
	{
		Variable x("x");
		fac1.add_var(x);
		fac1.add_goal(x);
	}
	System sys1(fac1);

	SystemFactory fac2;
	{
		Variable y(2,"y");
		fac2.add_var(y);
		fac2.add_ctr(y=IntervalVector(2,Interval::ZERO));
	}
	System sys2(fac2);

	System sys3(sys1,sys2);
	TEST_ASSERT(sys3.args.size()==2);
	TEST_ASSERT(strcmp(sys3.args[0].name,"x")==0);
	TEST_ASSERT(strcmp(sys3.args[1].name,"y")==0);
	TEST_ASSERT(sys3.nb_ctr==1);
	TEST_ASSERT(sameExpr(sys1.goal->expr(),sys3.goal->expr()));
}


void TestSystem::merge04() {
	System sys1("../benchs/benchs-optim/coconutbenchmark-library1/bearing.bch");
	System sys2("../benchs/benchs-satisfaction/benchs-coprin/I5.bch");
	System sys3(sys1,sys2);
	TEST_ASSERT(strcmp(sys3.args[0].name,"x1")==0);
	TEST_ASSERT(strcmp(sys3.args[4].name,"x6")==0);
	TEST_ASSERT(strcmp(sys3.args[12].name,"x14")==0);
	TEST_ASSERT(strcmp(sys3.args[13].name,"x5")==0);
	for (int i=0; i<sys1.nb_ctr; i++)
		TEST_ASSERT(sameExpr(sys3.ctrs[i].f.expr(),sys1.ctrs[i].f.expr()));
	for (int i=0; i<sys2.nb_ctr; i++)
		TEST_ASSERT(sameExpr(sys3.ctrs[sys1.nb_ctr+i].f.expr(),sys2.ctrs[i].f.expr()));
}

} // end namespace
