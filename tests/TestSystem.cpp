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
#include "ibex_ExtendedSystem.h"
#include "ibex_SystemFactory.h"
#include "ibex_SyntaxError.h"
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
	fac.add_ctr(x=y);

	return new System(fac);
}

void TestSystem::factory01() {

	System& sys(*sysex1());

	CPPUNIT_ASSERT(sys.nb_ctr==2);
	CPPUNIT_ASSERT(sys.nb_var==13);
	CPPUNIT_ASSERT(sys.args.size()==3);
	CPPUNIT_ASSERT(sys.args[0].dim==Dim::col_vec(3));
	CPPUNIT_ASSERT(sys.args[1].dim==Dim::matrix(3,3));
	CPPUNIT_ASSERT(sys.args[2].dim==Dim::scalar());
	CPPUNIT_ASSERT(sameExpr(sys.goal->expr(),"(y-cos(x(2)))"));

	CPPUNIT_ASSERT(sys.box.size()==13);

	CPPUNIT_ASSERT(sys.ctrs.size()==2);
	CPPUNIT_ASSERT(sys.f_ctrs.nb_arg()==3);
	CPPUNIT_ASSERT(sys.f_ctrs.nb_var()==13);
	CPPUNIT_ASSERT(sys.f_ctrs.image_dim()==4);
	CPPUNIT_ASSERT(sameExpr(sys.ctrs[0].f.expr(),"(A*x)"));
	CPPUNIT_ASSERT(sys.ctrs[0].op==EQ);
	CPPUNIT_ASSERT(sameExpr(sys.ctrs[1].f.expr(),"(y-x(1))"));
	CPPUNIT_ASSERT(sys.ctrs[1].op==GEQ);

	CPPUNIT_ASSERT(sys.ops[0]==EQ);
	CPPUNIT_ASSERT(sys.ops[1]==EQ);
	CPPUNIT_ASSERT(sys.ops[2]==EQ);
	CPPUNIT_ASSERT(sys.ops[3]==GEQ);

	delete &sys;
}


void TestSystem::factory02() {
	System sys(SRCDIR_TESTS "/quimper/unconstrained.qpr");

	CPPUNIT_ASSERT(sys.nb_ctr==0);
	CPPUNIT_ASSERT(sys.nb_var==2);
	CPPUNIT_ASSERT(sys.args.size()==2);
	CPPUNIT_ASSERT(sameExpr(sys.goal->expr(),"(x+y)"));
	CPPUNIT_ASSERT(sys.ctrs.size()==0);
}

void TestSystem::copy01() {
	System& _sys(*sysex1());
	System sys(_sys,System::COPY);
	delete &_sys;

	// all the lines below are copy-pasted from factory01...
	CPPUNIT_ASSERT(sys.nb_ctr==2);
	CPPUNIT_ASSERT(sys.nb_var==13);
	CPPUNIT_ASSERT(sys.args.size()==3);
	CPPUNIT_ASSERT(sys.args[0].dim==Dim::col_vec(3));
	CPPUNIT_ASSERT(sys.args[1].dim==Dim::matrix(3,3));
	CPPUNIT_ASSERT(sys.args[2].dim==Dim::scalar());
	CPPUNIT_ASSERT(sameExpr(sys.goal->expr(),"(y-cos(x(2)))"));

	CPPUNIT_ASSERT(sys.box.size()==13);

	CPPUNIT_ASSERT(sys.ctrs.size()==2);
	CPPUNIT_ASSERT(sys.f_ctrs.nb_arg()==3);
	CPPUNIT_ASSERT(sys.f_ctrs.nb_var()==13);
	CPPUNIT_ASSERT(sys.f_ctrs.image_dim()==4);
	CPPUNIT_ASSERT(sameExpr(sys.ctrs[0].f.expr(),"(A*x)"));
	CPPUNIT_ASSERT(sys.ctrs[0].op==EQ);
	CPPUNIT_ASSERT(sameExpr(sys.ctrs[1].f.expr(),"(y-x(1))"));
	CPPUNIT_ASSERT(sys.ctrs[1].op==GEQ);
}

