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
#include <utility>
#include "ibex_Interval.h"
#include "ibex_InvalidIntervalVectorOp.h"
#include "ibex_Vector.h"
#include "ibex_Matrix.h"
#include "ibex_Array.h"

namespace ibex {

class IntervalMatrix; // declared only for friendship

/**
 * \ingroup arithmetic
 *
 * \brief Vector of Intervals
 *
 * By convention an empty vector has a dimension. A vector becomes empty
 * when one of its component becomes empty and all the components
 * are set to the empty Interval.
 *
 * An IntervalVector is always a column vector.
 * If you want to insert a row vector in an
 * expression, use the transpose(...) operator.
 * Example:
 *
 *    const ExprSymbol& x=ExprSymbol::new_(Dim::col_vec(2));
 *    IntervalVector v=Vector::ones(2);
 *    const ExprNode& e=transpose(v)*x;
 */
class IntervalVector {

public:

	/**
	 * \brief Create an uninitialized interval vector.
	 *
	 * This constructor is useful, e.g., for building arrays
	 * of IntervalVector.
	 *
	 * \warning The object must be "resize" before being used.
	 * \see	#resize(int).
	 */
	IntervalVector();

	/**
	 * \brief Create [(-oo,+oo) ; ..; (-oo,+oo)]
	 *
	 * Create a n-sized vector. All the components are (-oo,+oo)
	 * \pre n>0
	 */
	explicit IntervalVector(int n);

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
	 * \brief Create the one-dimensional IntervalVector x
	 *
	 */
	IntervalVector(const Interval& x);

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

	/**
	 * \brief Absolute inflation.
	 *
	 * Add [-rad,+rad] to all the components of *this.
	 *
	 * \return *this.
	 */
	IntervalVector& inflate(double rad);

	/**
	 * \brief Absolute and relative inflation.
	 *
	 * [x] <- mid[x] + delta*([x]-mid[x]) + chi*[-1,+1]
	 *
	 * \return *this.
	 */
	IntervalVector& inflate(double delta, double chi);

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
	IntervalVector subvector(int start_index, int end_index) const;

	/**
	 * \brief Put a subvector into *this at a given position.
	 *
	 * \param start_index - the position where the subvector
	 * \param subvec - the subvector
	 *
	 * \pre (*this) must not be empty
	 */
	void put(int start_index, const IntervalVector& subvec);

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
	 * \brief True iff this interval vector is a subset of \a x, and not \a x itself.
	 *
	 * \pre Dimension of \a x must be equal to the dimension of this vector.
	 *
	 * \note return true if this interval vector is empty and \a x not.
	 *
	 * \sa #ibex::Interval::is_strict_subset(const Interval&) const.
	 */
	bool is_strict_subset(const IntervalVector& x) const;

	/**
	 * \brief True iff this interval vector is a subset of the interior of \a x.
	 *
	 * \pre Dimension of \a x must be equal to the dimension of this vector.

	 * \note Always return true if this interval vector is empty.

	 * \sa #ibex::Interval::is_interior_subset(const Interval&) const.
	 */
	bool is_interior_subset(const IntervalVector& x) const;

    /**
     * \brief True iff this interval vector is in the relative interior of \a x.
     *
     * The relative interior of an interval vector is defined as the vector
     * of the relative interiors of its components, where
     *
     *     rel([a,b]) := [a,b] (the closed interval) if a==b ;
     *                   (a,b) (the open interval) otherwise.
     */
    bool is_relative_interior_subset(const IntervalVector& x) const;

	/**
	 * \brief True iff this interval vector is a subset of the interior of \a x and different from x.
	 *
	 * \pre Dimension of \a x must be equal to the dimension of this vector.

	 * \note Always return true if this interval vector is empty.

	 * \sa #ibex::Interval::is_interior_subset(const Interval&) const.
	 */
	bool is_strict_interior_subset(const IntervalVector& x) const;

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
	 * \brief True iff this interval vector contains \a x.
	 *
	 * \pre Dimension of \a x must be equal to the dimension of (*this).
	 * \sa #ibex::Interval::contains(double) const.
	 */
	bool contains(const Vector& x) const;

