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

#ifndef __IBEX_INTERVAL_VECTOR_H__
#define __IBEX_INTERVAL_VECTOR_H__

#include <cassert>
#include <iostream>
#include "ibex_Interval.h"
#include "ibex_InvalidIntervalVectorOp.h"
#include "ibex_Vector.h"

namespace ibex {

class IntervalMatrix; // declared only for friendship
class Domains;        // declared for operator=(const Domains&)

/**
 * \ingroup arith
 *
 * \brief Vector of Intervals
 *
 * By convention an empty vector has a dimension. A vector becomes empty
 * when one of its component becomes empty and all the components
 * are set to the empty Interval.
 */
class IntervalVector {

public:
	/**
	 * \brief Create [(-oo,+oo) ; ..; (-oo,+oo)]
	 *
	 * Create a n-sized vector. All the components are (-oo,+oo)
	 * \pre n>0
	 */
	IntervalVector(int n);

	/**
	 * \brief Create [x; ....; x]
	 *
	 * Create a IntervalVector of dimension \a n with
	 * all the components initialized to \a x.
	 * \pre n>0
	 */
	IntervalVector(int n, const Interval& x);

	/**
	 * \brief Create a copy of \a x.
	 */
	IntervalVector(const IntervalVector& x);

	/**
	 * \brief Create the IntervalVector [bounds[0][0],bounds[0][1]]x...x[bounds[n-1][0],bounds[n-1][1]]
	 *
	 * \param bounds an nx2 array of doubles
	 * \pre n>0
	 */
	IntervalVector(int n, double  bounds[][2]);

	/**
	 * \brief Create the degenerated IntervalVector x
	 *
	 */
	IntervalVector(const Vector& x);

	/**
	 * \brief Load domains into this interval vector.
	 */
	IntervalVector& operator=(const Domains&); // REM: Implemented in ibex_Domains.cpp

	/**
	 * \brief Create [empty; ...; empty]
	 *
	 * Create an empty IntervalVector of dimension \a n
	 * (all the components being empty Intervals)
	 *
	 * \pre n>0
	 */
	static IntervalVector empty(int n);

	/**
	 * \brief Delete this vector
	 */
	virtual ~IntervalVector();

	/**
	 * \brief Return the ith Interval
	 *
	 * A return a const reference to the
	 * i^th component (i starts from 0)
	 */
	const Interval& operator[](int i) const;

	/**
	 * \brief Return the ith Interval
	 *
	 * A return a non-const reference to the
	 * i^th component (i starts from 0)
	 */
	Interval& operator[](int i);

	/**
	 * \brief Set this IntervalVector to the empty IntervalVector
	 *
	 * The dimension remains the same.
	 */
	void set_empty();

	/**
	 * \brief Resize this IntervalVector.
	 *
	 * If the size is increased, the existing components are not
	 * modified and the new ones are set to (-inf,+inf), even if
	 * (*this) is the empty Interval (however, in this case, the status of
	 * (*this) remains "empty").
	 */
	void resize(int n2);

	/**
	 * \brief Return a subvector.
	 *
	 * \pre (*this) must not be empty
	 * \return [ (*this)[start_index]; ...; (*this)[end_index] ].
	 */
	IntervalVector subvector(int start_index, int end_index);

	/**
	 * \brief Assign this IntervalVector to x.
	 *
	 * \pre Dimensions of this and x must match. 
	 * \note Emptiness is overridden.
	 */
	IntervalVector& operator=(const IntervalVector& x);

	/**
	 * \brief Set *this to its intersection with x
	 *
	 * \return a reference to this.
	 * \throws InvalidIntervalVectorOp if the vectors do not have the same dimensions.
	 */
	IntervalVector& operator&=(const IntervalVector& x);

	/**
	 * \brief Set this IntervalVector to the hull of itself and another.
	 *
	 * \return a reference to this.
	 * \throws InvalidIntervalVectorOp if IntervalVectores do not have the same dimensions.
	 */
	IntervalVector& operator|=(const IntervalVector& x);

	/**
	 * \brief Return the intersection of this and x.
	 */
	IntervalVector operator&(const IntervalVector& x) const;

	/**
	 * \brief Return the hull of this & x.
	 */
	IntervalVector operator|(const IntervalVector& x) const;

	/**
	 * \brief Return true if the bounds of this IntervalVector match that of \a x.
	 */
	bool operator==(const IntervalVector& x) const;

	/**
	 * \brief Return true if one bounds of one component of *this differs from \a x.
	 */
	bool operator!=(const IntervalVector& x) const;

