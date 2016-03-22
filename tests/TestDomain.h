/* ============================================================================
 * I B E X - Domain Tests
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Mar 2, 2012
 * ---------------------------------------------------------------------------- */

#ifndef __TEST_DOMAIN_H__
#define __TEST_DOMAIN_H__

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "ibex_Function.h"
#include "ibex_Eval.h"
#include "utils.h"

namespace ibex {

class TestDomain : public CppUnit::TestFixture {

public:

	CPPUNIT_TEST_SUITE(TestDomain);
	
/*
		CPPUNIT_TEST(cons01);
		CPPUNIT_TEST(scal01);
		CPPUNIT_TEST(vector01);
		CPPUNIT_TEST(matrix01);
		CPPUNIT_TEST(mix01);
		*/
	CPPUNIT_TEST_SUITE_END();

	// test: Domain()
/*	void cons01();

	void scal01();
	void vector01();
	void matrix01();
	void mix01(); */

};

CPPUNIT_TEST_SUITE_REGISTRATION(TestDomain);


} // namespace ibex
#endif // __TEST_DOMAIN_H__
