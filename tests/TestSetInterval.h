//============================================================================
//                                  I B E X                                   
// File        : TestSetInterval.h
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Aug 18, 2014
//============================================================================

#ifndef __TEST_SET_INTERVAL_H__
#define __TEST_SET_INTERVAL_H__

#include "cpptest.h"
#include "utils.h"

namespace ibex {

class TestSetInterval : public TestIbex {
public:
	TestSetInterval() {
		TEST_ADD(TestSetInterval::diff01);
		TEST_ADD(TestSetInterval::diff02);
		TEST_ADD(TestSetInterval::diff03);
		TEST_ADD(TestSetInterval::diff04);
		TEST_ADD(TestSetInterval::diff05);
		TEST_ADD(TestSetInterval::diff06);
		TEST_ADD(TestSetInterval::diff07);
		TEST_ADD(TestSetInterval::diff08);
		TEST_ADD(TestSetInterval::diff09);
		TEST_ADD(TestSetInterval::diff10);
		TEST_ADD(TestSetInterval::diff11);
		TEST_ADD(TestSetInterval::diff12);
		TEST_ADD(TestSetInterval::diff13);
		TEST_ADD(TestSetInterval::diff14);
	}

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

};

} // end namespace ibex
#endif // __TEST_SET_INTERVAL_H__
