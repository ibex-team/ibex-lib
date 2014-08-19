/* ============================================================================
 * I B E X - TestBitset
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Aug 19, 2014
 * ---------------------------------------------------------------------------- */

#include "TestBitSet.h"
#include "ibex_BitSet.h"
#include "utils.h"
#include <float.h>

using namespace std;

namespace {
int _bitset1[3] = { 3, 7, 9 };
int _bitset2[2] = { 4, 7 };
int _bitset3[4] = { 3, 4, 7, 9 };
BitSet bitset1(3, _bitset1);
BitSet bitset2(2, _bitset2);
BitSet bitset3(4, _bitset3);
}

void TestBitSet::initialise() {
	BitSet b;
	b.initialise(0,9,BitSet::empt);
	b.add(3);
	b.add(7);
	b.add(9);
	TEST_ASSERT(b==bitset1);
}

void TestBitSet::contains01() {
	TEST_ASSERT(!bitset1.contain(0));
	TEST_ASSERT(!bitset1.contain(1));
	TEST_ASSERT(!bitset1.contain(2));
	TEST_ASSERT(bitset1.contain(3));
	TEST_ASSERT(!bitset1.contain(4));
	TEST_ASSERT(!bitset1.contain(5));
	TEST_ASSERT(!bitset1.contain(6));
	TEST_ASSERT(bitset1.contain(7));
	TEST_ASSERT(!bitset1.contain(8));
	TEST_ASSERT(bitset1.contain(9));
	TEST_ASSERT(!bitset1.contain(10));
}

void TestBitSet::add01() {
	BitSet b(BitSet::empty(10));
	b.add(3);
	b.add(7);
	b.add(9);
	TEST_ASSERT(b==bitset1);
}

void TestBitSet::add02() {
	BitSet b(BitSet::empty(10));
	b.add(9);
	b.add(3);
	b.add(7);
	b.add(7); // try redundant addition
	b.add(7);
	TEST_ASSERT(b==bitset1);
}


void TestBitSet::size01() {
	BitSet b(BitSet::empty(10));
	TEST_ASSERT(b.size()==0);
	b.add(3);
	TEST_ASSERT(b.size()==1);
	b.add(7);
	TEST_ASSERT(b.size()==2);
	b.add(9);
	TEST_ASSERT(b.size()==3);
	b.remove(7);
	TEST_ASSERT(b.size()==2);

	TEST_ASSERT(bitset2.size()==2);
	TEST_ASSERT(bitset3.size()==4);
}

void TestBitSet::size02() {
	BitSet b(bitset1);
	b.clear();
	TEST_ASSERT(b.size()==0);
}

void TestBitSet::union01() {
	BitSet b(bitset1);
	b.union_with(bitset2);
	TEST_ASSERT(b==bitset3);
}

void TestBitSet::union02() {
	BitSet b(bitset1);
	BitSet b2(BitSet::empty(10));
	b.union_with(b2);
	TEST_ASSERT(b==bitset1);
}

void TestBitSet::next() {
	int i=bitset1.min();
	TEST_ASSERT(i==3);
	i=bitset1.next(i);
	TEST_ASSERT(i==7);
	i=bitset1.next(i);
	TEST_ASSERT(i==9);
}

void TestBitSet::clear() {
	BitSet b(bitset1);
	b.clear();
	TEST_ASSERT(b.empty());
}

void TestBitSet::remove() {
	BitSet b(bitset3);
	b.remove(4);
	TEST_ASSERT(b==bitset1);
	b.remove(4); // try removal of non-existing element
	TEST_ASSERT(b==bitset1);
}

void TestBitSet::fill01() {
	BitSet b(BitSet::empty(20));
	b.add(3); // something already here

	b.fill(1,9);
	TEST_ASSERT(!b.contain(0));
	for (int i=1; i<=9; i++)
		TEST_ASSERT(b.contain(i));
	TEST_ASSERT(!b.contain(10));
}

void TestBitSet::all_bits() {
	BitSet b(BitSet::all(10));
	for (int i=0; i<=9; i++)
		TEST_ASSERT(b.contain(i));
	TEST_ASSERT(!b.contain(10));
}
