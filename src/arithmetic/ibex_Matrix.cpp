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
#include "ibex_TemplateMatrix.h"

namespace ibex {

Matrix::Matrix() : _nb_rows(0), _nb_cols(0), M(NULL) {

}


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

Matrix Matrix::rand(int m, int n) {
	if (n==-1) n=m;

	double* _M=new double[m*n];
	for (int i=0; i<m*n; i++)
		_M[i]=RNG::rand(0,1);

	Matrix M(m,n,_M);

	delete[] _M;
	return M;
}

Matrix Matrix::diag(const Vector& v) {
	Matrix M=Matrix::zeros(v.size());
	for (int i=0; i<v.size(); i++)
		M[i][i]=v[i];
	return M;
}

Matrix& Matrix::operator=(const Matrix& x) {
	resize(x.nb_rows(), x.nb_cols());
	return _assignM(*this,x);
}

bool Matrix::operator==(const Matrix& m) const {
	return _equalsM(*this,m);
}

void Matrix::resize(int nb_rows1, int nb_cols1) {
	assert(nb_rows1>0);
	assert(nb_cols1>0);
	assert((M==NULL && _nb_rows==0) || (_nb_rows!=0 && M!=NULL));

	if (nb_rows1==_nb_rows && nb_cols1==_nb_cols) return;

	Vector* M2;

	if (nb_rows1!=_nb_rows)
		M2 = new Vector[nb_rows1];
	else
		M2 = M;

	for (int i=0; i<nb_rows1; i++) {
		M2[i].resize(nb_cols1);  // filled with 0 by default
		if (i<_nb_rows) {
			int min_cols=nb_cols1<_nb_cols?nb_cols1:_nb_cols;
			for (int j=0; j<min_cols; j++)
				M2[i][j]=M[i][j];
		}
	}

	if (M!=NULL && nb_rows1!=_nb_rows) delete[] M;  // should not happen (default constructor being private)
	M=M2;
	_nb_rows = nb_rows1;
	_nb_cols = nb_cols1;
}

Matrix Matrix::submatrix(int row_start_index, int row_end_index, int col_start_index, int col_end_index) const {
	return _submatrix(*this, row_start_index,row_end_index,col_start_index,col_end_index);
}

void Matrix::put(int row_start_index, int col_start_index, const Matrix& sub) {
	_put<Matrix,Matrix>(*this, row_start_index, col_start_index, sub);
}

void Matrix::put(int row_start_index, int col_start_index, const Vector& sub, bool row_vec) {
	_put<Matrix,Vector>(*this, row_start_index, col_start_index, sub, row_vec);
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

Matrix abs(const Matrix& m) {
	return _absM(m);
}

double infinite_norm(const Matrix& m) {
	return _infinite_normM(m);
}

std::ostream& operator<<(std::ostream& os, const Matrix& m) {
	return _displayM(os,m);
}

} // namespace ibex
