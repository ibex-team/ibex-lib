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

//#include <cppunit/TestFixture.h>
//#include <cppunit/extensions/HelperMacros.h>
#include "ibex_IntervalVector.h"
#include "utils.h"

using namespace ibex;

class TestIntervalVector : public CppUnit::TestFixture {

public:

	void testMe() {
		setUp();

		cons01();
		cons02();
		cons03();
		cons04();
		cons05();
		consInitList();

		set_empty01();

		is_empty01();
		is_empty02();

		resize01();
		resize02();
		resize03();
		resize04();

		subvector01();
		subvector02();
		subvector03();
		subvector04();
		subvector05();
		subvector06();

		cart_prod01();


		inter01();
		inter02();
		inter03();
		inter04();

		hull01();
		hull02();
		hull03();
		hull04();

		eq01();
		eq02();
		eq03();
		eq04();

		mid01();

		is_flat01();
		is_flat02();
		is_flat03();
		is_flat04();
		is_flat05();

		is_unbounded01();
		is_unbounded02();
		is_unbounded03();
		is_unbounded04();

		is_subset01();
		is_subset02();
		is_subset03();
		is_subset04();
		is_subset05();
		is_subset06();
		is_subset07();

		is_relative_interior01();
		is_relative_interior02();
		is_relative_interior03();
		is_relative_interior04();
		is_relative_interior05();

		extr_diam_index01();
		extr_diam_index02();
		extr_diam_index03();
		extr_diam_index04();
		extr_diam_index05();
		extr_diam_index06();
		extr_diam_index07();
		extr_diam_index08();
		extr_diam_index09();
		extr_diam_index10();

		sort_indices01();
		sort_indices02();

		perimeter01();
		perimeter02();

		volume01();
		volume02();
		volume03();

		rel_distance01();

		minus01();
		minus02();
		minus03();

		add01();

		sub01();

		compl01();
		compl02();

		diff01();
		diff02();
		diff03();
		diff04();
		diff05();
		diff06();
		diff07();
		diff08();
		diff09();
		diff10();
		diff11();
		diff12();
		diff13();
		diff14();
		diff15();
		diff16();
		diff17();
		diff18();
		diff19();
		diff20();
		diff21();
		diff22();
		diff23();
		diff24();
		diff25();
		diff26();
		diff27();
		diff28();
		diff29();
		diff30();
		diff31();
		diff32();
		diff33();
		diff34();
		diff35();
		diff36();
		diff37();
		diff38();

		issue228();

		random01();
		random02();

		tearDown();
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
	// test: IntervalVector(std::initializer_list<Interval>)
	void consInitList();
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
	void subvector06();

	void cart_prod01();

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
	void eq04();

	// test: mid() const
	void mid01();

	// test: is_flat() const
	void is_flat01();
	void is_flat02();
	void is_flat03();
	void is_flat04();
	void is_flat05();

	// test: is_unbounded() const
	void is_unbounded01();
	void is_unbounded02();
	void is_unbounded03();
	void is_unbounded04();

	// test:
	//  is_subset(const IntervalVector& x) const
	//  is_strict_interior_subset(const IntervalVector& x) const
	//  is_superset(const IntervalVector& x) cons
	//  is_strict_superset(const IntervalVector& x) const
	void is_subset01();
	void is_subset02();
	void is_subset03();
	void is_subset04();
	void is_subset05();
	void is_subset06();
	void is_subset07();

	void is_relative_interior01();
	void is_relative_interior02();
	void is_relative_interior03();
	void is_relative_interior04();
	void is_relative_interior05();

	// test:
	//    extr_diam_index() const
	//    min_diam() const
	//    max_diam() const
	void extr_diam_index01();
	void extr_diam_index02();
	void extr_diam_index03();
	void extr_diam_index04();
	void extr_diam_index05();
	void extr_diam_index06();
	void extr_diam_index07();
	void extr_diam_index08();
	void extr_diam_index09();
	void extr_diam_index10();

	void sort_indices01();
	void sort_indices02();

	// test: perimeter() const
	void perimeter01();
	void perimeter02();

	// test: volume() const
	void volume01();
	void volume02();
	void volume03();

	// test: rel_distance() const
	void rel_distance01();

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

	// test: complementary(IntervalVector*& result) const
	void compl01();
	void compl02();

	// test: diff(const IntervalVector& y, IntervalVector*& result) const

	// [-2,2]x[-2,2]x[-2,2] \ empty
	void diff01();
	// \empty \ [-2,2]x[-2,2]x[-2,2]
	void diff02();
	// [-2,2]x[-2,2]x[-2,2] \ [-2,2]x[3,4]x[-2,2]
	void diff03();

	// one test with x thick and y flat should be enough
	// [-2,2]x[-2,2]x[-2,2] \ [-1,1]x[-1,-1]x[-1,1]
	void diff04();
	// [-2,2]x[-2,2]x[-2,2] \ [-1,1]x[-1,1]x[-1,1]
	void diff05();
	// [-2,2]x[-2,2]x[-2,2] \ [-1,1]x[-1,1]x[-2,1]
	void diff06();
	// [-2,2]x[-2,2]x[-2,2] \ [-1,1]x[-1,1]x[-2,2]
	void diff07();
	// [-2,2]x[-2,2]x[-2,2] \ [-1,1]x[-2,1]x[-1,1]
	void diff08();
	// [-2,2]x[-2,2]x[-2,2] \ [-1,1]x[-2,2]x[-1,1]
	void diff09();
	// [-2,2]x[-2,2]x[-2,2] \ [-2,1]x[-1,1]x[-1,1]
	void diff10();
	// [-2,2]x[-2,2]x[-2,2] \ [-2,2]x[-1,2]x[-1,1]
	void diff11();

	// [-2,2]x[-2,2]x[-2,2] \ [-1,1]x[-2,1]x[-2,1]
	void diff12();
	// [-2,2]x[-2,2]x[-2,2] \ [-1,1]x[-2,1]x[-2,2]
	void diff13();
	// [-2,2]x[-2,2]x[-2,2] \ [-2,1]x[-1,1]x[-2,1]
	void diff14();
	// [-2,2]x[-2,2]x[-2,2] \ [-2,1]x[-1,1]x[-2,2]
	void diff15();
	// [-2,2]x[-2,2]x[-2,2] \ [-2,1]x[-2,1]x[-1,1]
	void diff16();
	// [-2,2]x[-2,2]x[-2,2] \ [-2,1]x[-2,2]x[-1,1]
	void diff17();

	// [-2,2]x[-2,2]x[-2,2] \ [-2,1]x[-2,1]x[-2,1]
	void diff18();
	// [-2,2]x[-2,2]x[-2,2] \ [-2,1]x[-2,1]x[-2,2]
	void diff19();
	// [-2,2]x[-2,2]x[-2,2] \ [-2,1]x[-2,2]x[-2,1]
	void diff20();
	// [-2,2]x[-2,2]x[-2,2] \ [-2,2]x[-2,1]x[-2,1]
	void diff21();

	// [-2,2]x[-2,2]x[-2,2] \ [-2,1]x[-2,2]x[-2,2]
	void diff22();
	// [-2,2]x[-2,2]x[-2,2] \ [-2,2]x[-2,2]x[-2,1]
	void diff23();
	// [-2,2]x[-2,2]x[-2,2] \ [-2,2]x[-2,1]x[-2,2]
	void diff24();
	// [-2,2]x[-2,2]x[-2,2] \ [-2,2]x[-2,2]x[-2,2]
	void diff25();

	void diff26();
	void diff27();
	void diff28();
	void diff29();

	// [0,0]x[-2,2]x[-2,2] \ [0,0]x[-1,-1]x[-1,1]
	void diff30();
	// [0,0]x[0,0]x[-2,2] \ [0,0]x[0,0]x[-1,1]
	void diff31();
	// [0,0]x[-2,2]x[0,0] \ [0,0]x[-1,1]x[0,0]
	void diff32();

	// [-2,2]x[0,0]x[0,0] \ [-1,1]x[0,0]x[0,0]
	void diff33();
	// [0,0]x[-2,2]x[-2,2] \ [-1,-1]x[-1,-1]x[-1,1]
	void diff34();

	// Tangential intersection
	// [-2,2]x[-2,2]x[-2,2] \ [2,4]x[-2,2]x[-2,2]
	void diff35();
	// [-2,2]x[-2,2]x[-2,2] \ [2,4]x[-1,1]x[-1,1]
	void diff36();
	// [-2,2]x[-2,2] \ [-2,2]x[1,1]
	void diff37();
	// [-2,2]x[1,1] \ [0,2]x[-2,2]
	void diff38();

	void issue228();

	// test: random() const
	void random01();
	void random02();

private:
	bool test_diff(int n, double x[][2], double y[][2], int m, double z[][2], bool compactness=true, bool debug=false);
};




#endif // __TEST_INTERVAL_VECTOR_H__
