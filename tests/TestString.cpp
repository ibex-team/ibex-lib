/* ============================================================================
 * I B E X - String Tests
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert, Simon Rohou
 * Created     : Mar 2, 2012
 * Updated     : April 18, 2016
 * ---------------------------------------------------------------------------- */

#include "TestString.h"

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

bool TestString::testConversion(const Interval& intv, int precision)
{
	string intv_string = "  " + intv.str(precision) + "  "; // adding unwanted spaces
	Interval intv_parsed = Interval(intv_string);
	return intv_parsed == intv ||
			fabs(intv_parsed.lb() - intv.lb()) < 1.0e-1 && fabs(intv_parsed.ub() - intv.ub()) < 1.0e-1;
}

void TestString::test03() {
	CPPUNIT_ASSERT(testConversion(Interval(-6.3588151,2.864632), 4));
	CPPUNIT_ASSERT(testConversion(Interval(0,27885.5523), 15));
	CPPUNIT_ASSERT(testConversion(Interval(-99,-97), 7));
	CPPUNIT_ASSERT(testConversion(Interval::EMPTY_SET, 4));
	CPPUNIT_ASSERT(testConversion(Interval::ALL_REALS, 4));
	CPPUNIT_ASSERT(testConversion(Interval::ZERO, 4));
	CPPUNIT_ASSERT(testConversion(Interval::PI, 4));
	CPPUNIT_ASSERT(testConversion(Interval::ONE, 3));
	CPPUNIT_ASSERT(testConversion(Interval::POS_REALS, 3));
	CPPUNIT_ASSERT(testConversion(Interval::NEG_REALS, 3));
}

} // end namespace
