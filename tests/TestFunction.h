/* ============================================================================
 * I B E X - Function Tests
 * ============================================================================
 * Copyright   : IMT Atlantique (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Mar 23, 2012
 * Last update : Jun 16, 2019
 * ---------------------------------------------------------------------------- */

#ifndef __TEST_FUNCTION_H__
#define __TEST_FUNCTION_H__

//#include <cppunit/TestFixture.h>
//#include <cppunit/extensions/HelperMacros.h>
#include "utils.h"

namespace ibex {

class TestFunction : public CppUnit::TestFixture {
private:
	Function *symbol_01;
	Function *symbol_02;
	Function *symbol_vec_01;
	Function *symbol_vec_02;
	Function *symbol_mat_01;
	Function *add_scalar;
	Function *add_vector_01;
	Function *add_vector_02;
	Function *add_mat;
	Function *mul_mat;
	Function *row_vec;
	Function *col_vec;

	bool test_evl(Function* f, const Interval& x_in, const Interval& y_out);
	bool test_evl(Function* f, const Interval& x1_in, const Interval& x2_in, const Interval& y_out);

public:

	void setUp();

	void tearDown();

	void testMe() {
		setUp();

		parser_symbol_01();
		parser_symbol_02();
		parser_symbol_vec_01();
		parser_symbol_vec_02();
		parser_symbol_mat_01();
		parser_add_scalar();
		parser_add_vector_01();
		parser_add_vector_02();
		parser_add_mat();
		parser_mul_mat();
		parser_row_vec();
		parser_col_vec();

		eval_symbol_01();
		eval_symbol_02();
		eval_symbol_vec_01();
		eval_symbol_vec_02();
		eval_symbol_mat_01();
		eval_add_scalar();
		eval_add_vector_01();
		eval_add_vector_02();
		eval_add_mat();
		eval_mul_mat();
		eval_row_vec();
		eval_col_vec();

		build01();
		add_symbol();
		copy();
		generate_comp01();
		generate_comp02();
		generate_comp03();

		used();
		used02();
		numctr01();
		apply01();
		from_string01();
		from_string02();
		from_string03();
		from_string04();
		issue43();
		issue43_bis();
		minibex01();
		minibex02();
		minibex03();
		def_domain01();
		def_domain02();
		tearDown();
	}

	void parser_symbol_01();
	void parser_symbol_02();
	void parser_symbol_vec_01();
	void parser_symbol_vec_02();
	void parser_symbol_mat_01();
	void parser_add_scalar();
	void parser_add_vector_01();
	void parser_add_vector_02();
	void parser_add_mat();
	void parser_mul_mat();
	void parser_row_vec();
	void parser_col_vec();

	void eval_symbol_01();
	void eval_symbol_02();
	void eval_symbol_vec_01();
	void eval_symbol_vec_02();
	void eval_symbol_mat_01();
	void eval_add_scalar();
	void eval_add_vector_01();
	void eval_add_vector_02();
	void eval_add_mat();
	void eval_mul_mat();
	void eval_row_vec();
	void eval_col_vec();

	// an uninitialized function must be deletable
	// without problem (the case happens when
	// the parser returns prematurely)
	void build01();
	// constructor with string arguments (char**)
	void build02();

	void add_symbol();

	void copy();

	void generate_comp01();
	void generate_comp02();
	void generate_comp03(); // matrix-valued

	void used();
	void used02();

	void numctr01();

	void apply01();

	void from_string01();
	void from_string02();
	void from_string03();
	void from_string04();

	void issue43();
	void issue43_bis();
	void minibex01();
	void minibex02();
	void minibex03();

	void def_domain01();
	void def_domain02();
};




} // end namespace

#endif /* __TEST_FUNCTION_H__ */
