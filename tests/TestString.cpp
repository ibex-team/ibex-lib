/* ============================================================================
 * I B E X - String Tests
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Mar 2, 2012
 * Last Update : Jan 17, 2018
 * ---------------------------------------------------------------------------- */

#include "TestString.h"
#include "ibex_String.h"
#include "ibex_SyntaxError.h"
#include "ibex_SymbolMap.h"

using namespace std;

namespace ibex {

void TestString::test01() {
	char* buf=append_index("foo",'[',']',12478);
	CPPUNIT_ASSERT(strcmp(buf,"foo[12478]")==0);
	CPPUNIT_ASSERT(strlen(buf)==10);
	free(buf);
}

void TestString::test02() {
	char* buf=append_index("bar",'{','}',0);
	CPPUNIT_ASSERT(strcmp(buf,"bar{0}")==0);
	CPPUNIT_ASSERT(strlen(buf)==6);
	free(buf);
}

void TestString::test_parse_indexed_symbol01() {
	const ExprSymbol& x=ExprSymbol::new_("x");
	SymbolMap<const ExprSymbol*> symbols;
	symbols.insert_new(x.name,&x);
	CPPUNIT_ASSERT_THROW(parse_indexed_symbol(symbols,""),SyntaxError);
	CPPUNIT_ASSERT_THROW(parse_indexed_symbol(symbols,"y"),SyntaxError);
	CPPUNIT_ASSERT_THROW(parse_indexed_symbol(symbols,"x["),SyntaxError);
	CPPUNIT_ASSERT_THROW(parse_indexed_symbol(symbols,"x]"),SyntaxError);
	CPPUNIT_ASSERT_THROW(parse_indexed_symbol(symbols,"x("),SyntaxError);
	CPPUNIT_ASSERT_THROW(parse_indexed_symbol(symbols,"x]"),SyntaxError);
	CPPUNIT_ASSERT_THROW(parse_indexed_symbol(symbols,"x()"),SyntaxError);
	CPPUNIT_ASSERT_THROW(parse_indexed_symbol(symbols,"x(1))"),SyntaxError);
	CPPUNIT_ASSERT_THROW(parse_indexed_symbol(symbols,"x[]"),SyntaxError);
	CPPUNIT_ASSERT_THROW(parse_indexed_symbol(symbols,"x[1)"),SyntaxError);
	CPPUNIT_ASSERT_THROW(parse_indexed_symbol(symbols,"x(1]"),SyntaxError);
	CPPUNIT_ASSERT_THROW(parse_indexed_symbol(symbols,"x(a)"),SyntaxError);
	CPPUNIT_ASSERT_THROW(parse_indexed_symbol(symbols,"x(1:a)"),SyntaxError);
	CPPUNIT_ASSERT_THROW(parse_indexed_symbol(symbols,"x(a:1)"),SyntaxError);
	CPPUNIT_ASSERT_THROW(parse_indexed_symbol(symbols,"x(1:1,a)"),SyntaxError);
	CPPUNIT_ASSERT_THROW(parse_indexed_symbol(symbols,"x(1:1,1:a)"),SyntaxError);
	CPPUNIT_ASSERT_THROW(parse_indexed_symbol(symbols,"x(-1)"),SyntaxError);
	CPPUNIT_ASSERT_THROW(parse_indexed_symbol(symbols,"x(2)"),SyntaxError);
	CPPUNIT_ASSERT_THROW(parse_indexed_symbol(symbols,"x[1]"),SyntaxError);
	CPPUNIT_ASSERT_THROW(parse_indexed_symbol(symbols,"x(1,1,1)"),SyntaxError);
	CPPUNIT_ASSERT_THROW(parse_indexed_symbol(symbols,"x(,)"),SyntaxError);
	CPPUNIT_ASSERT_THROW(parse_indexed_symbol(symbols,"x(1,)"),SyntaxError);
	CPPUNIT_ASSERT_THROW(parse_indexed_symbol(symbols,"x(,1)"),SyntaxError);
	delete &x;
}

void TestString::test_parse_indexed_symbol02() {
	const ExprSymbol& x=ExprSymbol::new_("x");
	SymbolMap<const ExprSymbol*> symbols;
	symbols.insert_new(x.name,&x);
	CPPUNIT_ASSERT(&parse_indexed_symbol(symbols,"x")==&x);
	CPPUNIT_ASSERT(&parse_indexed_symbol(symbols,"x(1)")==&x);
	CPPUNIT_ASSERT(&parse_indexed_symbol(symbols,"x[0]")==&x);
	CPPUNIT_ASSERT(&parse_indexed_symbol(symbols,"x[:]")==&x);
	CPPUNIT_ASSERT(&parse_indexed_symbol(symbols,"x(:)")==&x);
	CPPUNIT_ASSERT(&parse_indexed_symbol(symbols,"x(1,1)")==&x);
	CPPUNIT_ASSERT(&parse_indexed_symbol(symbols,"x(1:1,1)")==&x);
	CPPUNIT_ASSERT(&parse_indexed_symbol(symbols,"x(1,1:1)")==&x);
	CPPUNIT_ASSERT(&parse_indexed_symbol(symbols,"x(1:1,1:1)")==&x);
	CPPUNIT_ASSERT(&parse_indexed_symbol(symbols,"x[0,0]")==&x);
	CPPUNIT_ASSERT(&parse_indexed_symbol(symbols,"x[0:0,0]")==&x);
	CPPUNIT_ASSERT(&parse_indexed_symbol(symbols,"x[0,0:0]")==&x);
	CPPUNIT_ASSERT(&parse_indexed_symbol(symbols,"x[0:0,0:0]")==&x);
	CPPUNIT_ASSERT(&parse_indexed_symbol(symbols,"x[:,:]")==&x);
	delete &x;
}

bool TestString::test_symbol(const SymbolMap<const ExprSymbol*>& symbols, const char* input, const char* result) {
	const ExprNode* e=&parse_indexed_symbol(symbols,input);
	bool ok=sameExpr(*e,result);
	cleanup(*e,false);
	return ok;
}

void TestString::test_parse_indexed_symbol03() {
	const ExprSymbol& x=ExprSymbol::new_("x");
	const ExprSymbol& y=ExprSymbol::new_("y",Dim::col_vec(3));
	SymbolMap<const ExprSymbol*> symbols;
	symbols.insert_new(x.name,&x);
	symbols.insert_new(y.name,&y);
	CPPUNIT_ASSERT(&parse_indexed_symbol(symbols,"y")==&y);
	CPPUNIT_ASSERT(&parse_indexed_symbol(symbols,"y(:)")==&y);
	CPPUNIT_ASSERT(test_symbol(symbols,"y(1:2)","y(1:2)"));
	CPPUNIT_ASSERT(test_symbol(symbols,"y(1:2,1)","y(1:2)"));
	CPPUNIT_ASSERT(test_symbol(symbols,"y(1:2,:)","y(1:2)"));
	CPPUNIT_ASSERT(test_symbol(symbols,"y(1:2,1:1)","y(1:2)"));
	CPPUNIT_ASSERT(test_symbol(symbols,"y[0:1]","y(1:2)"));
	CPPUNIT_ASSERT(test_symbol(symbols,"y[0:1,0]","y(1:2)"));
	CPPUNIT_ASSERT(test_symbol(symbols,"y[0:1,:]","y(1:2)"));
	CPPUNIT_ASSERT(test_symbol(symbols,"y[0:1,0:0]","y(1:2)"));
	CPPUNIT_ASSERT_THROW(parse_indexed_symbol(symbols,"y[3]"),SyntaxError);
	CPPUNIT_ASSERT_THROW(parse_indexed_symbol(symbols,"y(4)"),SyntaxError);
	CPPUNIT_ASSERT_THROW(parse_indexed_symbol(symbols,"y(:,2)"),SyntaxError);
	CPPUNIT_ASSERT_THROW(parse_indexed_symbol(symbols,"y(2:4,:)"),SyntaxError);
	CPPUNIT_ASSERT_THROW(parse_indexed_symbol(symbols,"y(:,1:2)"),SyntaxError);

	delete &x;
	delete &y;
}

void TestString::test_parse_indexed_symbol04() {
	const ExprSymbol& x=ExprSymbol::new_("x");
	const ExprSymbol& A=ExprSymbol::new_("A",Dim::matrix(3,4));
	SymbolMap<const ExprSymbol*> symbols;
	symbols.insert_new(x.name,&x);
	symbols.insert_new(A.name,&A);
	CPPUNIT_ASSERT(&parse_indexed_symbol(symbols,"A")==&A);
	CPPUNIT_ASSERT(&parse_indexed_symbol(symbols,"A(:)")==&A);
	CPPUNIT_ASSERT(&parse_indexed_symbol(symbols,"A(:,:)")==&A);
	CPPUNIT_ASSERT(test_symbol(symbols,"A(1:2)","A(1:2,:)"));
	CPPUNIT_ASSERT(test_symbol(symbols,"A(1:2,:)","A(1:2,:)"));
	CPPUNIT_ASSERT(test_symbol(symbols,"A(1:2,1)","A(1:2,1)"));
	CPPUNIT_ASSERT(test_symbol(symbols,"A(1:2,1:1)","A(1:2,1)"));
	CPPUNIT_ASSERT(test_symbol(symbols,"A[0:1]","A(1:2,:)"));
	CPPUNIT_ASSERT(test_symbol(symbols,"A[0:1,:]","A(1:2,:)"));
	CPPUNIT_ASSERT(test_symbol(symbols,"A[0:1,0]","A(1:2,1)"));
	CPPUNIT_ASSERT(test_symbol(symbols,"A[0:1,0:0]","A(1:2,1)"));
	CPPUNIT_ASSERT(test_symbol(symbols,"A(:,1:2)","A(:,1:2)"));
	CPPUNIT_ASSERT(test_symbol(symbols,"A(1,1:2)","A(1,1:2)"));
	CPPUNIT_ASSERT(test_symbol(symbols,"A(1:1,1:2)","A(1,1:2)"));
	CPPUNIT_ASSERT(test_symbol(symbols,"A[:,0:1]","A(:,1:2)"));
	CPPUNIT_ASSERT(test_symbol(symbols,"A[0,0:1]","A(1,1:2)"));
	CPPUNIT_ASSERT(test_symbol(symbols,"A[0:0,0:1]","A(1,1:2)"));
	CPPUNIT_ASSERT(test_symbol(symbols,"A(1:2,2:3)","A(1:2,2:3)"));
	delete &x;
	delete &A;
}

} // end namespace
