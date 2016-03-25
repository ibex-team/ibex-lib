//============================================================================
//                                  I B E X                                   
// File        : TestSet.h
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Aug 18, 2014
//============================================================================

#ifndef __TEST_SET_H__
#define __TEST_SET_H__

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "utils.h"

namespace ibex {

class TestSet : public CppUnit::TestFixture {
public:

	CPPUNIT_TEST_SUITE(TestSet);
	
//		CPPUNIT_TEST(diff01);
//		CPPUNIT_TEST(diff02);
//		CPPUNIT_TEST(diff03);
//		CPPUNIT_TEST(diff04);
//		CPPUNIT_TEST(diff05);
//		CPPUNIT_TEST(diff06);
//		CPPUNIT_TEST(diff07);
//		CPPUNIT_TEST(diff08);
//		CPPUNIT_TEST(diff09);
//		CPPUNIT_TEST(diff10);
//		CPPUNIT_TEST(diff11);
//		CPPUNIT_TEST(diff12);
//		CPPUNIT_TEST(diff13);
//		CPPUNIT_TEST(diff14);
		CPPUNIT_TEST(diff15);
	CPPUNIT_TEST_SUITE_END();

	void diff01();
	void diff02();
	void diff03();
	void diff04();
	void diff05();
	void diff06();
	void diff07();
	void diff08();
	void diff09();
	void diff10();
	void diff11();
	void diff12();
	void diff13();
	void diff14();
	void diff15();

};

CPPUNIT_TEST_SUITE_REGISTRATION(TestSet);


} // end namespace ibex
#endif // __TEST_SET_H__
