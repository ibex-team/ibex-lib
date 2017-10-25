/* ============================================================================
 * I B E X - Function Tests
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Mar 23, 2012
 * ---------------------------------------------------------------------------- */

#ifndef __TEST_FUNCTION_H__
#define __TEST_FUNCTION_H__

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
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

	CPPUNIT_TEST_SUITE(TestFunction);

	CPPUNIT_TEST(parser_symbol_01);
	CPPUNIT_TEST(parser_symbol_02);
	CPPUNIT_TEST(parser_symbol_vec_01);
	CPPUNIT_TEST(parser_symbol_vec_02);
	CPPUNIT_TEST(parser_symbol_mat_01);
	CPPUNIT_TEST(parser_add_scalar);
	CPPUNIT_TEST(parser_add_vector_01);
	CPPUNIT_TEST(parser_add_vector_02);
	CPPUNIT_TEST(parser_add_mat);
	CPPUNIT_TEST(parser_mul_mat);
	CPPUNIT_TEST(parser_row_vec);
	CPPUNIT_TEST(parser_col_vec);

	CPPUNIT_TEST(eval_symbol_01);
	CPPUNIT_TEST(eval_symbol_02);
	CPPUNIT_TEST(eval_symbol_vec_01);
	CPPUNIT_TEST(eval_symbol_vec_02);
	CPPUNIT_TEST(eval_symbol_mat_01);
	CPPUNIT_TEST(eval_add_scalar);
	CPPUNIT_TEST(eval_add_vector_01);
	CPPUNIT_TEST(eval_add_vector_02);
	CPPUNIT_TEST(eval_add_mat);
	CPPUNIT_TEST(eval_mul_mat);
	CPPUNIT_TEST(eval_row_vec);
	CPPUNIT_TEST(eval_col_vec);

	CPPUNIT_TEST(build01);
	CPPUNIT_TEST(add_symbol);
	CPPUNIT_TEST(copy);
	CPPUNIT_TEST(generate_comp01);
	CPPUNIT_TEST(generate_comp02);
	CPPUNIT_TEST(used);
	CPPUNIT_TEST(used02);
	CPPUNIT_TEST(numctr01);
	CPPUNIT_TEST(apply01);
	CPPUNIT_TEST(from_string01);
	CPPUNIT_TEST(from_string02);
	CPPUNIT_TEST(from_string03);
	CPPUNIT_TEST(from_string04);
	CPPUNIT_TEST(issue43);
	CPPUNIT_TEST(issue43_bis);
	CPPUNIT_TEST(minibex01);
	CPPUNIT_TEST(minibex02);
	CPPUNIT_TEST(minibex03);
	CPPUNIT_TEST_SUITE_END();

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
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestFunction);


} // end namespace

#endif /* __TEST_FUNCTION_H__ */
