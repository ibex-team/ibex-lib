/* ============================================================================
 * I B E X - ibex_Affine2Matrix.cpp
 * ============================================================================
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Jordan Ninin
 * Created     : March 16, 2013
 * ---------------------------------------------------------------------------- */

#include "ibex_Affine2Matrix.h"
#include "ibex_Agenda.h"
#include "ibex_LinearArith.cpp"

namespace ibex {

IntervalMatrix& IntervalMatrix::operator=(const Affine2Matrix& x) {
	resize(x.nb_rows(), x.nb_cols());
	for (int i=0; i<nb_rows(); i++)
		(*this)[i] = x[i];
	return *this;
}

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

Affine2Matrix::Affine2Matrix(int nb_rows1, int nb_cols1, int sizeAF2) : _nb_rows(nb_rows1), _nb_cols(nb_cols1) {
	assert(nb_rows1>0);
	assert(nb_cols1>0);
	int b=0;// counter for "bounds"
	_M = new Affine2Vector[_nb_rows];
	for (int i=0; i<_nb_rows; i++) {
		_M[i].resize(_nb_cols);
		for (int j=0; j<_nb_cols; j++) {
			_M[i]._vec[j]=Affine2(sizeAF2);
			b++;
		}
	}
}

/*
Affine2Matrix::Affine2Matrix(int m, int n, double bounds[][2]) : _nb_rows(m), _nb_cols(n) {
	assert(m>0);
	assert(n>0);

	int b=0; // counter for "bounds"
	_M = new Affine2Vector[_nb_rows];
	for (int i=0; i<_nb_rows; i++) {
		_M[i].resize(_nb_cols);
		for (int j=0; j<_nb_cols; j++) {
			_M[i]._vec[j]=Affine2(m*n,b+1,bounds[b][0],bounds[b][1]);  <- I'm not sur
			b++;
		}
	}
}
*/

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
			for (int j=0; j<_nb_cols; j++) _M[i]._vec[j]=Affine2(m[i][j],true);
		}
	} else {
		for (int i=0; i<_nb_rows; i++) {
			_M[i].resize(_nb_cols);
			for (int j=0; j<_nb_cols; j++) _M[i]._vec[j]=m[i][j];
		}
	}
}

/*  It is too difficult to know the size of each AF2. So we let the user do what exactly he want to do.
Affine2Matrix::Affine2Matrix(const IntervalMatrix& m, int sizeAF2) : _nb_rows(m.nb_rows()), _nb_cols(m.nb_cols()){
	_M = new Affine2Vector[_nb_rows];
	int b=0; // counter for "bounds"
	for (int i=0; i<_nb_rows; i++) {
		_M[i].resize(_nb_cols);
		for (int j=0; j<_nb_cols; j++)  {
			_M[i]._vec[j]=Affine2(sizeAF2,b+1,m[i][j]);
			b++;
		}
	}
}
*/

Affine2Matrix::Affine2Matrix(const Matrix& m, int sizeAF2) : _nb_rows(m.nb_rows()), _nb_cols(m.nb_cols()){
	_M = new Affine2Vector[_nb_rows];
	for (int i=0; i<_nb_rows; i++) {
		_M[i].resize(_nb_cols);
		for (int j=0; j<_nb_cols; j++) {
			_M[i]._vec[j]=Affine2(sizeAF2,m[i][j]);
		}
	}
}

Affine2Matrix::~Affine2Matrix() {
	if (_M!=NULL) delete[] _M;
}

Affine2Matrix& Affine2Matrix::operator=(const Affine2Matrix& x) {
	resize(x.nb_rows(), x.nb_cols());
	for (int i=0; i<nb_rows(); i++)
		(*this)[i] = x[i];
	return *this;
}

