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

namespace ibex {

Vector::Vector(int n) : n(n), vec(new double[n]) {
	assert(n>=1);
	for (int i=0; i<n; i++) vec[i]=0;
}

Vector::Vector(int n, double x) : n(n), vec(new double[n]) {
	assert(n>=1);
	for (int i=0; i<n; i++) vec[i]=x;
}

Vector::Vector(const Vector& x) : n(x.n), vec(new double[x.n]) {
	for (int i=0; i<n; i++) vec[i]=x[i];
}

Vector::Vector(int n, double x[]) : n(n), vec(new double[n]) {
	assert(n>=1);
	for (int i=0; i<n; i++) vec[i]=x[i];
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

Vector& Vector::operator=(const Vector& x) {
	assert(size()==x.size()); // throw InvalidVectorOp("Cannot set a Vector to a Vector with different dimension");

	for (int i=0; i<size(); i++)
		(*this)[i]=x[i];

	return *this;
}

bool Vector::operator==(const Vector& x) const {
	if (n!=x.size()) return false;
	for (int i=0; i<n; i++)
		if ((*this)[i]!=(x[i])) return false;
	return true;
}

std::ostream& operator<<(std::ostream& os, const Vector& x) {
	os << "(";
	for (int i=0; i<x.size(); i++)
		os << x[i] << (i<x.size()-1? " ; " : "");
	os << ")";
	return os;
}

} // end namespace ibex
