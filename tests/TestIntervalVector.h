/* ============================================================================
 * I B E X - Interval Vector Tests
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Mar 2, 2012
 * ---------------------------------------------------------------------------- */

#ifndef __TEST_INTERVAL_VECTOR_H__
#define __TEST_INTERVAL_VECTOR_H__

#include "cpptest.h"
#include "ibex_IntervalVector.h"

namespace ibex {

class TestIntervalVector : public Test::Suite {

public:
	TestIntervalVector() {

		TEST_ADD(TestIntervalVector::cons01);
		TEST_ADD(TestIntervalVector::cons02);
		TEST_ADD(TestIntervalVector::cons03);
		TEST_ADD(TestIntervalVector::cons04);
		TEST_ADD(TestIntervalVector::cons05);

		TEST_ADD(TestIntervalVector::is_empty01);
		TEST_ADD(TestIntervalVector::is_empty02);

	}

	/* test:
	 * =======
	 *   IntervalVector(int n)
	 *   IntervalVector(const IntervalVector& x)
	 *   operator[](int i) const
	 *   size()
	 */
	void cons01();
	// test: IntervalVector(int dim, const Interval& x)
	void cons02();
	// test: IntervalVector(const IntervalVector& x)
	void cons03();
	// test: IntervalVector(int n, double  bounds[][2])
	void cons04();
	// test: empty(int n)
	void cons05();

	// test: is_empty()
	void is_empty01();
	void is_empty02();

private:
	void check(double y_actual, double y_expected);
	void check(const Interval& y_actual, const Interval& y_expected);
	void check(const IntervalVector& y_actual, const IntervalVector& y_expected);
};

} // namespace ibex
#endif // __TEST_INTERVAL_VECTOR_H__