	/**
	 * \brief True iff \a x is in the interior of this interval vector
	 *
	 * \pre Dimension of \a x must be equal to the dimension of (*this).
	 * \sa #ibex::Interval::strictly_contains(double) const.
	 */
	bool interior_contains(const Vector& x) const;

	/**
	 * \brief True iff this interval vector intersects \a x.
	 *
	 * \pre Dimension of \a x must be equal to the dimension of (*this).
	 * \sa #ibex::Interval::intersects(double) const.
	 */
	bool intersects(const IntervalVector& x) const;

	/**
	 * \brief True iff this interval vector intersects \a x and the intersection has a non-null volume
	 *
	 * \pre Dimension of \a x must be equal to the dimension of (*this).
	 * \sa #ibex::Interval::strictly_intersects(double) const.
	 */
	bool overlaps(const IntervalVector& x) const;

	/**
	 * \brief True iff this interval vector does not intersect \a x.
	 *
	 * \pre Dimension of \a x must be equal to the dimension of (*this).
	 * \sa #ibex::Interval::is_disjoint(double) const.
	 */
	bool is_disjoint(const IntervalVector& x) const;

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
	 *  \throws InvalidIntervalVectorOp if the IntervalVector is empty.
	 */
	int extr_diam_index(bool min) const;


	/**
	 * \brief Return the indices of all the components, sorted by increasing/decreasing diameter.
	 */
	void sort_indices(bool min, int tab[]) const;

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
	  * \brief Return max of the delta, for x\subseteq *this [deprecated]
	  *
	  * Deprecated. Kept for compatibility with ibex 1.xx.
	  */
	double maxdelta(const IntervalVector&);

	/**
	 * \brief Return the relative distance with x.
	 *
	 * \return \f$\displaystyle \max_{i=1..n} rel\_distance([this]_i, x_i)/diam([this]_i)\f$.
	 *
	 * \sa #ibex::distance(const IntervalVector& x1, const IntervalVector& x2).
	 * \sa #ibex::Interval::rel_distance(const Interval& x) const.
	 */
	double rel_distance(const IntervalVector& x) const;


	/**
	 * \brief Return *this \ y (set difference).
	 *
	 * Store the difference under the form of a union of non-overlapping IntervalVectors
	 * into \a result, and return the size of the union.
	 *
	 * <p>
	 * If the difference is empty, the returned value is 0 but \a result is an array of one
	 * element set to the empty box. It is <b>not</b> a zero-sized array containing no element
	 * (a zero-sized array is illegal in ISO C++). In particular, you can used delete[] whatever
	 * is the result of diff.
	 */
	int diff(const IntervalVector& y, IntervalVector*& result, bool compactness = true) const;

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
	std::pair<IntervalVector,IntervalVector> bisect(int i, double ratio=0.5) const;

	/**
	 * \brief Return a random vector inside *this.
	 *
	 * \pre (*this) must be nonempty.
	 */
	Vector random(int seed) const;
	Vector random() const;

	/**
	 * \brief (*this)+=x2.
	 */
	IntervalVector& operator+=(const Vector& x2);

	/**
	 * \brief (*this)+=x2.
	 */
	IntervalVector& operator+=(const IntervalVector& x2);

	/**
	 * \brief (*this)-=x2.
	 */
	IntervalVector& operator-=(const Vector& x2);

	/**
	 * \brief (*this)-=x2.
	 */
	IntervalVector& operator-=(const IntervalVector& x2);

	/**
	 * \brief x=d*x
	 */
	IntervalVector& operator*=(double d);

	/**
	 * \brief (*this)=x1*(*this).
	 */
	IntervalVector& operator*=(const Interval& x1);

    /**
     * \brief Cast the vector to an expression
     */
	operator const ExprConstant&() const;

