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
	BitSet b(10);
	b.add(3);
	b.add(7);
	b.add(9);
	CPPUNIT_ASSERT(b==bitset1);
}

void TestBitSet::contains01() {
	CPPUNIT_ASSERT(!bitset1[0]);
	CPPUNIT_ASSERT(!bitset1[1]);
	CPPUNIT_ASSERT(!bitset1[2]);
	CPPUNIT_ASSERT(bitset1[3]);
	CPPUNIT_ASSERT(!bitset1[4]);
	CPPUNIT_ASSERT(!bitset1[5]);
	CPPUNIT_ASSERT(!bitset1[6]);
	CPPUNIT_ASSERT(bitset1[7]);
	CPPUNIT_ASSERT(!bitset1[8]);
	CPPUNIT_ASSERT(bitset1[9]);
	CPPUNIT_ASSERT(!bitset1[10]);
}

void TestBitSet::add01() {
	BitSet b(BitSet::empty(10));
	b.add(3);
	b.add(7);
	b.add(9);
	CPPUNIT_ASSERT(b==bitset1);
}

void TestBitSet::add02() {
	BitSet b(BitSet::empty(10));
	b.add(9);
	b.add(3);
	b.add(7);
	b.add(7); // try redundant addition
	b.add(7);
	CPPUNIT_ASSERT(b==bitset1);
}


void TestBitSet::size01() {
	BitSet b(BitSet::empty(10));
	CPPUNIT_ASSERT(b.size()==0);
	b.add(3);
	CPPUNIT_ASSERT(b.size()==1);
	b.add(7);
	CPPUNIT_ASSERT(b.size()==2);
	b.add(9);
	CPPUNIT_ASSERT(b.size()==3);
	b.remove(7);
	CPPUNIT_ASSERT(b.size()==2);

	CPPUNIT_ASSERT(bitset2.size()==2);
	CPPUNIT_ASSERT(bitset3.size()==4);
}

void TestBitSet::size02() {
	BitSet b(bitset1);
	b.clear();
	CPPUNIT_ASSERT(b.size()==0);
}

void TestBitSet::resize() {
	BitSet b(1);
	b.resize(100);
	for (int i=0; i<100; i++) b.add(i);
	CPPUNIT_ASSERT(b.size()==100);
}

void TestBitSet::inter01() {
	BitSet b(bitset1);
	b&=bitset2;
	CPPUNIT_ASSERT(b==BitSet::singleton(1,7));
}

void TestBitSet::union01() {
	BitSet b(bitset1);
	b|=bitset2;
	CPPUNIT_ASSERT(b==bitset3);
}

void TestBitSet::union02() {
	BitSet b(bitset1);
	BitSet b2(BitSet::empty(10));
	b|=b2;
	CPPUNIT_ASSERT(b==bitset1);
}

void TestBitSet::next() {
	int i=bitset1.min();
	CPPUNIT_ASSERT(i==3);
	i=bitset1.next(i);
	CPPUNIT_ASSERT(i==7);
	i=bitset1.next(i);
	CPPUNIT_ASSERT(i==9);
}

void TestBitSet::clear() {
	BitSet b(bitset1);
	b.clear();
	CPPUNIT_ASSERT(b.empty());
}

void TestBitSet::remove() {
	BitSet b(bitset3);
	b.remove(4);
	CPPUNIT_ASSERT(b==bitset1);
	b.remove(4); // try removal of non-existing element
	CPPUNIT_ASSERT(b==bitset1);
}

void TestBitSet::fill01() {
	BitSet b(BitSet::empty(20));
	b.add(3); // something already here

	b.fill(1,9);
	CPPUNIT_ASSERT(!b[0]);
	for (int i=1; i<=9; i++)
		CPPUNIT_ASSERT(b[i]);
	CPPUNIT_ASSERT(!b[10]);
}

void TestBitSet::fill02() {
	BitSet b(BitSet::empty(10));
	b.fill(0,9);
	for (int i=0; i<=9; i++)
		CPPUNIT_ASSERT(b[i]);
	CPPUNIT_ASSERT(!b[10]);
}

void TestBitSet::all_bits() {
	BitSet b(BitSet::all(10));
	for (int i=0; i<=9; i++)
		CPPUNIT_ASSERT(b[i]);
	CPPUNIT_ASSERT(!b[10]);
}

void TestBitSet::set_minus_with01() {
	BitSet b(bitset1);
	b.diff(bitset2);
	for (int i=0; i<10; i++) {
		if (i==3 || i==9) {
			CPPUNIT_ASSERT(b[i]);
		} else
			CPPUNIT_ASSERT(!b[i]);
	}
}

void TestBitSet::set_minus_with02() {
	BitSet b(bitset3);
	b.diff(bitset1);
	for (int i=0; i<10; i++) {
		if (i==4) {
			CPPUNIT_ASSERT(b[i]);
		} else
			CPPUNIT_ASSERT(!b[i]);
	}
}
