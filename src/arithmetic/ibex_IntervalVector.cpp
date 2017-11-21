/* ============================================================================
 * I B E X - Interval Vector definition
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Dec 05, 2011
 * ---------------------------------------------------------------------------- */

#include "ibex_IntervalVector.h"
#include <vector>
#include <stdlib.h>
#include <sstream>
#include <math.h>
#include <float.h>
#include <algorithm>


#include "ibex_TemplateVector.h"

namespace ibex {

IntervalVector::IntervalVector(int nn) : n(nn), vec(new Interval[nn]) {
	assert(nn>=1);
	for (int i=0; i<nn; i++) vec[i]=Interval::ALL_REALS;
}

IntervalVector::IntervalVector(int n1, const Interval& x) : n(n1), vec(new Interval[n1]) {
	assert(n1>=1);
	for (int i=0; i<n1; i++) vec[i]=x;
}

IntervalVector::IntervalVector(const IntervalVector& x) : n(x.n), vec(new Interval[x.n]) {
	assert(x.vec!=NULL); // forbidden to copy uninitialized boxes
	for (int i=0; i<n; i++) vec[i]=x[i];
}

IntervalVector::IntervalVector(int n1, double bounds[][2]) : n(n1), vec(new Interval[n1]) {
	if (bounds==0) // probably, the user called IntervalVector(n,0) and 0 is interpreted as NULL!
		for (int i=0; i<n1; i++)
			vec[i]=Interval::ZERO;
	else
		for (int i=0; i<n1; i++)
			vec[i]=Interval(bounds[i][0],bounds[i][1]);
}

IntervalVector::IntervalVector(const Vector& x) : n(x.size()), vec(new Interval[n]) {
	for (int i=0; i<n; i++) vec[i]=x[i];
}

IntervalVector::IntervalVector(const Interval& x) : n(1), vec(new Interval[1]) {
	vec[0]=x;
}

void IntervalVector::init(const Interval& x) {
	for (int i=0; i<size(); i++)
		(*this)[i]=x;
}

void IntervalVector::resize(int n2) {
	assert(n2>=1);
	assert((vec==NULL && n==0) || (n!=0 && vec!=NULL));

	if (n2==size()) return;

	Interval* newVec=new Interval[n2];
	int i=0;
	for (; i<size() && i<n2; i++)
		newVec[i]=vec[i];
	for (; i<n2; i++)
		newVec[i]=Interval::ALL_REALS;
	if (vec!=NULL) // vec==NULL happens when default constructor is used (n==0)
		delete[] vec;

	n   = n2;
	vec = newVec;
}


IntervalVector& IntervalVector::operator&=(const IntervalVector& x)  {
	// dimensions are non zero henceforth
	if (size()!=x.size()) throw InvalidIntervalVectorOp("Cannot intersect IntervalVectores with different dimensions");

	if (is_empty()) return *this;
	if (x.is_empty()) { set_empty(); return *this; }

	for (int i=0; i<size(); i++) {
		(*this)[i] &= x[i];
		if ((*this)[i].is_empty()) {
			set_empty();
			return *this;
		}
	}
	return *this;
}

IntervalVector& IntervalVector::operator|=(const IntervalVector& x)  {
	// dimensions are non zero henceforth
	if (size()!=x.size()) throw InvalidIntervalVectorOp("Cannot make the hull of IntervalVectores with different dimensions");

	if (x.is_empty()) return *this;
	if (is_empty()) { *this=x; return *this; }

	for (int i=0; i<size(); i++) {
		(*this)[i] |= x[i];
	}
	return *this;
}


namespace {

class DiamLT {
	public:
	DiamLT(const IntervalVector& box) : box(box) { }

	bool operator()(const int& i, const int& j) {
		return box[i].diam()<box[j].diam();
	}
	const IntervalVector& box;
};

class DiamGT {
	public:
	DiamGT(const IntervalVector& box) : box(box) { }

