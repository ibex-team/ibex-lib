/* ============================================================================
 * I B E X - Diff Tests
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Apr 02, 2012
 * ---------------------------------------------------------------------------- */

#ifndef __TEST_GRADIENT_H__
#define __TEST_GRADIENT_H__

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "utils.h"
#include "ibex_Expr.h"

namespace ibex {

class TestGradient : public CppUnit::TestFixture {

public:

	CPPUNIT_TEST_SUITE(TestGradient);

	CPPUNIT_TEST(deco01);
	CPPUNIT_TEST(deco02);

	CPPUNIT_TEST(add01);
	CPPUNIT_TEST(add02);
	CPPUNIT_TEST(add03);
	CPPUNIT_TEST(add04);
	//CPPUNIT_TEST(add05);
	CPPUNIT_TEST(dist);
	CPPUNIT_TEST(jac01);
	CPPUNIT_TEST(jac02);
	CPPUNIT_TEST(jac03);
	CPPUNIT_TEST(hansen01);
	CPPUNIT_TEST(mulVV);
	CPPUNIT_TEST(transpose01);
	CPPUNIT_TEST(mulMV01);
	CPPUNIT_TEST(mulVM01);
	CPPUNIT_TEST(mulVM02);
	CPPUNIT_TEST(jacobian_components01);
	CPPUNIT_TEST(jacobian_components02);
	CPPUNIT_TEST_SUITE_END();

	void deco01();
	void deco02();

	void add01();
	void add02();
	void add03();
	void add04();
	void dist();
	void jac01();
	void jac02();
	void jac03();
	void hansen01();

	void mulVV();
	// for vectors
	void transpose01();
	// with constant matrix
	void mulMV01();
	void mulVM01();
	void mulVM02();

	void jacobian_components01();
	void jacobian_components02();
private:
	void check_deco(const ExprNode& e);
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestGradient);


} // end namespace

#endif // __TEST_GRADIENT_H__
