/* ============================================================================
 * I B E X - Interval Matrix Tests
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Mar 2, 2012
 * ---------------------------------------------------------------------------- */

#ifndef __TEST_INTERVAL_MATRIX_H__
#define __TEST_INTERVAL_MATRIX_H__

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "ibex_IntervalMatrix.h"
#include "utils.h"

using namespace ibex;

class TestIntervalMatrix : public CppUnit::TestFixture {

public:

	CPPUNIT_TEST_SUITE(TestIntervalMatrix);

	CPPUNIT_TEST(eq01);
	CPPUNIT_TEST(eq02);
	CPPUNIT_TEST(eq03);
	CPPUNIT_TEST(eq04);

	CPPUNIT_TEST(cons01);
	CPPUNIT_TEST(cons02);
	CPPUNIT_TEST(cons03);
	CPPUNIT_TEST(cons04);

	CPPUNIT_TEST(empty01);
	CPPUNIT_TEST(is_empty01);
	CPPUNIT_TEST(is_empty02);
	CPPUNIT_TEST(set_empty01);

	CPPUNIT_TEST(inter01);
	CPPUNIT_TEST(inter02);
	CPPUNIT_TEST(inter03);

	CPPUNIT_TEST(set_col01);

	CPPUNIT_TEST(rows01);
	CPPUNIT_TEST(rows02);
	CPPUNIT_TEST(rows03);

	CPPUNIT_TEST(cols01);
	CPPUNIT_TEST(cols02);
	CPPUNIT_TEST(cols03);
	CPPUNIT_TEST(cols04);
	CPPUNIT_TEST(cols05);

	CPPUNIT_TEST(resize01);
	CPPUNIT_TEST(resize02);
	CPPUNIT_TEST(resize03);

	CPPUNIT_TEST(minus01);
	CPPUNIT_TEST(minus02);

	CPPUNIT_TEST(add01);
	CPPUNIT_TEST(add02);

	CPPUNIT_TEST(sub01);
	CPPUNIT_TEST(sub02);

	CPPUNIT_TEST(mul01);
	CPPUNIT_TEST(mul02);

	CPPUNIT_TEST(put01);
	CPPUNIT_TEST(rad01);
	CPPUNIT_TEST(diam01);

	CPPUNIT_TEST_SUITE_END();

	// test:
	//   operator==(const IntervalMatrix& m)
	//   operator!=(const IntervalMatrix& m)
	void eq01();
	void eq02();

	/* test:
	 * ======
	 *   operator==(const IntervalMatrix& m)
	 *   operator!=(const IntervalMatrix& m)
	 *   nb_rows() const
	 *   nb_cols() const
	 *   operator[](int i)
	 */
	void eq03();
	void eq04();
	/* test:
	 * =======
	 *   IntervalMatrix(int nb_rows, int nb_cols)
	 *   operator=(const IntervalMatrix& x)
	 *   IntervalMatrix(const IntervalMatrix& x)
	 *   set_row(int, const IntervalVector&)
	 */
	void cons01();

	/* test:
	 * =======
	 *   row(int);
	 *   col(int) const;
	 *   rows(int,int) const;
	 *   cols(int,int) const;
	 */
	 void cons02();
	// test: IntervalMatrix(int nb_rows, int nb_cols, const Interval& x);
	void cons03();
	// test: IntervalMatrix(int m, int n, double x[][2])
	void cons04();

	// test: static IntervalMatrix empty(int m, int n)
	void empty01();
	// test: is_empty() const
	void is_empty01();
	void is_empty02();
	// test: set_empty()
	void set_empty01();

	// test: IntervalMatrix& operator&=(const IntervalMatrix& x)
	void inter01();
	void inter02();
	void inter03();

	void set_col01();

	// test: rows(int,int)
	void rows01();
	void rows02();
	void rows03();

	// test: cols(int,int)
	void cols01();
	void cols02();
	void cols03();
	void cols04();
	void cols05();

	// test: resize(int)
	void resize01();
	void resize02();
	void resize03();

	// test: operator-() const
	void minus01();
	void minus02();

	// test:
	//  operator+(const IntervalMatrix& x) const
	//  operator+=(const IntervalMatrix& x)
	void add01();
	void add02();

	// test:
	//  operator-(const IntervalMatrix& x) const
	//  operator-=(const IntervalMatrix& x)
	void sub01();
	void sub02();
	// test:
	//  operator*(const IntervalMatrix& x) const
	//  operator*=(const IntervalMatrix& x)
	void mul01();
	void mul02();

	void put01();
	void rad01();
	void diam01();
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestIntervalMatrix);


#endif // __TEST_INTERVAL_MATRIX_H__
