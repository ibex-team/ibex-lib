/* ============================================================================
 * I B E X - VarSet Tests
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Sep 02, 2015
 * ---------------------------------------------------------------------------- */

#include "TestVarSet.h"
#include "ibex_VarSet.h"
#include "ibex_Function.h"

namespace ibex {


void TestVarSet::test01() {
	const ExprSymbol& x=ExprSymbol::new_("x");
	const ExprSymbol& y=ExprSymbol::new_("y");
	const ExprSymbol& z=ExprSymbol::new_("z");
	Function f(x,y,z,x+y+z);

	IntervalVector y_box(1,Interval(1,2));
	VarSet set(f,x,z);

	CPPUNIT_ASSERT(set.is_var[0]);
	CPPUNIT_ASSERT(!set.is_var[1]);
	CPPUNIT_ASSERT(set.is_var[2]);
	CPPUNIT_ASSERT(set.nb_param==1);
	CPPUNIT_ASSERT(set.nb_var==2);
	CPPUNIT_ASSERT(set.var(0)==0);
	CPPUNIT_ASSERT(set.var(1)==2);
	CPPUNIT_ASSERT(set.param(0)==1);

	IntervalVector x_box(2,Interval(0,1));
	IntervalVector fullbox=set.full_box(x_box,y_box);
	double _fullbox[][2]={{0,1},{1,2},{0,1}};
	CPPUNIT_ASSERT(fullbox==IntervalVector(3,_fullbox));

}

void TestVarSet::test02() {
	const ExprSymbol& x=ExprSymbol::new_("x",Dim::col_vec(3));
	const ExprSymbol& y=ExprSymbol::new_("y",Dim::col_vec(3));
	const ExprSymbol& z=ExprSymbol::new_("z");
	Function f(x,y,z,(x+y)[0]+z);

	IntervalVector y_box(3,Interval(1,2));
	VarSet set(f,x,z);

	CPPUNIT_ASSERT(set.is_var[0]);
	CPPUNIT_ASSERT(set.is_var[1]);
	CPPUNIT_ASSERT(set.is_var[2]);
	CPPUNIT_ASSERT(!set.is_var[3]);
	CPPUNIT_ASSERT(!set.is_var[4]);
	CPPUNIT_ASSERT(!set.is_var[5]);
	CPPUNIT_ASSERT(set.is_var[6]);
	CPPUNIT_ASSERT(set.nb_param==3);
	CPPUNIT_ASSERT(set.nb_var==4);
	CPPUNIT_ASSERT(set.var(0)==0);
	CPPUNIT_ASSERT(set.var(1)==1);
	CPPUNIT_ASSERT(set.var(2)==2);
	CPPUNIT_ASSERT(set.var(3)==6);
	CPPUNIT_ASSERT(set.param(0)==3);
	CPPUNIT_ASSERT(set.param(1)==4);
	CPPUNIT_ASSERT(set.param(2)==5);

	IntervalVector x_box(4,Interval(0,1));
	IntervalVector fullbox=set.full_box(x_box,y_box);

	double _fullbox[][2]={{0,1},{0,1},{0,1},
			         {1,2},{1,2},{1,2},
			         {0,1}};
	CPPUNIT_ASSERT(fullbox==IntervalVector(7,_fullbox));
}

void TestVarSet::test03() {
	const ExprSymbol& x=ExprSymbol::new_("x",Dim::col_vec(3));
	const ExprSymbol& y=ExprSymbol::new_("y",Dim::col_vec(3));
	const ExprSymbol& z=ExprSymbol::new_("z");
	Function f(x,y,z,(x+y)[0]+z);

	IntervalVector y_box(3,Interval(1,2));
	VarSet set(f,x[0],x[2],y[0],y[1]);

	CPPUNIT_ASSERT(set.is_var[0]);
	CPPUNIT_ASSERT(!set.is_var[1]);
	CPPUNIT_ASSERT(set.is_var[2]);
	CPPUNIT_ASSERT(set.is_var[3]);
	CPPUNIT_ASSERT(set.is_var[4]);
	CPPUNIT_ASSERT(!set.is_var[5]);
	CPPUNIT_ASSERT(!set.is_var[6]);
	CPPUNIT_ASSERT(set.nb_param==3);
	CPPUNIT_ASSERT(set.nb_var==4);
	CPPUNIT_ASSERT(set.var(0)==0);
	CPPUNIT_ASSERT(set.var(1)==2);
	CPPUNIT_ASSERT(set.var(2)==3);
	CPPUNIT_ASSERT(set.var(3)==4);
	CPPUNIT_ASSERT(set.param(0)==1);
	CPPUNIT_ASSERT(set.param(1)==5);
	CPPUNIT_ASSERT(set.param(2)==6);

	IntervalVector x_box(4,Interval(0,1));
	IntervalVector fullbox=set.full_box(x_box,y_box);

	double _fullbox[][2]={{0,1},{1,2},{0,1},
			              {0,1},{0,1},{1,2},
			              {1,2}};
	CPPUNIT_ASSERT(fullbox==IntervalVector(7,_fullbox));
}

void TestVarSet::test04() {
	const ExprSymbol& x=ExprSymbol::new_("x",Dim::col_vec(3));
	const ExprSymbol& A=ExprSymbol::new_("A",Dim::matrix(2,3));
	const ExprSymbol& y=ExprSymbol::new_("y",Dim::col_vec(2));

	Function f(x,A,y,A*x+y);

	IntervalVector y_box(7,Interval(1,2));
	VarSet set(f,x,y[1]);

	int i=0;
	for (; i<3; i++) {
		CPPUNIT_ASSERT(set.is_var[i]);
		CPPUNIT_ASSERT(set.var(i)==i);
	}
	for (; i<10; i++) {
		CPPUNIT_ASSERT(!set.is_var[i]);
		CPPUNIT_ASSERT(set.param(i-3)==i);

	}
	CPPUNIT_ASSERT(set.is_var[10]);
	CPPUNIT_ASSERT(set.var(3)==10);
	CPPUNIT_ASSERT(set.nb_var==4);
	CPPUNIT_ASSERT(set.nb_param==7);

	IntervalVector x_box(4,Interval(0,1));
	IntervalVector fullbox=set.full_box(x_box,y_box);

	double _fullbox[][2]={{0,1},{0,1},{0,1},
	                 {1,2},{1,2},{1,2},
	                 {1,2},{1,2},{1,2},
			         {1,2},{0,1}};

	CPPUNIT_ASSERT(fullbox==IntervalVector(11,_fullbox));
}

void TestVarSet::test05() {
	const ExprSymbol& A=ExprSymbol::new_("A",Dim::matrix(2,3));
	const ExprSymbol& B=ExprSymbol::new_("B",Dim::matrix(2,3));

	Function f(A,B,A+B);

	IntervalVector y_box(4,Interval(1,2));
	VarSet set(f,A[0],B[0][0],B[0][2],B[1]);

	int i=0;
	int v=0;
	int p=0;
	for (; i<3; i++) {
		CPPUNIT_ASSERT(set.is_var[i]);
		CPPUNIT_ASSERT(set.var(v++)==i);
	}
	for (; i<6; i++) {
		CPPUNIT_ASSERT(!set.is_var[i]);
		CPPUNIT_ASSERT(set.param(p++)==i);
	}
	CPPUNIT_ASSERT(set.is_var[6]);
	CPPUNIT_ASSERT(set.var(v++)==6);
	CPPUNIT_ASSERT(!set.is_var[7]);
	CPPUNIT_ASSERT(set.param(p++)==7);
	for (i=8; i<11; i++) {
		CPPUNIT_ASSERT(set.is_var[i]);
		CPPUNIT_ASSERT(set.var(v++)==i);
	}

	CPPUNIT_ASSERT(set.nb_param==4);
	CPPUNIT_ASSERT(set.nb_var==8);

	IntervalVector x_box(8,Interval(0,1));
	IntervalVector fullbox=set.full_box(x_box,y_box);

	double _fullbox[][2]={{0,1},{0,1},{0,1},
	                 {1,2},{1,2},{1,2},
	                 {0,1},{1,2},{0,1},
	                 {0,1},{0,1},{0,1}};

	CPPUNIT_ASSERT(fullbox==IntervalVector(12,_fullbox));
}


void TestVarSet::test06() {
	const ExprSymbol& x=ExprSymbol::new_("x");
	const ExprSymbol& y=ExprSymbol::new_("y");
	const ExprSymbol& z=ExprSymbol::new_("z");
	Function f(x,y,z,x+y+z);

	BitSet vars=BitSet::empty(3);
	vars.add(0);
	vars.add(2);
	VarSet set(3,vars);

	IntervalVector y_box(1,Interval(1,2));

	CPPUNIT_ASSERT(set.is_var[0]);
	CPPUNIT_ASSERT(set.var(0)==0);
	CPPUNIT_ASSERT(!set.is_var[1]);
	CPPUNIT_ASSERT(set.param(0)==1);
	CPPUNIT_ASSERT(set.is_var[2]);
	CPPUNIT_ASSERT(set.var(1)==2);

	CPPUNIT_ASSERT(set.nb_param==1);
	CPPUNIT_ASSERT(set.nb_var==2);

	IntervalVector x_box(2,Interval(0,1));
	IntervalVector fullbox=set.full_box(x_box,y_box);
	double _fullbox[][2]={{0,1},{1,2},{0,1}};
	CPPUNIT_ASSERT(fullbox==IntervalVector(3,_fullbox));

}

} // namespace ibex
