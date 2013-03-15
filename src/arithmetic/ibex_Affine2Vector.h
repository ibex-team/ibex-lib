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

#ifndef __IBEX_AFFINE2_VECTOR_H__
#define __IBEX_AFFINE2_VECTOR_H__

#include <cassert>
#include <iostream>
#include "ibex_Interval.h"
#include "ibex_Affine2.h"
#include "ibex_IntervalVector.h"
#include "ibex_Vector.h"

namespace ibex {

/**
 * \ingroup arithmetic
 *
 * \brief Vector of Affine2 form
 *
 * By convention an empty vector has a dimension. A vector becomes empty
 * when one of its component becomes empty and all the components
 * are set to the empty Interval.
 */
class Affine2Vector {


private:
	friend class Affine2Matrix;

	Affine2Vector() : _n(0), _vec(NULL) { }

	int _n;             // dimension (size of vec)
	Affine2 *_vec;	   // vector of elements

public:

	/** \brief  Create \a n Affine2 form . All the components are Affine2([-oo,+oo])
	 * \pre n>0
	 */
	explicit Affine2Vector(int n);

	/**
	 * \brief  Create \a n Affine2Vector of dimension \a n with
	 * all the components initialized to \a x.
	 * \pre n>0
	 */
	Affine2Vector(int n, const Interval& x);

	/**
	 * \brief  Create \a n Affine2Vector of dimension \a n with
	 * all the components initialized to \a x.
	 * \pre n>0
	 */
	Affine2Vector(int n, const Affine2& x);

	/**
	 * \brief Create a copy of \a x.
	 */
	Affine2Vector(const Affine2Vector& x);


	/**
	 * \brief Create  a copy of \a -x if (b) else xif (!b) with x.
	 */
	Affine2Vector(const Affine2Vector& x, bool b);

	/**
	 * \brief Create \a n Affine2Vector  initialized by [bounds[i][0],bounds[i][1]]
	 *
	 * \param bounds an nx2 array of doubles
	 * \pre n>0
	 */
	Affine2Vector(int n, double  bounds[][2]);

	/**
	 * \brief Create \a x.size Affine2Vector of dimension \a x.size with
	 * the [i] component initialized to \a Affine2(x.size(), i+1,x[i]).
	 */
	Affine2Vector(const IntervalVector& x);

	/**
	 * \brief Create the degenerated Affine2Vector x
	 *
	 */
	Affine2Vector(const Vector& x);

	/**
	 * \brief Create [empty; ...; empty]
	 *
	 * Create an empty Affine2Vector of dimension \a n
	 * (all the components being empty Intervals)
	 *
	 * \pre n>0
	 */
	static Affine2Vector empty(int n);

	/**
	 * \brief Delete this vector
	 */
	virtual ~Affine2Vector();

	/**
	 * \brief Return the ith Affine2
	 *
	 * A return a const reference to the
	 * i^th component (i starts from 0)
	 */
	const Affine2& operator[](int i) const;

	/**
	 * \brief Return the ith Affine2
	 *
	 * A return a non-const reference to the
	 * i^th component (i starts from 0)
	 */
	Affine2& operator[](int i);

	/**
	 * \brief Set this Affine2Vector to the empty Affine2Vector
	 *
	 * The dimension remains the same.
	 */
	void set_empty();

	/**
	 * \brief Set all the elements to 0 (even if empty).
	 *
	 * \note Emptiness is "overridden".
	 */
	void clear();

	/**
	 * \brief Set all the elements to x (even if empty).
	 *
	 * \note Emptiness is "overridden".
	 */
	void init(const Interval& x);
	void init(const Affine2& x);

	/**
	 * \brief Add [-rad,+rad] to all the components of *this.
	 *
	 * \return *this.
	 */
	Affine2Vector& inflate(double rad);

	/**
	 * \brief Resize this Affine2Vector.
	 *
	 * If the size is increased, the existing components are not
	 * modified and the new ones are set to (ZERO), even if
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
	Affine2Vector subvector(int start_index, int end_index) const;

	/**
	 * \brief Put a subvector into *this at a given position.
	 *
	 * \param start_index - the position where the subvector
	 * \param subvec - the subvector
	 *
	 * \pre (*this) must not be empty
	 */
	void put(int start_index, const Affine2Vector& subvec);

