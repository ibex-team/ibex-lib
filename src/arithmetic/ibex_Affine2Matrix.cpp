/* ============================================================================
 * I B E X - ibex_Affine2Matrix.cpp
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Jan 6, 2012
 * ---------------------------------------------------------------------------- */

#include "ibex_Affine2Matrix.h"
#include "ibex_Agenda.h"
#include "ibex_LinearArith.cpp"

namespace ibex {

Affine2Matrix::Affine2Matrix() : _nb_rows(0), _nb_cols(0), _M(NULL) {

}

Affine2Matrix::Affine2Matrix(int nb_rows1, int nb_cols1) : _nb_rows(nb_rows1), _nb_cols(nb_cols1) {
	assert(nb_rows1>0);
	assert(nb_cols1>0);

	_M = new Affine2Vector[_nb_rows];
	for (int i=0; i<_nb_rows; i++) {
		_M[i].resize(_nb_cols);
	}
}

Affine2Matrix::Affine2Matrix(int nb_rows1, int nb_cols1, const Affine2& x) : _nb_rows(nb_rows1), _nb_cols(nb_cols1) {
	assert(nb_rows1>0);
	assert(nb_cols1>0);

	_M = new Affine2Vector[_nb_rows];
	for (int i=0; i<_nb_rows; i++) {
		_M[i].resize(_nb_cols);
		for (int j=0; j<_nb_cols; j++) _M[i]._vec[j]=x;
	}
}

Affine2Matrix::Affine2Matrix(int nb_rows1, int nb_cols1, const Interval& x) : _nb_rows(nb_rows1), _nb_cols(nb_cols1) {
	assert(nb_rows1>0);
	assert(nb_cols1>0);
	int b=0;// counter for "bounds"
	_M = new Affine2Vector[_nb_rows];
	for (int i=0; i<_nb_rows; i++) {
		_M[i].resize(_nb_cols);
		for (int j=0; j<_nb_cols; j++) {
			_M[i]._vec[j]=Affine2(_nb_rows*_nb_cols,b+1,x);
			b++;
		}
	}
}


Affine2Matrix::Affine2Matrix(int m, int n, double bounds[][2]) : _nb_rows(m), _nb_cols(n) {
	assert(m>0);
	assert(n>0);

	int b=0; // counter for "bounds"
	_M = new Affine2Vector[_nb_rows];
	for (int i=0; i<_nb_rows; i++) {
		_M[i].resize(_nb_cols);
		for (int j=0; j<_nb_cols; j++) {
			_M[i]._vec[j]=Affine2(m*n,b+1,bounds[b][0],bounds[b][1]);
			b++;
		}
	}
}

Affine2Matrix::Affine2Matrix(const Affine2Matrix& m) : _nb_rows(m.nb_rows()), _nb_cols(m.nb_cols()){
	_M = new Affine2Vector[_nb_rows];
	for (int i=0; i<_nb_rows; i++) {
		_M[i].resize(_nb_cols);
		for (int j=0; j<_nb_cols; j++) _M[i]._vec[j]=m[i][j];
	}
}


Affine2Matrix::Affine2Matrix(const Affine2Matrix& m, bool b) : _nb_rows(m.nb_rows()), _nb_cols(m.nb_cols()){
	_M = new Affine2Vector[_nb_rows];
	if (b) {
		for (int i=0; i<_nb_rows; i++) {
			_M[i].resize(_nb_cols);
			for (int j=0; j<_nb_cols; j++) _M[i]._vec[j]=(-m[i][j]);
		}
	} else {
		for (int i=0; i<_nb_rows; i++) {
			_M[i].resize(_nb_cols);
			for (int j=0; j<_nb_cols; j++) _M[i]._vec[j]=m[i][j];
		}
	}
}

Affine2Matrix::Affine2Matrix(const IntervalMatrix& m) : _nb_rows(m.nb_rows()), _nb_cols(m.nb_cols()){
	_M = new Affine2Vector[_nb_rows];
	int b=0; // counter for "bounds"
	for (int i=0; i<_nb_rows; i++) {
		_M[i].resize(_nb_cols);
		for (int j=0; j<_nb_cols; j++)  {
			_M[i]._vec[j]=Affine2(_nb_rows*_nb_cols,b+1,m[i][j]);
			b++;
		}
	}
}

Affine2Matrix::Affine2Matrix(const Matrix& m) : _nb_rows(m.nb_rows()), _nb_cols(m.nb_cols()){
	_M = new Affine2Vector[_nb_rows];
	for (int i=0; i<_nb_rows; i++) {
		_M[i].resize(_nb_cols);
		for (int j=0; j<_nb_cols; j++) {
			_M[i]._vec[j]=Affine2(_nb_rows*_nb_cols,m[i][j]);
		}
	}
}

Affine2Matrix::~Affine2Matrix() {
	if (_M!=NULL) delete[] _M;
}

Affine2Matrix& Affine2Matrix::operator=(const Affine2Matrix& x) {
	resize(x.nb_rows(), x.nb_cols());
	for (int i=0; i<nb_rows(); i++)
		(*this)[i]=x[i];
	return *this;
}

Affine2Matrix& Affine2Matrix::operator=(const IntervalMatrix& x) {
	resize(x.nb_rows(), x.nb_cols());
	for (int i=0; i<nb_rows(); i++){
		(*this)[i]=x[i];
	}
	return *this;
}

IntervalMatrix Affine2Matrix::itv() const {
	assert(!is_empty());
	IntervalMatrix intm(nb_rows(),nb_cols());
	for (int i = 0; i < nb_rows(); i++) {
		for (int j =0; j < nb_cols();j++){
			intm[i][j] = (*this)[i][j].itv();
		}
	}
	return intm;
}


void Affine2Matrix::init(const Affine2& x) {
	for (int i=0; i<nb_rows(); i++)
		(*this)[i].init(x);
}

void Affine2Matrix::init(const Interval& x) {
	for (int i=0; i<nb_rows(); i++) {
		for (int j = 0; i < nb_cols(); i++) {
			(*this)[i] = Affine2(nb_cols()*nb_rows(),x);
		}
	}
}

bool Affine2Matrix::operator==(const Affine2Matrix& m) const {
	if (m.nb_rows()!=nb_rows()) return false;
	if (m.nb_cols()!=nb_cols()) return false;

	if (is_empty()) return m.is_empty();
	if (m.is_empty()) return is_empty();

	for (int i=0; i<_nb_rows; i++) {
		if (row(i)!=m.row(i)) return false;
	}
	return true;
}
bool Affine2Matrix::operator==(const IntervalMatrix& m) const {
	if (m.nb_rows()!=nb_rows()) return false;
	if (m.nb_cols()!=nb_cols()) return false;

	if (is_empty()) return m.is_empty();
	if (m.is_empty()) return is_empty();

	for (int i=0; i<_nb_rows; i++) {
		if (row(i)!=m.row(i)) return false;
	}
	return true;
}

Matrix Affine2Matrix::lb() const {
	assert(!is_empty());

	Matrix l(nb_rows(), nb_cols());
	for (int i=0; i<nb_rows(); i++) {
		l[i]=(*this)[i].lb();
	}
	return l;
}

Matrix Affine2Matrix::ub() const {
	assert(!is_empty());

	Matrix u(nb_rows(), nb_cols());
	for (int i=0; i<nb_rows(); i++) {
		u[i]=(*this)[i].ub();
	}
	return u;
}

Matrix Affine2Matrix::mid() const {
	assert(!is_empty());

	Matrix mV(nb_rows(), nb_cols());
	for (int i=0; i<nb_rows(); i++) {
		mV[i]=(*this)[i].mid();
	}
	return mV;
}

Matrix Affine2Matrix::mig() const {
	assert(!is_empty());

	Matrix res(nb_rows(), nb_cols());
	for (int i=0; i<nb_rows(); i++) {
		res[i]=(*this)[i].mig();
	}
	return res;
}

Matrix Affine2Matrix::mag() const {
	assert(!is_empty());

	Matrix res(nb_rows(), nb_cols());
	for (int i=0; i<nb_rows(); i++) {
		res[i]=(*this)[i].mag();
	}
	return res;
}

void Affine2Matrix::resize(int nb_rows1, int nb_cols1) {
	assert(nb_rows1>0);
	assert(nb_cols1>0);
	assert((_M==NULL && _nb_rows==0) || (_nb_rows!=0 && _M!=NULL));

	if (nb_rows1==_nb_rows && nb_cols1==_nb_cols) return;

	Affine2Vector* M2;

	if (nb_rows1!=_nb_rows)
		M2 = new Affine2Vector[nb_rows1];
	else
		M2 = _M;

	for (int i=0; i<nb_rows1; i++) {
		M2[i].resize(nb_cols1);  // ALL_REALS by default
		if (i<_nb_rows) {
			int min_cols=nb_cols1<_nb_cols?nb_cols1:_nb_cols;
			for (int j=0; j<min_cols; j++)
				M2[i][j]=_M[i][j];
		}
	}

	if (_M!=NULL && nb_rows1!=_nb_rows) delete[] _M;

	_M = M2;
	_nb_rows = nb_rows1;
	_nb_cols = nb_cols1;
}

bool Affine2Matrix::is_zero() const {
	for (int i = 0; i < nb_rows(); i++) {
		if (!(*this)[i].is_zero())
			return false;
	}
	return true;
}

Affine2Matrix Affine2Matrix::submatrix(int row_start_index, int row_end_index,
		int col_start_index, int col_end_index) {
	assert(row_start_index >= 0 && row_start_index < nb_rows());
	assert(row_end_index >= 0 && row_end_index < nb_rows());
	assert(row_start_index <= row_end_index);
	assert(col_start_index >= 0 && col_start_index < nb_cols());
	assert(col_end_index >= 0 && col_end_index < nb_cols());
	assert(col_start_index <= col_end_index);
	Affine2Matrix sub(row_end_index - row_start_index + 1,
			col_end_index - col_start_index + 1);
	//cout << "m=" << (row_end_index-row_start_index+1) << "n=" << (col_end_index-col_start_index+1) << endl;
	//cout << sub << endl;
	int i2 = 0;
	for (int i = row_start_index; i <= row_end_index; i++, i2++) {
		int j2 = 0;
		for (int j = col_start_index; j <= col_end_index; j++, j2++)
			sub[i2][j2] = _M[i][j];
	}
	return sub;
}

Affine2Matrix Affine2Matrix::transpose() const {
	Affine2Matrix m(nb_cols(), nb_rows());
	for (int i = 0; i < nb_rows(); i++) {
		for (int j = 0; j < nb_cols(); j++) {
			m[j][i] = (*this)[i][j];
		}
	}

	return m;
}

Affine2Vector Affine2Matrix::col(int j) const {
	assert(j >= 0 && j < nb_cols());
	Affine2Vector res(nb_rows());
	for (int i = 0; i < nb_rows(); i++)
		res[i] = (*this)[i][j];
	return res;
}
void Affine2Matrix::set_col(int col1, const Affine2Vector& v) {
	assert(col1 >= 0 && col1 < nb_cols());
	assert(nb_rows() == v.size());
	for (int i = 0; i < nb_rows(); i++)
		_M[i]._vec[col1] = v[i];
}
Affine2Matrix& Affine2Matrix::operator +=(const Matrix& m) {  //  TODO
}

Affine2Matrix& Affine2Matrix::operator +=(const Affine2Matrix& m) { //  TODO
}

Affine2Matrix& Affine2Matrix::operator +=(const IntervalMatrix& m) { //  TODO
}

Affine2Matrix& Affine2Matrix::operator -=(const Matrix& m) {
}

Affine2Matrix& Affine2Matrix::operator -=(const Affine2Matrix& m) {
}

Affine2Matrix& Affine2Matrix::operator -=(const IntervalMatrix& m) {
}

Affine2Matrix& Affine2Matrix::inflate(double rad) {
	// see comment in Affine2Vector::inflate
	if (is_empty())
		return *this;

	Interval r(-rad, rad);
	for (int i = 0; i < nb_rows(); i++)
		for (int j = 0; j < nb_cols(); j++)
			(*this)[i][j] += r;
	return *this;
}
bool proj_add(const Affine2Matrix& y, Affine2Matrix& x1, Affine2Matrix& x2) {
	for (int i = 0; i < y.nb_rows(); i++)
		if (!proj_add(y[i], x1[i], x2[i])) {
			x1.set_empty();
			x2.set_empty();
			return false;
		}
	return true;
}

bool proj_sub(const Affine2Matrix& y, Affine2Matrix& x1, Affine2Matrix& x2) {
	for (int i = 0; i < y.nb_rows(); i++)
		if (!proj_sub(y[i], x1[i], x2[i])) {
			x1.set_empty();
			x2.set_empty();
			return false;
		}
	return true;
}

bool proj_mul(const Affine2Matrix& y, Affine2& x1, Affine2Matrix& x2) {
	int n = (y.nb_rows());
	assert((x2.nb_rows()) == n && (x2.nb_cols()) == (y.nb_cols()));
	for (int i = 0; i < n; i++) {
		if (!proj_mul(y[i], x1, x2[i])) {
			x2.set_empty();
			return false;
		}
	}

	return true;
}

bool proj_mul(const Affine2Vector& y, Affine2Matrix& x1, Affine2Vector& x2,
		double ratio) {
	assert(x1.nb_rows() == y.size());
	assert(x1.nb_cols() == x2.size());
	int last_row = 0;
	int i = 0;
	int n = y.size();
	do {
		Affine2Vector x2old = x2;
		if (!proj_mul(y[i], x1[i], x2)) {
			x1.set_empty();
			return false;
		}
		if (x2old.rel_distance(x2) > ratio)
			last_row = i;

		i = (i + 1) % n;
	} while (i != last_row);
	return true;
}

bool proj_mul(const Affine2Vector& y, Affine2Vector& x1, Affine2Matrix& x2,
		double ratio) {
	Affine2Matrix x2t = x2.transpose();
	bool res = proj_mul(y, x2t, x1, ratio);
	x2 = x2t.transpose();
	return res;
}

bool proj_mul(const Affine2Matrix& y, Affine2Matrix& x1, Affine2Matrix& x2,
		double ratio) {
	int m = y.nb_rows();
	int n = y.nb_cols();
	assert(x1.nb_cols() == x2.nb_rows());
	assert(x1.nb_rows() == m);
	assert(x2.nb_cols() == n);
	// each coefficient (i,j) of y is considered as a binary "dot product" constraint
	// between the ith row of x1 and the jth column of x2
	// (advantage: we have exact projection for the dot product)
	//
	// we propagate these constraints using a simple agenda.
	Agenda a(m * n);
	//init
	for (int i = 0; i < m; i++)
		for (int j = 0; j < n; j++)
			a.push(i * n + j);
	int k;
	while (!a.empty()) {
		a.pop(k);
		int i = k / n;
		int j = k % n;
		Affine2Vector x1old = x1[i];
		Affine2Vector x2j = x2.col(j);
		Affine2Vector x2old = x2j;
		if (!proj_mul(y[i][j], x1[i], x2j)) {
			x1.set_empty();
			x2.set_empty();
			return false;
		} else {
			if (x1old.rel_distance(x1[i]) >= ratio) {
				for (int j2 = 0; j2 < n; j2++)
					if (j2 != j)
						a.push(i * n + j2);
			}
			if (x2old.rel_distance(x2j) >= ratio) {
				for (int i2 = 0; i2 < m; i2++)
					if (i2 != i)
						a.push(i2 * n + j);
			}
			x2.set_col(j, x2j);
		}

	}

	return true;
} //  TODO

IntervalMatrix operator &(const Affine2Matrix& x,
		const Affine2Matrix& y) const {
}

IntervalMatrix operator &(const Affine2Matrix& x,
		const IntervalMatrix& y) const { //  TODO
}

IntervalMatrix operator |(const Affine2Matrix& x,
		const Affine2Matrix& y) const {
} //  TODO

IntervalMatrix operator |(const Affine2Matrix& x,
		const IntervalMatrix& y) const {
}
//  TODO
Affine2Matrix operator -(const Affine2Matrix& m) {
}

Affine2Matrix operator +(const Affine2Matrix& m1, const Matrix& m2) {
}

Affine2Matrix operator +(const Matrix& m1, const Affine2Matrix& m2) {
}

Affine2Matrix operator -(const Affine2Matrix& m1, const Matrix& m2) {
}

Affine2Matrix operator *(const Affine2& x, const Matrix& m) {
}

Affine2Matrix operator *(const Affine2& x, const IntervalMatrix& m) {
}

Affine2Vector operator *(const Affine2Matrix& m, const Vector& x) {
}

Affine2Vector operator *(const Matrix& m, const Affine2Vector& x) {
}

Affine2Vector operator *(const Affine2Matrix& m, const Affine2Vector& x) {
}
//  TODO
Affine2Vector operator *(const Affine2Matrix& m, const IntervalVector& x) {
}

Affine2Vector operator *(const IntervalMatrix& m, const Affine2Vector& x) {
}

Affine2Vector operator *(const Vector& x, const Affine2Matrix& m) {
}

Affine2Vector operator *(const Affine2Vector& x, const Matrix& m) {
}

Affine2Vector operator *(const Affine2Vector& x, const Affine2Matrix& m) {
}

Affine2Vector operator *(const Affine2Vector& x, const IntervalMatrix& m) {
}

Affine2Vector operator *(const IntervalVector& x, const Affine2Matrix& m) {
}

Affine2Matrix operator *(const Affine2Matrix& m1, const Matrix& m2) {
}

Affine2Matrix operator *(const Matrix& m1, const Affine2Matrix& m2) {
}
//  TODO
Affine2Matrix operator *(const Affine2Matrix& m1, const Affine2Matrix& m2) {
}

Affine2Matrix operator *(const Affine2Matrix& m1, const IntervalMatrix& m2) {
}

Affine2Matrix operator *(const IntervalMatrix& m1, const Affine2Matrix& m2) {
}

Affine2Matrix outer_product(const Affine2Vector& x1, const Vector& x2) {
}

Affine2Matrix outer_product(const Vector& x1, const Affine2Vector& x2) {
}

Affine2Matrix outer_product(const Affine2Vector& x1, const Affine2Vector& x2) {
}

Affine2Matrix outer_product(const Affine2Vector& x1, const IntervalVector& x2) {
}

Affine2Matrix outer_product(const IntervalVector& x1, const Affine2Vector& x2) {
}

Affine2Matrix abs(const Affine2Matrix& m) {
}
//  TODO
bool proj_add(const IntervalMatrix& y, Affine2Matrix& x1, Affine2Matrix& x2) {
}

bool proj_add(const IntervalMatrix& y, Affine2Matrix& x1, IntervalMatrix& x2) {
}

bool proj_sub(const IntervalMatrix& y, Affine2Matrix& x1, Affine2Matrix& x2) {
}

bool proj_sub(const Affine2Matrix& y, IntervalMatrix& x1, Affine2Matrix& x2) {
}

bool proj_sub(const IntervalMatrix& y, IntervalMatrix& x1, Affine2Matrix& x2) {
}

bool proj_sub(const IntervalMatrix& y, Affine2Matrix& x1, IntervalMatrix& x2) {
}

bool proj_mul(const IntervalMatrix& y, Affine2& x1, Affine2Matrix& x2) {
}

bool proj_mul(const IntervalMatrix& y, Interval& x1, Affine2Matrix& x2) {
}

bool proj_mul(const IntervalMatrix& y, Affine2& x1, IntervalMatrix& x2) {
}

bool proj_mul(const IntervalVector& y, Affine2Matrix& x1, Affine2Vector& x2, //  TODO
		double ratio) {
}

bool proj_mul(const IntervalVector& y, IntervalMatrix& x1, Affine2Vector& x2,
		double ratio) {
}

bool proj_mul(const IntervalVector& y, Affine2Matrix& x1, IntervalVector& x2,
		double ratio) {
}

bool proj_mul(const IntervalVector& y, Affine2Vector& x1, Affine2Matrix& x2,
		double ratio) {
}

bool proj_mul(const IntervalVector& y, IntervalVector& x1, Affine2Matrix& x2,
		double ratio) {
}

bool proj_mul(const IntervalVector& y, Affine2Vector& x1, IntervalMatrix& x2,
		double ratio) {
}

bool proj_mul(const IntervalMatrix& y, Affine2Matrix& x1, Affine2Matrix& x2,
		double ratio) {
}

bool proj_mul(const IntervalMatrix& y, IntervalMatrix& x1, Affine2Matrix& x2,
		double ratio) {
}

bool proj_mul(const IntervalMatrix& y, Affine2Matrix& x1, IntervalMatrix& x2, //  TODO
		double ratio) {
}

std::ostream& operator <<(std::ostream& os, const Affine2Matrix& m) {
	return print < Affine2Matrix > (os, m);
}

Affine2Matrix& Affine2Matrix::operator *=(double x) {
}

Affine2Matrix& Affine2Matrix::operator *=(const Affine2& x) {
}
//  TODO
Affine2Matrix& Affine2Matrix::operator *=(const Interval& x) {
}

Affine2Matrix& Affine2Matrix::operator *=(const Matrix& m) {
}

Affine2Matrix& Affine2Matrix::operator *=(const Affine2Matrix& m) {
}

Affine2Matrix& Affine2Matrix::operator *=(const IntervalMatrix& m) {
}

} // namespace ibex
