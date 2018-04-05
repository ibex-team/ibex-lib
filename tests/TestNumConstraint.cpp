/* ============================================================================
 * I B E X - NumConstraint Tests
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Aug 23, 2012
 * ---------------------------------------------------------------------------- */

#include "TestNumConstraint.h"
#include "ibex_NumConstraint.h"
#include "ibex_SyntaxError.h"
#include <iostream>
#include <fstream>

using namespace std;

#define TMP_FILE_NAME "__tmp__.txt"

namespace ibex {

void TestNumConstraint::build_from_string01() {
	try {
	NumConstraint c("x","x+1<=2");

//	CPPUNIT_ASSERT(sameExpr(c.f.expr(),"((x+1)-2)"));
	CPPUNIT_ASSERT(sameExpr(c.f.expr(),"(x+-1)"));
	CPPUNIT_ASSERT(c.op==LEQ);

	} catch(SyntaxError&) {
		CPPUNIT_ASSERT(false);
	}
}

void TestNumConstraint::build_from_string02() {
	try {
	NumConstraint c("x","y","x+y>0");

	CPPUNIT_ASSERT(sameExpr(c.f.expr(),"(x+y)"));
	CPPUNIT_ASSERT(c.op==GT);

	} catch(SyntaxError&) {
		CPPUNIT_ASSERT(false);
	}
}

void TestNumConstraint::build_from_file01() {
	std::ofstream outfile (TMP_FILE_NAME);

	outfile << "variables x,y;" << endl << endl;
	outfile << "constraints" << endl << endl;
	outfile << " x+y<=1;" << endl;
	outfile << "end" << endl;
	outfile.close();

	NumConstraint c(TMP_FILE_NAME);

	sameExpr(c.f.expr(), "((x+y)-1)");
	CPPUNIT_ASSERT(c.op==LEQ);

	remove(TMP_FILE_NAME);
}

void TestNumConstraint::build_from_file02() {
	std::ofstream outfile (TMP_FILE_NAME);

	outfile << "variables x,y;" << endl << endl;
	outfile << "constraints" << endl << endl;
	outfile << " x+y<=1;" << endl;
	outfile << " x*y<=1;" << endl;
	outfile << "end" << endl;
	outfile.close();

	NumConstraint c(TMP_FILE_NAME);

	sameExpr(c.f.expr(), "((x+y)-1)");
	CPPUNIT_ASSERT(c.op==LEQ);

	remove(TMP_FILE_NAME);
}
//
//void TestNumConstraint::is_thick_eq01() {
//	Variable x,y;
//	// a subtraction with no constant term is not a thick equality
//	Function f1(x,y,x-y);
//	NumConstraint c1(f1,EQ);
//	pair<const ExprNode*,const Interval*> p=c1.is_thick_equality();
//
//	CPPUNIT_ASSERT(p.first==NULL);
//
//	// a "good" subtraction but with LEQ is not a thick equality
//	Function f2(x,x-Interval::ZERO);
//	NumConstraint c2(f2,LEQ);
//	p=c2.is_thick_equality();
//	CPPUNIT_ASSERT(p.first==NULL);
//}
//
//void TestNumConstraint::is_thick_eq02() {
//	Variable x("x"),y("y");
//	NumConstraint c1(x,y,x-y=Interval(0,1));
//	pair<const ExprNode*,const Interval*> p=c1.is_thick_equality();
//	CPPUNIT_ASSERT(p.first!=NULL);
//	CPPUNIT_ASSERT(sameExpr(*p.first,"(x-y)"));
//	CPPUNIT_ASSERT(*p.second==Interval(0,1));
//
//	const ExprNode& cst=ExprConstant::new_scalar(Interval(0,1));
//	NumConstraint c2(x,y,cst=x-y);
//	p=c2.is_thick_equality();
//	CPPUNIT_ASSERT(p.first!=NULL);
//	CPPUNIT_ASSERT(sameExpr(*p.first,"(x-y)"));
//	CPPUNIT_ASSERT(*p.second==Interval(0,1));
//}


} // end namespace