	bool operator()(const int& i, const int& j) {
		return box[i].diam()>box[j].diam();
	}
	const IntervalVector& box;
};

} // end anonymous namespace

void IntervalVector::sort_indices(bool min, int tab[]) const {
	for (int i=0; i<n; i++) tab[i]=i;
	if (min)
		std::sort(tab,tab+n,DiamLT(*this));
	else
		std::sort(tab,tab+n,DiamGT(*this));
}

double distance(const IntervalVector& x1, const IntervalVector& x2) {
	assert(x1.size()==x2.size());

	double max = ibex::distance(x1[0],x2[0]);
	for (int i=1; i<x1.size(); i++) {
		double cand = ibex::distance(x1[i],x2[i]);
		if (max<cand) max = cand;
	}
	return max;
}

double IntervalVector::maxdelta(const IntervalVector& x) {
	double max = (*this)[0].delta(x[0]);
	for (int i=1; i<size(); i++) {
		double cand = (*this)[i].delta(x[i]);
		if (max<cand) max = cand;
	}
	return max;
}

int IntervalVector::diff(const IntervalVector& y, IntervalVector*& result, bool compactness) const {
	const int nn=size();
	IntervalVector x=*this;
	IntervalVector *tmp = new IntervalVector[2*nn]; // in the worst case, there is 2n boxes
	IntervalVector z=x & y;
	Interval c1, c2;
	bool return_x=z.is_empty();

	// check if in one dimension y is flat and x not,
	// in which case the diff returns also x directly
	if (!return_x && compactness) {
		for (int i=0; i<nn; i++) {
			if (z[i].is_degenerated() && !x[i].is_degenerated()) { return_x=true; break; }
		}
	}

	int b=0;

	if (return_x) { // includes the case of empty intersection
		tmp[b].resize(nn);
		tmp[b]=x; // copy of this
		if (!x.is_empty()) b++;
	} else {
		for (int var=0; var<nn; var++) {

			x[var].diff(y[var],c1,c2, compactness);

			//std::cout << x[var] << " diff " << y[var] << "=" << c1  << " and " << c2 << std::endl;
			if (!c1.is_empty()) {
				tmp[b].resize(nn);
				IntervalVector& v=tmp[b++];
				for (int i=0; i<var; i++) v[i]=x[i];
				v[var]=c1;
				for (int i=var+1; i<nn; i++) v[i]=x[i];

				if (!c2.is_empty()) {
					tmp[b].resize(nn);
					IntervalVector& v=tmp[b++];
					for (int i=0; i<var; i++) v[i]=x[i];
					v[var]=c2;
					for (int i=var+1; i<nn; i++) v[i]=x[i];
				}
				x[var] = z[var];
			}
		}
	}

	if (b==0) { // happens when x \subset y
		result = new IntervalVector[1];
		result[0].resize(nn);
		result[0].set_empty();
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

int IntervalVector::complementary(IntervalVector*& result) const {
	return IntervalVector(size()).diff(*this,result);
}


bool bwd_add(const IntervalVector& y, IntervalVector& x1, IntervalVector& x2) {
	x1 &= y-x2;
	x2 &= y-x1;
	return !x1.is_empty() && !x2.is_empty();
}

bool bwd_sub(const IntervalVector& y, IntervalVector& x1, IntervalVector& x2) {
	x1 &= y+x2;
	x2 &= x1-y;
	return !x1.is_empty() && !x2.is_empty();
}

bool bwd_mul(const IntervalVector& y, Interval& x1, IntervalVector& x2) {
	assert(y.size()==x2.size());

	for (int i=0; i<x2.size(); i++)
		if (!bwd_mul(y[i], x1, x2[i])) {
			x2.set_empty();
			return false;
		}
	return true;
}

bool bwd_mul(const Interval& z, IntervalVector& x, IntervalVector& y) {
	assert(x.size()==y.size());
	int n=x.size();

	if (n==1) {
		if (bwd_mul(z,x[0],y[0])) return true;
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
	if (!bwd_add(z, sum[n-2], xy[n-1])) { x.set_empty(); y.set_empty(); delete[] sum; delete[] xy; return false; }

	for (int i=n-3; i>=0; i--)
		if (!bwd_add(sum[i+1],sum[i],xy[i+1])) { x.set_empty(); y.set_empty(); delete[] sum; delete[] xy; return false; }

	if ((xy[0] &= sum[0]).is_empty()) { x.set_empty(); y.set_empty(); delete[] sum; delete[] xy; return false; }

	for (int i=0; i<n; i++)
		if (!bwd_mul(xy[i],x[i],y[i])) { x.set_empty(); y.set_empty(); delete[] sum; delete[] xy; return false; }

	delete[] sum;
	delete[] xy;
	return true;
}

IntervalVector cart_prod(const Array<const IntervalVector>& x) {
	int size=0;
	for (int i=0; i<x.size(); i++) {
		size+=x[i].size();
	}

	IntervalVector z(size);
//	std::cout << "size=" << size << " z=" << z << std::endl;

	for (int i=0, j=0; i<x.size(); j+=x[i].size(), i++) {
		if (x[i].is_empty()) {
			z.set_empty();
			return z;
		} else {
			z.put(j,x[i]);
		}
	}
	return z;
}

IntervalVector& IntervalVector::inflate(double delta, double chi) {
	if (is_empty()) return *this;
	for (int i=0; i<size(); i++) {
		vec[i].inflate(delta,chi);
	}
	return *this;
}

double infinite_norm(const IntervalVector& v) {
	assert(!v.is_empty());
    double norm = v[0].mag();
    for (int i=1; i<v.size(); i++) {
        double x = v[i].mag();
        if (x > norm)
            norm = x;
    }
    return norm;
}

// why inflate is template?
IntervalVector& IntervalVector::inflate(double rad1)                              { return _inflate(*this,rad1); }
IntervalVector  IntervalVector::subvector(int start_index, int end_index) const   { return _subvector(*this,start_index,end_index); }
void            IntervalVector::put(int start_index, const IntervalVector& x)     { _put(*this, start_index, x); }
IntervalVector& IntervalVector::operator=(const IntervalVector& x)                { resize(x.size()); // see issue #10
                                                                                    return _assignV(*this,x); }
bool            IntervalVector::operator==(const IntervalVector& x) const         { return _equalsV(*this,x); }
Vector          IntervalVector::lb() const                                        { return _lb(*this); }
Vector          IntervalVector::ub() const                                        { return _ub(*this); }
Vector          IntervalVector::mid() const                                       { return _mid(*this); }
Vector          IntervalVector::mig() const                                       { return _mig(*this); }
Vector          IntervalVector::mag() const                                       { return _mag(*this); }
bool            IntervalVector::is_flat() const                                   { return _is_flat(*this); }
bool            IntervalVector::is_unbounded() const                              { return _is_unbounded(*this); }
bool            IntervalVector::is_zero() const                                   { return _is_zero(*this); }
bool            IntervalVector::is_bisectable() const                             { return _is_bisectable(*this); }
Vector          IntervalVector::rad() const                                       { return _rad(*this); }
Vector          IntervalVector::diam() const                                      { return _diam(*this); }
int             IntervalVector::extr_diam_index(bool min) const                   { return _extr_diam_index(*this,min); }
std::ostream&   operator<<(std::ostream& os, const IntervalVector& x)             { return _displayV(os,x); }
double          IntervalVector::volume() const                                    { return _volume(*this); }
double          IntervalVector::perimeter() const                                 { return _perimeter(*this); }
double          IntervalVector::rel_distance(const IntervalVector& x) const       { return _rel_distance(*this,x); }
Vector          IntervalVector::random(int seed) const                            { return _random<IntervalVector,Interval>(*this,seed); }
Vector          IntervalVector::random() const                            		  { return _random<IntervalVector,Interval>(*this); }
std::pair<IntervalVector,IntervalVector> IntervalVector::bisect(int i, double ratio) const  { return _bisect(*this, i, ratio); }
IntervalVector  abs(const IntervalVector& v)                                      {	return _abs(v); }

} // end namespace
