//============================================================================
//                                  I B E X                                   
// File        : ibex_Vector.h
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Apr 17, 2012
// Last Update : Apr 17, 2012
//============================================================================

#ifndef __IBEX_VECTOR_H__
#define __IBEX_VECTOR_H__

#include <cassert>
#include <iostream>

namespace ibex {

/**
 * \ingroup arith
 *
 * \brief Vector of reals
 *
 */
class Vector {
public:
	/**
	 * \brief Create [0; ...; 0]
	 *
	 * Create a n-sized vector. All the components are 0
	 * \pre n>0
	 */
	Vector(int n);

	/**
	 * \brief Create [x; ....; x]
	 *
	 * Create a Vector of dimension \a n with
	 * all the components initialized to \a x.
	 * \pre n>0
	 */
	Vector(int n, double x);

	/**
	 * \brief Create a copy of \a x.
	 */
	Vector(const Vector& x);

	/**
	 * \brief Create the Vector [x[0]; ..; x[n]]
	 *
	 * \param a array of n doubles
	 * \pre n>0
	 */
	Vector(int n, double  x[]);

	/**
	 * \brief Delete this vector
	 */
	virtual ~Vector();

	/**
	 * \brief Return the ith double
	 *
	 * A return a const reference to the
	 * i^th double (i starts from 0)
	 */
	const double& operator[](int i) const;

	/**
	 * \brief Return the ith double
	 *
	 * A return a non-const reference to the
	 * i^th component (i starts from 0)
	 */
	double& operator[](int i);

	/**
	 * \brief Assign this Vector to x.
	 *
	 * \pre Dimensions of this and x must match.
	 */
	Vector& operator=(const Vector& x);

	/**
	 * \brief Return true if the components of this Vector match that of \a x.
	 */
	bool operator==(const Vector& x) const;

	/**
	 * \brief Return true if one component of *this differs from \a x.
	 */
	bool operator!=(const Vector& x) const;

	/**
	 * \brief The dimension (number of components)
	 */
	int size() const;

private:
	int n;             // dimension (size of vec)
	double *vec;	   // vector of elements
};

/**
 * \brief Display the Vector \a x
 */
std::ostream& operator<<(std::ostream& os, const Vector& x);

/*================================== inline implementations ========================================*/

inline Vector::Vector(int n) : n(n), vec(new double[n]) {
	assert(n>=1);
	for (int i=0; i<n; i++) vec[i]=0;
}

inline Vector::Vector(int n, double x) : n(n), vec(new double[n]) {
	assert(n>=1);
	for (int i=0; i<n; i++) vec[i]=x;
}

inline Vector::Vector(const Vector& x) : n(x.n), vec(new double[x.n]) {
	for (int i=0; i<n; i++) vec[i]=x[i];
}

inline Vector::Vector(int n, double x[]) : n(n), vec(new double[n]) {
	for (int i=0; i<n; i++)
		vec[i]=x[i];
}

inline Vector::~Vector() {
	delete[] vec;
}

inline const double& Vector::operator[](int i) const {
	assert(i>=0 && i<n);
	return vec[i];
}

inline double& Vector::operator[](int i) {
	assert(i>=0 && i<n);
	return vec[i];
}

inline Vector& Vector::operator=(const Vector& x) {
	assert(size()==x.size()); // throw InvalidVectorOp("Cannot set a Vector to a Vector with different dimension");

	for (int i=0; i<size(); i++)
		(*this)[i]=x[i];

	return *this;
}

inline bool Vector::operator==(const Vector& x) const {
	if (n!=x.size()) return false;
	for (int i=0; i<n; i++)
		if ((*this)[i]!=(x[i])) return false;
	return true;
}

inline bool Vector::operator!=(const Vector& x) const {
	return !(*this==x);
}

inline int Vector::size() const {
	return n;
}

inline std::ostream& operator<<(std::ostream& os, const Vector& x) {
	os << "(";
	for (int i=0; i<x.size(); i++)
		os << x[i] << (i<x.size()-1? " ; " : "");
	os << ")";
	return os;
}


} // end namespace ibex
#endif // __IBEX_VECTOR_H__