	/**
	 * \brief The dimension (number of components)
	 */
	int size() const;

	/**
	 * \brief Return the midpoint
	 * \pre (*this) must be nonempty
	 */
	Vector mid() const;

	/**
	 * \brief Return true iff this IntervalVector is empty
	 */
	bool is_empty() const;

	/**
	 * \brief Return true iff this IntervalVector is flat.
	 *
	 * An IntervalVector is "flat" if the radius is 0 on at least one dimension
	 * An empty interval vector is considered as flat.
	 */
	bool is_flat() const;

	/**
	 * \brief True iff this interval vector contains \a x.
	 *
	 * \pre Dimension of \a x must be equal to the dimension of (*this).
	 * \sa #ibex::Interval::contains(double) const.
	 */
	bool contains(const Vector& x) const;

	/**
	 * \brief true iff this interval vector contains an infinite bound.
	 *
	 * \note An empty interval vector is always bounded.
	 */
	bool is_unbounded() const;

	/**
	 * \brief True iff this interval vector is a subset of \a x.
	 *
	 * \pre Dimension of \a x must be equal to the dimension of this vector.

	 * \note Always return true if this interval vector is empty.

	 * \sa #ibex::Interval::is_subset(const Interval&) const.
	 */
	bool is_subset(const IntervalVector& x) const;

	/**
	 * \brief True iff this interval vector is inside the interior of \a x.
	 *
	 * \pre Dimension of \a x must be equal to the dimension of this vector.
	 *
	 * \note return true if this interval vector is empty and \a x not.
	 *
	 * \sa #ibex::Interval::is_strict_subset(const Interval&) const.
	 */
	bool is_strict_subset(const IntervalVector& x) const;

	/**
	 * \brief True iff this interval vector is a superset of \a x.
	 *
	 * \pre Dimension of \a x must be equal to the dimension of this vector.

	 * \note Always return true if \a x is empty.

	 * \sa #ibex::Interval::is_superset(const Interval&) const.
	 */
	bool is_superset(const IntervalVector& x) const;

	/**
	 * \brief True iff \a x is inside the interior of (*this).
	 *
	 * \pre Dimension of \a x must be equal to the dimension of this vector.
	 *
	 * \note return true if x is empty and not (*this).
	 *
	 * \sa #ibex::Interval::is_strict_superset(const Interval&) const.
	 */
	bool is_strict_superset(const IntervalVector& x) const;

	/**
	 * \brief Return the index of a component with minimal/maximal diameter.
	 *
	 *  \param min true => minimal diameter
	 *  \throws InvalidIntervalVectorOp if the IntervalVector is empty.
	 */
	int extr_diam_index(bool min) const;

	/**
	 * \brief Return the maximal diameter among all the components.
	 *
	 *  \throws InvalidIntervalVectorOp if the IntervalVector is empty.
	 */
	double max_diam() const;

	/**
	 * \brief Return the minimal diameter among all the components.
	 *
	 * \throws InvalidIntervalVectorOp if the IntervalVector is empty.
	 */
	double min_diam() const;

	/**
	 * \brief Return the volume of this interval vector.
	 *
	 * \note Return \c POS_INFINITY if the vector is unbounded and not flat.
	 * \note Return 0 if the vector is flat and not unbounded.
	 * \warning If the interval vector is both flat and unbounded, the result is undefined.
	 * \sa #flat()
	 * \sa #unbounded()
	 */
	double volume() const;

	/**
	 * \brief Return the perimeter of this interval vector.
	 *
	 * \note Return \c POS_INFINITY if unbounded.
	 */
	double perimeter() const;

	/**
	 * \brief Return the relative distance with x.
	 *
	 * \return \f$\displaystyle \max_{i=1..n} rel_distance([this]_i, x_i)/diam([this]_i)\f$.
	 *
	 * \sa #ibex::distance(const IntervalVector& x1, const IntervalVector& x2).
	 * \sa #Interval::rel_distance(const Interval& x) const.
	 */
	double rel_distance(const IntervalVector& x) const;


	/**
	 * \brief Return *this \ y (set difference).
	 *
	 * Store the difference under the form of a union of non-overlapping IntervalVectors
	 * into \a result, and return the size of the union.
	 *
	 * <p>
	 * If the difference is empty, \a result is an array of one element set to the empty box.
	 * It is <b>not</b> a zero-sized array containing no element.
	 */
	int diff(const IntervalVector& y, IntervalVector*& result) const;

