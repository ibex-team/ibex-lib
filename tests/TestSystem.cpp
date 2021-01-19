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
#include "ibex_DefaultSolver.h"

#include <sstream>

using namespace std;

namespace ibex {

static const int simpl = 2;

static System* sysex1() {
	SystemFactory fac;
	fac.set_simplification_level(simpl);

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
	fac.set_simplification_level(simpl);

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
	fac.set_simplification_level(simpl);

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

void TestSystem::empty() {
	System sys(SRCDIR_TESTS "/minibex/empty.mbx", simpl);
	CPPUNIT_ASSERT(sys.nb_ctr==0);
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
	CPPUNIT_ASSERT(sameExpr(sys.ctrs[1].f.expr(),"((-x(1))+y)"));
	CPPUNIT_ASSERT(sys.ctrs[1].op==GEQ);

	CPPUNIT_ASSERT(sys.ops[0]==EQ);
	CPPUNIT_ASSERT(sys.ops[1]==EQ);
	CPPUNIT_ASSERT(sys.ops[2]==EQ);
	CPPUNIT_ASSERT(sys.ops[3]==GEQ);

	delete &sys;
}


void TestSystem::factory02() {
System sys(SRCDIR_TESTS "/quimper/unconstrained.qpr", simpl);

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
CPPUNIT_ASSERT(sameExpr(sys.ctrs[1].f.expr(),"((-x(1))+y)"));
CPPUNIT_ASSERT(sys.ctrs[1].op==GEQ);
}

void TestSystem::copy02() {
System _sys(SRCDIR_TESTS "/quimper/unconstrained.qpr", simpl);
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
CPPUNIT_ASSERT(sameExpr(sys.ctrs[0].f.expr(),"((-1+x)-y)"));
CPPUNIT_ASSERT(sameExpr(sys.ctrs[1].f.expr(),"((1+x)-y)"));
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

CPPUNIT_ASSERT(sameExpr(sys.ctrs[0].f.expr(),"(([-1,1]+x)+y)"));
CPPUNIT_ASSERT(sameExpr(sys.ctrs[1].f.expr(),"(x-y)"));
CPPUNIT_ASSERT(sys.ctrs[0].op==EQ);
CPPUNIT_ASSERT(sys.ctrs[1].op==EQ);
}

void TestSystem::extend01() {
System& _sys(*sysex2());
ExtendedSystem sys(_sys,0,simpl);
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
CPPUNIT_ASSERT(sameExpr(sys.ctrs[1].f.expr(),"((-x(1))-x(2))"));
CPPUNIT_ASSERT(sys.ctrs[1].op==LEQ);
CPPUNIT_ASSERT(sameExpr(sys.ctrs[2].f.expr(),"((x(2)+x(3))-y)"));
CPPUNIT_ASSERT(sys.ctrs[2].op==LEQ);
CPPUNIT_ASSERT(sameExpr(sys.ctrs[3].f.expr(),"(x(1)-y)"));
CPPUNIT_ASSERT(sys.ctrs[3].op==LEQ);
}

void TestSystem::extend02() {
System _sys(SRCDIR_TESTS "/quimper/unconstrained.qpr", simpl);
ExtendedSystem sys(_sys,0,simpl);

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
	NormalizedSystem sys(_sys,0.5,false,simpl);
	delete &_sys;

	CPPUNIT_ASSERT(sys.nb_ctr==6);
	CPPUNIT_ASSERT(sys.nb_var==2);
	CPPUNIT_ASSERT(sys.goal==NULL);
	CPPUNIT_ASSERT(sys.ctrs.size()==6);
	CPPUNIT_ASSERT(sys.f_ctrs.nb_arg()==2);
	CPPUNIT_ASSERT(sys.f_ctrs.nb_var()==2);
	CPPUNIT_ASSERT(sys.f_ctrs.image_dim()==6);

//	CPPUNIT_ASSERT(sameExpr(sys.ctrs[0].f.expr(),"((-1+x)+y)"));
//	CPPUNIT_ASSERT(sameExpr(sys.ctrs[1].f.expr(),"((-1-x)-y)"));

	// simpl=2

	CPPUNIT_ASSERT(sameExpr(sys.ctrs[2].f.expr(),"((-1+x)-y)"));
	CPPUNIT_ASSERT(sameExpr(sys.ctrs[3].f.expr(),"((-1-x)+y)"));
	CPPUNIT_ASSERT(sameExpr(sys.ctrs[4].f.expr(),"((-0.5+x)-y)"));
	CPPUNIT_ASSERT(sameExpr(sys.ctrs[5].f.expr(),"((-0.5-x)+y)"));

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
	  fac.set_simplification_level(simpl);
	  fac.add_var(x);
	  fac.add_var(y);
	  const ExprNode& e=x+y;
	  Vector v(2);
	  v[0]=1; v[1]=2;
	  fac.add_ctr(((const ExprNode&) ExprVector::new_col(e,e))=ExprConstant::new_vector(v,false));
	  System sys(fac);
	  NormalizedSystem nsys(sys,1,false,simpl);
	  //CPPUNIT_ASSERT(sys.f_ctrs.expr().size==12); // the DAG structure not kept anymore with ExprSimplify2
	  CPPUNIT_ASSERT(sameExpr(nsys.ctrs[0].f.expr(),"(((-2+x)+y);((-3+x)+y))"));
	  CPPUNIT_ASSERT(sameExpr(nsys.ctrs[1].f.expr(),"(((-x)-y);((1-x)-y))"));
}

void TestSystem::merge01() {
	SystemFactory fac1;
	fac1.set_simplification_level(simpl);
	{
		Variable x("x");
		Variable y(2,"y");
		fac1.add_var(x);
		fac1.add_var(y);
		fac1.add_ctr(x=0);
	}
	System sys1(fac1);

	SystemFactory fac2;
	fac2.set_simplification_level(simpl);
	{
		Variable x("x");
		Variable y(2,"y");
		fac2.add_var(x);
		fac2.add_var(y);
		fac2.add_ctr(y=IntervalVector(2,Interval::zero()));
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
	fac1.set_simplification_level(simpl);
	{
		Variable x("x");
		fac1.add_var(x);
		fac1.add_goal(x);
	}
	System sys1(fac1);

	SystemFactory fac2;
	fac2.set_simplification_level(simpl);
	{
		Variable x("x");
		Variable y(2,"y");
		fac2.add_var(x);
		fac2.add_var(y);
		fac2.add_ctr(y=IntervalVector(2,Interval::zero()));
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
	fac1.set_simplification_level(simpl);
	{
		Variable x("x");
		fac1.add_var(x);
		fac1.add_goal(x);
	}
	System sys1(fac1);

	SystemFactory fac2;
	fac2.set_simplification_level(simpl);
	{
		Variable y(2,"y");
		fac2.add_var(y);
		fac2.add_ctr(y=IntervalVector(2,Interval::zero()));
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
	System sys1(SRCDIR_TESTS "/minibex/I5.bch", simpl);
	System sys2(SRCDIR_TESTS "/minibex/alkyl.bch", simpl);
	System sys3(sys1,sys2);

	CPPUNIT_ASSERT(strcmp(sys3.args[0].name,"x1")==0);
	CPPUNIT_ASSERT(strcmp(sys3.args[9].name,"x10")==0);
	CPPUNIT_ASSERT(strcmp(sys3.args[10].name,"x11")==0);
	CPPUNIT_ASSERT(strcmp(sys3.args[14].name,"x15")==0);

	for (int i=0; i<sys1.nb_ctr; i++)
		CPPUNIT_ASSERT(sameExpr(sys3.ctrs[i].f.expr(),sys1.ctrs[i].f.expr()));
	for (int i=0; i<sys2.nb_ctr; i++) {
		CPPUNIT_ASSERT(sameExpr(sys3.ctrs[sys1.nb_ctr+i].f.expr(),sys2.ctrs[i].f.expr()));

	}
}

void TestSystem::mutable_cst() {
	System sys(SRCDIR_TESTS "/minibex/mutable_cst.mbx", simpl);
	sys.constant("a").i()=4;
	sys.constant("b").v()[0]=5;
	sys.constant("b").v()[1]=6;

	DefaultSolver solver(sys);
	solver.solve(sys.box);
	CPPUNIT_ASSERT(solver.get_data().solution(0)[0]==Interval(4));
	CPPUNIT_ASSERT(solver.get_data().solution(1)[0]==Interval(5));
	CPPUNIT_ASSERT(solver.get_data().solution(2)[0]==Interval(6));

	sys.constant("a").i()=7;
	sys.constant("b").v()[0]=8;
	sys.constant("b").v()[1]=9;

	solver.solve(sys.box);
	CPPUNIT_ASSERT(solver.get_data().solution(0)[0]==Interval(7));
	CPPUNIT_ASSERT(solver.get_data().solution(1)[0]==Interval(8));
	CPPUNIT_ASSERT(solver.get_data().solution(2)[0]==Interval(9));
}
} // end namespace
