//============================================================================
//                                  I B E X
// File        : ibex_BitSet.h
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Jul 20, 2017
//============================================================================

#ifndef __IBEX_BITSET_H__
#define __IBEX_BITSET_H__

#include "ibex_mistral_Bitset.h"

#include <cassert>

namespace ibex {

/**
 * \brief Bit set (of dynamically-fixed size).
 *
 * A bit is usually viewed (and used) as a sorted list of
 * (all distinct) positive integers. Viewing this structure
 * as an array of bits is only necessary at construction, because
 * the size of the array determines the greatest integer that
 * can be stored (called the "capacity"). So, except in the constructor,
 * we consider an object of this class as a sorted list of integers.
 *
 * E.g., the array of bits [0,0,1,0,1] will always be noted (2,4).
 *
 * Current implementation: export from Mistral:
 * https://github.com/ehebrard/Mistral-2.0
 * (note: this implementation seems to allow negative integers).
 */
class BitSet {
public:

	/**
	 * \brief Create an "empty" bitset () of capacity n.
	 *
	 * The bitset contains n bits, indexed from 0 to n-1, all set to "false".
	 */
	explicit BitSet(int n);

	/**
	 * \brief Copy constructor.
	 */
	BitSet(const BitSet& b);

	/**
	 * \brief Create a copy of a list (given as an array of n integers).
	 *
	 * The list in argument does not need to be sorted.
	 *
	 * The greatest integer in the list l determines the array capacity.
	 */
	BitSet(const int n, const int* l);

	/**
	 * \brief Create an "empty" bitset () of capacity n.
	 *
	 * The bitset contains n bits, indexed from 0 to n-1, all set to "false".
	 */
	static BitSet empty(int n);

	/**
	 * \brief Create a "filled" bitset (0,...,n-1) (of capacity n).
	 *
	 * The bitset contains n bits, indexed from 0 to n-1, all set to "true".
	 */
	static BitSet all(int n);

	/**
	 * \brief Create {i} (a bitset of capacity n with one element: i).
	 */
	static BitSet singleton(int n, int i);

	/**
	 * \brief Compose two bitsets.
	 *
	 * E.g. if b1 is (1,2,5,8,9) and b2 is (1,3) then
	 *
	 *      b1 o b2 = (2,8).
	 */
	BitSet compose(const BitSet& b) const;

	bool operator==(const BitSet& b);

	bool operator!=(const BitSet& b);

	/**
	 * \brief Set this to another.
	 */
	BitSet& operator=(const BitSet& b);

	/**
	 * \brief Set this list to the union with another.
	 *
	 * E.g. if b1=(1,2,5,8) and b2=(1,3) then b1 |= b2 gives
	 *
	 *      b1 = (1,2,3,5,8)
	 */
	BitSet& operator|=(const BitSet& b);

	/**
	 * \brief The minimum of the list.
	 *
	 * E.g, min(1,2,3)=1.
	 *
	 */
	int min() const;

	/**
	 * \brief The maximum of the list.
	 *
	 * E.g, max(1,2,3)=3.
	 *
	 */
	int max() const;

	/**
	 * \brief Remove an integer from the list.
	 */
	void remove(const int elt);

	/**
	 * \brief Return the integer following elt in the list.
	 *
	 */
	int next(const int elt) const;

	/**
	 * \brief Return the size of the list.
	 *
	 * E.g., size(1,4,8)=3.
	 *
	 * \note not to be confused with the capacity given in argument
	 *       of the constructor.
	 */
	int size() const;

	/**
	 * \brief Add an integer in the list.
	 *
	 * \pre elt should be less than the capacity.
	 */
	void add(const int elt);

	/**
	 * \brief True if the list is empty.
	 */
	bool empty() const;

	/**
	 * \brief Add all element between lb and ub in the list.
	 */
	void fill(const int lb, const int ub);

	/**
	 * \brief Remove all elements from the list.
	 */
	void clear();

	/**
	 * \brief True if i is in the list.
	 */
	bool operator[](const int i) const;

	/**
	 * \brief Set difference between two lists.
	 *
	 * E.g.: if b1=(2,4,8) and b2=(1,2,7) then
	 *
	 *       b1.diff(b2)=(4,8).
	 */
	void diff(const BitSet& b);

private:
	Mistral::BitSet bitset;

};

std::ostream& operator<<(std::ostream& os, const BitSet& b);

/*================================== inline implementations ========================================*/

inline BitSet::BitSet(int n) : bitset(0,n-1,Mistral::BitSet::empt) { }

inline BitSet::BitSet(const BitSet& b) : bitset(b.bitset) { }

inline BitSet::BitSet(const int n, const int* l) : bitset(n,l) { }

inline BitSet BitSet::empty(int n) { return BitSet(n); }

inline BitSet BitSet::all(int n) {
	BitSet b(n);
	b.bitset.fill(0,n-1);
	return b;
}

inline BitSet BitSet::singleton(int n, int i) {
	BitSet b=empty(n);
	b.add(i);
	return b;
}

inline bool BitSet::operator==(const BitSet& b) { return bitset==b.bitset; }

inline bool BitSet::operator!=(const BitSet& b) { return bitset!=b.bitset; }

inline BitSet& BitSet::operator=(const BitSet& b) { bitset=b.bitset; return *this; }

inline BitSet& BitSet::operator|=(const BitSet& b) { bitset.union_with(b.bitset); return *this; }

inline int BitSet::min() const { return bitset.min(); }

inline int BitSet::max() const { return bitset.max(); }

inline void BitSet::remove(const int elt) { bitset.fast_remove(elt); }

inline int BitSet::next(const int elt) const { return bitset.next(elt); }

inline int BitSet::size() const { return (int) bitset.size(); }

inline void BitSet::add(const int elt) { bitset.fast_add(elt); }

inline bool BitSet::empty() const { return bitset.empty(); }

inline void BitSet::fill(const int lb, const int ub) { bitset.fill(lb,ub); }

inline void BitSet::clear() { bitset.clear(); }

inline bool BitSet::operator[](const int i) const { return bitset.fast_contain(i); }

inline void BitSet::diff(const BitSet& b) { bitset.setminus_with(b.bitset); }

} // end namespace ibex
  
#endif