void TestSystem::copy02() {
	System _sys(SRCDIR_TESTS "/quimper/unconstrained.qpr");
	System sys(_sys, System::COPY);

	CPPUNIT_ASSERT(sys.nb_ctr==0);
	CPPUNIT_ASSERT(sys.nb_var==2);
	CPPUNIT_ASSERT(sameExpr(sys.goal->expr(),"(x+y)"));
	CPPUNIT_ASSERT(sys.ctrs.size()==0);
}

void TestSystem::ineq_only01() {
	System& _sys(*sysex3());
	System sys(_sys,System::INEQ_ONLY);
	delete &_sys;

	CPPUNIT_ASSERT(sys.nb_ctr==2);
	CPPUNIT_ASSERT(sys.nb_var==2);
	CPPUNIT_ASSERT(sys.goal==NULL);
	CPPUNIT_ASSERT(sys.ctrs.size()==2);
	CPPUNIT_ASSERT(sys.f_ctrs.nb_arg()==2);
	CPPUNIT_ASSERT(sys.f_ctrs.nb_var()==2);
	CPPUNIT_ASSERT(sys.f_ctrs.image_dim()==2);

	CPPUNIT_ASSERT(sameExpr(sys.ctrs[0].f.expr(),"((x-y)-1)"));
	CPPUNIT_ASSERT(sameExpr(sys.ctrs[1].f.expr(),"((x-y)--1)"));
	CPPUNIT_ASSERT(sys.ctrs[0].op==LEQ);
	CPPUNIT_ASSERT(sys.ctrs[1].op==GEQ);
}

void TestSystem::eq_only01() {
	System& _sys(*sysex3());
	System sys(_sys,System::EQ_ONLY);
	delete &_sys;

	CPPUNIT_ASSERT(sys.nb_ctr==2);
	CPPUNIT_ASSERT(sys.nb_var==2);
	CPPUNIT_ASSERT(sys.goal==NULL);
	CPPUNIT_ASSERT(sys.ctrs.size()==2);
	CPPUNIT_ASSERT(sys.f_ctrs.nb_arg()==2);
	CPPUNIT_ASSERT(sys.f_ctrs.nb_var()==2);
	CPPUNIT_ASSERT(sys.f_ctrs.image_dim()==2);

	stringstream expr0;
	expr0 << "((x+y)-" << Interval(-1,1) << ")"; // the way the interval is printed depends on the library
	CPPUNIT_ASSERT(sameExpr(sys.ctrs[0].f.expr(),expr0.str().c_str()));
	CPPUNIT_ASSERT(sameExpr(sys.ctrs[1].f.expr(),"(x-y)"));
	CPPUNIT_ASSERT(sys.ctrs[0].op==EQ);
	CPPUNIT_ASSERT(sys.ctrs[1].op==EQ);
}

void TestSystem::extend01() {
	System& _sys(*sysex2());
	ExtendedSystem sys(_sys);
	delete &_sys;

	CPPUNIT_ASSERT(sys.nb_ctr==4);
	CPPUNIT_ASSERT(sys.nb_var==5);
	CPPUNIT_ASSERT(sys.args.size()==3);
	CPPUNIT_ASSERT(sys.args[0].dim==Dim::col_vec(3));
	CPPUNIT_ASSERT(sys.args[1].dim==Dim::scalar());
	CPPUNIT_ASSERT(sys.args[2].dim==Dim::scalar());
	CPPUNIT_ASSERT(sameExpr(sys.goal->expr(),"__goal__"));
	//CPPUNIT_ASSERT(sys.goal==NULL);

	CPPUNIT_ASSERT(sys.box.size()==5);

	CPPUNIT_ASSERT(sys.ctrs.size()==4);
	CPPUNIT_ASSERT(sys.f_ctrs.nb_arg()==3);
	CPPUNIT_ASSERT(sys.f_ctrs.nb_var()==5);
	CPPUNIT_ASSERT(sys.f_ctrs.image_dim()==4);
	CPPUNIT_ASSERT(sameExpr(sys.ctrs[0].f.expr(),"((y-cos(x(2)))-__goal__)"));
	CPPUNIT_ASSERT(sys.ctrs[0].op==EQ);
	CPPUNIT_ASSERT(sameExpr(sys.ctrs[1].f.expr(),"(-(x(1)+x(2)))"));
	CPPUNIT_ASSERT(sys.ctrs[1].op==LEQ);
	CPPUNIT_ASSERT(sameExpr(sys.ctrs[2].f.expr(),"((x(2)+x(3))-y)"));
	CPPUNIT_ASSERT(sys.ctrs[2].op==LEQ);
	CPPUNIT_ASSERT(sameExpr(sys.ctrs[3].f.expr(),"(-(y-x(1)))"));
	CPPUNIT_ASSERT(sys.ctrs[3].op==LEQ);
}

