/* ============================================================================
 * I B E X - TestBitset
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Dec 07, 2011
 * Last Update : Aug 19, 2014
 * ---------------------------------------------------------------------------- */

#ifndef __TEST_BITSET_H__
#define __TEST_BITSET_H__

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "utils.h"

using namespace ibex;

class TestBitSet : public CppUnit::TestFixture {
public:

	CPPUNIT_TEST_SUITE(TestBitSet);
	CPPUNIT_TEST(initialise);
	CPPUNIT_TEST(contains01);
	CPPUNIT_TEST(add01);
	CPPUNIT_TEST(add02);
	CPPUNIT_TEST(size01);
	CPPUNIT_TEST(size02);
	CPPUNIT_TEST(resize);
	CPPUNIT_TEST(union01);
	CPPUNIT_TEST(union02);
	CPPUNIT_TEST(next);
	CPPUNIT_TEST(clear);
	CPPUNIT_TEST(remove);
	CPPUNIT_TEST(fill01);
	CPPUNIT_TEST(fill02);
	CPPUNIT_TEST(all_bits);
	CPPUNIT_TEST(set_minus_with01);
	CPPUNIT_TEST(set_minus_with02);
	CPPUNIT_TEST_SUITE_END();
	
private:
	void initialise();
	void contains01();
	void add01();
	void add02();
	void size01();
	void size02();
	void resize();
	void inter01();
	void union01();
	void union02();
	void next();
	void clear();
	void remove();
	void fill01();
	void fill02();
	void all_bits();
	void set_minus_with01();
	void set_minus_with02();
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestBitSet);


#endif // __TEST_BITSET__
