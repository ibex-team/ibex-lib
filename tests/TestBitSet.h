/* ============================================================================
 * I B E X - TestBitset
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Dec 07, 2011
 * Last Update : Aug 19, 2014
 * ---------------------------------------------------------------------------- */

#ifndef __TEST_BITSET_H__
#define __TEST_BITSET_H__

#include "cpptest.h"
#include "utils.h"

using namespace ibex;

class TestBitSet : public TestIbex {
public:
	TestBitSet() {
		TEST_ADD(TestBitSet::initialise);
		TEST_ADD(TestBitSet::contains01);
		TEST_ADD(TestBitSet::add01);
		TEST_ADD(TestBitSet::add02);
		TEST_ADD(TestBitSet::size01);
		TEST_ADD(TestBitSet::size02);
		TEST_ADD(TestBitSet::union01);
		TEST_ADD(TestBitSet::union02);
		TEST_ADD(TestBitSet::next);
		TEST_ADD(TestBitSet::clear);
		TEST_ADD(TestBitSet::remove);
		TEST_ADD(TestBitSet::fill01);
		TEST_ADD(TestBitSet::all_bits);
	}
private:
	void initialise();
	void contains01();
	void add01();
	void add02();
	void size01();
	void size02();
	void union01();
	void union02();
	void next();
	void clear();
	void remove();
	void fill01();
	void all_bits();
};

#endif // __TEST_BITSET__
