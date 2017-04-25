//============================================================================
//                                  I B E X                                   
// File        : ibex_LinearArith.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Apr 18, 2012
// Last Update : Apr 18, 2012
//============================================================================


#ifndef __IBEX_LINEAR_ARITH__
#define __IBEX_LINEAR_ARITH__

namespace ibex {

class Vector;
class Matrix;
class Interval;
class IntervalVector;
class IntervalMatrix;

//
//double abs(double x) ;
//Interval abs(const Interval& x);
//Vector abs(const Vector& v);
//IntervalVector abs(const IntervalVector& v);
//Matrix abs(const Matrix& m);
//IntervalMatrix abs(const IntervalMatrix& m);

// the following functions are
// introduced to allow genericity
bool ___is_empty(double x)  ;
bool ___is_empty(const Interval& x) ;
bool ___is_empty(const Vector& v);
bool ___is_empty(const IntervalVector& v);
bool ___is_empty(const Matrix& m);
bool ___is_empty(const IntervalMatrix& m);

void ___set_empty(double x) ;
void ___set_empty(Interval& x);
void ___set_empty(Vector& v);
void ___set_empty(IntervalVector& v);
void ___set_empty(Matrix& m);
void ___set_empty(IntervalMatrix& m);

namespace {

template<typename V>
inline V minusV(const V& v) {
	const int n=v.size();

	V y(n);

	if (___is_empty(v)) { ___set_empty(y); return y; }

	for (int i=0; i<n; i++) {
		y[i]= (-v[i]);
	}
	return y;
}

template<typename V1, typename V2>
inline V1& set_addV(V1& v1, const V2& v2) {
	assert(v1.size()==v2.size());

	if (___is_empty(v1) || ___is_empty(v2)) { ___set_empty(v1); return v1; }

	for (int i=0; i<v1.size(); i++)
		v1[i]+=v2[i];

	return v1;
}

template<typename V1, typename V2>
inline V1& set_subV(V1& v1, const V2& v2) {
	assert(v1.size()==v2.size());

	if (___is_empty(v1) || ___is_empty(v2)) { ___set_empty(v1); return v1; }

	for (int i=0; i<v1.size(); i++)
		v1[i]-=v2[i];

	return v1;
}

template<typename M>
inline M minusM(const M& m) {
	M res(m.nb_rows(),m.nb_cols());

	if (___is_empty(m)) { ___set_empty(res); return res; }

	for (int i=0; i<m.nb_rows(); i++)
		res[i]= (-m[i]);

	return res;
}

template<typename M1, typename M2>
inline M1& set_addM(M1& m1, const M2& m2) {
	assert(m1.nb_rows()==m2.nb_rows());
	assert(m1.nb_cols()==m2.nb_cols());

	if (___is_empty(m1) || ___is_empty(m2)) { ___set_empty(m1); return m1; }

	for (int i=0; i<m1.nb_rows(); i++)
		m1[i]+=m2[i];

	return m1;
}

template<typename M1, typename M2>
inline M1& set_subM(M1& m1, const M2& m2) {
	assert(m1.nb_rows()==m2.nb_rows());
	assert(m1.nb_cols()==m2.nb_cols());

	if (___is_empty(m1) || ___is_empty(m2)) { ___set_empty(m1); return m1; }

	for (int i=0; i<m1.nb_rows(); i++)
		m1[i]-=m2[i];

	return m1;
}

template<typename S, class V>
inline V& set_mulSV(const S& x, V& v) {

	if (___is_empty(x) || ___is_empty(v)) { ___set_empty(v); return v; }

	for (int i=0; i<v.size(); i++)
		v[i]*=x;
	return v;
}

template<typename S, class M>
inline M& set_mulSM(const S& x, M& m) {

	if (___is_empty(x) || ___is_empty(m)) { ___set_empty(m); return m; }

	for (int i=0; i<m.nb_rows(); i++)
		m[i]*=x;
	return m;
}

template<class Vin1, class Vin2, class S>
inline S mulVV(const Vin1& v1, const Vin2& v2) {
	assert(v1.size()==v2.size());

	const int n=v1.size();
	S y=0;

	if (___is_empty(v1) || ___is_empty(v2)) {
		___set_empty(y);
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

	if (___is_empty(v1) || ___is_empty(v2)) {
		___set_empty(y);
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

	if (___is_empty(v1) || ___is_empty(v2)) {
		___set_empty(y);
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

	if (___is_empty(m) || ___is_empty(v)) {
		Vout res(m.nb_rows());
		___set_empty(res);
		return res;
	}

	Vout y(m.nb_rows());

	for (int i=0; i<m.nb_rows(); i++)
		y[i]=m[i]*v;

	return y;
}

template<class Vin, class M, class Vout>
inline Vout mulVM(const Vin& v, const M& m) {
	assert(m.nb_rows()==v.size());

	if (___is_empty(m) || ___is_empty(v)) {
		Vout res(m.nb_cols());
		___set_empty(res);
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

	if (___is_empty(m1) || ___is_empty(m2)) { ___set_empty(m3); return m3; }

	for (int i=0; i<m1.nb_rows(); i++) {
		for (int j=0; j<m2.nb_cols(); j++) {
			m3[i][j]=0;
			for (int k=0; k<m1.nb_cols(); k++)
				m3[i][j]+=m1[i][k]*m2[k][j];
		}
	}
	return m3;
}

} // anonymous namespace

} // namespace ibex

#endif // __IBEX_LINEAR_ARITH__