void TestSystem::extend02() {
	System _sys(SRCDIR_TESTS "/quimper/unconstrained.qpr");
	ExtendedSystem sys(_sys);

	CPPUNIT_ASSERT(sys.nb_ctr==1);
	CPPUNIT_ASSERT(sys.nb_var==3);
	CPPUNIT_ASSERT(sameExpr(sys.goal->expr(),"__goal__"));
	//CPPUNIT_ASSERT(sys.goal==NULL);
	CPPUNIT_ASSERT(sys.ctrs.size()==1);
	CPPUNIT_ASSERT(sys.f_ctrs.nb_arg()==3);
	CPPUNIT_ASSERT(sys.f_ctrs.nb_var()==3);
	CPPUNIT_ASSERT(sys.f_ctrs.image_dim()==1);
	CPPUNIT_ASSERT(sameExpr(sys.ctrs[0].f.expr(),"((x+y)-__goal__)"));
	CPPUNIT_ASSERT(sys.ctrs[0].op==EQ);
}

void TestSystem::normalize01() {
	System& _sys(*sysex3());
	NormalizedSystem sys(_sys,0.5);
	delete &_sys;

	CPPUNIT_ASSERT(sys.nb_ctr==6);
	CPPUNIT_ASSERT(sys.nb_var==2);
	CPPUNIT_ASSERT(sys.goal==NULL);
	CPPUNIT_ASSERT(sys.ctrs.size()==6);
	CPPUNIT_ASSERT(sys.f_ctrs.nb_arg()==2);
	CPPUNIT_ASSERT(sys.f_ctrs.nb_var()==2);
	CPPUNIT_ASSERT(sys.f_ctrs.image_dim()==6);

	CPPUNIT_ASSERT(sameExpr(sys.ctrs[0].f.expr(),"((x+y)-1)"));
	CPPUNIT_ASSERT(sameExpr(sys.ctrs[1].f.expr(),"((-(x+y))-1)"));
	CPPUNIT_ASSERT(sameExpr(sys.ctrs[2].f.expr(),"((x-y)-1)"));
	CPPUNIT_ASSERT(sameExpr(sys.ctrs[3].f.expr(),"(-((x-y)--1))"));
	CPPUNIT_ASSERT(sameExpr(sys.ctrs[4].f.expr(),"((x-y)-0.5)"));
	CPPUNIT_ASSERT(sameExpr(sys.ctrs[5].f.expr(),"((-(x-y))-0.5)"));
	CPPUNIT_ASSERT(sys.ctrs[0].op==LEQ);
	CPPUNIT_ASSERT(sys.ctrs[1].op==LEQ);
	CPPUNIT_ASSERT(sys.ctrs[2].op==LEQ);
	CPPUNIT_ASSERT(sys.ctrs[3].op==LEQ);
	CPPUNIT_ASSERT(sys.ctrs[4].op==LEQ);
	CPPUNIT_ASSERT(sys.ctrs[5].op==LEQ);
}


