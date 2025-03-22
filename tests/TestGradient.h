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

//#include <cppunit/TestFixture.h>
//#include <cppunit/extensions/HelperMacros.h>
#include "utils.h"
#include "ibex_Expr.h"

namespace ibex {

class TestGradient : public CppUnit::TestFixture {

public:

	void testMe() {
		setUp();

		deco01();
		deco02();

		add01();
		add02();
		add03();
		add04();
	//	add05();
		dist();
		jac01();
		jac02();
		jac03();
		hansen01();
		mulVV();
		transpose01();
		mulMV01();
		mulVM01();
		mulVM02();
		jacobian_components01();
		jacobian_components02();
		tearDown();
	}

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




} // end namespace

#endif // __TEST_GRADIENT_H__
