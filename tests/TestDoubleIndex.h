/* ============================================================================
 * I B E X - DoubleIndex Tests
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : May 25, 2016
 * Last Update : May 25, 2016
 * ---------------------------------------------------------------------------- */

#ifndef __TEST_DOUBLE_INDEX_H__
#define __TEST_DOUBLE_INDEX_H__

//#include <cppunit/TestFixture.h>
//#include <cppunit/extensions/HelperMacros.h>
#include "ibex_DoubleIndex.h"
#include "utils.h"

namespace ibex {

class TestDoubleIndex : public CppUnit::TestFixture {

public:

	void testMe() {
		setUp();
		matrix_one_elt();
		matrix_sub_row();
		matrix_sub_col();
		matrix_one_row();
		matrix_one_col();
		matrix_rows();
		matrix_cols();
		matrix_all();
		row_vec_one_elt();
		row_vec_sub_row();
		row_vec_all();
		col_vec_one_elt();
		col_vec_sub_col();
		col_vec_all();
		scalar_all();
		tearDown();
	}

	void matrix_one_elt();
	void matrix_sub_row();
	void matrix_sub_col();
	void matrix_one_row();
	void matrix_one_col();
	void matrix_rows();
	void matrix_cols();
	void matrix_all();

	void row_vec_one_elt();
	void row_vec_sub_row();
	void row_vec_all();

	void col_vec_one_elt();
	void col_vec_sub_col();
	void col_vec_all();

	void scalar_all();
};



} // namespace ibex

#endif // __TEST_DOUBLE_INDEX_H__
