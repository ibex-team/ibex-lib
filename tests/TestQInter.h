//============================================================================
//                                  I B E X                                   
// File        : TestQInter.h
// Author      : Benoit Desrochers
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jan 02, 2015
// Last Update : Jan 02, 2015
//============================================================================

#ifndef __TEST_Q_INTER__
#define __TEST_Q_INTER__

#include "cpptest.h"
#include "ibex_QInter.h"
#include "utils.h"

namespace ibex {

class TestQInter : public TestIbex {

public:
	TestQInter() {
        TEST_ADD(TestQInter::test_projF_1);
        TEST_ADD(TestQInter::test_projF_2);
        TEST_ADD(TestQInter::test_projF_3);
	}

    void test_projF_1();
    void test_projF_2();
    void test_projF_3();
};

} // end namespace ibex
#endif // __TEST_Q_INTER__
