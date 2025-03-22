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

//#include <cppunit/TestFixture.h>
//#include <cppunit/extensions/HelperMacros.h>
#include "utils.h"

namespace ibex {

class TestSet : public CppUnit::TestFixture {
public:

	void testMe() {
		setUp();
	
//			diff01();
//			diff02();
//			diff03();
//			diff04();
//			diff05();
//			diff06();
//			diff07();
//			diff08();
//			diff09();
//			diff10();
//			diff11();
//			diff12();
//			diff13();
//			diff14();
			diff15();
		tearDown();
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