	/**
	 * \brief Assign this Affine2Vector to x.
	 *
	 * \pre Dimensions of this and x must match.
	 * \note Emptiness is overridden.
	 */
	Affine2Vector& operator=(const Affine2Vector& x);
	Affine2Vector& operator=(const IntervalVector& x);

	/**
	 * \brief Return true if the bounds of this Affine2Vector match that of \a x.
	 */
	bool operator==(const Affine2Vector& x) const;
	bool operator==(const IntervalVector& x) const;

	/**
	 * \brief Return true if one bounds of one component of *this differs from \a x.
	 */
	bool operator!=(const IntervalVector& x) const;
	bool operator!=(const Affine2Vector& x) const;


	/**
	 * \brief Return the IntervalVector compose by the interval of each Affine2 form
	 * \pre (*this) must be nonempty
	 */
	IntervalVector itv() const;

	/**
	 * \brief The dimension (number of components)
	 */
	int size() const;

	/**
	 * \brief Return the lower bound vector
	 * \pre (*this) must be nonempty
	 */
	Vector lb() const;

	/**
	 * \brief Return the upper bound vector
	 * \pre (*this) must be nonempty
	 */
	Vector ub() const;

	/**
	 * \brief Return the midpoint
	 * \pre (*this) must be nonempty
	 */
	Vector mid() const;

	/**
	 * \brief Return the mignitude vector.
	 * \pre (*this) must be nonempty
	 */
	Vector mig() const;

	/**
	 * \brief Return the magnitude vector.
	 * \pre (*this) must be nonempty
	 */
	Vector mag() const;

	/**
	 * \brief Return true iff this Affine2Vector is empty
	 */
	bool is_empty() const;

	/**
	 * \brief Return true iff this Affine2Vector is flat.
	 *
	 * An Affine2Vector is "flat" if the radius is 0 on at least one dimension
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
	bool is_subset(const Affine2Vector& x) const;
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
	bool is_strict_subset(const Affine2Vector& x) const;
	bool is_strict_subset(const IntervalVector& x) const;

	/**
	 * \brief True iff this interval vector is a superset of \a x.
	 *
	 * \pre Dimension of \a x must be equal to the dimension of this vector.

	 * \note Always return true if \a x is empty.

	 * \sa #ibex::Interval::is_superset(const Interval&) const.
	 */
	bool is_superset(const Affine2Vector& x) const;
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
	bool is_strict_superset(const Affine2Vector& x) const;
	bool is_strict_superset(const IntervalVector& x) const;

	/**
	 * \brief True iff *this is a vector of zeros.
	 */
	bool is_zero() const;

	/**
	 * \brief True iff *this can be bisected along one dimension.
	 *
	 * \sa #ibex::Interval::is_bisectable().
	 */
	bool is_bisectable() const;

	/**
	 * \brief Vector of radii.
	 */
	Vector rad() const;

	/**
	 * \brief Return the vector of diameters.
	 */
	Vector diam() const;

	/**
	 * \brief Return the index of a component with minimal/maximal diameter.
	 *
	 *  \param min true => minimal diameter
	 *  \throws InvalidIntervalVectorOp if the Affine2Vector is empty.
	 */
	int extr_diam_index(bool min) const;


	/**
	 * \brief Return the indices of all the components, sorted by increasing/decreasing diameter.
	 */
	void sort_indices(bool min, int tab[]) const;

	/**
	 * \brief Return the maximal diameter among all the components.
	 *
	 *  \throws InvalidIntervalVectorOp if the Affine2Vector is empty.
	 */
	double max_diam() const;

	/**
	 * \brief Return the minimal diameter among all the components.
	 *
	 * \throws InvalidIntervalVectorOp if the Affine2Vector is empty.
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
	  * \brief Return max of the delta, for x\subseteq *this [deprecated]
	  *
	  * Deprecated. Kept for compatibility with ibex 1.xx.
	  */
	double maxdelta(const Affine2Vector&);
	double maxdelta(const IntervalVector&);

