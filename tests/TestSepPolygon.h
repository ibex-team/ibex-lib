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

#include "cpptest.h"
#include "ibex_SepPolygon.h"
#include "ibex_SepNot.h"
#include "ibex_SepInter.h"

#include "utils.h"

namespace ibex {

class TestSepPolygon : public TestIbex {

public:
	TestSepPolygon() {
        TEST_ADD(TestSepPolygon::test_SepPolygon_01);
        TEST_ADD(TestSepPolygon::test_SepPolygon_02);
        TEST_ADD(TestSepPolygon::test_SepPolygon_03);
	}

    void setup();
    void test_SepPolygon_01();
    void test_SepPolygon_02();
    void test_SepPolygon_03();

private:
    vector<double> murs_xa,murs_xb,murs_ya,murs_yb;
    vector<double> murs_xa2,murs_xb2,murs_ya2,murs_yb2;

};

} // end namespace ibex
#endif // __TEST_SEPPOLYGON_H__