	/**
	 * \brief Interval vector iterator
	 */
	typedef Interval* iterator;

	/**
	 * \brief Interval vector const iterator
	 */
	typedef const Interval* const_iterator;

	/**
	 * \brief First iterator
	 */
	iterator begin() { return &vec[0]; }

	/**
	 * \brief Past-the-end iterator
	 */
	iterator end() { return &vec[n]; }

private:
	friend class IntervalMatrix;

	int n;             // dimension (size of vec)
	Interval *vec;	   // vector of elements
};

/** \ingroup arithmetic */
/*@{*/

/**
 * \brief Return the infinite Hausdorff distance (i.e. the maximum of the distance componentwise).
 *
 * \return \f$\displaystyle \max_{i=1..n} distance(x1_i, x2_i)\f$.
 *
 * \pre Dimension of \a x1 and \a x2 must be equal.
 *
 * \sa #ibex::distance(const ibex::Interval& x1, const ibex::Interval& x2).
 */
double distance(const IntervalVector& x1, const IntervalVector& x2);

/**
 * \brief -x.
 */
IntervalVector operator-(const IntervalVector& x);

/**
 * \brief x1+x2.
 */

IntervalVector operator+(const Vector& x1, const IntervalVector& x2);

/**
 * \brief x1+x2.
 */
IntervalVector operator+(const IntervalVector& x1, const Vector& x2);

/**
 * \brief x1+x2.
 */
IntervalVector operator+(const IntervalVector& x1, const IntervalVector& x2);

/**
 * \brief x1-x2.
 */
IntervalVector operator-(const Vector& x1, const IntervalVector& x2);

/**
 * \brief x1-x2.
 */

IntervalVector operator-(const IntervalVector& x1, const Vector& x2);

/**
 * \brief x1-x2.
 */
IntervalVector operator-(const IntervalVector& x1, const IntervalVector& x2);

/**
 * \brief x1*x2.
 */
Interval operator*(const Vector& x1, const IntervalVector& x2);

/**
 * \brief x1*x2.
 */
Interval operator*(const IntervalVector& x1, const Vector& x2);

/**
 * \brief x1*x2.
 */
Interval operator*(const IntervalVector& x1, const IntervalVector& x2);

/**
 * \brief d*x
 */
IntervalVector operator*(double d, const IntervalVector& x);

/**
 * \brief x1*x2.
 */
IntervalVector operator*(const Interval& x1, const Vector& x2);

/**
 *  \brief x1*x2.
 */
IntervalVector operator*(const Interval& x1, const IntervalVector& x2);

/**
 * \brief Hadamard product of x and y.
 *
 * The Hadamard product is a component-wise multiplication:
 * z[i]=x[i]*y[i].
 */
IntervalVector hadamard_product(const IntervalVector& x, const Vector& y);

/**
 * \brief Hadamard product of x and y.
 *
 * The Hadamard product is a component-wise multiplication:
 * z[i]=x[i]*y[i].
 */
IntervalVector hadamard_product(const Vector& x, const IntervalVector& y);

/**
 * \brief Hadamard product of x and y.
 *
 * The Hadamard product is a component-wise multiplication:
 * z[i]=x[i]*y[i].
 */
IntervalVector hadamard_product(const IntervalVector& x, const IntervalVector& y);

/**
 * \brief |x|.
 */
IntervalVector abs(const IntervalVector& x);

/**
 * \brief ||x||_oo.
 */
double infinite_norm(const IntervalVector& x);

/**
 * \brief Projection of $y=x_1+x_2$.
 *
 * Set $([x]_1,[x]_2)$ to $([x]_1,[x]_2])\cap\{ (x_1,x_2)\in [x]_1\times[x]_2 \ | \ \exists y\in[y],\ y=x_1+x_2\}$.
 */
bool bwd_add(const IntervalVector& y, IntervalVector& x1, IntervalVector& x2);

/**
 * \brief Projection of $y=x_1-x_2$.
 *
 * Set $([x]_1,[x]_2)$ to $([x]_1,[x]_2])\cap\{ (x_1,x_2)\in [x]_1\times[x]_2 \ | \ \exists y\in[y],\ y=x_1-x_2\}$.
 */
bool bwd_sub(const IntervalVector& y, IntervalVector& x1, IntervalVector& x2);

/**
 * \brief Projection of $y=x_1*x_2$ (scalar product).
 *
 * Set $([x]_1,[x]_2)$ to $([x]_1,[x]_2])\cap\{ (x_1,x_2)\in [x]_1\times[x]_2 \ | \ \exists y\in[y],\ y=x_1*x_2\}$.
 */
bool bwd_mul(const IntervalVector& y, Interval& x1, IntervalVector& x2);

/**
 * \brief Projection of $y=x_1*x_2$ (dot product).
 *
 * Set $([x]_1,[x]_2)$ to $([x]_1,[x]_2])\cap\{ (x_1,x_2)\in [x]_1\times[x]_2 \ | \ \exists y\in[y],\ y=x_1*x_2\}$.
 */
bool bwd_mul(const Interval& y, IntervalVector& x1, IntervalVector& x2);

/**
 * \brief Display the IntervalVector \a x
 */
std::ostream& operator<<(std::ostream& os, const IntervalVector& x);

/**
 * \brief Cartesian product of x1 and x2.
 */
IntervalVector cart_prod(const IntervalVector& x1, const IntervalVector& x2);

/**
 * \brief Cartesian product of x1, x2, x3.
 */
IntervalVector cart_prod(const IntervalVector& x1, const IntervalVector& x2, const IntervalVector& x3);

/**
 * \brief Cartesian product of x1, x2, x3, x4.
 */
IntervalVector cart_prod(const IntervalVector& x1, const IntervalVector& x2, const IntervalVector& x3, const IntervalVector& x4);

/**
 * \brief Cartesian product of x1, x2, x3, x4, x5.
 */
IntervalVector cart_prod(const IntervalVector& x1, const IntervalVector& x2, const IntervalVector& x3, const IntervalVector& x4, const IntervalVector& x5);

/**
 * \brief Cartesian product of x1, x2, x3, x4, x5, x6.
 */
IntervalVector cart_prod(const IntervalVector& x1, const IntervalVector& x2, const IntervalVector& x3, const IntervalVector& x4, const IntervalVector& x5, const IntervalVector& x6);

/**
 * \brief Cartesian product of x1, x2, x3, x4, x5, x6, x7.
 */
IntervalVector cart_prod(const IntervalVector& x1, const IntervalVector& x2, const IntervalVector& x3, const IntervalVector& x4, const IntervalVector& x5, const IntervalVector& x6, const IntervalVector& x7);

/**
 * \brief Cartesian product of x1, x2, x3, x4, x5, x6, x7, x8.
 */
IntervalVector cart_prod(const IntervalVector& x1, const IntervalVector& x2, const IntervalVector& x3, const IntervalVector& x4, const IntervalVector& x5, const IntervalVector& x6, const IntervalVector& x7, const IntervalVector& x8);

/**
 * \brief Cartesian product of an arbitrary number of arguments.
 */
IntervalVector cart_prod(const Array<const IntervalVector>& x);

/*@}*/

/*============================================ inline implementation ============================================ */

// the following functions are
// introduced to allow genericity
inline bool ___is_empty(const IntervalVector& v) { return v.is_empty(); }
inline void ___set_empty(IntervalVector& v) { v.set_empty(); }

} // end namespace ibex