	/**
	 * \brief Return the relative distance with x.
	 *
	 * \return \f$\displaystyle \max_{i=1..n} rel\_distance([this]_i, x_i)/diam([this]_i)\f$.
	 *
	 * \sa #ibex::distance(const Affine2Vector& x1, const Affine2Vector& x2).
	 * \sa #ibex::Interval::rel_distance(const Interval& x) const.
	 */
	double rel_distance(const Affine2Vector& x) const;
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
	int diff(const Affine2Vector& y, IntervalVector*& result) const;

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
	 * \brief Bisect the box
	 *
	 * The box is bisected along the dimension \a i
	 * and with a ratio \a ratio. If (*this)[i] is the interval [a,a+d]:
	 * <ul>
	 * <li> The first box of the result is (*this)[0]x...x(*this)[i-1]x[a+ratio*d]x...
	 * <li> The second box is (*this)[0]x...x(*this)[i-1]x[a+ratio*d,a+d]x...
	 * </ul>
	 * Default value for the ratio is 0.5.
	 * \pre 0<ratio<1
	 */
	std::pair<IntervalVector,IntervalVector> bisect(int i, double ratio) const;

	/**
	 * \brief Return a random vector inside *this.
	 *
	 * \pre (*this) must be nonempty.
	 */
	Vector random() const;

	/**
	 * \brief (*this)+=x2.
	 */
	Affine2Vector& operator+=(const Vector& x2);

	/**
	 * \brief (*this)+=x2.
	 */
	Affine2Vector& operator+=(const IntervalVector& x2);
	Affine2Vector& operator+=(const Affine2Vector& x2);

	/**
	 * \brief (*this)-=x2.
	 */
	Affine2Vector& operator-=(const Vector& x2);

	/**
	 * \brief (*this)-=x2.
	 */
	Affine2Vector& operator-=(const IntervalVector& x2);
	Affine2Vector& operator-=(const Affine2Vector& x2);

	/**
	 * \brief x=d*x
	 */
	Affine2Vector& operator*=(double d);