Affine2Matrix& Affine2Matrix::operator=(const IntervalMatrix& x) {
	resize(x.nb_rows(), x.nb_cols());
	for (int i=0; i<nb_rows(); i++){
		(*this)[i] = x[i];
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
			(*this)[i][j] = x;
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

Affine2Matrix Affine2Matrix::submatrix(int row_start_index, int row_end_index,	int col_start_index, int col_end_index) {
	assert(row_start_index >= 0 && row_start_index < nb_rows());
	assert(row_end_index >= 0 && row_end_index < nb_rows());
	assert(row_start_index <= row_end_index);
	assert(col_start_index >= 0 && col_start_index < nb_cols());
	assert(col_end_index >= 0 && col_end_index < nb_cols());
	assert(col_start_index <= col_end_index);

	Affine2Matrix sub(row_end_index - row_start_index + 1,	col_end_index - col_start_index + 1);
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
Affine2Matrix& Affine2Matrix::operator+=(const Matrix& m) {
	return set_addM<Affine2Matrix,Matrix>(*this,m);
}

Affine2Matrix& Affine2Matrix::operator+=(const Affine2Matrix& m) {
	return set_addM<Affine2Matrix,Affine2Matrix>(*this,m);
}

Affine2Matrix& Affine2Matrix::operator+=(const IntervalMatrix& m) {
	return set_addM<Affine2Matrix,IntervalMatrix>(*this,m);
}

Affine2Matrix& Affine2Matrix::operator-=(const Matrix& m) {
	return set_subM<Affine2Matrix,Matrix>(*this,m);
}

Affine2Matrix& Affine2Matrix::operator-=(const Affine2Matrix& m) {
	return set_subM<Affine2Matrix,Affine2Matrix>(*this,m);
}

Affine2Matrix& Affine2Matrix::operator-=(const IntervalMatrix& m) {
	return set_subM<Affine2Matrix,IntervalMatrix>(*this,m);
}

Affine2Matrix& Affine2Matrix::operator*=(double x) {
	return set_mulSM<double,Affine2Matrix>(x,*this);
}

Affine2Matrix& Affine2Matrix::operator*=(const Affine2& x) {
	return set_mulSM<Affine2,Affine2Matrix>(x,*this);
}

Affine2Matrix& Affine2Matrix::operator*=(const Interval& x) {
	return set_mulSM<Interval,Affine2Matrix>(x,*this);
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

bool proj_mul(const Affine2Vector& y, Affine2Matrix& x1, Affine2Vector& x2,double ratio) {
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

bool proj_mul(const Affine2Vector& y, Affine2Vector& x1, Affine2Matrix& x2,double ratio) {
	Affine2Matrix x2t = x2.transpose();
	bool res = proj_mul(y, x2t, x1, ratio);
	x2 = x2t.transpose();
	return res;
}

bool proj_mul(const Affine2Matrix& y, Affine2Matrix& x1, Affine2Matrix& x2,double ratio) {
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
}

IntervalMatrix operator&(const Affine2Matrix& x, const Affine2Matrix& y) const {
	assert(x.nb_rows()==y.nb_rows());
	assert(x.nb_cols()==y.nb_cols());

	if (y.is_empty()||x.is_empty())
		return Affine2Matrix::empty(x.nb_rows(),x.nb_cols());

	Affine2Matrix res(x.nb_rows(),x.nb_cols());
	for (int i=0; i<x.nb_rows(); i++) {
		res [i] = x[i] & y[i];
		if (res[i].is_empty()) {
			res.set_empty();
			return res;
		}
	}
	return res;
}

IntervalMatrix operator&(const Affine2Matrix& x, const IntervalMatrix& y) const {
	assert(x.nb_rows()==y.nb_rows());
	assert(x.nb_cols()==y.nb_cols());

	if (y.is_empty()||x.is_empty())
		return Affine2Matrix::empty(x.nb_rows(),x.nb_cols());

	Affine2Matrix res(x.nb_rows(),x.nb_cols());
	for (int i=0; i<x.nb_rows(); i++) {
		res [i] = x[i] & y[i];
		if (res[i].is_empty()) {
			res.set_empty();
			return res;
		}
	}
	return res;
}

IntervalMatrix operator|(const Affine2Matrix& x,	const Affine2Matrix& y) const {
	assert(x.nb_rows()==y.nb_rows());
	assert(x.nb_cols()==y.nb_cols());

	if (y.is_empty()&&x.is_empty())
		return Affine2Matrix::empty(x.nb_rows(),x.nb_cols());

	Affine2Matrix res(x.nb_rows(),x.nb_cols());
	for (int i=0; i<x.nb_rows(); i++) {
		res [i] = x[i] | y[i];
		if (res[i].is_empty()) {
			res.set_empty();
			return res;
		}
	}
	return res;
}

IntervalMatrix operator|(const Affine2Matrix& x,	const IntervalMatrix& y) const {
	assert(x.nb_rows()==y.nb_rows());
	assert(x.nb_cols()==y.nb_cols());

	if (y.is_empty()&&x.is_empty())
		return Affine2Matrix::empty(x.nb_rows(),x.nb_cols());

	Affine2Matrix res(x.nb_rows(),x.nb_cols());
	for (int i=0; i<x.nb_rows(); i++) {
		res [i] = x[i] | y[i];
		if (res[i].is_empty()) {
			res.set_empty();
			return res;
		}
	}
	return res;
}


Affine2Matrix operator*(const Affine2& x, const Matrix& m) {
	if (x.is_empty()) {
		return Affine2Matrix::empty(m.nb_rows(),m.nb_cols());
	}
	Affine2Matrix  res(m.nb_rows(),m.nb_cols(),x);
	for (int i=0; i<m.nb_rows(); i++) {
		for (int j= 0;j<m.nb_cols();j++){
		res[i][j] *= m[i][j];
		}
	}
	return res;
}

Affine2Matrix operator*(const Affine2& x, const IntervalMatrix& m) {
	if (x.is_empty()||m.is_empty()) {
		return Affine2Matrix::empty(m.nb_rows(),m.nb_cols());
	}
	Affine2Matrix  res(m.nb_rows(),m.nb_cols(),x);
	for (int i=0; i<m.nb_rows(); i++) {
		for (int j= 0;j<m.nb_cols();j++){
		res[i][j] *= m[i][j];
		}
	}
	return res;
}
//  TODO to check what if really this sub function
Affine2Vector operator*(const Affine2Matrix& m, const Vector& x) {
	return mulMV<Affine2Matrix,Vector,Affine2Vector>(m,x);
}

Affine2Vector operator*(const Matrix& m, const Affine2Vector& x) {
	return mulMV<Matrix,Affine2Vector,Affine2Vector>(m,x);
}

Affine2Vector operator*(const Affine2Matrix& m, const Affine2Vector& x) {
	return mulMV<Affine2Matrix,Affine2Vector,Affine2Vector>(m,x);
}

Affine2Vector operator*(const Affine2Matrix& m, const IntervalVector& x) {
	return mulMV<Affine2Matrix,IntervalVector,Affine2Vector>(m,x);
}

Affine2Vector operator*(const IntervalMatrix& m, const Affine2Vector& x) {
	return mulMV<IntervalMatrix,Affine2Vector,Affine2Vector>(m,x);
}

Affine2Vector operator*(const Vector& x, const Affine2Matrix& m) {
	return mulVM<Vector,Affine2Matrix,Affine2Vector>(x,m);
}

Affine2Vector operator*(const Affine2Vector& x, const Matrix& m) {
	return mulVM<Affine2Vector,Matrix,Affine2Vector>(x,m);
}

Affine2Vector operator*(const Affine2Vector& x, const Affine2Matrix& m) {
	return mulVM<Affine2Vector,Affine2Matrix,Affine2Vector>(x,m);
}

Affine2Vector operator*(const Affine2Vector& x, const IntervalMatrix& m) {
	return mulVM<Affine2Vector,IntervalMatrix,Affine2Vector>(x,m);
}

Affine2Vector operator*(const IntervalVector& x, const Affine2Matrix& m) {
	return mulVM<IntervalVector,Affine2Matrix,Affine2Vector>(x,m);
}

Affine2Matrix operator*(const Affine2Matrix& m1, const Matrix& m2) {
	return mulMM<Affine2Matrix,Matrix,Affine2Matrix>(m1,m2);
}

Affine2Matrix operator*(const Matrix& m1, const Affine2Matrix& m2) {
	return mulMM<Matrix,Affine2Matrix,Affine2Matrix>(m1,m2);
}

Affine2Matrix operator*(const Affine2Matrix& m1, const Affine2Matrix& m2) {
	return mulMM<Affine2Matrix,Affine2Matrix,Affine2Matrix>(m1,m2);
}

Affine2Matrix operator*(const Affine2Matrix& m1, const IntervalMatrix& m2) {
	return mulMM<Affine2Matrix,IntervalMatrix,Affine2Matrix>(m1,m2);
}

Affine2Matrix operator*(const IntervalMatrix& m1, const Affine2Matrix& m2) {
	return mulMM<IntervalMatrix,Affine2Matrix,Affine2Matrix>(m1,m2);
}


Affine2Matrix outer_product(const Affine2Vector& x1, const Vector& x2) {
	return outer_prod<Affine2Vector,Vector,Affine2Matrix>(x1,x2);
}

Affine2Matrix outer_product(const Vector& x1, const Affine2Vector& x2) {
	return outer_prod<Vector,Affine2Vector,Affine2Matrix>(x1,x2);
}

Affine2Matrix outer_product(const Affine2Vector& x1, const Affine2Vector& x2) {
	return outer_prod<Affine2Vector,Affine2Vector,Affine2Matrix>(x1,x2);
}

Affine2Matrix outer_product(const Affine2Vector& x1, const IntervalVector& x2) {
	return outer_prod<Affine2Vector,IntervalVector,Affine2Matrix>(x1,x2);
}

Affine2Matrix outer_product(const IntervalVector& x1, const Affine2Vector& x2) {
	return outer_prod<IntervalVector,Affine2Vector,Affine2Matrix>(x1,x2);
}

Affine2Matrix abs(const Affine2Matrix& m) {
	return absM(m);
}

//  TODO

bool proj_add(const IntervalMatrix& y, Affine2Matrix& x1, Affine2Matrix& x2) {
	for (int i=0; i<y.nb_rows(); i++)
		if (!proj_add(y[i], x1[i], x2[i])) {
			x1.set_empty();
			x2.set_empty();
			return false;
		}
	return true;
}
bool proj_add(const IntervalMatrix& y, Affine2Matrix& x1, IntervalMatrix& x2) {
	for (int i=0; i<y.nb_rows(); i++)
		if (!proj_add(y[i], x1[i], x2[i])) {
			x1.set_empty();
			x2.set_empty();
			return false;
		}
	return true;
}

bool proj_sub(const IntervalMatrix& y, Affine2Matrix& x1, Affine2Matrix& x2) {
	for (int i=0; i<y.nb_rows(); i++)
		if (!proj_sub(y[i], x1[i], x2[i])) {
			x1.set_empty();
			x2.set_empty();
			return false;
		}
	return true;
}
bool proj_sub(const Affine2Matrix& y, IntervalMatrix& x1, Affine2Matrix& x2) {
	for (int i=0; i<y.nb_rows(); i++)
		for (int j=0; j<y.nb_cols(); j++){
			if (!proj_sub(y[i][j].itv(), x1[i][j], x2[i][j])) {
				x1.set_empty();
				x2.set_empty();
				return false;
			}
		}
	return true;
}
bool proj_sub(const Affine2Matrix& y, Affine2Matrix& x1, IntervalMatrix& x2) {
	for (int i=0; i<y.nb_rows(); i++)
		for (int j=0; j<y.nb_cols(); j++){
			if (!proj_sub(y[i][j].itv(), x1[i][j], x2[i][j])) {
				x1.set_empty();
				x2.set_empty();
				return false;
			}
		}
	return true;
}
bool proj_sub(const IntervalMatrix& y, IntervalMatrix& x1, Affine2Matrix& x2) {
	for (int i=0; i<y.nb_rows(); i++)
		if (!proj_sub(y[i], x1[i], x2[i])) {
			x1.set_empty();
			x2.set_empty();
			return false;
		}
	return true;
}
bool proj_sub(const IntervalMatrix& y, Affine2Matrix& x1, IntervalMatrix& x2) {
	for (int i=0; i<y.nb_rows(); i++)
		if (!proj_sub(y[i], x1[i], x2[i])) {
			x1.set_empty();
			x2.set_empty();
			return false;
		}
	return true;
}

bool proj_mul(const IntervalMatrix& y, Affine2& x1, Affine2Matrix& x2) {
	int n=(y.nb_rows());
	assert((x2.nb_rows())==n && (x2.nb_cols())==(y.nb_cols()));

	for (int i=0; i<n; i++) {
		if (!proj_mul(y[i],x1,x2[i])) {
			x2.set_empty();
			return false;
		}
	}
	return true;
}
bool proj_mul(const IntervalMatrix& y, Interval& x1, Affine2Matrix& x2) {
	int n=(y.nb_rows());
	assert((x2.nb_rows())==n && (x2.nb_cols())==(y.nb_cols()));

	for (int i=0; i<n; i++) {
		if (!proj_mul(y[i],x1,x2[i])) {
			x2.set_empty();
			return false;
		}
	}
	return true;
}
bool proj_mul(const IntervalMatrix& y, Affine2& x1, IntervalMatrix& x2) {
	int n=(y.nb_rows());
	assert((x2.nb_rows())==n && (x2.nb_cols())==(y.nb_cols()));

	for (int i=0; i<n; i++) {
		if (!proj_mul(y[i],x1,x2[i])) {
			x2.set_empty();
			return false;
		}
	}
	return true;
}


//  TODO
bool proj_mul(const IntervalVector& y, Affine2Matrix& x1, Affine2Vector& x2, 	double ratio) {
	assert(x1.nb_rows()==y.size());
	assert(x1.nb_cols()==x2.size());

	int last_row=0;
	int i=0;
	int n=y.size();

	do {
		IntervalVector x2old=x2;
		if (!proj_mul(y[i],x1[i],x2)) {
			x1.set_empty();
			return false;
		}
		if (x2old.rel_distance(x2.itv())>ratio) last_row=i;
		i=(i+1)%n;
	} while(i!=last_row);

	return true;
}

bool proj_mul(const IntervalVector& y, IntervalMatrix& x1, Affine2Vector& x2,	double ratio) {
	assert(x1.nb_rows()==y.size());
	assert(x1.nb_cols()==x2.size());

	int last_row=0;
	int i=0;
	int n=y.size();

	do {
		IntervalVector x2old=x2;
		if (!proj_mul(y[i],x1[i],x2)) {
			x1.set_empty();
			return false;
		}
		if (x2old.rel_distance(x2.itv())>ratio) last_row=i;
		i=(i+1)%n;
	} while(i!=last_row);

	return true;
}

bool proj_mul(const IntervalVector& y, Affine2Matrix& x1, IntervalVector& x2,	double ratio) {
	assert(x1.nb_rows()==y.size());
	assert(x1.nb_cols()==x2.size());

	int last_row=0;
	int i=0;
	int n=y.size();

	do {
		IntervalVector x2old=x2;
		if (!proj_mul(y[i],x1[i],x2)) {
			x1.set_empty();
			return false;
		}
		if (x2old.rel_distance(x2)>ratio) last_row=i;
		i=(i+1)%n;
	} while(i!=last_row);

	return true;
}

bool proj_mul(const IntervalVector& y, Affine2Vector& x1, Affine2Matrix& x2,	double ratio) {
	IntervalMatrix x2t=x2.transpose();

	bool res=proj_mul(y,x2t,x1,ratio);

	x2=x2t.transpose();
	return res;
}

bool proj_mul(const IntervalVector& y, IntervalVector& x1, Affine2Matrix& x2,	double ratio) {
	IntervalMatrix x2t=x2.transpose();

	bool res=proj_mul(y,x2t,x1,ratio);

	x2=x2t.transpose();
	return res;
}

bool proj_mul(const IntervalVector& y, Affine2Vector& x1, IntervalMatrix& x2,	double ratio) {
	IntervalMatrix x2t=x2.transpose();

	bool res=proj_mul(y,x2t,x1,ratio);

	x2=x2t.transpose();
	return res;
}


//  TODO to check
bool proj_mul(const IntervalMatrix& y, Affine2Matrix& x1, Affine2Matrix& x2,	double ratio) {
	int m=y.nb_rows();
	int n=y.nb_cols();
	assert(x1.nb_cols()==x2.nb_rows());
	assert(x1.nb_rows()==m);
	assert(x2.nb_cols()==n);

	// each coefficient (i,j) of y is considered as a binary "dot product" constraint
	// between the ith row of x1 and the jth column of x2
	// (advantage: we have exact projection for the dot product)
	//
	// we propagate these constraints using a simple agenda.
	Agenda a(m*n);

	//init
	for (int i=0; i<m; i++)
		for (int j=0; j<n; j++)
			a.push(i*n+j);

	int k;
	while (!a.empty()) {
		a.pop(k);
		int i=k/n;
		int j=k%n;
		IntervalVector x1old=x1[i];
		IntervalVector x2j=x2.col(j);
		IntervalVector x2old=x2j;
		if (!proj_mul(y[i][j],x1[i],x2j)) {
			x1.set_empty();
			x2.set_empty();
			return false;
		} else {
			if (x1old.rel_distance(x1[i].itv())>=ratio) {
				for (int j2=0; j2<n; j2++)
					if (j2!=j) a.push(i*n+j2);
			}
			if (x2old.rel_distance(x2j)>=ratio) {
				for (int i2=0; i2<m; i2++)
					if (i2!=i) a.push(i2*n+j);
			}
			x2.set_col(j,x2j);
		}
	}
	return true;
}
bool proj_mul(const IntervalMatrix& y, IntervalMatrix& x1, Affine2Matrix& x2,	double ratio) {
	int m=y.nb_rows();
	int n=y.nb_cols();
	assert(x1.nb_cols()==x2.nb_rows());
	assert(x1.nb_rows()==m);
	assert(x2.nb_cols()==n);

	// each coefficient (i,j) of y is considered as a binary "dot product" constraint
	// between the ith row of x1 and the jth column of x2
	// (advantage: we have exact projection for the dot product)
	//
	// we propagate these constraints using a simple agenda.
	Agenda a(m*n);

	//init
	for (int i=0; i<m; i++)
		for (int j=0; j<n; j++)
			a.push(i*n+j);

	int k;
	while (!a.empty()) {
		a.pop(k);
		int i=k/n;
		int j=k%n;
		IntervalVector x1old=x1[i];
		IntervalVector x2j=x2.col(j);
		IntervalVector x2old=x2j;
		if (!proj_mul(y[i][j],x1[i],x2j)) {
			x1.set_empty();
			x2.set_empty();
			return false;
		} else {
			if (x1old.rel_distance(x1[i])>=ratio) {
				for (int j2=0; j2<n; j2++)
					if (j2!=j) a.push(i*n+j2);
			}
			if (x2old.rel_distance(x2j)>=ratio) {
				for (int i2=0; i2<m; i2++)
					if (i2!=i) a.push(i2*n+j);
			}
			x2.set_col(j,x2j);
		}
	}
	return true;
}
bool proj_mul(const IntervalMatrix& y, Affine2Matrix& x1, IntervalMatrix& x2,	double ratio) {
	int m=y.nb_rows();
	int n=y.nb_cols();
	assert(x1.nb_cols()==x2.nb_rows());
	assert(x1.nb_rows()==m);
	assert(x2.nb_cols()==n);

	// each coefficient (i,j) of y is considered as a binary "dot product" constraint
	// between the ith row of x1 and the jth column of x2
	// (advantage: we have exact projection for the dot product)
	//
	// we propagate these constraints using a simple agenda.
	Agenda a(m*n);

	//init
	for (int i=0; i<m; i++)
		for (int j=0; j<n; j++)
			a.push(i*n+j);

	int k;
	while (!a.empty()) {
		a.pop(k);
		int i=k/n;
		int j=k%n;
		IntervalVector x1old=x1[i];
		IntervalVector x2j=x2.col(j);
		IntervalVector x2old=x2j;
		if (!proj_mul(y[i][j],x1[i],x2j)) {
			x1.set_empty();
			x2.set_empty();
			return false;
		} else {
			if (x1old.rel_distance(x1[i].itv())>=ratio) {
				for (int j2=0; j2<n; j2++)
					if (j2!=j) a.push(i*n+j2);
			}
			if (x2old.rel_distance(x2j)>=ratio) {
				for (int i2=0; i2<m; i2++)
					if (i2!=i) a.push(i2*n+j);
			}
			x2.set_col(j,x2j);
		}
	}
	return true;
}

std::ostream& operator<<(std::ostream& os, const Affine2Matrix& m) {
	return print < Affine2Matrix > (os, m);
}


} // namespace ibex
