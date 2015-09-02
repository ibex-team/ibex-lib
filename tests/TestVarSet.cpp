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

namespace ibex {


void TestVarSet::test01() {
	const ExprSymbol& x=ExprSymbol::new_("x");
	const ExprSymbol& y=ExprSymbol::new_("y");
	const ExprSymbol& z=ExprSymbol::new_("z");
	Function f(x,y,z,x+y+z);

	IntervalVector y_init(1);
	VarSet set(f,y,y_init);
	TEST_ASSERT(set.vars[0]);
	TEST_ASSERT(!set.vars[1]);
	TEST_ASSERT(set.vars[2]);
	TEST_ASSERT(set.y_init.size()==1);
	TEST_ASSERT(set.nb_param==1);
	TEST_ASSERT(set.nb_var==2);
}

void TestVarSet::test02() {
	const ExprSymbol& x=ExprSymbol::new_("x",Dim::col_vec(3));
	const ExprSymbol& y=ExprSymbol::new_("y",Dim::col_vec(3));
	const ExprSymbol& z=ExprSymbol::new_("z");
	Function f(x,y,z,(x+y)[0]+z);

	IntervalVector y_init(3);
	VarSet set(f,y,y_init);
	TEST_ASSERT(set.vars[0]);
	TEST_ASSERT(set.vars[1]);
	TEST_ASSERT(set.vars[2]);
	TEST_ASSERT(!set.vars[3]);
	TEST_ASSERT(!set.vars[4]);
	TEST_ASSERT(!set.vars[5]);
	TEST_ASSERT(set.vars[6]);
	TEST_ASSERT(set.y_init.size()==3);
	TEST_ASSERT(set.nb_param==3);
	TEST_ASSERT(set.nb_var==4);
}

void TestVarSet::test03() {
	const ExprSymbol& x=ExprSymbol::new_("x",Dim::col_vec(3));
	const ExprSymbol& y=ExprSymbol::new_("y",Dim::col_vec(3));
	const ExprSymbol& z=ExprSymbol::new_("z");
	Function f(x,y,z,(x+y)[0]+z);

	IntervalVector y_init(3);
	VarSet set(f,x[1],y[2],z,y_init);
	TEST_ASSERT(set.vars[0]);
	TEST_ASSERT(!set.vars[1]);
	TEST_ASSERT(set.vars[2]);
	TEST_ASSERT(set.vars[3]);
	TEST_ASSERT(set.vars[4]);
	TEST_ASSERT(!set.vars[5]);
	TEST_ASSERT(!set.vars[6]);
	TEST_ASSERT(set.y_init.size()==3);
	TEST_ASSERT(set.nb_param==3);
	TEST_ASSERT(set.nb_var==4);

}

void TestVarSet::test04() {
	const ExprSymbol& x=ExprSymbol::new_("x",Dim::col_vec(3));
	const ExprSymbol& A=ExprSymbol::new_("A",Dim::matrix(2,3));
	const ExprSymbol& y=ExprSymbol::new_("y",Dim::col_vec(2));

	Function f(x,A,y,A*x+y);

	IntervalVector y_init(7);
	VarSet set(f,A,y[0],y_init);
	int i=0;
	for (; i<3; i++) TEST_ASSERT(set.vars[i]);
	for (; i<10; i++) TEST_ASSERT(!set.vars[i]);
	TEST_ASSERT(set.vars[10]);

	TEST_ASSERT(set.y_init.size()==7);
	TEST_ASSERT(set.nb_param==7);
	TEST_ASSERT(set.nb_var==4);

}

void TestVarSet::test05() {
	const ExprSymbol& A=ExprSymbol::new_("A",Dim::matrix(2,3));
	const ExprSymbol& B=ExprSymbol::new_("B",Dim::matrix(2,3));

	Function f(A,B,A+B);

	IntervalVector y_init(4);
	VarSet set(f,A[1],B[0][1],y_init);
	int i=0;
	for (; i<3; i++) TEST_ASSERT(set.vars[i]);
	for (; i<6; i++) TEST_ASSERT(!set.vars[i]);
	TEST_ASSERT(set.vars[6]);
	TEST_ASSERT(!set.vars[7]);
	for (i=8; i<11; i++) TEST_ASSERT(set.vars[i]);
	TEST_ASSERT(set.y_init.size()==4);
	TEST_ASSERT(set.nb_param==4);
	TEST_ASSERT(set.nb_var==8);

}

} // namespace ibex
