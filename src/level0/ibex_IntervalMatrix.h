/* ============================================================================
 * I B E X - Matrix of intervals
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Jan 6, 2012
 * ---------------------------------------------------------------------------- */

#ifndef _IBEX_INTERVAL_MATRIX_H_
#define _IBEX_INTERVAL_MATRIX_H_

#include "ibex_IntervalVector.h"
#include <iostream>

namespace ibex {

class IntervalMatrix {

public:
	/* Create an interval matrix with 0 rows and 0 columns */
	IntervalMatrix();

	void resize(int nb_rows, int nb_cols);

	IntervalMatrix(int nb_rows, int nb_cols);
	IntervalMatrix(double d);

	/**
	 * \brief Return the number of columns.
	 */
	int nb_cols();

	/**
	 * \brief Return the number of rows.
	 */
	int nb_rows();

	/**
	 * \brief Return the ith row.
	 *
	 * Use (*this)[i][j] to get a reference to the element M(i,j).
	 */
	IntervalVector& operator[](int i);

	/**
	 * \brief Return a const reference to the ith row.
	 *
	 * Use (*this)[i][j] to get a reference to the element M(i,j).
	 */
	const IntervalVector& operator[](int i) const;

	/**
	 * \brief Return a submatrix.
	 */
	IntervalMatrix submatrix(int row_start_index, int row_end_index, int col_start_index, int col_end_index);

	/**
	 * \brief Return the ith row.

	 * Equivalent to (*this)[i.
	 */
	IntervalVector& row(int i) {
		return (*this)[i];
	}

	/**
	 * \brief Return a const reference to the ith row.

	 * Equivalent to (*this)[i.
	 */
	const IntervalVector& row(int i) const {
		return (*this)[i];
	}

	/**
	 * \brief Return a column
	 */
	IntervalVector col(int i) const;

	/**
	 * \brief Return a subset of columns.
	 */
	IntervalMatrix cols(int start_index, int end_index) {
		return submatrix(0, nb_rows()-1, start_index, end_index);
	}

	/**
	 * \brief Set a row.
	 */
	void set_row(int row, const IntervalVector& v);

	/**
	 * \brief Set a column.
	 */
	void set_col(int col, const IntervalVector& v);

    /** \brief Return -*this. */
    IntervalMatrix operator-() const;

	IntervalMatrix& operator&=(const IntervalMatrix& x);
};

/** \brief $[m]_1+[m]_2$. */
IntervalMatrix operator+(const IntervalMatrix& m1, const IntervalMatrix& m2);

/** \brief $[m]_1-[m]_2$. */
IntervalMatrix operator-(const IntervalMatrix& m1, const IntervalMatrix& m2);

/** \brief $[m]_1*[m]_2$. */
IntervalMatrix operator*(const IntervalMatrix& m1, const IntervalMatrix& m2);

std::ostream& operator<<(std::ostream& os, const IntervalMatrix&);

} // namespace ibex
#endif // _IBEX_INTERVAL_MATRIX_H_
