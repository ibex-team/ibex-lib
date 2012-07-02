/* ============================================================================
 * I B E X - Boolean mask
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Feb 27, 2012
 * ---------------------------------------------------------------------------- */

#ifndef __IBEX_BOOL_MASK_H__
#define __IBEX_BOOL_MASK_H__

#include <vector>
#include <cassert>
#include <iostream>

namespace ibex {

/** \ingroup tools
 * \brief Boolean mask.
 */
class BoolMask {

public:
	/**
	 * \brief Create a boolean mask of size \a n with all bits unset.
	 */
	BoolMask(int n);

	/**
	 * \brief Create a boolean mask of size \a n with all bits set to value.
	 */
	BoolMask(int n, bool value);

	/**
	 * \brief Create a 0-sized mask (to be used with #resize()).
	 */
	BoolMask();

	/**
	 * \brief Resize the mask.
	 */
	void resize(int n);

	/**
	 * \brief Size of the mask.
	 */
	int size() const;

	/**
	 * \brief Set a bool mask to another
	 */
	BoolMask& operator=(const BoolMask& m);

	/**
	 * \brief Logical AND.
	 */
	BoolMask& operator&=(const BoolMask& m);

	/**
	 * \brief Logical OR.
	 */
	BoolMask& operator|=(const BoolMask& m);

	/**
	 * \brief set all bits to true.
	 */
	void set_all();

	/**
	 * \brief set all bits to false.
	 */
	void unset_all();

	/**
	 * \brief true iff all bits are true.
	 */
	bool all_set() const;

	/**
	 * \brief true iff all bits are false.
	 */
	bool all_unset() const;

	/**
	 * \brief Number of bits set
	 *
	 * Complexity: O(n)
	 */
	int nb_set() const;

	/**
	 * \brief Number of bits unset
	 *
	 * Complexity: O(n)
	 */
	int nb_unset() const;

	/**
	 * \brief The ith bit.
	 */
	bool& operator[](int i);

	/**
	 * \brief The ith bit.
	 */
	const bool& operator[](int i) const;

	/**
	 * \brief set the ith bit to true.
	 */
	void set(int i);

	/**
	 * \brief Set the ith bit to false.
	 */
	void unset(int i);

	/**
	 * \brief Delete *this.
	 */
	~BoolMask();
private:
	int n;
	bool* mask;
};

std::ostream& operator<<(std::ostream& os, const BoolMask& m);

/*================================== inline implementations ========================================*/

inline BoolMask::BoolMask() : n(0), mask(NULL) {

}

inline BoolMask::BoolMask(int n) : n(n), mask(new bool[n]) {
	unset_all();
}

inline BoolMask::BoolMask(int n, bool value) : n(n), mask(new bool[n]) {
	if (value) set_all();
	else unset_all();
}

inline void BoolMask::resize(int n2) {
	assert(n>=0);
	bool* new_mask=new bool[n2];
	int i=0;
	for (; i<n; i++) {
		if (i<n2) new_mask[i] = mask[i];
	}
	for (; i<n2; i++) {
		new_mask[i]=false;
	}
	if (mask) delete[] mask;
	mask=new_mask;
	n=n2;
}

inline int BoolMask::size() const {
	return n;
}

inline int BoolMask::nb_set() const {
	int k=0;
	for (int i=0; i<n; i++) if ((*this)[i]) k++;
	return k;
}

inline int BoolMask::nb_unset() const {
	int k=0;
	for (int i=0; i<n; i++) if (!(*this)[i]) k++;
	return k;
}

inline BoolMask& BoolMask::operator=(const BoolMask& m) {
	for (int i=0; i<n; i++)
		(*this)[i] = m[i];
	return *this;
}

inline BoolMask& BoolMask::operator&=(const BoolMask& m) {
	for (int i=0; i<n; i++)
		(*this)[i] &= m[i];
	return *this;
}

inline BoolMask& BoolMask::operator|=(const BoolMask& m) {
	for (int i=0; i<n; i++)
		(*this)[i] |= m[i];
	return *this;
}

inline void BoolMask::set_all() {
	for (int i=0; i<n; i++)
		set(i);
}

inline void BoolMask::unset_all() {
	for (int i=0; i<n; i++)
		unset(i);
}

inline bool BoolMask::all_set() const {
	for (int i=0; i<n; i++)
		if (!(*this)[i]) return false;
	return true;
}

inline bool BoolMask::all_unset() const {
	for (int i=0; i<n; i++)
		if ((*this)[i]) return false;
	return true;
}

inline bool& BoolMask::operator[](int i) {
	return mask[i];
}

inline const bool& BoolMask::operator[](int i) const {
	return mask[i];
}

inline void BoolMask::set(int i) {
	mask[i]=true;
}

inline void BoolMask::unset(int i) {
	mask[i]=false;
}

inline BoolMask::~BoolMask() {
	delete[] mask;
}

inline std::ostream& operator<<(std::ostream& os, const BoolMask& m) {
	os << "(";
	for (int i=0; i<m.size(); i++) {
		os << (m[i]?"1":"0") << (i<m.size()-1?" ":"");
	}
	return os << ")";
}

} // namespace ibex
#endif // __IBEX_BOOL_MASK_H__
