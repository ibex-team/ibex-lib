/* ============================================================================
 * I B E X - Implementation of the IntervalVector class based on Profil/BIAS
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Mar 5, 2012
 * ---------------------------------------------------------------------------- */

#ifndef __IBEX_BIAS_INTERVAL_VECTOR_H__
#define __IBEX_BIAS_INTERVAL_VECTOR_H__

#include "IntervalVector.h"
#include "ibex_NonRecoverableException.h"
#include <cassert>

namespace ibex {

inline IntervalVector::IntervalVector(int n) : vec(n) {
}

inline IntervalVector::IntervalVector(int n, const Interval& x) : vec(n) {
	Initialize(vec,x.itv);
}

inline IntervalVector::IntervalVector(const IntervalVector& x) : vec(x.vec) {

}

inline IntervalVector::IntervalVector(int n, double  bounds[][2]) : vec(n) {
	for (int i=0; i<n; i++)
		vec(i+1) = INTERVAL(bounds[i][0], bounds[i][1]);
}

inline IntervalVector::~IntervalVector() {
  // nothing to do
}

inline const Interval& IntervalVector::operator[](int i) const {
	return vec(i+1);
}

inline Interval& IntervalVector::operator[](int i) {
	return vec(i+1);
}

inline void IntervalVector::resize(int n2) {
	int n=size();
	Resize(vec,n2);
	for (int i=n-1; i<n2; i++) vec(i+1)=INTERVAL(BiasNegInf,BiasPosInf);
}

inline IntervalVector& IntervalVector::operator=(const IntervalVector& x) {
	assert(size()==x.size()); // throw InvalidIntervalVectorOp("Cannot set a IntervalVector to a IntervalVector with different dimension");

	if (x.is_empty())
		set_empty();
	else
		vec = x.vec;

	return *this;
}

inline IntervalVector IntervalVector::operator&(const IntervalVector& x) const {
	if (is_empty() || x.is_empty()) return empty(size());
	INTERVAL_VECTOR tmp(size());
	Intersection(tmp, vec, x.vec);
	return tmp;
}

inline IntervalVector IntervalVector::operator|(const IntervalVector& x) const {
	if (is_empty()) return x;
	else if (x.is_empty()) return *this;
	else return Hull(vec, x.vec);
}

inline bool IntervalVector::operator==(const IntervalVector& x) const {
	return vec==x.vec;
}

inline int IntervalVector::size() const {
	return Dimension(vec);
}

inline IntervalVector IntervalVector::mid() const {
	return Mid(vec);
}

inline IntervalVector IntervalVector::operator-() const {
	if (is_empty()) return empty(size());
	else return IntervalVector(-vec);
}

inline IntervalVector IntervalVector::operator+(const IntervalVector& x) const {
	assert(x.size()==size());
	if (is_empty() || x.is_empty())
		return empty(size());
	else
		return IntervalVector(vec+x.vec);
}

inline IntervalVector& IntervalVector::operator+=(const IntervalVector& x) {
	assert(x.size()==size());
	if (!is_empty()) {
		if (x.is_empty())
			set_empty();
		else
			vec += x.vec;
	}
	return *this;
}

inline IntervalVector IntervalVector::operator-(const IntervalVector& x) const {
	assert(x.size()==size());
	if (is_empty() || x.is_empty())
		return empty(size());
	else
		return IntervalVector(vec-x.vec);
}

inline IntervalVector& IntervalVector::operator-=(const IntervalVector& x) {
	assert(x.size()==size());
	if (!is_empty()) {
		if (x.is_empty())
			set_empty();
		else
			vec -= x.vec;
	}
	return *this;
}

} // end namespace

#endif // __IBEX_BIAS_INTERVAL_VECTOR_H__
