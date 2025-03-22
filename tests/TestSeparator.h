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

//#include <cppunit/TestFixture.h>
//#include <cppunit/extensions/HelperMacros.h>
#include "ibex_SepFwdBwd.h"
#include "ibex_SepCtcPair.h"
#include "ibex_SepInter.h"
#include "ibex_SepUnion.h"
#include "ibex_SepNot.h"
#include "ibex_CtcEmpty.h"
#include "ibex_CtcIdentity.h"
#include "ibex_SepQInter.h"
#include "ibex_SepInverse.h"
#include "utils.h"

namespace ibex {

class TestSeparator : public CppUnit::TestFixture {

public:

	void testMe() {
		setUp();

		SepFwdBwd_LT_LEQ();
		SepFwdBwd_GT_GEQ();
		test_SepCtcPair();
		test_SepUnionInter();
		test_SepNot();
		test_SepQInter();
		test_SepInverse();
		tearDown();
	}

	void SepFwdBwd_LT_LEQ();
	void SepFwdBwd_GT_GEQ();
	void test_SepCtcPair();
	void test_SepUnionInter();
	void test_SepNot();
	void test_SepQInter();
	void test_SepInverse();

};




} // end namespace ibex
#endif // __TEST_SEPARATOR_H__
