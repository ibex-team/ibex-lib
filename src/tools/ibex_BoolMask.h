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

namespace ibex {

/**
 * Boolean mask.
 */
class BoolMask {

public:
	/**
	 * \brief Create a boolean mask of size \a n with all bits unset.
	 */
	BoolMask(int n);

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
	 * \brief The ith bit.
	 */
	bool operator[](int i);

	/**
	 * \brief The ith bit.
	 */
	const bool operator[](int i) const;

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

/*================================== inline implementations ========================================*/

inline BoolMask::BoolMask(int n) : n(n), mask(new bool[n]) {
	unset_all();
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

inline bool BoolMask::operator[](int i) {
	return mask[i];
}

inline const bool BoolMask::operator[](int i) const {
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

} // namespace ibex
#endif // __IBEX_BOOL_MASK_H__
