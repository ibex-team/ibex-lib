/* ============================================================================
 * I B E X - Implementation of the IntervalVector class based on Gaol
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Mar 5, 2012
 * ---------------------------------------------------------------------------- */

#ifndef __IBEX_GAOL_INTERVAL_VECTOR_H__
#define __IBEX_GAOL_INTERVAL_VECTOR_H__

#include "ibex_IntervalVector.h"

inline IntervalVector::IntervalVector(int n) : n(n), vec(new Interval[n]) {
	assert(n>=1);
}

inline IntervalVector::IntervalVector(int n, const Interval& x) : n(n), vec(new Interval[n]) {
	assert(n>=1);
	for (int i=0; i<n; i++) {
		vec[i]=x;
	}
}

inline IntervalVector::IntervalVector(const IntervalVector& x) : n(x.n), vec(new Interval(x.n)) {
	for (int i=0; i<n; i++) {
		vec[i]=x[i];
	}
}

inline IntervalVector::IntervalVector(int n, double bounds[][2]) : n(n), vec(new Interval[n]) {
	for (int i=0; i<n; i++)
		vec[i]=Interval(bounds[i][0],bounds[i][1]);
}

inline IntervalVector IntervalVector::empty(int n) {
	return IntervalVector(n, Interval::EMPTY_SET);
}


inline IntervalVector::~IntervalVector() {
	delete[] vec;
}

inline const Interval& IntervalVector::operator[](int i) const {
	assert(i>=0 && i<n);
	return vec[i];
}

inline IntervalVector& IntervalVector::operator=(const IntervalVector& x) {
	assert(size()==x.size()); // throw InvalidIntervalVectorOp("Cannot set a IntervalVector to a IntervalVector with different dimension");

	if (x.is_empty())
		set_empty();
	else
		// don't use "set(...)" because the test "is_empty()" called inside
		// may return prematurely in case "this" is empty.
		// use physical copy instead:
		for (int i=0; i<size(); i++)
			(*this)[i]=x[i];

	return *this;
}

inline IntervalVector IntervalVector::operator&(const IntervalVector& x) const {
	return IntervalVector(*this) &= x;
}

IntervalVector IntervalVector::operator|(const IntervalVector& x) const {
	return IntervalVector(*this) |= x;
}

inline int IntervalVector::size() const {
	return n;
}

inline void IntervalVector::resize(int n2) {
	assert(n2>=1);
	if (n2==size()) return;

	Interval* newVec=new Interval[n2];
	int i=0;
	for (; i<size() && i<n2; i++)
		newVec[i]=vec[i];
	for (; i<n2; i++)
		newVec[i]=is_empty()? Interval::EMPTY_SET : Interval::ALL_REALS;
	delete[] vec;

	n   = n2;
	vec = newVec;
}

/*
bool IntervalVector::operator==(const IntervalVector& x) const {
	if (n!=x.size()) return false;
	if (is_empty() || x.is_empty()) return is_empty() && x.is_empty();
	for (int i=0; i<n; i++)
		if (!(*this)[i].set_eq(x[i])) return false;
	return true;
}*/



inline IntervalVector IntervalVector::mid() const {
	IntervalVector mIntervalVector(size());
	for (int i=0; i<size(); i++) {
		Interval m =  (*this)[i].mid();
		mIntervalVector.set(i, m);
	}
	return mIntervalVector;
}

inline IntervalVector IntervalVector::operator-() const {
	const int n=size();
	if (is_empty()) return empty(n);

	IntervalVector x(n);
	for (int i=0; i<n; i++) {
		x[i]=-(*this)[i];
	}
	return x;
}

inline IntervalVector IntervalVector::operator+(const IntervalVector& x) const {

	const int n=size();
	assert(x.size()==n);
	if (is_empty() || x.is_empty()) return empty(n);
	else {
		IntervalVector y(n);
		for (int i=0; i<n; i++) {
			(y[i]=(*this)[i])+=x[i]; // faster than y[i]=(*this)[i]+x[i]
		}
		return y;
	}
}

inline IntervalVector& IntervalVector::operator+=(const IntervalVector& x) {
	const int n=size();
	assert(x.size()==n);
	if (!is_empty()) {
		if (x.is_empty()) { set_empty(); }
		else {
			for (int i=0; i<n; i++) {
				(*this)[i]+=x[i];
			}
		}
	}
	return *this;
}

inline IntervalVector IntervalVector::operator-(const IntervalVector& x) const {
	const int n=size();
	assert(x.size()==n);
	if (is_empty() || x.is_empty()) return empty(n);
	else {
		IntervalVector y(n);
		for (int i=0; i<n; i++) {
			(y[i]=(*this)[i])-=x[i]; // faster than y[i]=(*this)[i]-x[i]
		}
		return y;
	}
}

inline IntervalVector& IntervalVector::operator-=(const IntervalVector& x) {
	const int n=size();
	assert(x.size()==n);
	if (!is_empty()) {
		if (x.is_empty()) { set_empty(); }
		else {
			for (int i=0; i<n; i++) {
				(*this)[i]-=x[i];
			}
		}
	}
	return *this;
}

#endif // __IBEX_GAOL_INTERVAL_VECTOR_H__
