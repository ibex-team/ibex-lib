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
#include "ibex_Domain.h"
#include <vector>
#include <stdlib.h>
#include <math.h>

namespace ibex {

IntervalVector& IntervalVector::operator=(const Domain& d) {
	int i=0;
	assert (size()==d._size);
	std::vector<Dim>::const_iterator it;
	std::vector<void*>::const_iterator it2;
	for (it=d.symbol_dims.begin(), it2=d.doms.begin(); it!=d.symbol_dims.end(); it++,it2++) {
		Dim dim=*it;
		switch (dim.type()) {
		case Dim::SCALAR:
			(*this)[i++]=*((Interval*) *it2);
			break;
		case Dim::ROW_VECTOR:
		{
			const IntervalVector& v=(*((IntervalVector*) *it2));
			for (int j=0; j<dim.dim2; j++)
				(*this)[i++]=v[j];
		}
		break;
		case Dim::COL_VECTOR:
		{
			const IntervalVector& v=(*((IntervalVector*) *it2));
			for (int j=0; j<dim.dim3; j++)
				(*this)[i++]=v[j];
		}
		break;
		case Dim::MATRIX:
		{
			const IntervalMatrix& M=(*((IntervalMatrix*) *it2));
			for (int k=0; k<dim.dim2; k++)
				for (int j=0; j<dim.dim3; j++)
					(*this)[i++]=M[k][j];
		}
		break;
		case Dim::MATRIX_ARRAY:
		{
			const IntervalMatrixArray& A=(*((IntervalMatrixArray*) *it2));
			for (int l=0; l<dim.dim1; l++)
				for (int k=0; k<dim.dim2; k++)
					for (int j=0; j<dim.dim3; j++)
						(*this)[i++]=A[l][k][j];
		}
		break;
		}
	}
	return *this;
}

double IntervalVector::volume() const {
	if ((*this)[0].is_unbounded()) return POS_INFINITY;
	if ((*this)[0].is_degenerated()) return 0;
	double vol=::log(((*this)[0]).diam());
	for (int i=1; i<size(); i++) {
		if ((*this)[i].is_unbounded()) return POS_INFINITY;
		if ((*this)[i].is_degenerated()) return 0;
		vol+=::log(((*this)[i]).diam());
	}
	return ::exp(vol);
}

double IntervalVector::perimeter() const {
	if ((*this)[0].is_unbounded()) return POS_INFINITY;
	double per=((*this)[0]).diam();
	for (int i=1; i<size(); i++) {
		if ((*this)[i].is_unbounded()) return POS_INFINITY;
		per+=((*this)[i]).diam();
	}
	return per;
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

double IntervalVector::rel_distance(const IntervalVector& x) const {
	double max = (*this)[0].rel_distance(x[0]);
	for (int i=1; i<size(); i++) {
		double cand = (*this)[i].rel_distance(x[i]);
		if (max<cand) max = cand;
	}
	return max;
}

namespace { // to create anonymous structure/functions

/** \brief Complementary of an Interval
 *
 * Compute the complementary of x. The result is (c1 union c2)
 */
void complI(const Interval& x, Interval& c1, Interval& c2) {
	if (x.is_empty() || x.is_degenerated()) { // x.is_empty() should not happen if called from compl()
		c1=Interval::ALL_REALS;
		c2=Interval::EMPTY_SET;
		return;
	}
	else {
		if (x.lb()>NEG_INFINITY) {
			c1=Interval(NEG_INFINITY,x.lb());
			if (x.ub()<POS_INFINITY)
				c2=Interval(x.ub(),POS_INFINITY);
			else
				c2=Interval::EMPTY_SET;
		} else if (x.ub()<POS_INFINITY) {
			c1=Interval(x.ub(),POS_INFINITY);
			c2=Interval::EMPTY_SET;
		} else {
			c1=c2=Interval::EMPTY_SET;
		}
	}
}

/** \brief x\y
 *
 */
void diffI(const Interval& x, const Interval& y, Interval& c1, Interval& c2) {
	complI(y,c1,c2);
	c1 &= x;
	if (c1.is_degenerated()) c1=Interval::EMPTY_SET;
	c2 &= x;
	if (c2.is_degenerated()) c2=Interval::EMPTY_SET;

	if (c1.is_empty()) {
		c2=c1;
		c2=Interval::EMPTY_SET;
	}
}

} // end namespace


int IntervalVector::diff(const IntervalVector& y, IntervalVector*& result) const {
	const int n=size();
	const IntervalVector& x=*this;
	IntervalVector *tmp = new IntervalVector[2*n]; // in the worst case, there is 2n boxes
	Interval c1, c2;
	int b=0;
	if (y.is_empty()) {
		tmp[b].resize(n);
		tmp[b]=x; // copy of this
		b++;
	} else {
		for (int var=0; var<n; var++) {

			diffI(x[var],y[var],c1,c2);

			if (!c1.is_empty()) {
				tmp[b].resize(n);
				IntervalVector& v=tmp[b++];
				for (int i=0; i<var; i++) v[i]=y[i];
				v[var]=c1;
				for (int i=var+1; i<n; i++) v[i]=x[i];

				if (!c2.is_empty()) {
					tmp[b].resize(n);
					IntervalVector& v=tmp[b++];
					for (int i=0; i<var; i++) v[i]=y[i];
					v[var]=c2;
					for (int i=var+1; i<n; i++) v[i]=x[i];
				}
			}
		}
	}

	if (b==0) {
		result = new IntervalVector[1];
		result[0].resize(n);
		result[0].set_empty();
		b=1;
	} else {
		result=new IntervalVector[b];
		for (int i=0; i<b; i++) {
			result[i].resize(n);
			result[i]=tmp[i];
		}
	}
	delete[] tmp;

	return b;
}

int IntervalVector::complementary(IntervalVector*& result) const {
	return IntervalVector(size()).diff(*this,result);
}

IntervalVector IntervalVector::random() const {
	assert(!is_empty());

	IntervalVector b(size());
	for (int i=0; i<size(); i++) {
		const Interval& xi=(*this)[i];
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

} // end namespace