	/**
	 * \brief (*this)=x1*(*this).
	 */
	Affine2Vector& operator*=(const Interval& x1);
	Affine2Vector& operator*=(const Affine2& x1);


};

/** \ingroup arithmetic */
/*@{*/



/**
 * \brief Return the intersection of x and y.
 */
IntervalVector operator&(const Affine2Vector& x, const Affine2Vector& y) const;
IntervalVector operator&(const IntervalVector& x, const Affine2Vector& y) const;
IntervalVector operator&(const Affine2Vector& x, const IntervalVector& y) const;

/**
 * \brief Return the hull of x & y.
 */
IntervalVector operator|(const Affine2Vector& x, const Affine2Vector& y) const;
IntervalVector operator|(const IntervalVector& x, const Affine2Vector& y) const;
IntervalVector operator|(const Affine2Vector& x, const IntervalVector& y) const;

/**
 * \brief Return the infinite Hausdorff distance (i.e. the maximum of the distance componentwise).
 *
 * \return \f$\displaystyle \max_{i=1..n} distance(x1_i, x2_i)\f$.
 *
 * \pre Dimension of \a x1 and \a x2 must be equal.
 *
 * \sa #ibex::distance(const Interval&, const Interval&).
 */
double distance(const IntervalVector& x1, const Affine2Vector& x2);
double distance(const Affine2Vector& x1, const IntervalVector& x2);
double distance(const Affine2Vector& x1, const Affine2Vector& x2);

/**
 * \brief -x.
 */
Affine2Vector operator-(const Affine2Vector& x);

/**
 * \brief x1+x2.
 */

Affine2Vector operator+(const Vector& x1, const Affine2Vector& x2);

/**
 * \brief x1+x2.
 */
Affine2Vector operator+(const Affine2Vector& x1, const Vector& x2);

/**
 * \brief x1+x2.
 */
Affine2Vector operator+(const Affine2Vector& x1, const IntervalVector& x2);
Affine2Vector operator+(const IntervalVector& x1, const Affine2Vector& x2);
Affine2Vector operator+(const Affine2Vector& x1, const Affine2Vector& x2);

/**
 * \brief x1-x2.
 */
Affine2Vector operator-(const Vector& x1, const Affine2Vector& x2);

/**
 * \brief x1-x2.
 */

Affine2Vector operator-(const Affine2Vector& x1, const Vector& x2);

/**
 * \brief x1-x2.
 */
Affine2Vector operator-(const Affine2Vector& x1, const IntervalVector& x2);
Affine2Vector operator-(const IntervalVector& x1, const Affine2Vector& x2);
Affine2Vector operator-(const Affine2Vector& x1, const Affine2Vector& x2);

/**
 * \brief x1*x2.
 */
Affine2 operator*(const Vector& x1, const Affine2Vector& x2);

/**
 * \brief x1*x2.
 */
Affine2 operator*(const Affine2Vector& x1, const Vector& x2);

/**
 * \brief x1*x2.
 */
Affine2 operator*(const Affine2Vector& x1, const IntervalVector& x2);
Affine2 operator*(const IntervalVector& x1, const Affine2Vector& x2);
Affine2 operator*(const Affine2Vector& x1, const Affine2Vector& x2);

/**
 * \brief d*x
 */
Affine2Vector operator*(double d, const Affine2Vector& x);

/**
 * \brief x1*x2.
 */
Affine2Vector operator*(const Affine2& x1, const Vector& x2);

/**
 *  \brief x1*x2.
 */
Affine2Vector operator*(const Affine2& x1, const Affine2Vector& x2);
Affine2Vector operator*(const Interval& x1, const Affine2Vector& x2);

/**
 * \brief |x|.
 */
Affine2Vector abs(const Affine2Vector& x);

/**
 * \brief Projection of $y=x_1+x_2$.
 *
 * Set $([x]_1,[x]_2)$ to $([x]_1,[x]_2])\cap\{ (x_1,x_2)\in [x]_1\times[x]_2 \ | \ \exists y\in[y],\ y=x_1+x_2\}$.
 */
bool proj_add(const Affine2Vector& y, Affine2Vector& x1, Affine2Vector& x2);
bool proj_add(const IntervalVector& y, Affine2Vector& x1, Affine2Vector& x2);
bool proj_add(const Affine2Vector& y, IntervalVector& x1, Affine2Vector& x2);
bool proj_add(const Affine2Vector& y, Affine2Vector& x1, IntervalVector& x2);
bool proj_add(const IntervalVector& y, IntervalVector& x1, Affine2Vector& x2);
bool proj_add(const Affine2Vector& y, IntervalVector& x1, IntervalVector& x2);
bool proj_add(const IntervalVector& y, Affine2Vector& x1, IntervalVector& x2);

/**
 * \brief Projection of $y=x_1-x_2$.
 *
 * Set $([x]_1,[x]_2)$ to $([x]_1,[x]_2])\cap\{ (x_1,x_2)\in [x]_1\times[x]_2 \ | \ \exists y\in[y],\ y=x_1-x_2\}$.
 */
bool proj_sub(const Affine2Vector& y, Affine2Vector& x1, Affine2Vector& x2);
bool proj_sub(const IntervalVector& y, Affine2Vector& x1, Affine2Vector& x2);
bool proj_sub(const Affine2Vector& y, IntervalVector& x1, Affine2Vector& x2);
bool proj_sub(const Affine2Vector& y, Affine2Vector& x1, IntervalVector& x2);
bool proj_sub(const IntervalVector& y, IntervalVector& x1, Affine2Vector& x2);
bool proj_sub(const Affine2Vector& y, IntervalVector& x1, IntervalVector& x2);
bool proj_sub(const IntervalVector& y, Affine2Vector& x1, IntervalVector& x2);

/**
 * \brief Projection of $y=x_1*x_2$ (scalar product).
 *
 * Set $([x]_1,[x]_2)$ to $([x]_1,[x]_2])\cap\{ (x_1,x_2)\in [x]_1\times[x]_2 \ | \ \exists y\in[y],\ y=x_1*x_2\}$.
 */
bool proj_mul(const Affine2Vector& y, Affine2& x1, Affine2Vector& x2);
bool proj_mul(const IntervalVector& y, Affine2& x1, Affine2Vector& x2);
bool proj_mul(const Affine2Vector& y, Interval& x1, Affine2Vector& x2);
bool proj_mul(const IntervalVector& y, Interval& x1, Affine2Vector& x2);
bool proj_mul(const Affine2Vector& y, Affine2& x1, IntervalVector& x2);
bool proj_mul(const IntervalVector& y, Affine2& x1, IntervalVector& x2);
bool proj_mul(const Affine2Vector& y, Interval& x1, IntervalVector& x2);

/**
 * \brief Projection of $y=x_1*x_2$ (dot product).
 *
 * Set $([x]_1,[x]_2)$ to $([x]_1,[x]_2])\cap\{ (x_1,x_2)\in [x]_1\times[x]_2 \ | \ \exists y\in[y],\ y=x_1*x_2\}$.
 */
bool proj_mul(const Affine2& y, Affine2Vector& x1, Affine2Vector& x2);
bool proj_mul(const Interval& y, Affine2Vector& x1, Affine2Vector& x2);
bool proj_mul(const Affine2& y, IntervalVector& x1, Affine2Vector& x2);
bool proj_mul(const Affine2& y, Affine2Vector& x1, IntervalVector& x2);
bool proj_mul(const Interval& y, IntervalVector& x1, Affine2Vector& x2);
bool proj_mul(const Affine2& y, IntervalVector& x1, IntervalVector& x2);
bool proj_mul(const Interval& y, Affine2Vector& x1, IntervalVector& x2);

/**
 * \brief Display the Affine2Vector \a x
 */
std::ostream& operator<<(std::ostream& os, const Affine2Vector& x);

/**
 * \brief Cartesian product of x and y.
 *
 */
Affine2Vector cart_prod(const Affine2Vector& x, const Affine2Vector& y);

/*@}*/

/*============================================ inline implementation ============================================ */

inline Affine2Vector Affine2Vector::empty(int n) {
	return Affine2Vector(n, Interval::EMPTY_SET);
}

inline Affine2Vector::~Affine2Vector() {
	delete[] _vec;
}

inline void Affine2Vector::set_empty() {
	(*this)[0] = Interval::EMPTY_SET;
}

inline const Affine2& Affine2Vector::operator[](int i) const {
	assert(i>=0 && i<_n);
	return _vec[i];
}

inline Affine2& Affine2Vector::operator[](int i) {
	assert(i>=0 && i<_n);
	return _vec[i];
}

inline void Affine2Vector::clear() {
	init(0);
}

inline bool Affine2Vector::operator!=(const IntervalVector& x) const {
	return !(*this==x);
}
inline bool Affine2Vector::operator!=(const Affine2Vector& x) const {
	return !(*this==x);
}

inline int Affine2Vector::size() const {
	return _n;
}

inline bool Affine2Vector::is_empty() const {
	return (*this)[0].is_empty();
}

inline bool Affine2Vector::is_superset(const IntervalVector& x) const {
	return x.is_subset((*this).itv());
}

inline bool Affine2Vector::is_superset(const Affine2Vector& x) const {
	return x.is_subset(*this);
}

inline bool Affine2Vector::is_strict_superset(const IntervalVector& x) const {
	return x.is_strict_subset((*this).itv());
}
inline bool Affine2Vector::is_strict_superset(const Affine2Vector& x) const {
	return x.is_strict_subset(*this);
}

inline double Affine2Vector::max_diam() const {
	return (*this)[extr_diam_index(false)].diam();
}

inline double Affine2Vector::min_diam() const {
	return (*this)[extr_diam_index(true)].diam();
}


inline IntervalVector operator&(const IntervalVector& x, const Affine2Vector& y) const{
	return (y &  x);
}

inline IntervalVector operator|(const IntervalVector& x, const Affine2Vector& y) const{
	return (y |  x);
}

inline double distance(const IntervalVector& x1, const Affine2Vector& x2) {
	return distance(x2,x1);
}


inline Affine2Vector operator+(const IntervalVector& x1, const Affine2Vector& x2) {
	return x2 + x1;
}

inline Affine2 operator*(const IntervalVector& x1, const Affine2Vector& x2){
	return x2*x1;
}


inline Affine2Vector operator -(const Affine2Vector& x) {
	return Affine2Vector(x,true);
}

inline Affine2Vector operator +(const Vector& x1, const Affine2Vector& x2) {
	return Affine2Vector(x2)+=x1;
}

inline Affine2Vector operator +(const Affine2Vector& x1, const Vector& x2) {
	return Affine2Vector(x1)+=x2;
}

inline Affine2Vector operator +(const Affine2Vector& x1, const IntervalVector& x2) {
	return Affine2Vector(x1)+=x2;
}

inline Affine2Vector operator +(const Affine2Vector& x1, const Affine2Vector& x2) {
	return Affine2Vector(x1)+=x2;
}

inline Affine2Vector operator -(const Vector& x1, const Affine2Vector& x2) {
	return Affine2Vector(x2,true)+=x2;
}

inline Affine2Vector operator -(const Affine2Vector& x1, const Vector& x2) {
	return Affine2Vector(x1)-=x2;
}

inline Affine2Vector operator -(const Affine2Vector& x1, const IntervalVector& x2) {
	return Affine2Vector(x1)-=x2;
}

inline Affine2Vector operator -(const IntervalVector& x1, const Affine2Vector& x2) {
	return Affine2Vector(x2,true)+=x1;
}

inline Affine2Vector operator -(const Affine2Vector& x1, const Affine2Vector& x2) {
	return Affine2Vector(x2,true)+=x1;
}

inline Affine2 operator *(const Vector& x1, const Affine2Vector& x2) {
	return Affine2Vector(x2)*=x1;
}

inline Affine2 operator *(const Affine2Vector& x1, const Vector& x2) {
	return Affine2Vector(x1)*=x2;
}

inline Affine2 operator *(const Affine2Vector& x1, const IntervalVector& x2) {
	return Affine2Vector(x1)*=x2;
}

inline Affine2 operator *(const Affine2Vector& x1, const Affine2Vector& x2) {
	return Affine2Vector(x1)*=x2;
}

inline Affine2Vector operator *(double d, const Affine2Vector& x) {
	return Affine2Vector(x)*=d;
}

inline Affine2Vector operator *(const Affine2& x1, const Vector& x2) {
	return Affine2Vector(x2.size(),x1)*=x2;
}

inline Affine2Vector operator *(const Affine2& x1, const Affine2Vector& x2) {
	return Affine2Vector(x2)*=x1;
}

inline Affine2Vector operator *(const Interval& x1, const Affine2Vector& x2) {
	return Affine2Vector(x2)*=x1;
}

inline Affine2Vector cart_prod(const Affine2Vector& x, const Affine2Vector& y) {
	Affine2Vector z(x.size()+y.size());
	z.put(0,x);
	z.put(x.size(),y);
	return z;
}

inline bool proj_add(const Affine2Vector& y, Affine2Vector& x1, Affine2Vector& x2) {
	return proj_add(y.itv(),x1,x2);
}
inline bool proj_add(const Affine2Vector& y, IntervalVector& x1, Affine2Vector& x2){
	return proj_add(y.itv(),x2,x1);
}
inline bool proj_add(const Affine2Vector& y, Affine2Vector& x1, IntervalVector& x2){
	return proj_add(y.itv(),x1,x2);
}
inline bool proj_add(const IntervalVector& y, IntervalVector& x1, Affine2Vector& x2){
	return proj_add(y,x2,x1);
}
inline bool proj_add(const Affine2Vector& y, IntervalVector& x1, IntervalVector& x2){
	return proj_add(y.itv(),x1,x2);
}



inline bool proj_sub(const Affine2Vector& y, Affine2Vector& x1, Affine2Vector& x2) {
	return proj_sub(y.itv(),x1,x2);
}
inline bool proj_sub(const Affine2Vector& y, IntervalVector& x1, Affine2Vector& x2){
	return proj_sub(y.itv(),x1,x2);
}
inline bool proj_sub(const Affine2Vector& y, Affine2Vector& x1, IntervalVector& x2){
	return proj_sub(y.itv(),x1,x2);
}
inline bool proj_sub(const Affine2Vector& y, IntervalVector& x1, IntervalVector& x2){
	return proj_sub(y.itv(),x1,x2);
}


inline bool proj_mul(const Affine2Vector& y, Affine2& x1, Affine2Vector& x2){
	return proj_mul(y.itv(),x1,x2);
}
inline bool proj_mul(const Affine2Vector& y, Interval& x1, Affine2Vector& x2){
	return proj_mul(y.itv(),x1,x2);
}
inline bool proj_mul(const Affine2Vector& y, Affine2& x1, IntervalVector& x2){
	return proj_mul(y.itv(),x1,x2);
}
inline bool proj_mul(const Affine2Vector& y, Interval& x1, IntervalVector& x2){
	return proj_mul(y.itv(),x1,x2);
}



inline bool proj_mul(const Affine2& y, Affine2Vector& x1, Affine2Vector& x2){
	return proj_mul(y.itv(),x1,x2);
}
inline bool proj_mul(const Affine2& y, IntervalVector& x1, Affine2Vector& x2){
	return proj_mul(y.itv(),x2,x1);
}
inline bool proj_mul(const Affine2& y, Affine2Vector& x1, IntervalVector& x2){
	return proj_mul(y.itv(),x1,x2);
}
inline bool proj_mul(const Interval& y, IntervalVector& x1, Affine2Vector& x2){
	return proj_mul(y,x2,x1);
}
inline bool proj_mul(const Affine2& y, IntervalVector& x1, IntervalVector& x2){
	return proj_mul(y.itv(),x1,x2);
}



} // end namespace

#endif /* _IBEX_AFFINE2_VECTOR_H_ */
