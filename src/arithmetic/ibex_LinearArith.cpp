//============================================================================
//                                  I B E X                                   
// File        : ibex_LinearArith.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Apr 18, 2012
// Last Update : Apr 18, 2012
//============================================================================


#include "ibex_Affine2Matrix.h"
#include "ibex_IntervalMatrix.h"

namespace ibex {

namespace {

// the following functions are
// introduced to allow genericity
inline bool is_empty(double x)                { return false; }
inline bool is_empty(const Interval& x)       { return x.is_empty(); }
inline bool is_empty(const Vector& v)         { return false; }
inline bool is_empty(const IntervalVector& v) { return v.is_empty(); }
inline bool is_empty(const Matrix& m)         { return false; }
inline bool is_empty(const IntervalMatrix& m) { return m.is_empty(); }
inline bool is_empty(const Affine2& x)       { return x.is_empty(); }
inline bool is_empty(const Affine2Vector& v) { return v.is_empty(); }
inline bool is_empty(const Affine2Matrix& m) { return m.is_empty(); }



inline void set_empty(double x)          { }
inline void set_empty(Interval& x)       { x.set_empty(); }
inline void set_empty(Vector& v)         { }
inline void set_empty(IntervalVector& v) { v.set_empty(); }
inline void set_empty(Matrix& m)         { }
inline void set_empty(IntervalMatrix& m) { m.set_empty(); }
inline void set_empty(Affine2& x)       { x.set_empty(); }
inline void set_empty(Affine2Vector& v) { v.set_empty(); }
inline void set_empty(Affine2Matrix& m) { m.set_empty(); }

inline double abs(const double& x) { return fabs(x); }

template<typename V>
inline V minusV(const V& v) {
	const int n=v.size();

	V y(n);

	if (is_empty(v)) { set_empty(y); return y; }

	for (int i=0; i<n; i++) {
		y[i]= (-v[i]);
	}
	return y;
}

template<typename V1, typename V2>
inline V1& set_addV(V1& v1, const V2& v2) {
	assert(v1.size()==v2.size());

	if (is_empty(v1) || is_empty(v2)) { set_empty(v1); return v1; }

	for (int i=0; i<v1.size(); i++)
		v1[i]+=v2[i];

	return v1;
}

template<typename V1, typename V2>
inline V1& set_subV(V1& v1, const V2& v2) {
	assert(v1.size()==v2.size());

	if (is_empty(v1) || is_empty(v2)) { set_empty(v1); return v1; }

	for (int i=0; i<v1.size(); i++)
		v1[i]-=v2[i];

	return v1;
}

template<typename M>
inline M minusM(const M& m) {
	M res(m.nb_rows(),m.nb_cols());

	if (is_empty(m)) { set_empty(res); return res; }

	for (int i=0; i<m.nb_rows(); i++)
		res[i]= (-m[i]);

	return res;
}

template<typename M1, typename M2>
inline M1& set_addM(M1& m1, const M2& m2) {
	assert(m1.nb_rows()==m2.nb_rows());
	assert(m1.nb_cols()==m2.nb_cols());

	if (is_empty(m1) || is_empty(m2)) { set_empty(m1); return m1; }

	for (int i=0; i<m1.nb_rows(); i++)
		m1[i]+=m2[i];

	return m1;
}

template<typename M1, typename M2>
inline M1& set_subM(M1& m1, const M2& m2) {
	assert(m1.nb_rows()==m2.nb_rows());
	assert(m1.nb_cols()==m2.nb_cols());

	if (is_empty(m1) || is_empty(m2)) { set_empty(m1); return m1; }

	for (int i=0; i<m1.nb_rows(); i++)
		m1[i]-=m2[i];

	return m1;
}

template<typename S, class V>
inline V& set_mulSV(const S& x, V& v) {

	if (is_empty(x) || is_empty(v)) { set_empty(v); return v; }

	for (int i=0; i<v.size(); i++)
		v[i]*=x;
	return v;
}

template<typename S, class M>
inline M& set_mulSM(const S& x, M& m) {

	if (is_empty(x) || is_empty(m)) { set_empty(m); return m; }

	for (int i=0; i<m.nb_rows(); i++)
		m[i]*=x;
	return m;
}

template<class Vin1, class Vin2, class S>
inline S mulVV(const Vin1& v1, const Vin2& v2) {
	assert(v1.size()==v2.size());

	const int n=v1.size();
	S y=0;

	if (is_empty(v1) || is_empty(v2)) {
		set_empty(y);
		return y;
	}

	for (int i=0; i<n; i++) {
		y+=v1[i]*v2[i];
	}
	return y;
}

template<class Vin1, class Vin2, class M>
inline M outer_prod(const Vin1& v1, const Vin2& v2) {

	M y(v1.size(),v2.size());

	if (is_empty(v1) || is_empty(v2)) {
		set_empty(y);
		return y;
	}

	for (int i=0; i<v1.size(); i++) {
		for (int j=0; j<v2.size(); j++) {
			y[i][j]=v1[i]*v2[j];
		}
	}
	return y;
}

template<class Vin1, class Vin2, class Vout>
inline Vout hadamard_prod(const Vin1& v1, const Vin2& v2) {

	assert(v1.size()==v2.size());
	Vout y(v1.size());

	if (is_empty(v1) || is_empty(v2)) {
		set_empty(y);
		return y;
	}

	for (int i=0; i<v1.size(); i++) {
			y[i]=v1[i]*v2[i];
	}
	return y;
}

template<class M, class Vin, class Vout>
inline Vout mulMV(const M& m, const Vin& v) {
	assert(m.nb_cols()==v.size());

	if (is_empty(m) || is_empty(v)) {
		Vout res(m.nb_rows());
		set_empty(res);
		return res;
	}

	Vout y(m.nb_rows());

	for (int i=0; i<m.nb_rows(); i++)
		y[i]=m[i]*v;

	return y;
}

template<class Vin, class M, class Vout>
inline Vout mulVM(const Vin& v, const M& m) {
	assert(m.nb_cols()==v.size());

	if (is_empty(m) || is_empty(v)) {
		Vout res(m.nb_cols());
		set_empty(res);
		return res;
	}

	Vout y(m.nb_cols());

	for (int j=0; j<m.nb_cols(); j++) {
		y[j]=0;
		for (int i=0; i<m.nb_rows(); i++) {
			y[j]+=m[i][j]*v[i];
		}
	}

	return y;
}

template<class Min1, class Min2, class Mout>
inline Mout mulMM(const Min1& m1, const Min2& m2) {
	assert(m1.nb_cols()==m2.nb_rows());

	Mout m3(m1.nb_rows(),m2.nb_cols());

	if (is_empty(m1) || is_empty(m2)) { set_empty(m3); return m3; }

	for (int i=0; i<m1.nb_rows(); i++) {
		for (int j=0; j<m2.nb_cols(); j++) {
			m3[i][j]=0;
			for (int k=0; k<m1.nb_cols(); k++)
				m3[i][j]+=m1[i][k]*m2[k][j];
		}
	}
	return m3;
}

template<typename V>
inline V absV(const V& v) {
	V res(v.size());

	if (is_empty(v)) { set_empty(res); return res; }

	for (int i=0; i<v.size(); i++)
		res[i]=abs(v[i]);

	return res;
}

template<typename M>
inline M absM(const M& m) {
	M res(m.nb_rows(),m.nb_cols());

	if (is_empty(m)) { set_empty(res); return res; }

	for (int i=0; i<m.nb_rows(); i++)
		res[i]=abs(m[i]);

	return res;
}

template<class M>
inline std::ostream& print(std::ostream& os, const M& m) {
	if (is_empty(m)) { return os << "empty matrix"; }
	os << "(";
	for (int i=0; i<m.nb_rows(); i++) {
		os << "(";
		for (int j=0; j<m.nb_cols(); j++) {
			os << m[i][j];
			if (j<m.nb_cols()-1) os << " ; ";
		}
		os << ")";
		if (i<m.nb_rows()-1) os << std::endl;
	}
	os << ")";
	return os;
}

}

Vector& Vector::operator+=(const Vector& x) {
	return set_addV<Vector,Vector>(*this,x);
}

IntervalVector& IntervalVector::operator+=(const Vector& x) {
	return set_addV<IntervalVector,Vector>(*this,x);
}

IntervalVector& IntervalVector::operator+=(const IntervalVector& x) {
	return set_addV<IntervalVector,IntervalVector>(*this,x);
}

Matrix& Matrix::operator+=(const Matrix& m) {
	return set_addM<Matrix,Matrix>(*this,m);
}

IntervalMatrix& IntervalMatrix::operator+=(const Matrix& m) {
	return set_addM<IntervalMatrix,Matrix>(*this,m);
}

IntervalMatrix& IntervalMatrix::operator+=(const IntervalMatrix& m) {
	return set_addM<IntervalMatrix,IntervalMatrix>(*this,m);
}

Vector& Vector::operator-=(const Vector& x) {
	return set_subV<Vector,Vector>(*this,x);
}

IntervalVector& IntervalVector::operator-=(const Vector& x) {
	return set_subV<IntervalVector,Vector>(*this,x);
}

IntervalVector& IntervalVector::operator-=(const IntervalVector& x) {
	return set_subV<IntervalVector,IntervalVector>(*this,x);
}

Matrix& Matrix::operator-=(const Matrix& m) {
	return set_subM<Matrix,Matrix>(*this,m);
}

IntervalMatrix& IntervalMatrix::operator-=(const Matrix& m) {
	return set_subM<IntervalMatrix,Matrix>(*this,m);
}

IntervalMatrix& IntervalMatrix::operator-=(const IntervalMatrix& m) {
	return set_subM<IntervalMatrix,IntervalMatrix>(*this,m);
}

Vector& Vector::operator*=(double x) {
	return set_mulSV<double,Vector>(x,*this);
}

IntervalVector& IntervalVector::operator*=(double x) {
	return set_mulSV<double,IntervalVector>(x,*this);
}

IntervalVector& IntervalVector::operator*=(const Interval& x) {
	return set_mulSV<Interval,IntervalVector>(x,*this);
}

Matrix& Matrix::operator*=(double x) {
	return set_mulSM<double,Matrix>(x,*this);
}

IntervalMatrix& IntervalMatrix::operator*=(double x) {
	return set_mulSM<double,IntervalMatrix>(x,*this);
}

IntervalMatrix& IntervalMatrix::operator*=(const Interval& x) {
	return set_mulSM<Interval,IntervalMatrix>(x,*this);
}

IntervalMatrix& IntervalMatrix::operator*=(const Matrix& m) {
	return *this=(*this*m);
}

IntervalMatrix& IntervalMatrix::operator*=(const IntervalMatrix& m) {
	return *this=(*this*m);
}

Vector operator-(const Vector& x) {
	return minusV(x);
}

IntervalVector operator-(const IntervalVector& x) {
	return minusV(x);
}

Matrix operator-(const Matrix& m) {
	return minusM(m);
}

IntervalMatrix operator-(const IntervalMatrix& m) {
	return minusM(m);
}

Vector operator+(const Vector& m1, const Vector& m2) {
	return Vector(m1)+=m2;
}

IntervalVector operator+(const IntervalVector& m1, const Vector& m2) {
	return IntervalVector(m1)+=m2;
}

IntervalVector operator+(const Vector& m1, const IntervalVector& m2) {
	return IntervalVector(m1)+=m2;
}

IntervalVector operator+(const IntervalVector& m1, const IntervalVector& m2) {
	return IntervalVector(m1)+=m2;
}

Matrix operator+(const Matrix& m1, const Matrix& m2) {
	return Matrix(m1)+=m2;
}

IntervalMatrix operator+(const IntervalMatrix& m1, const Matrix& m2) {
	return IntervalMatrix(m1)+=m2;
}

IntervalMatrix operator+(const Matrix& m1, const IntervalMatrix& m2) {
	return IntervalMatrix(m1)+=m2;
}

IntervalMatrix operator+(const IntervalMatrix& m1, const IntervalMatrix& m2) {
	return IntervalMatrix(m1)+=m2;
}

Vector operator-(const Vector& m1, const Vector& m2) {
	return Vector(m1)-=m2;
}

IntervalVector operator-(const IntervalVector& m1, const Vector& m2) {
	return IntervalVector(m1)-=m2;
}

IntervalVector operator-(const Vector& m1, const IntervalVector& m2) {
	return IntervalVector(m1)-=m2;
}

IntervalVector operator-(const IntervalVector& m1, const IntervalVector& m2) {
	return IntervalVector(m1)-=m2;
}

Matrix operator-(const Matrix& m1, const Matrix& m2) {
	return Matrix(m1)-=m2;
}

IntervalMatrix operator-(const IntervalMatrix& m1, const Matrix& m2) {
	return IntervalMatrix(m1)-=m2;
}

IntervalMatrix operator-(const Matrix& m1, const IntervalMatrix& m2) {
	return IntervalMatrix(m1)-=m2;
}

IntervalMatrix operator-(const IntervalMatrix& m1, const IntervalMatrix& m2) {
	return IntervalMatrix(m1)-=m2;
}

Vector operator*(double x, const Vector& v) {
	return Vector(v)*=x;
}

IntervalVector operator*(double x, const IntervalVector& v) {
	return IntervalVector(v)*=x;
}

IntervalVector operator*(const Interval& x, const Vector& v) {
	return IntervalVector(v)*=x;
}

IntervalVector operator*(const Interval& x, const IntervalVector& v) {
	return IntervalVector(v)*=x;
}

Matrix operator*(double x, const Matrix& m) {
	return Matrix(m)*=x;
}

IntervalMatrix operator*(double x, const IntervalMatrix& m) {
	return IntervalMatrix(m)*=x;
}

IntervalMatrix operator*(const Interval& x, const Matrix& m) {
	return IntervalMatrix(m)*=x;
}

IntervalMatrix operator*(const Interval& x, const IntervalMatrix& m) {
	return IntervalMatrix(m)*=x;
}

double operator*(const Vector& v1, const Vector& v2) {
	return mulVV<Vector,Vector,double>(v1,v2);
}

Interval operator*(const Vector& v1, const IntervalVector& v2) {
	return mulVV<Vector,IntervalVector,Interval>(v1,v2);
}

Interval operator*(const IntervalVector& v1, const Vector& v2) {
	return mulVV<IntervalVector,Vector,Interval>(v1,v2);
}

Interval operator*(const IntervalVector& v1, const IntervalVector& v2) {
	return mulVV<IntervalVector,IntervalVector,Interval>(v1,v2);
}

Matrix outer_product(const Vector& v1, const Vector& v2) {
	return outer_prod<Vector,Vector,Matrix>(v1,v2);
}

IntervalMatrix outer_product(const Vector& v1, const IntervalVector& v2) {
	return outer_prod<Vector,IntervalVector,IntervalMatrix>(v1,v2);
}

IntervalMatrix outer_product(const IntervalVector& v1, const Vector& v2) {
	return outer_prod<IntervalVector,Vector,IntervalMatrix>(v1,v2);
}

IntervalMatrix outer_product(const IntervalVector& v1, const IntervalVector& v2) {
	return outer_prod<IntervalVector,IntervalVector,IntervalMatrix>(v1,v2);
}

Vector hadamard_product(const Vector& v1, const Vector& v2) {
	return hadamard_prod<Vector,Vector,Vector>(v1,v2);
}

Vector hadamard_product(const Vector& v1, const IntervalVector& v2) {
	return hadamard_prod<Vector,IntervalVector,IntervalVector>(v1,v2);
}

Vector hadamard_product(const IntervalVector& v1, const Vector& v2) {
	return hadamard_prod<IntervalVector,Vector,IntervalVector>(v1,v2);
}

Vector hadamard_product(const IntervalVector& v1, const IntervalVector& v2) {
	return hadamard_prod<IntervalVector,IntervalVector,IntervalVector>(v1,v2);
}

Vector operator*(const Matrix& m, const Vector& v) {
	return mulMV<Matrix,Vector,Vector>(m,v);
}

IntervalVector operator*(const Matrix& m, const IntervalVector& v) {
	return mulMV<Matrix,IntervalVector,IntervalVector>(m,v);
}

IntervalVector operator*(const IntervalMatrix& m, const Vector& v) {
	return mulMV<IntervalMatrix,Vector,IntervalVector>(m,v);
}

IntervalVector operator*(const IntervalMatrix& m, const IntervalVector& v) {
	return mulMV<IntervalMatrix,IntervalVector,IntervalVector>(m,v);
}

Vector operator*(const Vector& v, const Matrix& m) {
	return mulVM<Vector,Matrix,Vector>(v,m);
}

IntervalVector operator*(const IntervalVector& v, const Matrix& m) {
	return mulVM<IntervalVector,Matrix,IntervalVector>(v,m);
}

IntervalVector operator*(const Vector& v, const IntervalMatrix& m) {
	return mulVM<Vector,IntervalMatrix,IntervalVector>(v,m);
}

IntervalVector operator*(const IntervalVector& v, const IntervalMatrix& m) {
	return mulVM<IntervalVector,IntervalMatrix,IntervalVector>(v,m);
}

Matrix operator*(const Matrix& m1, const Matrix& m2) {
	return mulMM<Matrix,Matrix,Matrix>(m1,m2);
}

IntervalMatrix operator*(const Matrix& m1, const IntervalMatrix& m2) {
	return mulMM<Matrix,IntervalMatrix,IntervalMatrix>(m1,m2);
}

IntervalMatrix operator*(const IntervalMatrix& m1, const Matrix& m2) {
	return mulMM<IntervalMatrix,Matrix,IntervalMatrix>(m1,m2);
}

IntervalMatrix operator*(const IntervalMatrix& m1, const IntervalMatrix& m2) {
	return mulMM<IntervalMatrix,IntervalMatrix,IntervalMatrix>(m1,m2);
}

Vector abs(const Vector& v) {
	return absV(v);
}

IntervalVector abs(const IntervalVector& v) {
	return absV(v);
}

Matrix abs(const Matrix& m) {
	return absM(m);
}

IntervalMatrix abs(const IntervalMatrix& m) {
	return absM(m);
}

std::ostream& operator<<(std::ostream& os, const Matrix& m) {
	return print<Matrix>(os,m);
}

std::ostream& operator<<(std::ostream& os, const IntervalMatrix& m) {
	return print<IntervalMatrix>(os,m);
}


//================================== Affine2 Implementation ======


Affine2Vector operator-(const Affine2Vector& x) {
	return minusV(x);
}

Affine2Matrix operator-(const Affine2Matrix& m) {
	return minusM(m);
}

Affine2Vector& Affine2Vector::operator+=(const Vector& x2) {
	return set_addV<Affine2Vector,Vector>(*this,x2);
}

Affine2Vector& Affine2Vector::operator+=(const IntervalVector& x2) {
	return set_addV<Affine2Vector,IntervalVector>(*this,x2);
}

Affine2Vector& Affine2Vector::operator+=(const Affine2Vector& x2) {
	return set_addV<Affine2Vector,Affine2Vector>(*this,x2);
}

Affine2Vector& Affine2Vector::operator-=(const Vector& x2) {
	return set_subV<Affine2Vector,Vector>(*this,x2);
}

Affine2Vector& Affine2Vector::operator-=(const IntervalVector& x2) {
	return set_subV<Affine2Vector,IntervalVector>(*this,x2);
}

Affine2Vector& Affine2Vector::operator-=(const Affine2Vector& x2) {
	return set_subV<Affine2Vector,Affine2Vector>(*this,x2);
}

Affine2Vector& Affine2Vector::operator*=(double d) {
	return set_mulSV<double,Affine2Vector>(d,*this);
}

Affine2Vector& Affine2Vector::operator*=(const Interval& x1) {
	return set_mulSV<Interval,Affine2Vector>(x1,*this);
}

Affine2Vector& Affine2Vector::operator*=(const Affine2& x1) {
	return set_mulSV<Affine2,Affine2Vector>(x1,*this);
}

Affine2Vector abs( const Affine2Vector& x) {
	return absV(x);
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

Affine2Vector operator+(const Vector& x1, const Affine2Vector& x2) {
	return Affine2Vector(x2)+=x1;
}

Affine2Vector operator+(const Affine2Vector& x1, const Vector& x2) {
	return Affine2Vector(x1)+=x2;
}

Affine2Vector operator+(const IntervalVector& x1, const Affine2Vector& x2) {
	return x2 + x1;
}

Affine2Vector operator+(const Affine2Vector& x1, const IntervalVector& x2) {
	return Affine2Vector(x1)+=x2;
}

Affine2Vector operator+(const Affine2Vector& x1, const Affine2Vector& x2) {
	return Affine2Vector(x1)+=x2;
}


Affine2Vector operator-(const Vector& x1, const Affine2Vector& x2) {
	Affine2Vector res(x2.size());
	res = (-x2);
	return res += x1;
}

Affine2Vector operator-(const Affine2Vector& x1, const Vector& x2) {
	return Affine2Vector(x1)-=x2;
}

Affine2Vector operator-(const Affine2Vector& x1, const IntervalVector& x2) {
	return Affine2Vector(x1)-=x2;
}

Affine2Vector operator-(const IntervalVector& x1, const Affine2Vector& x2) {
	Affine2Vector res(x2.size());
	res = (-x2);
	return res += x1;
}

Affine2Vector operator-(const Affine2Vector& x1, const Affine2Vector& x2) {
	return Affine2Vector(x1) += (-x2);
}

Affine2Vector operator*(double d, const Affine2Vector& x) {
	return Affine2Vector(x)*=d;
}

Affine2Vector operator*(const Affine2& x1, const Affine2Vector& x2) {
	return Affine2Vector(x2)*=x1;
}

Affine2Vector operator*(const Interval& x1, const Affine2Vector& x2) {
	return Affine2Vector(x2)*=x1;
}

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


std::ostream& operator<<(std::ostream& os, const Affine2Matrix& m) {
	return print < Affine2Matrix > (os, m);
}


} // namespace ibex

