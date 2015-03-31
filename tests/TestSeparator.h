//============================================================================
//                                  I B E X                                   
// File        : TestSeparator.h
// Author      : Benoit Desrochers
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jan 02, 2015
// Last Update : Jan 02, 2015
//============================================================================

#ifndef __TEST_SEPARATOR_H__
#define __TEST_SEPARATOR_H__

#include "cpptest.h"
#include "ibex_SepFwdBwd.h"
#include "ibex_SepCtcPair.h"
#include "ibex_SepInter.h"
#include "ibex_SepUnion.h"
#include "ibex_SepNot.h"
#include "ibex_CtcEmpty.h"
#include "ibex_CtcIdentity.h"
#include "utils.h"

namespace ibex {

class TestSeparator : public TestIbex {

public:
	TestSeparator() {
        TEST_ADD(TestSeparator::SepFwdBwd_LT_LEQ);
        TEST_ADD(TestSeparator::SepFwdBwd_GT_GEQ);
        TEST_ADD(TestSeparator::test_SepCtcPair);
        TEST_ADD(TestSeparator::test_SepUnionInter);
		TEST_ADD(TestSeparator::test_SepNot);
	}

    void SepFwdBwd_LT_LEQ();
    void SepFwdBwd_GT_GEQ();
    void test_SepCtcPair();
    void test_SepUnionInter();
	void test_SepNot();

};

} // end namespace ibex
#endif // __TEST_SEPARATOR_H__
