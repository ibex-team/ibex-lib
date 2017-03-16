//============================================================================
//                                  I B E X                                   
// File        : ibex_Vector.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Apr 18, 2012
// Last Update : Apr 18, 2012
//============================================================================

#include "ibex_Vector.h"
#include <float.h>
#include <math.h>
#include "ibex_TemplateVector.h"

namespace ibex {

Vector::Vector(int nn) : n(nn), vec(new double[nn]) {
	assert(nn>=1);
	for (int i=0; i<nn; i++) vec[i]=0;
}

Vector::Vector(int nn, double x) : n(nn), vec(new double[nn]) {
	assert(nn>=1);
	for (int i=0; i<nn; i++) vec[i]=x;
}

Vector::Vector(const Vector& x) : n(x.n), vec(new double[x.n]) {
	for (int i=0; i<n; i++) vec[i]=x[i];
}

Vector::Vector(int nn, double x[]) : n(nn), vec(new double[nn]) {
	assert(nn>=1);
	for (int i=0; i<nn; i++) vec[i]=x[i];
}

Vector::~Vector() {
	delete[] vec;
}

void Vector::resize(int n2) {
	assert(n2>=1);
	assert((vec==NULL && n==0) || (n!=0 && vec!=NULL));

	if (n2==size()) return;

	double* newVec=new double[n2];
	int i=0;
	for (; i<size() && i<n2; i++)
		newVec[i]=vec[i];
	for (; i<n2; i++)
		newVec[i]=0.0;
	if (vec!=NULL) // vec==NULL happens when default constructor is used (n==0)
		delete[] vec;

	n   = n2;
	vec = newVec;
}

double Vector::min() const {
	double res=DBL_MAX;
	for (int i=0; i<n; i++)
		if ((*this)[i]<res) res=(*this)[i];
	return res;
}

double Vector::max() const {
	double res=-DBL_MAX;
	for (int i=0; i<n; i++)
		if ((*this)[i]>res) res=(*this)[i];
	return res;
}


double norm(const Vector& x) {
	double n=0;
	for (int i=0; i<x.size(); i++)
		n+=::pow(x[i],2);
	return ::sqrt(n);
}

double infinite_norm(const Vector& v) {
    double norm = ___mag(v[0]);
    for (int i=1; i<v.size(); i++) {
        double x = ___mag(v[i]);
        if (x > norm)
            norm = x;
    }
    return norm;
}

Vector  Vector::subvector(int start_index, int end_index) const   { return _subvector(*this,start_index,end_index); }
void    Vector::put(int start_index, const Vector& x)             { _put(*this, start_index, x); }
Vector& Vector::operator=(const Vector& x)                        { resize(x.size()); // see issue #10
                                                                    return _assignV(*this,x); }
Vector abs(const Vector& v)                                       { return _abs(v); }
bool   Vector::operator==(const Vector& x) const                  { return _equalsV(*this,x); }
std::ostream&   operator<<(std::ostream& os, const Vector& x)     { return _displayV(os,x); }

} // end namespace ibex
