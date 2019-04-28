/* ============================================================================
 * I B E X - Template Domain Vector implementation
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Mar 21, 2013
 * ---------------------------------------------------------------------------- */

#ifndef __IBEX_TEMPLATE_VECTOR_H__
#define __IBEX_TEMPLATE_VECTOR_H__

#include "ibex_Interval.h"
#include "ibex_IntervalVector.h"
#include "ibex_Random.h"

#include <cassert>
#include <sstream>

namespace ibex {

namespace { // to create anonymous functions

template<class V>
inline V& _inflate(V& v, double rad) {
	if (v.is_empty()) return v;
	Interval r(-rad,rad);
	// little optim: we do not call (*this)[i].inflate(rad)
	// because this would create n times
	// the interval [-rad,rad]
	for (int i=0; i<v.size(); i++)
		v[i]+=r;
	return v;
}

template<class V>
inline V _subvector(const V& v, int start_index, int end_index) {
	//assert(!___is_empty(v));
	assert(end_index>=0 && start_index>=0);
	assert(end_index<v.size() && start_index<=end_index);
	//throw InvalidIntervalVectorOp("Invalid indices for IntervalVector::subvector");

	V v2(end_index-start_index+1);
	int j=0;
	for (int i=start_index; i<=end_index; i++) {
		v2[j++]=v[i];
	}
	return v2;
}

template<class V>
inline void _put(V& v, int start_index, const V& subvec) {
	assert(!___is_empty(v));
	assert(!___is_empty(subvec));
	int end_index=start_index+subvec.size()-1;
	assert(start_index>=0 && end_index<v.size());

	int j=0;
	for (int i=start_index; i<=end_index; i++) {
		v[i]=subvec[j++];
	}
}

template<class V1,class V2>
V1& _assignV(V1& v, const V2& x) {
	assert(v.size()==x.size()); // throw InvalidIntervalVectorOp("Cannot set a IntervalVector to a IntervalVector with different dimension");

	if (___is_empty(x))
		___set_empty(v);
	else
		// don't use "set(...)" because the test "is_empty()" called inside
		// may return prematurely in case "this" is empty.
		// use physical copy instead:
		for (int i=0; i<v.size(); i++)
			v[i]=x[i];

	return v;
}

template<class V1,class V2>
bool _equalsV(const V1& v, const V2& x) {
	if (v.size()!=x.size()) return false;
	if (___is_empty(v) || ___is_empty(x)) return ___is_empty(v) && ___is_empty(x);
	for (int i=0; i<v.size(); i++)
		if (v[i]!=(x[i])) return false;
	return true;
}


template<class V>
Vector _lb(const V& v) {
	assert(!v.is_empty());

	Vector l(v.size());
	for (int i=0; i<v.size(); i++) {
		l[i]=v[i].lb();
	}
	return l;
}

template<class V>
Vector _ub(const V& v) {
	assert(!v.is_empty());

	Vector u(v.size());
	for (int i=0; i<v.size(); i++) {
		u[i]=v[i].ub();
	}
	return u;
}

template<class V>
Vector _mid(const V& v) {
	assert(!v.is_empty());

	Vector mV(v.size());
	for (int i=0; i<v.size(); i++) {
		mV[i]=v[i].mid();
	}
	return mV;
}

template<class V>
Vector _mig(const V& v) {
	assert(!v.is_empty());

	Vector res(v.size());
	for (int i=0; i<v.size(); i++) {
		res[i]=v[i].mig();
	}
	return res;
}

template<class V>
Vector _mag(const V& v) {
	assert(!v.is_empty());

	Vector res(v.size());
	for (int i=0; i<v.size(); i++) {
		res[i]=v[i].mag();
	}
	return res;
}

template<class V>
bool _is_flat(const V& v) {
	if (v.is_empty()) return true;
	for (int i=0; i<v.size(); i++)
		if (v[i].is_degenerated()) // don't use diam() because of roundoff
			return true;
	return false;
}

template<class V>
bool _is_unbounded(const V& v) {
	if (v.is_empty()) return false;
	for (int i=0; i<v.size(); i++)
		if (v[i].is_unbounded()) return true;
	return false;
}

template<class V>
bool _is_zero(const V& v) {
	for (int i=0; i<v.size(); i++)
		if (v[i]!=Interval::zero()) return false;
	return true;
}

template<class V>
bool _is_bisectable(const V& v) {
	for (int i=0; i<v.size(); i++)
		if (v[i].is_bisectable()) return true;
	return false;
}

template<class V>
Vector _rad(const V& v) {
	Vector r(v.size());
	for (int i=0; i<v.size(); i++)
		r[i]=v[i].rad();
	return r;
}

template<class V>
Vector _diam(const V& v) {
	Vector d(v.size());
	for (int i=0; i<v.size(); i++)
		d[i]=v[i].diam();
	return d;
}

template<class V>
V _abs(const V& v) {
	V res(v.size());

	if (___is_empty(v)) { ___set_empty(res); return res; }

	for (int i=0; i<v.size(); i++)
		res[i]=___abs(v[i]);

	return res;
}

template<class V>
int _extr_diam_index(const V& v, bool min) {
	double d=min? POS_INFINITY : -1; // -1 to be sure that even a 0-diameter interval will be selected
	int selectedIndex=-1;
	bool unbounded=false; //v[0].is_unbounded();
	if (v.is_empty()) throw InvalidIntervalVectorOp("Diameter of an empty IntervalVector is undefined");
	int i;

	for (i=0; i<v.size(); i++) {
		if (v[i].is_unbounded()) {
			unbounded=true;
			if (!min) break;
		} else {
			double w=v[i].diam();
			if (min? w<d : w>d) {
				selectedIndex=i;
				d=w;
			}
		}
	}

	if (min && selectedIndex==-1) {
		assert(unbounded);
		// the selected interval is the first one.
		i=0;
	}

	// the unbounded intervals are not considered if we look for the minimal diameter
	// and some bounded intervals have been found (selectedIndex!=-1)
	if (unbounded && (!min || selectedIndex==-1)) {
		double pt = min? NEG_INFINITY : POS_INFINITY; // keep the point less/most distant from +oo (we normalize if the lower bound is -oo)
		selectedIndex=i;
		for (; i<v.size(); i++) {
			if (v[i].lb()==NEG_INFINITY) {
				if (v[i].ub()==POS_INFINITY) {
					if (!min) {
						selectedIndex=i;
						break;
					}
				}
				if ((min && (-v[i].ub() > pt)) || (!min && (-v[i].ub() < pt))) {
					selectedIndex=i;
					pt=-v[i].ub();
				}
			} else if (v[i].ub()==POS_INFINITY) {
				if ((min && (v[i].lb() > pt)) || (!min && (v[i].lb() < pt))) {
					selectedIndex=i;
					pt=v[i].lb();
				}
			}
		}
	}

	return selectedIndex;
}

template<class V>
std::ostream& _displayV(std::ostream& os, const V& x) {
	if (___is_empty(x)) return os << "empty vector";

	os << "(";
	for (int i=0; i<x.size(); i++)
		os << x[i] << (i<x.size()-1? " ; " : "");
	os << ")";
	return os;
}

template<class V>
double _volume(const V& v) {
	if (v[0].is_unbounded()) return POS_INFINITY;
	if (v[0].is_degenerated()) return 0;
	double vol=::log((v[0]).diam());
	for (int i=1; i<v.size(); i++) {
		if (v[i].is_unbounded()) return POS_INFINITY;
		if (v[i].is_degenerated()) return 0;
		vol+=::log((v[i]).diam());
	}
	return ::exp(vol);
}

template<class V>
double _perimeter(const V& v) {
	if (v[0].is_unbounded()) return POS_INFINITY;
	double per=(v[0]).diam();
	for (int i=1; i<v.size(); i++) {
		if (v[i].is_unbounded()) return POS_INFINITY;
		per+=(v[i]).diam();
	}
	return per;
}

template<class V1, class V2>
double _rel_distance(const V1& v, const V2& x) {
	double max = v[0].rel_distance(x[0]);
	for (int i=1; i<v.size(); i++) {
		double cand = v[i].rel_distance(x[i]);
		if (max<cand) max = cand;
	}
	return max;
}

template<class V>
std::pair<IntervalVector,IntervalVector> _bisect(const V& v, int i, double ratio) {
	assert(0<ratio && ratio<1.0);
	assert(0<=i && i<v.size());

	if (!v[i].is_bisectable()) {
		std::ostringstream oss;
		oss << "Unable to bisect " << v;
		throw InvalidIntervalVectorOp(oss.str());
	}
	IntervalVector left(v);
	IntervalVector right(v);

	std::pair<Interval,Interval> p=v[i].bisect(ratio);

	left[i] = p.first;
	right[i] = p.second;

	return std::pair<IntervalVector,IntervalVector>(left,right);
}

template<class V,class T>
Vector _random(const V& v,int seed) {
	assert(!v.is_empty());
	RNG::srand(seed);
	Vector b(v.size());
	for (int i=0; i<v.size(); i++) {
		const T& xi=v[i];
		// get a random number in [-0.5,0.5]
			double p;
		// pick a point in the domain of the ith variable
		if (xi.lb()==NEG_INFINITY)
			if (xi.ub()==POS_INFINITY)
				p=RNG::rand();
			else
				p=xi.ub()-RNG::rand();
		else
			if (xi.ub()==POS_INFINITY)
				p=xi.lb()+RNG::rand();
			else {
				//double r=(rand()/(double)RAND_MAX) -0.5;
				// p = xi.mid() + r*xi.diam();
				p = RNG::rand(xi.lb(), xi.ub());
			}

		// watch dog
		if (p<xi.lb()) p=xi.lb();
		else if (p>xi.ub()) p=xi.ub();
		b[i]=p;
	}
	return b;
}


template<class V,class T>
Vector _random(const V& v) {
	assert(!v.is_empty());
	Vector b(v.size());
	for (int i=0; i<v.size(); i++) {
		const T& xi=v[i];
		// get a random number in [-0.5,0.5]
			double p;
		// pick a point in the domain of the ith variable
		if (xi.lb()==NEG_INFINITY)
			if (xi.ub()==POS_INFINITY)
				p=RNG::rand();
			else
				p=xi.ub()-RNG::rand();
		else
			if (xi.ub()==POS_INFINITY)
				p=xi.lb()+RNG::rand();
			else {
				//double r=(rand()/(double)RAND_MAX) -0.5;
				// p = xi.mid() + r*xi.diam();
				p = RNG::rand(xi.lb(), xi.ub());
			}

		// watch dog
		if (p<xi.lb()) p=xi.lb();
		else if (p>xi.ub()) p=xi.ub();
		b[i]=p;
	}
	return b;
}


} // end anonymous namespace

} // end namespace ibex

#endif // __IBEX_TEMPLATE_VECTOR_H__
