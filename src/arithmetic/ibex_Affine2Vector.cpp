/* ============================================================================
 * I B E X - Affine2 Vector definition
 * ============================================================================
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Jordan Ninin
 * Created     : March 13, 2013
 * ---------------------------------------------------------------------------- */



#include "ibex_Affine2Vector.h"
#include <sstream>
#include "ibex_TemplateVector.cpp_"

namespace ibex {


IntervalVector& IntervalVector::operator=(const Affine2Vector& x) {
	assert(size()==x.size());
	for (int i = 0; i < size(); i++){
		vec[i] = x[i].itv();
	}
	return *this;
}


IntervalVector::IntervalVector(const Affine2Vector& x) : n(x.size()), vec(new Interval[x.size()]) {
	for (int i=0; i<n; i++) vec[i]=x[i].itv();
}


Affine2Vector::Affine2Vector(int n) :
		_n(n),
		_vec(new Affine2[n]) {
	assert(n>=1);
	for (int i = 0; i < n; i++){
		_vec[i] = Affine2(); // OR Affine(0,0) OR  Affine2(n, 0.0);  I do not know
	}
}

Affine2Vector::Affine2Vector(int n, const Interval& x) :
		_n(n),
		_vec(new Affine2[n]) {
	assert(n>=1);
	for (int i = 0; i < n; i++) {
		_vec[i] = Affine2(n, i + 1, x);
	}
}

Affine2Vector::Affine2Vector(int n, const Affine2& x) :
		_n(n),
		_vec(new Affine2[n]) {
	assert(n>=1);
	for (int i = 0; i < n; i++) {
		_vec[i] = x;
	}
}


Affine2Vector::Affine2Vector(const Affine2Vector& x) :
		_n(x.size()),
		_vec(new Affine2[x.size()]) {
	for (int i = 0; i < _n; i++){
		_vec[i] = x[i];
	}
}

Affine2Vector::Affine2Vector(const Affine2Vector& x, bool b) :
		_n(x.size()),
		_vec(new Affine2[x.size()]) {
	if (b) {
		for (int i = 0; i < _n; i++){
			_vec[i] = Affine2(x[i],b);
		}
	} else {
		for (int i = 0; i < _n; i++){
			_vec[i] = x[i];
		}
	}
}

Affine2Vector::Affine2Vector(const IntervalVector& x) :
		_n(x.size()),
		_vec(new Affine2[x.size()]) {
	for (int i = 0; i < x.size(); i++) {
		_vec[i] = Affine2(x.size(), i + 1, x[i]);
	}
}

Affine2Vector::Affine2Vector(int n, double bounds[][2]) :
		_n(n),
		_vec(new Affine2[n]) {
	if (bounds == 0){ // probably, the user called Affine2Vector(n,0) and 0 is interpreted as NULL!
		for (int i = 0; i < n; i++){
			_vec[i] = Affine2(n, i + 1, 0.0);
		}
	} else {
		for (int i = 0; i < n; i++){
			_vec[i] = Affine2(n, i + 1, Interval(bounds[i][0], bounds[i][1]));
		}
	}
}

/*
Affine2Vector::Affine2Vector(const Vector& x) :
		_n(x.size()),
		_vec(new Affine2[x.size()]) {
	for (int i = 0; i < _n; i++){
		_vec[i] = Affine2(_n, i + 1, x[i]);
	}
}*/

void Affine2Vector::init(const Interval& x) {
	for (int i = 0; i < size(); i++) {
		(*this)[i] = x;
	}
}
void Affine2Vector::init(const Affine2& x) {
	for (int i = 0; i < size(); i++) {
		(*this)[i] = x;
	}
}

void Affine2Vector::resize(int n) {
	assert(n>=1);
	assert((_vec==NULL && _n==0) || (size()!=0 && _vec!=NULL));

	if (n==size()) return;

	Affine2* newVec=new Affine2[n];
	int i=0;
	for (; i<size() && i<n; i++){
		newVec[i]=_vec[i];
	}
	for (; i<n; i++){
		newVec[i]= Affine2();
	}
	if (_vec!=NULL) { // vec==NULL happens when default constructor is used (n==0)
		delete[] _vec;
	}
	_n   = n;
	_vec = newVec;
}

IntervalVector operator&(const Affine2Vector& y,const IntervalVector& x)  {
	// dimensions are non zero henceforth
	if (y.size()!=x.size()) throw InvalidIntervalVectorOp("Cannot intersect Affine2Vectores with different dimensions");

	if (y.is_empty()||x.is_empty())
		return IntervalVector::empty(y.size());

	IntervalVector res(y.size());
	for (int i=0; i<y.size(); i++) {
		res [i] = y[i] & x[i];
		if (res[i].is_empty()) {
			res.set_empty();
			return res;
		}
	}
	return res;
}
IntervalVector operator&(const Affine2Vector& y,const Affine2Vector& x)  {
	// dimensions are non zero henceforth
	if (y.size()!=x.size()) throw InvalidIntervalVectorOp("Cannot intersect Affine2Vectores with different dimensions");

	if (y.is_empty()||x.is_empty())
		return IntervalVector::empty(y.size());

	IntervalVector res(y.size());
	for (int i=0; i<y.size(); i++) {
		res [i] = y[i] & x[i];
		if (res[i].is_empty()) {
			res.set_empty();
			return res;
		}
	}
	return res;
}
IntervalVector operator|(const Affine2Vector& y,const IntervalVector& x)  {
	// dimensions are non zero henceforth
	if (y.size()!=x.size()) throw InvalidIntervalVectorOp("Cannot make the hull of Affine2Vectores with different dimensions");

	if (y.is_empty()&&x.is_empty())
		return IntervalVector::empty(y.size());

	IntervalVector res(y.size());
	for (int i=0; i<y.size(); i++) {
		res [i] = y[i] | x[i];
	}
	return res;
}
IntervalVector operator|(const Affine2Vector& y,const Affine2Vector& x)  {
	// dimensions are non zero henceforth
	if (y.size()!=x.size()) throw InvalidIntervalVectorOp("Cannot make the hull of Affine2Vectores with different dimensions");

	if (y.is_empty()&&x.is_empty())
		return IntervalVector::empty(y.size());

	IntervalVector res(y.size());
	for (int i=0; i<y.size(); i++) {
		res [i] = y[i] | x[i];
	}
	return res;
}


IntervalVector Affine2Vector::itv() const {
	assert(!is_empty());
	IntervalVector intv(size());
	for (int i = 0; i < _n; i++) {
		intv[i] = (*this)[i].itv();
	}
	return intv;
}


//TODO
Affine2 operator*(const Vector& v1, const Affine2Vector& v2) {
	assert(v1.size()==v2.size());

	const int n=v1.size();
	Affine2 y(v2[0].size(),0);

	if (v2.is_empty()) {
		y.set_empty();
		return y;
	}

	for (int i=0; i<n; i++) {
		y+=v1[i]*v2[i];
	}
	return y;
}

Affine2 operator*(const Affine2Vector& v1, const Vector& v2) {
	assert(v1.size()==v2.size());

	const int n=v1.size();
	Affine2 y(v1[0].size(),0);

	if (v1.is_empty()) {
		y.set_empty();
		return y;
	}

	for (int i=0; i<n; i++) {
		y+=v1[i]*v2[i];
	}
	return y;
}


Affine2 operator*(const IntervalVector& x1, const Affine2Vector& x2){
	return x2*x1;
}


Affine2 operator*(const Affine2Vector& v1, const IntervalVector& v2) {
	assert(v1.size()==v2.size());

	const int n=v1.size();
	Affine2 y(v1[0].size(),0);

	if (v1.is_empty() || v2.is_empty()) {
		y.set_empty();
		return y;
	}

	for (int i=0; i<n; i++) {
		y+=v1[i] * v2[i];
	}
	return y;
}

Affine2 operator*(const Affine2Vector& v1, const Affine2Vector& v2) {
	assert(v1.size()==v2.size());
	assert(v1.size()==v2.size());

	const int n=v1.size();
	Affine2 y(v1[0].size(),0);

	if (v1.is_empty() || v2.is_empty()) {
		y.set_empty();
		return y;
	}

	for (int i=0; i<n; i++) {
		y+=v1[i] * v2[i];
	}
	return y;
}

Affine2Vector operator*(const Affine2& x1, const Vector& x2) {
	Affine2Vector res(x2.size(),x1);
	for (int i=0; i<x2.size(); i++) {
		res[i] *= x2[i];
	}
	return  res;
}

Affine2Vector& Affine2Vector::inflate(double rad1)                              { return _inflate(*this,rad1); }
Affine2Vector  Affine2Vector::subvector(int start_index, int end_index) const   { return _subvector(*this,start_index,end_index); }
void           Affine2Vector::put(int start_index, const Affine2Vector& subvec) { _put(*this, start_index, subvec); }
Affine2Vector& Affine2Vector::operator=(const Affine2Vector& x)                 { return _assign(*this,x); }
Affine2Vector& Affine2Vector::operator=(const IntervalVector& x)                { return _assign(*this,x); }
bool           Affine2Vector::operator==(const Affine2Vector& x) const          { return _equals(*this,x); }
bool           Affine2Vector::operator==(const IntervalVector& x) const         { return _equals(*this,x); }
Vector         Affine2Vector::lb() const                                        { return _lb(*this); }
Vector         Affine2Vector::ub() const                                        { return _ub(*this); }
Vector         Affine2Vector::mid() const                                       { return _mid(*this); }
Vector         Affine2Vector::mig() const                                       { return _mig(*this); }
Vector         Affine2Vector::mag() const                                       { return _mag(*this); }
bool           Affine2Vector::is_flat() const                                   { return _is_flat(*this); }
bool           Affine2Vector::contains(const Vector& x) const                   { return _contains(*this,x); }
bool           Affine2Vector::is_unbounded() const                              { return _is_unbounded(*this); }
bool           Affine2Vector::is_subset(const Affine2Vector& x) const           { return _is_subset(*this,x); }
bool           Affine2Vector::is_subset(const IntervalVector& x) const          { return _is_subset(*this,x); }
bool           Affine2Vector::is_strict_subset(const Affine2Vector& x) const    { return _is_strict_subset(*this,x); }
bool           Affine2Vector::is_strict_subset(const IntervalVector& x) const   { return _is_strict_subset(*this,x); }
bool           Affine2Vector::is_zero() const                                   { return _is_zero(*this); }
bool           Affine2Vector::is_bisectable() const                             { return _is_bisectable(*this); }
Vector         Affine2Vector::rad() const                                       { return _rad(*this); }
Vector         Affine2Vector::diam() const                                      { return _diam(*this); }
int            Affine2Vector::extr_diam_index(bool min) const                   { return _extr_diam_index(*this,min); }
std::ostream& operator<<(std::ostream& os, const Affine2Vector& x)              { return _display(os,x); }
double         Affine2Vector::volume() const                                    { return _volume(*this); }
double         Affine2Vector::perimeter() const                                 { return _perimeter(*this); }
double         Affine2Vector::rel_distance(const Affine2Vector& x) const        { return _rel_distance(*this,x); }
double         Affine2Vector::rel_distance(const IntervalVector& x) const       { return _rel_distance(*this,x); }
Vector         Affine2Vector::random() const                                    { return _random<Affine2Vector,Affine2>(*this); }

//int          Affine2Vector::diff(const Affine2Vector& y, IntervalVector*& result) const   { return _diff(itv(), y.itv(), result); }
//int          Affine2Vector::diff(const IntervalVector& y, IntervalVector*& result) const  { return _diff(itv(), y, result); }
//int          Affine2Vector::complementary(IntervalVector*& result) const                  { return _complementary(itv(), result); }
//std::pair<IntervalVector,IntervalVector> Affine2Vector::bisect(int i, double ratio) const { return _bisect(*this, i, ratio); }



bool proj_add(const IntervalVector& y, Affine2Vector& x1, Affine2Vector& x2) {
	for (int i=0; i<y.size(); i++)
		if (!proj_add(y[i], x1[i], x2[i])) {
			x1.set_empty();
			x2.set_empty();
			return false;
		}
	return true;
}
bool proj_add(const IntervalVector& y, Affine2Vector& x1, IntervalVector& x2) {
	for (int i=0; i<y.size(); i++)
		if (!proj_add(y[i], x1[i], x2[i])) {
			x1.set_empty();
			x2.set_empty();
			return false;
		}
	return true;
}


bool proj_sub(const IntervalVector& y, Affine2Vector& x1, Affine2Vector& x2) {
	for (int i=0; i<y.size(); i++)
		if (!proj_sub(y[i], x1[i], x2[i])) {
			x1.set_empty();
			x2.set_empty();
			return false;
		}
	return true;
}
bool proj_sub(const IntervalVector& y, IntervalVector& x1, Affine2Vector& x2) {
	for (int i=0; i<y.size(); i++)
		if (!proj_sub(y[i], x1[i], x2[i])) {
			x1.set_empty();
			x2.set_empty();
			return false;
		}
	return true;
}
bool proj_sub(const IntervalVector& y, Affine2Vector& x1, IntervalVector& x2) {
	for (int i=0; i<y.size(); i++)
		if (!proj_sub(y[i], x1[i], x2[i])) {
			x1.set_empty();
			x2.set_empty();
			return false;
		}
	return true;
}

bool proj_mul(const IntervalVector& y, Affine2& x1, Affine2Vector& x2) {
	assert(y.size()==x2.size());

	for (int i=0; i<x2.size(); i++)
		if (!proj_mul(y[i], x1, x2[i])) {
			x2.set_empty();
			return false;
		}
	return true;
}
bool proj_mul(const IntervalVector& y, Interval& x1, Affine2Vector& x2) {
	assert(y.size()==x2.size());

	for (int i=0; i<x2.size(); i++)
		if (!proj_mul(y[i], x1, x2[i])) {
			x2.set_empty();
			return false;
		}
	return true;
}
bool proj_mul(const IntervalVector& y, Affine2& x1, IntervalVector& x2) {
	assert(y.size()==x2.size());

	for (int i=0; i<x2.size(); i++)
		if (!proj_mul(y[i], x1, x2[i])) {
			x2.set_empty();
			return false;
		}
	return true;
}


bool proj_mul(const Interval& z, Affine2Vector& x, Affine2Vector& y) {
	assert(x.size()==y.size());
	int n=x.size();

	if (n==1) {
		if (proj_mul(z,x[0],y[0])) return true;
		else { x.set_empty(); y.set_empty(); return false; }
	}

	Interval* xy= new Interval[n];  // xy[i] := x[i]y[i]
	Interval* sum= new Interval[n-1]; // sum[i] := x[0]y[0]+...x[i]y[i]

	// ------------- forward --------------------
	for (int i=0; i<n; i++) xy[i]=x[i]*y[i];
	sum[0]=xy[0];
	for (int i=1; i<n-1; i++) sum[i]=sum[i-1]+xy[i];

	// ------------- backward -------------------
	// (rem: we have n>=2)
	if (!proj_add(z, sum[n-2], xy[n-1])) { x.set_empty(); y.set_empty(); delete[] sum; delete[] xy; return false; }

	for (int i=n-3; i>=0; i--)
		if (!proj_add(sum[i+1],sum[i],xy[i+1])) { x.set_empty(); y.set_empty(); delete[] sum; delete[] xy; return false; }

	if ((xy[0] &= sum[0]).is_empty()) { x.set_empty(); y.set_empty(); delete[] sum; delete[] xy; return false; }

	for (int i=0; i<n; i++)
		if (!proj_mul(xy[i],x[i],y[i])) { x.set_empty(); y.set_empty(); delete[] sum; delete[] xy; return false; }

	delete[] sum;
	delete[] xy;
	return true;
}
bool proj_mul(const Interval& z, Affine2Vector& x, IntervalVector& y) {
	assert(x.size()==y.size());
	int n=x.size();

	if (n==1) {
		if (proj_mul(z,x[0],y[0])) return true;
		else { x.set_empty(); y.set_empty(); return false; }
	}

	Interval* xy= new Interval[n];  // xy[i] := x[i]y[i]
	Interval* sum= new Interval[n-1]; // sum[i] := x[0]y[0]+...x[i]y[i]

	// ------------- forward --------------------
	for (int i=0; i<n; i++) xy[i]=x[i]*y[i];
	sum[0]=xy[0];
	for (int i=1; i<n-1; i++) sum[i]=sum[i-1]+xy[i];

	// ------------- backward -------------------
	// (rem: we have n>=2)
	if (!proj_add(z, sum[n-2], xy[n-1])) { x.set_empty(); y.set_empty(); delete[] sum; delete[] xy; return false; }

	for (int i=n-3; i>=0; i--)
		if (!proj_add(sum[i+1],sum[i],xy[i+1])) { x.set_empty(); y.set_empty(); delete[] sum; delete[] xy; return false; }

	if ((xy[0] &= sum[0]).is_empty()) { x.set_empty(); y.set_empty(); delete[] sum; delete[] xy; return false; }

	for (int i=0; i<n; i++)
		if (!proj_mul(xy[i],x[i],y[i])) { x.set_empty(); y.set_empty(); delete[] sum; delete[] xy; return false; }

	delete[] sum;
	delete[] xy;
	return true;
}






}// end namespace ibex
