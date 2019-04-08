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

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "ibex_AffineVector.h"
#include "utils.h"

using namespace ibex;

template <class T>
class TestAffineVector : public CppUnit::TestFixture {

public:

	CPPUNIT_TEST_SUITE(TestAffineVector<T>);

	CPPUNIT_TEST(cons01);
	CPPUNIT_TEST(cons02);
	CPPUNIT_TEST(cons03);
	CPPUNIT_TEST(cons04);
	CPPUNIT_TEST(cons05);

	CPPUNIT_TEST(set_empty01);

	CPPUNIT_TEST(is_empty02);

	CPPUNIT_TEST(resize01);
	CPPUNIT_TEST(resize02);
	CPPUNIT_TEST(resize03);
	CPPUNIT_TEST(resize04);

	CPPUNIT_TEST(subvector01);
	CPPUNIT_TEST(subvector02);
	CPPUNIT_TEST(subvector03);
	CPPUNIT_TEST(subvector04);
	CPPUNIT_TEST(subvector05);

	CPPUNIT_TEST(eq01);
	CPPUNIT_TEST(eq02);
	CPPUNIT_TEST(eq03);

	CPPUNIT_TEST(mid01);

	CPPUNIT_TEST(is_unbounded02);
	CPPUNIT_TEST(is_unbounded03);
	CPPUNIT_TEST(is_unbounded04);

	CPPUNIT_TEST(minus01);
	CPPUNIT_TEST(minus02);

	CPPUNIT_TEST(add01);

	CPPUNIT_TEST(sub01);

	CPPUNIT_TEST_SUITE_END();

	/* test:
	 * =======
	 *   AffineMainVector(int n)
	 *   AffineMainVector(const AffineMainVector& x)
	 *   operator[](int i) const
	 *   operator=(const AffineMainVector& x)
	 *   size() const
	 */
	void cons01();
	// test: AffineMainVector(int dim, const Interval& x)
	void cons02();
	// test: AffineMainVector(const AffineMainVector& x)
	void cons03();
	// test: AffineMainVector(int n, double  bounds[][2])
	void cons04();
	// test: empty(int n)
	void cons05();
	// test: set_empty()
	void set_empty01();

	// test: is_empty() const
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
	//    operator==(const AffineMainVector& x) const
	//    operator!=(const AffineMainVector& x) const
	void eq01();
	void eq02();
	void eq03();

	// test: mid() const
	void mid01();


	// test: is_unbounded() const
	void is_unbounded02();
	void is_unbounded03();
	void is_unbounded04();

	// test: operator-() const
	void minus01();
	void minus02();

	// test:
	//  operator+(const AffineMainVector& x) const
	//  operator+=(const AffineMainVector& x)
	void add01();

	// test:
	//  operator-(const AffineMainVector& x) const
	//  operator-=(const AffineMainVector& x)
	void sub01();


private:

	void check_af(const AffineMain<T>& y_actual, const Interval& y_expected, double err = ERROR) {
		check(y_actual.itv(), y_expected, err);
	}
	void check_af(const AffineMainVector<T>& y_actual, const IntervalVector& y_expected, double err = ERROR) {
		check(y_actual.itv(), y_expected, err);
	}
	void check_af(const AffineMainMatrix<T>& y_actual, const IntervalMatrix& y_expected, double err = ERROR) {
		check(y_actual.itv(), y_expected, err);
	}
	void check_af(const AffineMain<T>& y_actual, const AffineMain<T>& y_expected, double err = ERROR) {
		check(y_actual.itv(), y_expected.itv(), err);
	}
	void check_af(const AffineMainVector<T>& y_actual, const AffineMainVector<T>& y_expected, double err = ERROR) {
		check(y_actual.itv(), y_expected.itv(), err);
	}
	void check_af(const AffineMainMatrix<T>& y_actual, const AffineMainMatrix<T>& y_expected, double err = ERROR) {
		check(y_actual.itv(), y_expected.itv(), err);
	}

};

CPPUNIT_TEST_SUITE_REGISTRATION(TestAffineVector<AF_Default>);
CPPUNIT_TEST_SUITE_REGISTRATION(TestAffineVector<AF_Other>);



#ifdef _IBEX_WITH_AFFINE_EXTENDED_

CPPUNIT_TEST_SUITE_REGISTRATION(TestAffineVector<AF_fAF2>);
//CPPUNIT_TEST_SUITE_REGISTRATION(TestAffineVector<AF_fAFFullI>);
CPPUNIT_TEST_SUITE_REGISTRATION(TestAffineVector<AF_iAF>);
CPPUNIT_TEST_SUITE_REGISTRATION(TestAffineVector<AF_fAF1>);
CPPUNIT_TEST_SUITE_REGISTRATION(TestAffineVector<AF_No>);
CPPUNIT_TEST_SUITE_REGISTRATION(TestAffineVector<AF_sAF>);

#endif


#endif // __TEST_INTERVAL_VECTOR_H__