#include "ibex_LinearArith.h"

namespace ibex {

inline IntervalVector::IntervalVector() : n(0), vec(NULL) {

}

inline IntervalVector IntervalVector::empty(int n) {
	return IntervalVector(n, Interval::EMPTY_SET);
}

inline IntervalVector::~IntervalVector() {
	delete[] vec;
}

inline void IntervalVector::set_empty() {
	for (int i=0; i<size(); i++)
		(*this)[i]=Interval::EMPTY_SET;
}

inline const Interval& IntervalVector::operator[](int i) const {
	assert(i>=0 && i<n);
	return vec[i];
}

inline Interval& IntervalVector::operator[](int i) {
	assert(i>=0 && i<n);
	return vec[i];
}

inline void IntervalVector::clear() {
	init(0);
}

inline IntervalVector IntervalVector::operator&(const IntervalVector& x) const {
	return IntervalVector(*this) &= x;
}

inline IntervalVector IntervalVector::operator|(const IntervalVector& x) const {
	return IntervalVector(*this) |= x;
}

inline bool IntervalVector::operator!=(const IntervalVector& x) const {
	return !(*this==x);
}

//__IBEX_GENERATE_BASIC_SET_OP_VECTOR_AND__(Interval,Interval,basic_is_subset)
//__IBEX_GENERATE_BASIC_SET_OP_VECTOR_AND__(Interval,Interval,basic_is_strict_subset)
//__IBEX_GENERATE_BASIC_SET_OP_VECTOR_AND__(Interval,__Real,  basic_contains)
//__IBEX_GENERATE_BASIC_SET_OP_VECTOR_AND__(Interval,__Real,  basic_strictly_contains)
//__IBEX_GENERATE_BASIC_SET_OP_VECTOR_AND__(Interval,Interval,basic_intersects)
//__IBEX_GENERATE_BASIC_SET_OP_VECTOR_AND__(Interval,Interval,basic_strictly_intersects)
//
//__IBEX_GENERATE_BASIC_SET_OP_VECTOR_OR__ (Interval,Interval,basic_is_disjoint)
//
//inline bool IntervalVector::is_subset(const IntervalVector& x) const        { return ibex::is_subset(*this,x); }
//inline bool IntervalVector::is_strict_subset(const IntervalVector& x) const { return ibex::is_strict_subset(*this,x); }


inline int IntervalVector::size() const {
	return n;
}

inline bool IntervalVector::is_empty() const {
	return (*this)[0].is_empty();
}

inline double IntervalVector::max_diam() const {
	return (*this)[extr_diam_index(false)].diam();
}

inline double IntervalVector::min_diam() const {
	return (*this)[extr_diam_index(true)].diam();
}

inline IntervalVector& IntervalVector::operator+=(const Vector& x) {
	return set_addV<IntervalVector,Vector>(*this,x);
}

inline IntervalVector& IntervalVector::operator+=(const IntervalVector& x) {
	return set_addV<IntervalVector,IntervalVector>(*this,x);
}

inline IntervalVector& IntervalVector::operator-=(const Vector& x) {
	return set_subV<IntervalVector,Vector>(*this,x);
}

inline IntervalVector& IntervalVector::operator-=(const IntervalVector& x) {
	return set_subV<IntervalVector,IntervalVector>(*this,x);
}

inline IntervalVector& IntervalVector::operator*=(double x) {
	return set_mulSV<double,IntervalVector>(x,*this);
}

inline IntervalVector& IntervalVector::operator*=(const Interval& x) {
	return set_mulSV<Interval,IntervalVector>(x,*this);
}

inline IntervalVector operator-(const IntervalVector& x) {
	return minusV(x);
}

inline IntervalVector operator+(const IntervalVector& m1, const Vector& m2) {
	return IntervalVector(m1)+=m2;
}

inline IntervalVector operator+(const Vector& m1, const IntervalVector& m2) {
	return IntervalVector(m1)+=m2;
}

inline IntervalVector operator+(const IntervalVector& m1, const IntervalVector& m2) {
	return IntervalVector(m1)+=m2;
}

inline IntervalVector operator-(const IntervalVector& m1, const Vector& m2) {
	return IntervalVector(m1)-=m2;
}

inline IntervalVector operator-(const Vector& m1, const IntervalVector& m2) {
	return IntervalVector(m1)-=m2;
}

inline IntervalVector operator-(const IntervalVector& m1, const IntervalVector& m2) {
	return IntervalVector(m1)-=m2;
}

inline IntervalVector operator*(double x, const IntervalVector& v) {
	return IntervalVector(v)*=x;
}

inline IntervalVector operator*(const Interval& x, const Vector& v) {
	return IntervalVector(v)*=x;
}

inline IntervalVector operator*(const Interval& x, const IntervalVector& v) {
	return IntervalVector(v)*=x;
}

inline Interval operator*(const Vector& v1, const IntervalVector& v2) {
	return mulVV<Vector,IntervalVector,Interval>(v1,v2);
}

inline Interval operator*(const IntervalVector& v1, const Vector& v2) {
	return mulVV<IntervalVector,Vector,Interval>(v1,v2);
}

inline Interval operator*(const IntervalVector& v1, const IntervalVector& v2) {
	return mulVV<IntervalVector,IntervalVector,Interval>(v1,v2);
}

inline IntervalVector hadamard_product(const Vector& v1, const IntervalVector& v2) {
	return hadamard_prod<Vector,IntervalVector,IntervalVector>(v1,v2);
}

inline IntervalVector hadamard_product(const IntervalVector& v1, const Vector& v2) {
	return hadamard_prod<IntervalVector,Vector,IntervalVector>(v1,v2);
}

inline IntervalVector hadamard_product(const IntervalVector& v1, const IntervalVector& v2) {
	return hadamard_prod<IntervalVector,IntervalVector,IntervalVector>(v1,v2);
}

inline IntervalVector operator*(const Matrix& m, const IntervalVector& v) {
	return mulMV<Matrix,IntervalVector,IntervalVector>(m,v);
}

inline IntervalVector operator*(const IntervalVector& v, const Matrix& m) {
	return mulVM<IntervalVector,Matrix,IntervalVector>(v,m);
}

inline IntervalVector cart_prod(const IntervalVector& x1, const IntervalVector& x2) {
	return cart_prod(Array<const IntervalVector>(x1,x2));
}

inline IntervalVector cart_prod(const IntervalVector& x1, const IntervalVector& x2, const IntervalVector& x3) {
	return cart_prod(Array<const IntervalVector>(x1,x2,x3));
}

inline IntervalVector cart_prod(const IntervalVector& x1, const IntervalVector& x2, const IntervalVector& x3, const IntervalVector& x4) {
	return cart_prod(Array<const IntervalVector>(x1,x2,x3,x4));
}

inline IntervalVector cart_prod(const IntervalVector& x1, const IntervalVector& x2, const IntervalVector& x3, const IntervalVector& x4, const IntervalVector& x5) {
	return cart_prod(Array<const IntervalVector>(x1,x2,x3,x4,x5));
}

inline IntervalVector cart_prod(const IntervalVector& x1, const IntervalVector& x2, const IntervalVector& x3, const IntervalVector& x4, const IntervalVector& x5, const IntervalVector& x6) {
	return cart_prod(Array<const IntervalVector>(x1,x2,x3,x4,x5,x6));
}

inline IntervalVector cart_prod(const IntervalVector& x1, const IntervalVector& x2, const IntervalVector& x3, const IntervalVector& x4, const IntervalVector& x5, const IntervalVector& x6, const IntervalVector& x7) {
	return cart_prod(Array<const IntervalVector>(x1,x2,x3,x4,x5,x6,x7));
}

inline IntervalVector cart_prod(const IntervalVector& x1, const IntervalVector& x2, const IntervalVector& x3, const IntervalVector& x4, const IntervalVector& x5, const IntervalVector& x6, const IntervalVector& x7, const IntervalVector& x8) {
	return cart_prod(Array<const IntervalVector>(x1,x2,x3,x4,x5,x6,x7,x8));
}

} // end namespace

#endif /* _IBEX_INTERVAL_VECTOR_H_ */
