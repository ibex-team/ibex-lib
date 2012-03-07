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
#include "utils.h"

using namespace ibex;

class TestIntervalVector : public TestIbex {

public:
	TestIntervalVector() {

		TEST_ADD(TestIntervalVector::cons01);
		TEST_ADD(TestIntervalVector::cons02);
		TEST_ADD(TestIntervalVector::cons03);
		TEST_ADD(TestIntervalVector::cons04);
		TEST_ADD(TestIntervalVector::cons05);

		TEST_ADD(TestIntervalVector::set_empty01);

		TEST_ADD(TestIntervalVector::is_empty01);
		TEST_ADD(TestIntervalVector::is_empty02);

		TEST_ADD(TestIntervalVector::resize01);
		TEST_ADD(TestIntervalVector::resize02);
		TEST_ADD(TestIntervalVector::resize03);
		TEST_ADD(TestIntervalVector::resize04);

		TEST_ADD(TestIntervalVector::subvector01);
		TEST_ADD(TestIntervalVector::subvector02);
		TEST_ADD(TestIntervalVector::subvector03);
		TEST_ADD(TestIntervalVector::subvector04);
		TEST_ADD(TestIntervalVector::subvector05);

		TEST_ADD(TestIntervalVector::inter01);
		TEST_ADD(TestIntervalVector::inter02);
		TEST_ADD(TestIntervalVector::inter03);
		TEST_ADD(TestIntervalVector::inter04);

		TEST_ADD(TestIntervalVector::hull01);
		TEST_ADD(TestIntervalVector::hull02);
		TEST_ADD(TestIntervalVector::hull03);
		TEST_ADD(TestIntervalVector::hull04);

		TEST_ADD(TestIntervalVector::eq01);
		TEST_ADD(TestIntervalVector::eq02);
		TEST_ADD(TestIntervalVector::eq03);

		TEST_ADD(TestIntervalVector::mid01);

		TEST_ADD(TestIntervalVector::extr_diam_index01);
		TEST_ADD(TestIntervalVector::extr_diam_index02);
		TEST_ADD(TestIntervalVector::extr_diam_index03);
		TEST_ADD(TestIntervalVector::extr_diam_index04);
		TEST_ADD(TestIntervalVector::extr_diam_index05);

		TEST_ADD(TestIntervalVector::minus01);
		TEST_ADD(TestIntervalVector::minus02);
		TEST_ADD(TestIntervalVector::minus03);

		TEST_ADD(TestIntervalVector::add01);

		TEST_ADD(TestIntervalVector::sub01);


	}

	/* test:
	 * =======
	 *   IntervalVector(int n)
	 *   IntervalVector(const IntervalVector& x)
	 *   operator[](int i) const
	 *   operator=(const IntervalVector& x)
	 *   size() const
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
	// test: set_empty()
	void set_empty01();

	// test: is_empty() const
	void is_empty01();
	void is_empty02();

	// test: resize(int)
	void resize01();
	void resize02();
	void resize03();
	void resize04();

	// test: subvector(int start_index, int end_index)
	void subvector01();
	void subvector02();
	void subvector03();
	void subvector04();
	void subvector05();

	// test:
	//   operator&=(const IntervalVector& x)
	//   operator&(const IntervalVector& x) const
	void inter01();
	void inter02();
	void inter03();
	void inter04();

	// test:
	//   operator|=(const IntervalVector| x)
	//   operator|(const IntervalVector| x) const
	void hull01();
	void hull02();
	void hull03();
	void hull04();

	// test:
	//    operator==(const IntervalVector& x) const
	//    operator!=(const IntervalVector& x) const
	void eq01();
	void eq02();
	void eq03();

	// test: mid() const
	void mid01();

	// test: is_flat() const
	void is_flat01();
	void is_flat02();
	void is_flat03();
	void is_flat04();
	void is_flat05();

	// test:
	//    extr_diam_index() const
	//    min_diam() const
	//    max_diam() const
	void extr_diam_index01();
	void extr_diam_index02();
	void extr_diam_index03();
	void extr_diam_index04();
	void extr_diam_index05();

	// test: operator-() const
	void minus01();
	void minus02();
	void minus03();

	// test:
	//  operator+(const IntervalVector& x) const
	//  operator+=(const IntervalVector& x)
	void add01();

	// test:
	//  operator-(const IntervalVector& x) const
	//  operator-=(const IntervalVector& x)
	void sub01();

private:

};

#endif // __TEST_INTERVAL_VECTOR_H__
