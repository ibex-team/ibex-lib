/* ============================================================================
 * I B E X - Dim Tests
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Mar 2, 2012
 * Last Update : May 25, 2016
 * ---------------------------------------------------------------------------- */

#ifndef __TEST_DIM_H__
#define __TEST_DIM_H__

//#include <cppunit/TestFixture.h>
//#include <cppunit/extensions/HelperMacros.h>
#include "ibex_Dim.h"
#include "utils.h"

namespace ibex {

class TestDim : public CppUnit::TestFixture {

public:

	void testMe() {
		setUp();
		scalar();
		row_vec();
		col_vec();
		add();
		mul();
		vec();
		index();
		tearDown();
	}

	void scalar();
	void row_vec();
	void col_vec();
	void matrix();
	void add();
	void mul();
	void vec();
	void index();
};



} // namespace ibex

#endif // __TEST_DIM_H__
