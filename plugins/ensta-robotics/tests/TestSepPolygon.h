//============================================================================
//                                  I B E X                                   
// File        : TestSeparator.h
// Author      : Benoit Desrochers
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jan 02, 2015
// Last Update : Jan 02, 2015
//============================================================================

#ifndef __TEST_SEPPOLYGON_H__
#define __TEST_SEPPOLYGON_H__

#include <vector>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "ibex_SepPolygon.h"
#include "ibex_SepNot.h"
#include "ibex_SepInter.h"

#include "utils.h"

namespace ibex {

class TestSepPolygon : public CppUnit::TestFixture {

public:

	CPPUNIT_TEST_SUITE(TestSepPolygon);

	CPPUNIT_TEST(test_SepPolygon_01);
	CPPUNIT_TEST(test_SepPolygon_02);
	CPPUNIT_TEST(test_SepPolygon_03);
	CPPUNIT_TEST_SUITE_END();

	void setup();
	void test_SepPolygon_01();
    void test_SepPolygon_02();
    void test_SepPolygon_03();

private:
    std::vector<double> murs_xa,murs_xb,murs_ya,murs_yb;
    std::vector<double> murs_xa2,murs_xb2,murs_ya2,murs_yb2;

};

CPPUNIT_TEST_SUITE_REGISTRATION(TestSepPolygon);


} // end namespace ibex
#endif // __TEST_SEPPOLYGON_H__
