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

#include "cpptest.h"
#include "ibex_IntervalMatrix.h"
#include "utils.h"

using namespace ibex;

class TestIntervalMatrix : public TestIbex {

public:
	TestIntervalMatrix() {

		TEST_ADD(TestIntervalMatrix::eq01);
		TEST_ADD(TestIntervalMatrix::eq02);
		TEST_ADD(TestIntervalMatrix::eq03);

		TEST_ADD(TestIntervalMatrix::cons01);
		TEST_ADD(TestIntervalMatrix::cons02);
		TEST_ADD(TestIntervalMatrix::cons03);
		TEST_ADD(TestIntervalMatrix::cons04);

		TEST_ADD(TestIntervalMatrix::set_col01);

		TEST_ADD(TestIntervalMatrix::rows01);
		TEST_ADD(TestIntervalMatrix::rows02);
		TEST_ADD(TestIntervalMatrix::rows03);

		TEST_ADD(TestIntervalMatrix::cols01);
		TEST_ADD(TestIntervalMatrix::cols02);
		TEST_ADD(TestIntervalMatrix::cols03);
		TEST_ADD(TestIntervalMatrix::cols04);
		TEST_ADD(TestIntervalMatrix::cols05);

		TEST_ADD(TestIntervalMatrix::resize01);
		TEST_ADD(TestIntervalMatrix::resize02);

		TEST_ADD(TestIntervalMatrix::minus01);

		TEST_ADD(TestIntervalMatrix::add01);

		TEST_ADD(TestIntervalMatrix::sub01);

		TEST_ADD(TestIntervalMatrix::mul01);
	}

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

	// test: operator-() const
	void minus01();

	// test:
	//  operator+(const IntervalMatrix& x) const
	//  operator+=(const IntervalMatrix& x)
	void add01();

	// test:
	//  operator-(const IntervalMatrix& x) const
	//  operator-=(const IntervalMatrix& x)
	void sub01();

	// test:
	//  operator*(const IntervalMatrix& x) const
	//  operator*=(const IntervalMatrix& x)
	void mul01();
};

#endif // __TEST_INTERVAL_MATRIX_H__