void TestSystem::normalize02() {
	  const ExprSymbol& x=ExprSymbol::new_("x");
	  const ExprSymbol& y=ExprSymbol::new_("y");

	  SystemFactory fac;
	  fac.add_var(x);
	  fac.add_var(y);
	  const ExprNode& e=x+y;
	  Vector v(2);
	  v[0]=1; v[1]=2;
	  fac.add_ctr(((const ExprNode&) ExprVector::new_col(e,e))=ExprConstant::new_vector(v,false));
	  System sys(fac);
	  NormalizedSystem nsys(sys,1);
	  CPPUNIT_ASSERT(sys.f_ctrs.expr().size==12); // the DAG structure must be kept!
	  CPPUNIT_ASSERT(sameExpr(sys.ctrs[0].f.expr(),"(((x+y);(x+y))+(-2 ; -3))"));
	  CPPUNIT_ASSERT(sameExpr(sys.ctrs[1].f.expr(),"((-((x+y);(x+y)))+(0 ; 1))"));
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
	CPPUNIT_ASSERT(sys3.args.size()==2);
	CPPUNIT_ASSERT(strcmp(sys3.args[0].name,"x")==0);
	CPPUNIT_ASSERT(strcmp(sys3.args[1].name,"y")==0);
	CPPUNIT_ASSERT(sys3.nb_ctr==2);
	CPPUNIT_ASSERT(sameExpr(sys1.ctrs[0].f.expr(),sys3.ctrs[0].f.expr()));
	CPPUNIT_ASSERT(sameExpr(sys2.ctrs[0].f.expr(),sys3.ctrs[1].f.expr()));
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

	CPPUNIT_ASSERT(sys3.args.size()==2);
	CPPUNIT_ASSERT(strcmp(sys3.args[0].name,"x")==0);
	CPPUNIT_ASSERT(strcmp(sys3.args[1].name,"y")==0);
	CPPUNIT_ASSERT(sys3.nb_ctr==1);
	CPPUNIT_ASSERT(sameExpr(sys1.goal->expr(),sys3.goal->expr()));
	CPPUNIT_ASSERT(sameExpr(sys2.ctrs[0].f.expr(),sys3.ctrs[0].f.expr()));

	System sys4(sys2,sys1);
	CPPUNIT_ASSERT(sys4.args.size()==2);
	CPPUNIT_ASSERT(strcmp(sys4.args[0].name,"x")==0);
	CPPUNIT_ASSERT(strcmp(sys4.args[1].name,"y")==0);
	CPPUNIT_ASSERT(sys4.nb_ctr==1);
	CPPUNIT_ASSERT(sameExpr(sys1.goal->expr(),sys4.goal->expr()));
	CPPUNIT_ASSERT(sameExpr(sys2.ctrs[0].f.expr(),sys4.ctrs[0].f.expr()));

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
	CPPUNIT_ASSERT(sys3.args.size()==2);
	CPPUNIT_ASSERT(strcmp(sys3.args[0].name,"x")==0);
	CPPUNIT_ASSERT(strcmp(sys3.args[1].name,"y")==0);
	CPPUNIT_ASSERT(sys3.nb_ctr==1);
	CPPUNIT_ASSERT(sameExpr(sys1.goal->expr(),sys3.goal->expr()));
}


void TestSystem::merge04() {
	System sys1(SRCDIR_TESTS "/minibex/bearing.bch");
	System sys2(SRCDIR_TESTS "/minibex/I5.bch");
	System sys3(sys1,sys2);
	CPPUNIT_ASSERT(strcmp(sys3.args[0].name,"x1")==0);
	CPPUNIT_ASSERT(strcmp(sys3.args[4].name,"x6")==0);
	CPPUNIT_ASSERT(strcmp(sys3.args[12].name,"x14")==0);
	CPPUNIT_ASSERT(strcmp(sys3.args[13].name,"x5")==0);
	for (int i=0; i<sys1.nb_ctr; i++)
		CPPUNIT_ASSERT(sameExpr(sys3.ctrs[i].f.expr(),sys1.ctrs[i].f.expr()));
	for (int i=0; i<sys2.nb_ctr; i++)
		CPPUNIT_ASSERT(sameExpr(sys3.ctrs[sys1.nb_ctr+i].f.expr(),sys2.ctrs[i].f.expr()));
}

} // end namespace
