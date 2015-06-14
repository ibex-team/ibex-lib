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

#include "cpptest.h"
#include "utils.h"

namespace ibex {

class TestSet : public TestIbex {
public:
	TestSet() {
//		TEST_ADD(TestSet::diff01);
//		TEST_ADD(TestSet::diff02);
//		TEST_ADD(TestSet::diff03);
//		TEST_ADD(TestSet::diff04);
//		TEST_ADD(TestSet::diff05);
//		TEST_ADD(TestSet::diff06);
//		TEST_ADD(TestSet::diff07);
//		TEST_ADD(TestSet::diff08);
//		TEST_ADD(TestSet::diff09);
//		TEST_ADD(TestSet::diff10);
//		TEST_ADD(TestSet::diff11);
//		TEST_ADD(TestSet::diff12);
//		TEST_ADD(TestSet::diff13);
//		TEST_ADD(TestSet::diff14);
		TEST_ADD(TestSet::diff15);
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
	void diff15();

};

} // end namespace ibex
#endif // __TEST_SET_H__
