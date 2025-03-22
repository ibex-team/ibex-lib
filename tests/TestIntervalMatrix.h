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

//#include <cppunit/TestFixture.h>
//#include <cppunit/extensions/HelperMacros.h>
#include "ibex_IntervalMatrix.h"
#include "utils.h"

using namespace ibex;

class TestIntervalMatrix : public CppUnit::TestFixture {

public:

	void testMe() {
		setUp();

		eq01();
		eq02();
		eq03();
		eq04();

		cons01();
		cons02();
		cons03();
		cons04();

		consInitList();

		empty01();
		is_empty01();
		is_empty02();
		set_empty01();

		inter01();
		inter02();
		inter03();

		set_col01();

		rows01();
		rows02();
		rows03();

		cols01();
		cols02();
		cols03();
		cols04();
		cols05();

		resize01();
		resize02();
		resize03();

		minus01();
		minus02();

		add01();
		add02();

		sub01();
		sub02();

		mul01();
		mul02();

		put01();
		rad01();
		diam01();

		tearDown();
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
	// test: IntervalMatrix(std::initializer_list<IntervalVector>)
	void consInitList();

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




#endif // __TEST_INTERVAL_MATRIX_H__
