/*
 * ibex_Affine2Vector.cpp
 *
 *  Created on: 12 nov. 2012
 *      Author: nininjo
 */

#include "ibex_IntervalVector.h"
#include "ibex_IntervalVector.cpp"
#include "ibex_Affine2Vector.h"
#include <sstream>

namespace ibex {


IntervalVector& IntervalVector::operator=(const Affine2Vector& x) {
	assert(size()==x.size());
	for (int i = 0; i < size(); i++){
		vec[i] = x[i].itv();
	}
	return *this;
}

Affine2Vector::Affine2Vector(int n) :
		_n(n),
		_vec(new Affine2[n]) {
	assert(n>=1);
	for (int i = 0; i < n; i++){
		_vec[i] = Affine2(n, 0.0);
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

Affine2Vector::Affine2Vector(const Vector& x) :
		_n(x.size()),
		_vec(new Affine2[x.size()]) {
	for (int i = 0; i < _n; i++){
		_vec[i] = Affine2(_n, i + 1, x[i]);
	}
}

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

Affine2Vector& Affine2Vector::inflate(double rad) {
	if (is_empty()) {
		return *this;
	}
	Interval r(-rad, rad);
	// little optim: we do not call (*this)[i].inflate(rad)
	// because this would create n times
	// the interval [-rad,rad]
	for (int i = 0; i < size(); i++) {
		(*this)[i] += r;
	}
	return *this;
}

void Affine2Vector::resize(int n) {
	assert(n>=1);
	assert((_vec==NULL && _n==0) || (_n!=0 && _vec!=NULL));

	if (n==size()) return;

	Affine2* newVec=new Affine2[n];
	int i=0;
	for (; i<_n && i<n; i++){
		newVec[i]=_vec[i];
	}
	for (; i<n; i++){
		newVec[i]= Affine2(n);
	}
	if (_vec!=NULL) { // vec==NULL happens when default constructor is used (n==0)
		delete[] _vec;
	}
	_n   = n;
	_vec = newVec;
}

Affine2Vector Affine2Vector::subvector(int start_index, int end_index) const {
	assert(!is_empty());
	assert(end_index>=0 && start_index>=0);
	assert(end_index<size() && start_index<=end_index);
	//throw InvalidAffine2VectorOp("Invalid indices for Affine2Vector::subvector");

	Affine2Vector v(end_index-start_index+1);
	int j=0;
	for (int i=start_index; i<=end_index; i++) {
		v[j++]=(*this)[i];
	}
	return v;
}

void Affine2Vector::put(int start_index, const Affine2Vector& subvec) {
	assert(!is_empty());
	int end_index=start_index+subvec.size()-1;
	assert(start_index>=0 && end_index<size());

	int j=0;
	for (int i=start_index; i<=end_index; i++) {
		(*this)[i] = subvec[j++];
	}
}

Affine2Vector& Affine2Vector::operator=(const Affine2Vector& x) {
	assert(size()==x.size());
	if (x.is_empty()) {
		set_empty();
	} else {
		// don't use "set(...)" because the test "is_empty()" called inside
		// may return prematurely in case "this" is empty.
		// use physical copy instead:
		for (int i = 0; i < size(); i++) {
			(*this)[i] = x[i];
		}
	}

	return *this;
}


Affine2Vector& Affine2Vector::operator=(const IntervalVector& x) {
	assert(size()==x.size());
	if (x.is_empty()) {
		set_empty();
	} else {
		// don't use "set(...)" because the test "is_empty()" called inside
		// may return prematurely in case "this" is empty.
		// use physical copy instead:
		for (int i = 0; i < size(); i++) {
			(*this)[i] = Affine2(size(), i+1,x[i]);
		}
	}

	return *this;
}

bool Affine2Vector::operator ==(const Affine2Vector& x) const {
	if (_n != x.size()) {
		return false;
	}
	if (is_empty() || x.is_empty()) {
		return is_empty() && x.is_empty();
	}
	for (int i = 0; i < _n; i++) {
		if ((*this)[i] != (x[i])) {
			return false;
		}
	}

	return true;
}
bool Affine2Vector::operator ==(const IntervalVector& x) const {
	if (_n != x.size()) {
		return false;
	}
	if (is_empty() || x.is_empty()) {
		return is_empty() && x.is_empty();
	}
	for (int i = 0; i < _n; i++) {
		if ((*this)[i] != (x[i])) {
			return false;
		}
	}

	return true;
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

	if (y.is_empty()||x.is_empty())
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

	if (y.is_empty()||x.is_empty())
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


Vector Affine2Vector::lb() const {
	assert(!is_empty());
	Vector l(size());
	for (int i = 0; i < size(); i++) {
		l[i] = (*this)[i].lb();
	}
	return l;
}

Vector Affine2Vector::ub() const {
	assert(!is_empty());
	Vector u(size());
	for (int i = 0; i < size(); i++) {
		u[i] = (*this)[i].ub();
	}
	return u;
}

Vector Affine2Vector::mid() const {
	assert(!is_empty());
	Vector mV(size());
	for (int i = 0; i < size(); i++) {
		mV[i] = (*this)[i].mid();
	}
	return mV;
}

Vector Affine2Vector::mig() const {
	assert(!is_empty());
	Vector res(size());
	for (int i = 0; i < size(); i++) {
		res[i] = (*this)[i].mig();
	}
	return res;
}

Vector Affine2Vector::mag() const {
	assert(!is_empty());
	Vector res(size());
	for (int i = 0; i < size(); i++) {
		res[i] = (*this)[i].mag();
	}
	return res;
}

bool Affine2Vector::is_flat() const {
	if (is_empty())
		return true;

	for (int i = 0; i < size(); i++)
		if ((*this)[i].is_degenerated())
			// don't use diam() because of roundoff
			return true;
	return false;
}

bool Affine2Vector::contains(const Vector& x) const {
	for (int i = 0; i < size(); i++)
		if (!(*this)[i].contains(x[i]))
			return false;
	return true;
}

bool Affine2Vector::is_unbounded() const {
	if (is_empty())
		return false;

	for (int i = 0; i < size(); i++)
		if ((*this)[i].is_unbounded())
			return true;
	return false;
}



bool Affine2Vector::is_subset(const IntervalVector& x) const {
	if (is_empty())
		return true;

	for (int i = 0; i < size(); i++) {
		if (!(*this)[i].is_subset(x[i]))
			return false;
	}
	return true;
}


bool Affine2Vector::is_subset(const Affine2Vector& x) const {
	if (is_empty())
		return true;

	for (int i = 0; i < size(); i++) {
		if (!(*this)[i].is_subset(x[i]))
			return false;
	}
	return true;
}

bool Affine2Vector::is_strict_subset(const IntervalVector& x) const {
	if (is_empty() && !x.is_empty())
		return true;

	if (x.is_empty())
		return false;

	for (int i = 0; i < size(); i++) {
		if (!(*this)[i].is_strict_subset(x[i]))
			return false;
	}
	return true;
}

bool Affine2Vector::is_strict_subset(const Affine2Vector& x) const {
	if (is_empty() && !x.is_empty())
		return true;

	if (x.is_empty())
		return false;

	for (int i = 0; i < size(); i++) {
		if (!(*this)[i].is_strict_subset(x[i]))
			return false;
	}
	return true;
}

bool Affine2Vector::is_zero() const {
	for (int i = 0; i < size(); i++) {
		if ((*this)[i] != Interval::ZERO)
			return false;
	}
	return true;
}

bool Affine2Vector::is_bisectable() const {
	for (int i = 0; i < size(); i++) {
		if ((*this)[i].is_bisectable())
			return true;
	}
	return false;
}

Vector Affine2Vector::rad() const {
	Vector r(size());
	for (int i = 0; i < size(); i++) {
		r[i] = (*this)[i].rad();
	}
	return r;
}

Vector Affine2Vector::diam() const {
	Vector d(size());
	for (int i = 0; i < size(); i++) {
		d[i] = (*this)[i].diam();
	}
	return d;
}

int Affine2Vector::extr_diam_index(bool min) const {
	double d = (*this)[0].diam();
	int selectedIndex = 0;
	if (is_empty())
		throw InvalidIntervalVectorOp("Diameter of an empty Affine2Vector is undefined");

	for (int i = 1; i < size(); i++) {
		double w = (*this)[i].diam();
		if (min ? w < d : w > d) {
			selectedIndex = i;
			d = w;
		}
	}

	return selectedIndex;
}

namespace {

const Affine2Vector* tmpA;

bool diam_ltA(const int& i, const int& j) {
	return (*tmpA)[i].diam() < (*tmpA)[j].diam();
}

bool diam_gtA(const int& i, const int& j) {
	return (*tmpA)[i].diam() > (*tmpA)[j].diam();
}

} // end namespace



void Affine2Vector::sort_indices(bool min, int tab[]) const {
	for (int i = 0; i < _n; i++)
		tab[i] = i;
	tmpA = this;
	std::sort(tab, tab + _n, min ? diam_ltA : diam_gtA);
}

std::ostream& operator <<(std::ostream& os, const Affine2Vector& x) {
	if (x.is_empty())
		return os << "empty vector";

	os << "( " << x[0] << " ; " << std::endl;
	for (int i = 1; i < x.size(); i++) {
		os << "  " << x[i] << (i < x.size() - 1 ? " ; " : " ) ") << std::endl;
	}
	return os;
}


double Affine2Vector::volume() const {
	if ((*this)[0].is_unbounded())
		return POS_INFINITY;

	if ((*this)[0].is_degenerated())
		return 0;

	double vol = ::log(((*this)[0]).diam());
	for (int i = 1; i < size(); i++) {
		if ((*this)[i].is_unbounded())
			return POS_INFINITY;
		if ((*this)[i].is_degenerated())
			return 0;
		vol += ::log(((*this)[i]).diam());
	}
	return ::exp(vol);
}
double Affine2Vector::perimeter() const {
	if ((*this)[0].is_unbounded())
		return POS_INFINITY;

	double per = ((*this)[0]).diam();
	for (int i = 1; i < size(); i++) {
		if ((*this)[i].is_unbounded())
			return POS_INFINITY;
		per += ((*this)[i]).diam();
	}
	return per;
}

double distance(const Affine2Vector& x1, const Affine2Vector& x2) {
	assert(x1.size() == x2.size());
	double max = ibex::distance(x1[0], x2[0]);
	for (int i = 1; i < x1.size(); i++) {
		double cand = ibex::distance(x1[i], x2[i]);
		if (max < cand)
			max = cand;
	}
	return max;
}

double Affine2Vector::maxdelta(const Affine2Vector& x) {
	double max = (*this)[0].delta(x[0]);
	for (int i = 1; i < size(); i++) {
		double cand = (*this)[i].delta(x[i]);
		if (max < cand)
			max = cand;
	}
	return max;
}

double Affine2Vector::rel_distance(const Affine2Vector& x) const {
	double max = (*this)[0].rel_distance(x[0]);
	for (int i = 1; i < size(); i++) {
		double cand = (*this)[i].rel_distance(x[i]);
		if (max < cand)
			max = cand;
	}
	return max;
}



int Affine2Vector::diff(const IntervalVector& y, IntervalVector*& result) const {
	const int nn=size();
	const IntervalVector& x=*this;
	IntervalVector *tmp = new IntervalVector[2*nn]; // in the worst case, there is 2n boxes
	Interval c1, c2;
	int b=0;
	if (y.is_empty()) {
		tmp[b].resize(nn);
		tmp[b]=x; // copy of this
		b++;
	} else {
		for (int var=0; var<nn; var++) {

			diffI(x[var],y[var],c1,c2);

			if (!c1.is_empty()) {
				tmp[b].resize(nn);
				IntervalVector& v=tmp[b++];
				for (int i=0; i<var; i++) v[i]=y[i];
				v[var]=c1;
				for (int i=var+1; i<nn; i++) v[i]=x[i];

				if (!c2.is_empty()) {
					tmp[b].resize(nn);
					IntervalVector& v=tmp[b++];
					for (int i=0; i<var; i++) v[i]=y[i];
					v[var]=c2;
					for (int i=var+1; i<nn; i++) v[i]=x[i];
				}
			}
		}
	}

	if (b==0) {
		result = new IntervalVector[1];
		result[0].resize(nn);
		result[0].set_empty();
		b=1;
	} else {
		result=new IntervalVector[b];
		for (int i=0; i<b; i++) {
			result[i].resize(nn);
			result[i]=tmp[i];
		}
	}
	delete[] tmp;

	return b;
}

int Affine2Vector::diff(const Affine2Vector& y, IntervalVector*& result) const {
	return (this->itv()).diff(y.itv(),result);
}
int Affine2Vector::diff(const IntervalVector& y, IntervalVector*& result) const {
	return ((this->itv()).diff(y,result));
}

int Affine2Vector::complementary(IntervalVector*& result) const {
	IntervalVector tmp= *this->itv();
	return IntervalVector(size()).diff(tmp,result);
}


std::pair<IntervalVector,IntervalVector> Affine2Vector::bisect(int i, double ratio) const {
	assert(0<ratio && ratio<1.0);
	assert(0<=i && i<size());

	if (!(*this)[i].is_bisectable()) {
		std::stringstream oss;
		oss << "Unable to bisect " << (*this);
		throw InvalidIntervalVectorOp(oss.str());
	}
	IntervalVector left((*this).itv());
	IntervalVector right((*this).itv());

	std::pair<Interval,Interval> p=(*this)[i].bisect(ratio);

	left[i] = p.first;
	right[i] = p.second;

	return std::pair<IntervalVector,IntervalVector>(left,right);
}

Vector Affine2Vector::random() const {
	assert(!is_empty());

	Vector b(size());
	for (int i=0; i<size(); i++) {
		const Affine2& xi=(*this)[i];
		// get a random number in [-0.5,0.5]
		double r=rand()/(double) RAND_MAX -0.5;
		// pick a point in the domain of the ith variable
		double p = xi.mid() + r*xi.diam();
		// watch dog
		if (p<xi.lb()) p=xi.lb();
		else if (p>xi.ub()) p=xi.ub();
		b[i]=p;
	}
	return b;
}



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

	Affine2* xy= new Affine2[n];  // xy[i] := x[i]y[i]
	Affine2* sum= new Affine2[n-1]; // sum[i] := x[0]y[0]+...x[i]y[i]

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

	Affine2* xy= new Affine2[n];  // xy[i] := x[i]y[i]
	Affine2* sum= new Affine2[n-1]; // sum[i] := x[0]y[0]+...x[i]y[i]

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




// TODO




Affine2Vector& Affine2Vector::operator +=(const Vector& x2) {
}

Affine2Vector& Affine2Vector::operator +=(const IntervalVector& x2) {
}

Affine2Vector& Affine2Vector::operator +=(const Affine2Vector& x2) {
}

Affine2Vector& Affine2Vector::operator -=(const Vector& x2) {
}

Affine2Vector& Affine2Vector::operator -=(const IntervalVector& x2) {
}

Affine2Vector& Affine2Vector::operator -=(const Affine2Vector& x2) {
}

Affine2Vector& Affine2Vector::operator *=(double d) {
}

Affine2Vector& Affine2Vector::operator *=(const Interval& x1) {
}

Affine2Vector& Affine2Vector::operator *=(const Affine2& x1) {
}



}// end namespace ibex
