/* ============================================================================
 * I B E X - ibex_IntervalMatrixArray.h
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Jan 21, 2012
 * ---------------------------------------------------------------------------- */

#ifndef __IBEX_INTERVAL_MATRIX_ARRAY_H__
#define __IBEX_INTERVAL_MATRIX_ARRAY_H__

#include <iostream>
#include "ibex_IntervalMatrix.h"
#include "ibex_MatrixArray.h"

namespace ibex {

/**
 * \ingroup arithmetic
 *
 * \brief Array of interval matrices.
 */
class IntervalMatrixArray {

public:
	/**
	 * \brief Create an array of n (nb_rowsxnb_cols) interval matrices.
	 */
	IntervalMatrixArray(int n, int nb_rows, int nb_cols);

	/**
	 * \brief Copy a.
	 */
	IntervalMatrixArray(const IntervalMatrixArray& a);

	/**
	 * \brief Assign a to *this.
	 */
	IntervalMatrixArray& operator=(const IntervalMatrixArray& a);

	/**
	 * \brief Set *this to its intersection with x
	 *
	 * \return a reference to this.
	 */
	IntervalMatrixArray& operator&=(const IntervalMatrixArray& x);

	/**
	 * \brief Check equality
	 */
	bool operator==(const IntervalMatrixArray& m) const;

	/**
	 * \brief Set all the elements to x.
	 *
	 */
	void init(const Interval& x);

	/**
	 * \brief Set this IntervalMatrixArray to the empty IntervalMatrixArray
	 *
	 * The dimensions remain the same.
	 */
	void set_empty();

	/**
	 * \brief Delete this.
	 */
	~IntervalMatrixArray();

	/**
	 * \brief Size of the array.
	 */
	int size() const;

	/**
	 * \brief Number of rows of each matrix
	 */
	int nb_rows() const;

	/**
	 * \brief Number of columns of each matrix
	 */
	int nb_cols() const;

	/**
	 * \brief Return true iff this IntervalMatrixArray is empty
	 */
	bool is_empty() const;

	/**
	 * \brief Return the ith matrix.
	 *
	 */
	IntervalMatrix& operator[](int i);

	/**
	 * \brief Return a const reference to the ith matrix.
	 */
	const IntervalMatrix& operator[](int i) const;

	/**
	 * \brief True iff this interval matrix array is a subset of \a x.
	 *
	 * \pre Dimension of \a x must be equal to the dimension of *this.

	 * \note Always return true if *this is empty.

	 * \sa #ibex::Interval::is_subset(const Interval&) const.
	 */
	bool is_subset(const IntervalMatrixArray& x) const;

	/**
	 * \brief True iff this interval matrix array is inside the interior of \a x.
	 *
	 * \pre Dimension of \a x must be equal to the dimension of this matrix.
	 *
	 * \note return true if this interval matrix array is empty and \a x not.
	 *
	 * \sa #ibex::Interval::is_strict_subset(const Interval&) const.
	 */
	bool is_strict_subset(const IntervalMatrixArray& x) const;

	/**
	 * \brief True iff this interval matrix array is a subset of the interior of \a x.
	 *
	 * \pre Dimension of \a x must be equal to the dimension of *this.
	 *
	 * \note Always return true if *this is empty.
	 *
	 * \sa #ibex::Interval::is_interior_subset(const Interval&) const.
	 */
	bool is_interior_subset(const IntervalMatrixArray& x) const;

	/**
	 * \brief True iff this interval matrix array is a subset of the interior of \a x and different from x.
	 *
	 * \pre Dimension of \a x must be equal to the dimension of *this.
	 *
	 * \note Always return true if *this is empty.
	 *
	 * \sa #ibex::Interval::is_interior_subset(const Interval&) const.
	 */
	bool is_strict_interior_subset(const IntervalMatrixArray& x) const;

	/**
	 * \brief True iff this interval matrix array is a superset of \a x.
	 *
	 * \pre Dimension of \a x must be equal to the dimension of this matrix.

	 * \note Always return true if \a x is empty.

	 * \sa #ibex::Interval::is_superset(const Interval&) const.
	 */
	bool is_superset(const IntervalMatrixArray& x) const;

	/**
	 * \brief True iff \a x is inside the interior of (*this).
	 *
	 * \pre Dimension of \a x must be equal to the dimension of this matrix.
	 *
	 * \note return true if x is empty and not (*this).
	 *
	 * \sa #ibex::Interval::is_strict_superset(const Interval&) const.
	 */
	bool is_strict_superset(const IntervalMatrixArray& x) const;

	/**
	 * \brief True iff this interval matrix array contains \a x.
	 *
	 * \pre Dimension of \a x must be equal to the dimension of (*this).
	 * \sa #ibex::Interval::contains(double) const.
	 */
	bool contains(const MatrixArray& x) const;

	/**
	 * \brief True iff \a x is in the interior of this interval matrix array.
	 *
	 * \pre Dimension of \a x must be equal to the dimension of (*this).
	 * \sa #ibex::Interval::strictly_contains(double) const.
	 */
	bool interior_contains(const MatrixArray& x) const;

	/**
	 * \brief True iff this interval matrix array intersects \a x.
	 *
	 * \pre Dimension of \a x must be equal to the dimension of (*this).
	 * \sa #ibex::Interval::intersects(double) const.
	 */
	bool intersects(const IntervalMatrixArray& x) const;

	/**
	 * \brief True iff this interval matrix array intersects \a x and the intersection has a non-null volume
	 *
	 * \pre Dimension of \a x must be equal to the dimension of (*this).
	 * \sa #ibex::Interval::strictly_intersects(double) const.
	 */
	bool overlaps(const IntervalMatrixArray& x) const;

	/**
	 * \brief True iff this interval matrix array does not intersect \a x.
	 *
	 * \pre Dimension of \a x must be equal to the dimension of (*this).
	 * \sa #ibex::Interval::is_disjoint(double) const.
	 */
	bool is_disjoint(const IntervalMatrixArray& x) const;


private:
	IntervalMatrix* array;
	int n;
};

/** \ingroup arithmetic */
/*@{*/


/**
 * \brief Output an array of interval matrices.
 */
std::ostream& operator<<(std::ostream& os, const IntervalMatrixArray&);

/*@}*/

/*================================== inline implementations ========================================*/

inline int IntervalMatrixArray::size() const {
	return n;
}

inline int IntervalMatrixArray::nb_rows() const {
	assert(n>0);
	return array[0].nb_rows();
}

inline int IntervalMatrixArray::nb_cols() const {
	assert(n>0);
	return array[0].nb_cols();
}

inline IntervalMatrix& IntervalMatrixArray::operator[](int i) {
	return array[i];
}

inline const IntervalMatrix& IntervalMatrixArray::operator[](int i) const {
	return array[i];
}

inline void IntervalMatrixArray::set_empty() {
	(*this)[0].set_empty();
}

inline bool IntervalMatrixArray::is_empty() const {
	return (*this)[0].is_empty();
}

} // namespace ibex

#endif // __IBEX_INTERVAL_MATRIX_ARRAY_H__
