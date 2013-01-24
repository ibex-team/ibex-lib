/* ============================================================================
 * I B E X - ibex_Matrix.cpp
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Apr 18, 2012
 * ---------------------------------------------------------------------------- */

#include "ibex_Matrix.h"
#include "ibex_Agenda.h"

namespace ibex {


Matrix::Matrix(int nb_rows1, int nb_cols1) : _nb_rows(nb_rows1), _nb_cols(nb_cols1) {
	assert(nb_rows1>0);
	assert(nb_cols1>0);

	M = new Vector[_nb_rows];
	for (int i=0; i<_nb_rows; i++) {
		M[i].resize(_nb_cols);
	}
}

Matrix::Matrix(int nb_rows1, int nb_cols1, double x) : _nb_rows(nb_rows1), _nb_cols(nb_cols1) {
	assert(nb_rows1>0);
	assert(nb_cols1>0);

	M = new Vector[_nb_rows];
	for (int i=0; i<_nb_rows; i++) {
		M[i].resize(_nb_cols);
		for (int j=0; j<_nb_cols; j++) M[i].vec[j]=x;
	}
}

Matrix::Matrix(int m, int n, double x[]) : _nb_rows(m), _nb_cols(n) {
	assert(m>0);
	assert(n>0);

	int b=0; // counter for "bounds"
	M = new Vector[_nb_rows];
	for (int i=0; i<_nb_rows; i++) {
		M[i].resize(_nb_cols);
		for (int j=0; j<_nb_cols; j++) {
			M[i].vec[j]=x[b];
			b++;
		}
	}
}

Matrix::Matrix(const Matrix& m) : _nb_rows(m.nb_rows()), _nb_cols(m.nb_cols()){
	M = new Vector[_nb_rows];
	for (int i=0; i<_nb_rows; i++) {
		M[i].resize(_nb_cols);
		for (int j=0; j<_nb_cols; j++) M[i].vec[j]=m[i][j];
	}
}

Matrix::~Matrix() {
	delete[] M;
}

Matrix& Matrix::operator=(const Matrix& x) {
	assert(nb_rows()==x.nb_rows() && nb_cols()==x.nb_cols()); // or implement "resize"
	//resize(x.nb_rows(), x.nb_cols());

	for (int i=0; i<nb_rows(); i++)
		(*this)[i]=x[i];
	return *this;
}

bool Matrix::operator==(const Matrix& m) const {
	if (m.nb_rows()!=nb_rows()) return false;
	if (m.nb_cols()!=nb_cols()) return false;

	for (int i=0; i<_nb_rows; i++) {
		if (row(i)!=m.row(i)) return false;
	}
	return true;
}


Vector Matrix::col(int j) const {
	assert(j>=0 && j<nb_cols());

	Vector res(nb_rows());
	for (int i=0; i<nb_rows(); i++)
		res[i]=(*this)[i][j];
	return res;
}

void Matrix::set_col(int col1, const Vector& v) {
	assert(col1>=0 && col1<nb_cols());
	assert(nb_rows()==v.size());

	for (int i=0; i<nb_rows(); i++)
		M[i][col1]=v[i];
}


} // namespace ibex
