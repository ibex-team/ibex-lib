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

		TEST_ADD(TestIntervalMatrix::cons01);
		TEST_ADD(TestIntervalMatrix::cons02);
		TEST_ADD(TestIntervalMatrix::cons03);
	}

	/* test:
	 * =======
	 *   IntervalMatrix(int nb_rows, int nb_cols);
	 *   IntervalMatrix(int nb_rows, int nb_cols, const Interval& x);
	 *   IntervalMatrix(const IntervalMatrix& x)
	 *   operator[](int i)
	 *   operator[](int i) const
	 *   operator=(const IntervalMatrix& x)
	 *   nb_rows() const
	 *   nb_cols() const
	 *   row(int);
	 *   col(int) const;
	 *   rows(int,int) const;
	 *   cols(int,int) const;
	 */
	void cons01();
	void cons02();
	void cons03();
	//void cons04();
	//void cons05();

	void set_empty01();

	// test: is_empty() const
	void is_empty01();
	void is_empty02();

	// test: resize(int)
	void resize01();
	void resize02();
	void resize03();
	void resize04();

	// test:
	//   operator&=(const IntervalMatrix& x)
	//   operator&(const IntervalMatrix& x) const
	void inter01();
	void inter02();
	void inter03();
	void inter04();

	// test:
	//   operator|=(const IntervalMatrix| x)
	//   operator|(const IntervalMatrix| x) const
	void hull01();
	void hull02();
	void hull03();
	void hull04();

	// test:
	//    operator==(const IntervalMatrix& x) const
	//    operator!=(const IntervalMatrix& x) const
	void eq01();
	void eq02();
	void eq03();

	// test: mid() const
	void mid01();

	// test: operator-() const
	void minus01();
	void minus02();
	void minus03();

	// test:
	//  operator+(const IntervalMatrix& x) const
	//  operator+=(const IntervalMatrix& x)
	void add01();

	// test:
	//  operator-(const IntervalMatrix& x) const
	//  operator-=(const IntervalMatrix& x)
	void sub01();

};

#endif // __TEST_INTERVAL_MATRIX_H__
