/* ============================================================================
 * I B E X - ibex_IntervalMatrix.cpp
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Jan 6, 2012
 * ---------------------------------------------------------------------------- */

#include "ibex_IntervalMatrix.h"

namespace ibex {

IntervalMatrix::IntervalMatrix(int nb_rows, int nb_cols) : _nb_rows(nb_rows), _nb_cols(nb_cols) {
	M = new IntervalVector[_nb_rows];
	for (int i=0; i<_nb_rows; i++) {
		M[i].n=_nb_cols;
		M[i].vec=new Interval[_nb_cols];
	}
}

IntervalMatrix::IntervalMatrix(int nb_rows, int nb_cols, const Interval& x) : _nb_rows(nb_rows), _nb_cols(nb_cols) {
	M = new IntervalVector[_nb_rows];
	for (int i=0; i<_nb_rows; i++) {
		M[i].n=_nb_cols;
		M[i].vec=new Interval[_nb_cols];
		for (int j=0; j<_nb_cols; j++) M[i].vec[j]=x;
	}
}

void IntervalMatrix::resize(int nb_rows, int nb_cols) {
	IntervalVector* M2 = new IntervalVector[nb_rows];
	for (int i=0; i<nb_rows; i++) {
		M2[i].n=nb_cols;
		M2[i].vec=new Interval[nb_cols]; // ALL_REALS by default
		if (i<_nb_rows) {
			int min_cols=nb_cols<_nb_cols?nb_cols:_nb_cols;
			for (int j=0; j<min_cols; j++)
				M2[i][j]=M[i][j];
		}
	}
	delete[] M;
	M=M2;
}

IntervalMatrix IntervalMatrix::submatrix(int row_start_index, int row_end_index, int col_start_index, int col_end_index) {
	assert(row_start_index>=0 && row_start_index<nb_rows());
	assert(row_end_index>=0 && row_end_index<nb_rows());
	assert(row_start_index<=row_end_index);
	assert(col_start_index>=0 && col_start_index<nb_cols());
	assert(col_end_index>=0 && col_end_index<nb_cols());
	assert(col_start_index<=col_end_index);

	IntervalMatrix sub(row_end_index-row_start_index+1, col_end_index-col_start_index+1);
	int i2=0;
	for (int i=row_start_index; i<=row_end_index; i++, i2++) {
		int j2=0;
		for (int j=col_start_index; j<=col_end_index; j++,j2++)
			sub[i2][j2] = M[i][j];
	}
	return sub;
}

IntervalVector IntervalMatrix::col(int j) const {
	assert(j>=0 && j<nb_cols());

	IntervalVector res(nb_rows());
	for (int i=0; i<nb_rows(); i++)
		res[i]=(*this)[i][j];
	return res;
}

void IntervalMatrix::set_col(int col, const IntervalVector& v) {
	assert(col>=0 && col<nb_cols());
	assert(nb_rows()==v.size());

	for (int i=0; i<nb_rows(); i++)
		M[i][col]=v[i];
}

IntervalMatrix IntervalMatrix::operator-() const {
	IntervalMatrix res(nb_rows(),nb_cols());
	for (int i=0; i<nb_rows(); i++)
		res[i]=-(*this)[i];
	return res;
}

IntervalMatrix& IntervalMatrix::operator&=(const IntervalMatrix& x) {
	assert(x.nb_rows()==nb_rows());
	assert(x.nb_cols()==nb_cols());

	for (int i=0; i<nb_rows(); i++)
		(*this)[i] &= x[i];
	return *this;
}

IntervalMatrix operator+(const IntervalMatrix& m1, const IntervalMatrix& m2) {
	assert(m1.nb_rows()==m2.nb_rows());
	assert(m1.nb_cols()==m2.nb_cols());

	IntervalMatrix m3(m1);

	for (int i=0; i<m1.nb_rows(); i++)
		m3[i]+=m2[i];

	return m3;
}

/** \brief $[m]_1-[m]_2$. */
IntervalMatrix operator-(const IntervalMatrix& m1, const IntervalMatrix& m2) {
	assert(m1.nb_rows()==m2.nb_rows());
	assert(m1.nb_cols()==m2.nb_cols());

	IntervalMatrix m3(m1);

	for (int i=0; i<m1.nb_rows(); i++)
		m3[i]-=m2[i];

	return m3;
}

/** \brief $[m]_1*[m]_2$. */
IntervalMatrix operator*(const IntervalMatrix& m1, const IntervalMatrix& m2) {
	assert(m1.nb_cols()==m2.nb_rows());

	IntervalMatrix m3(m1.nb_rows(),m2.nb_cols());

	for (int i=0; i<m1.nb_rows(); i++) {
		for (int j=0; j<m2.nb_rows(); j++) {
			m3[i][j]=0;
			for (int k=0; k<m1.nb_cols(); k++)
				m3[i][j]+=m1[i][k]*m2[k][j];
		}
	}

	return m3;
}

std::ostream& operator<<(std::ostream& os, const IntervalMatrix& m) {
	os << "(";
	for (int i=0; i<m.nb_rows(); i++) {
		os << "(";
		for (int j=0; j<m.nb_rows(); j++) {
			os << m[i][j];
			if (j<m.nb_rows()-1) os << " ; ";
		}
		os << ")";
	}
	os << ")";
	return os;
}



} // namespace ibex
