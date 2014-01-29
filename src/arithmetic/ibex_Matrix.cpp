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
#include "ibex_TemplateMatrix.cpp_"

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
	return _assign(*this,x);
}

bool Matrix::operator==(const Matrix& m) const {
	return _equals(*this,m);
}

Matrix Matrix::submatrix(int row_start_index, int row_end_index, int col_start_index, int col_end_index) const {
	return _submatrix(*this,row_start_index,row_end_index,col_start_index,col_end_index);
}

void Matrix::put(int row_start_index, int col_start_index, const Matrix& sub) {
	_put(*this,row_start_index, col_start_index, sub);
}

Matrix Matrix::transpose() const {
	return _transpose(*this);
}

Vector Matrix::col(int j) const {
	return _col<Matrix,Vector>(*this,j);
}

void Matrix::set_col(int col1, const Vector& v) {
	_set_col(*this,col1,v);
}

std::ostream& operator<<(std::ostream& os, const Matrix& m) {
	return display(os,m);
}



} // namespace ibex