	/**
	 * \brief Return the complementary of *this.
	 *
	 * Store the complementary under the form of a union of non-overlapping IntervalVectors,
	 * into \a result, and return the size of the union.
	 *
	 * <p>
	 * If (*this) is the empty set with n components, the complementary of (*this) is
	 * the n-dimensional box (-oo,oo)x...(-oo,oo).
	 *
	 * <p>
	 * If the complementary is empty, \a result is an array of one element set to the empty box.
	 * It is <b>not</b> a zero-sized array containing no element.
	 */
	int complementary(IntervalVector*& result) const;

	/**
	 * \brief Return a random vector (degenerated box) inside *this.
	 *
	 * \pre (*this) must be nonempty.
	 */
	IntervalVector random() const;

private:
	friend class IntervalMatrix;

	IntervalVector() : n(0), vec(NULL) { } // for IntervalMatrix & complementary()

	int n;             // dimension (size of vec)
	Interval *vec;	   // vector of elements
};

/**
 * \brief Return the infinite Hausdorff distance (i.e. the maximum of the distance componentwise).
 *
 * \return \f$\displaystyle \max_{i=1..n} distance(x1_i, x2_i)\f$.
 *
 * \pre Dimension of \a x1 and \a x2 must be equal.
 *
 * \sa #ibex::distance(const Interval&, const Interval&) const.
 */
double distance(const IntervalVector& x1, const IntervalVector& x2);

/** \brief -x. */
Vector operator-(const Vector& x);
/** \brief -x. */
IntervalVector operator-(const IntervalVector& x);

/** \brief x1+x2. */
Vector         operator+(const Vector& x1,         const Vector& x2);
/** \brief x1+x2. */
IntervalVector operator+(const Vector& x1,         const IntervalVector& x2);
/** \brief x1+x2. */
IntervalVector operator+(const IntervalVector& x1, const Vector& x2);
/** \brief x1+x2. */
IntervalVector operator+(const IntervalVector& x1, const IntervalVector& x2);

/** \brief x1-x2. */
Vector         operator-(const Vector& x1,         const Vector& x2);
/** \brief x1-x2. */
IntervalVector operator-(const Vector& x1,         const IntervalVector& x2);
/** \brief x1-x2. */
IntervalVector operator-(const IntervalVector& x1, const Vector& x2);
/** \brief x1-x2. */
IntervalVector operator-(const IntervalVector& x1, const IntervalVector& x2);

/** \brief x1*x2. */
double   operator*(const Vector& x1,         const Vector& x2);
/** \brief x1*x2. */
Interval operator*(const Vector& x1,         const IntervalVector& x2);
/** \brief x1*x2. */
Interval operator*(const IntervalVector& x1, const Vector& x2);
/** \brief x1*x2. */
Interval operator*(const IntervalVector& x1, const IntervalVector& x2);

/** \brief x1+=x2. */
Vector&         operator+=(Vector& x1,         const Vector& x2);
/** \brief x1+=x2. */
IntervalVector& operator+=(IntervalVector& x1, const Vector& x2);
/** \brief x1+=x2. */
IntervalVector& operator+=(IntervalVector& x1, const IntervalVector& x2);

/** \brief x1-=x2. */
Vector&         operator-=(Vector& x1,         const Vector& x2);
/** \brief x1-=x2. */
IntervalVector& operator-=(IntervalVector& x1, const Vector& x2);
/** \brief x1-=x2. */
IntervalVector& operator-=(IntervalVector& x1, const IntervalVector& x2);

/** \brief d*x */
Vector operator*(double d, const Vector& x);
/** \brief d*x */
IntervalVector operator*(double d, const IntervalVector& x);
/** \brief x1*x2. */
IntervalVector operator*(const Interval& x1, const Vector& x2);
/** \brief x1*x2. */
IntervalVector operator*(const Interval& x1, const IntervalVector& x2);

/** \brief x=d*x */
Vector& operator*=(Vector& x, double d);
/** \brief x=d*x */
IntervalVector& operator*=(IntervalVector& x, double d);
/** \brief x2=x1*x2. */
IntervalVector& operator*=(IntervalVector& x2, const Interval& x1);

/**
 * \brief Display the IntervalVector \a x
 */
std::ostream& operator<<(std::ostream& os, const IntervalVector& x);

} // end namespace

// --------- inline implementation --------------------

#include "ibex_IntervalVector.h_"


#endif /* _IBEX_INTERVAL_VECTOR_H_ */
