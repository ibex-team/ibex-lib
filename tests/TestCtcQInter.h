//============================================================================
//                                  I B E X                                   
// File        : TestCtcQInter.h
// Author      : Benoit Desrochers
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jan 02, 2015
// Last Update : Jan 02, 2015
//============================================================================

#ifndef __TEST_CTC_Q_INTER__
#define __TEST_CTC_Q_INTER__

#include "cpptest.h"
#include "ibex_QInter.h"
#include "utils.h"

namespace ibex {

class TestCtcQInter : public TestIbex {

public:
	TestCtcQInter() {
        TEST_ADD(TestCtcQInter::test_projF);
	}

    void test_projF();
};

} // end namespace ibex
#endif // __TEST_CTC_Q_INTER__
