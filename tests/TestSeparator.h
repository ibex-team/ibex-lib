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

#include "ibex_SepCtcPairProj.h"
#include "ibex_SepFixPoint.h"
#include "ibex_SepInverse.h"
#include "ibex_SepQInter.h"
#include "ibex_SepTransform.h"

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
        TEST_ADD(TestSeparator::test_SepCtcPairProj);
        TEST_ADD(TestSeparator::test_SepFixPoint);
        TEST_ADD(TestSeparator::test_SepQInter);
        TEST_ADD(TestSeparator::test_SepInverse);
        TEST_ADD(TestSeparator::test_SepTransform);

	}

    void SepFwdBwd_LT_LEQ();
    void SepFwdBwd_GT_GEQ();
    void test_SepCtcPair();
    void test_SepUnionInter();
	void test_SepNot();
    
    void test_SepCtcPairProj();
    void test_SepFixPoint();
    void test_SepInverse();
    void test_SepQInter();
    void test_SepTransform();

};

} // end namespace ibex
#endif // __TEST_SEPARATOR_H__
