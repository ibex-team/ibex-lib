/* ============================================================================
 * I B E X - Domain Tests
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Mar 2, 2012
 * ---------------------------------------------------------------------------- */

#include "TestBoolInterval.h"
#include "ibex_BoolInterval.h"

using namespace std;

namespace ibex {

#define ERROR 1e-10

void TestBoolInterval::test_constructor1(){
	BoolInterval a(YES);
	TEST_ASSERT(a == YES);
	BoolInterval b(NO);
	TEST_ASSERT(b == NO);
	BoolInterval c(MAYBE);
	TEST_ASSERT(c == MAYBE);
	BoolInterval d(EMPTY);
	TEST_ASSERT(d == EMPTY);
}

void TestBoolInterval::test_constructor2(){

//	BoolInterval a=true;
//	TEST_ASSERT(a == YES );
//	BoolInterval b=false;
//	TEST_ASSERT(b == NO);
	BoolInterval c;
	TEST_ASSERT(c == MAYBE);
}

void TestBoolInterval::test_constructor3(){
	BoolInterval a(NO);
	BoolInterval b(a);
	TEST_ASSERT(a == b);
	TEST_ASSERT(b == NO);
}

void TestBoolInterval::test_assignement(){
	BoolInterval a;
	BoolInterval b(NO);
	TEST_ASSERT(a == MAYBE);
	a = YES;
	TEST_ASSERT(a == YES);
	a = b;
	TEST_ASSERT(a == NO);
//	a = true;
//	TEST_ASSERT(a == YES);

}

void TestBoolInterval::test_inter1(){
	BoolInterval a(MAYBE), b(YES), c(NO);
	TEST_ASSERT( (a & b) == YES);
	TEST_ASSERT( (a & NO) == NO);
	TEST_ASSERT( (b & c) == EMPTY);
}

void TestBoolInterval::test_inter2(){
	BoolInterval a(MAYBE), b(YES), c(NO);
	a &= b;
	TEST_ASSERT(a == YES);	
}

void TestBoolInterval::test_union1(){
	BoolInterval a(MAYBE), b(YES), c(NO);
	TEST_ASSERT( (a | b) == MAYBE);
	TEST_ASSERT( (a | NO) == MAYBE);
	TEST_ASSERT( (b | c) == MAYBE);
}

void TestBoolInterval::test_union2(){
	BoolInterval a(MAYBE), b(YES), c(NO);
	a |= b;
	TEST_ASSERT(a == MAYBE);	

}

void TestBoolInterval::test_and(){
	BoolInterval a(MAYBE), b(YES), c(NO);
	TEST_ASSERT( (a && b) == MAYBE);
	TEST_ASSERT( (a && NO) == NO);
	TEST_ASSERT( (b && c) == NO);

}

void TestBoolInterval::test_or(){
	BoolInterval a(MAYBE), b(YES), c(NO);
	TEST_ASSERT( (a || b) == YES);
	TEST_ASSERT( (a || NO) == a);
	TEST_ASSERT( (b || c) == YES);

}



} // namespace ibex
