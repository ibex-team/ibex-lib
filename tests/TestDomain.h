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

//#include <cppunit/TestFixture.h>
//#include <cppunit/extensions/HelperMacros.h>
#include "ibex_Function.h"
#include "ibex_Eval.h"
#include "utils.h"

namespace ibex {

class TestDomain : public CppUnit::TestFixture {

public:

	void testMe() {
		setUp();
		index_vec_elt();
		index_vec_subrow();
		index_vec_subcol();
		index_mat_elt();
		index_mat_subrow();
		index_mat_row();
		index_mat_subcol();
		index_mat_col();
		index_mat_rows();
		index_mat_cols();
		index_mat_submat();
		load01();
		load02();
		load03();
		load04();
		tearDown();
	}

public:
	virtual void setUp();
	virtual void tearDown();

	void row_vec();
	void col_vec();
	void matrix_01();
	void matrix_02();
	void index_vec_elt();
	void index_vec_subrow();
	void index_vec_subcol();
	void index_mat_elt();
	void index_mat_subrow();
	void index_mat_row();
	void index_mat_subcol();
	void index_mat_col();
	void index_mat_rows();
	void index_mat_cols();
	void index_mat_submat();
	void load01();
	void load02();
	void load03();
	void load04();

private:
	Domain *rv; // row vec
	Domain *cv; // col vec
	Domain *m; // matrix

	// test: Domain()
/*	void cons01();

	void scal01();
	void vector01();
	void matrix01();
	void mix01(); */

};



} // namespace ibex

#endif // __TEST_DOMAIN_H__
