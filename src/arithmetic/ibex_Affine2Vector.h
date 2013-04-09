/* ============================================================================
 * I B E X - Affine2 Vector definition
 * ============================================================================
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Jordan Ninin
 * Created     : March 13, 2013
 * ---------------------------------------------------------------------------- */

#ifndef __IBEX_AFFINE2_VECTOR_H__
#define __IBEX_AFFINE2_VECTOR_H__

#include <cassert>
#include <iostream>
#include "ibex_Interval.h"
#include "ibex_IntervalVector.h"
#include "ibex_Vector.h"
#include "ibex_Affine2.h"

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
	 * \brief  Create an Affine2Vector of dimension \a n with
	 * all the components initialized to
	 *  if (!b) Affine2(\a x)
	 *  else Affine2(\a n,i+1,\a x) }.
	 * \pre n>0
	 */
	Affine2Vector(int n, const Interval& x, bool b =false);

	/**
	 * \brief  Create \a n Affine2Vector of dimension \a n with
	 * all the components initialized to \a x.
	 * \pre n>0
	 */
	Affine2Vector(int n, const Affine2& x);

	/**
	 * \brief Create a copy of \a x.
	 */
//	Affine2Vector(const Affine2Vector& x);


	/**
	 * \brief Create  a copy of  { \a  x if !(\a b)  else -(\a x) }.
	 */
	Affine2Vector(const Affine2Vector& x);

	/**
	 * \brief Create \a n Affine2Vector  initialized by
	 * if !(\a b)  Affine2(Interval(bounds[i][0],bounds[i][1]) )
	 * else Affine2(\a n, i+1,Interval(bounds[i][0],bounds[i][1]) )
	 *
	 * \param bounds an nx2 array of doubles
	 * \pre n>0
	 */
	Affine2Vector(int n, double  bounds[][2], bool b =false);

	/**
	 * \brief Create \a x.size Affine2Vector of dimension \a x.size with
	 * the [i] component initialized to
	 * if !(\a b) Affine2(x[i])
	 * else  Affine2(x.size(), i+1,x[i])
	 */
	Affine2Vector(const IntervalVector& x, bool b =false);

	/**
	 * \brief Create the degenerated Affine2Vector x
	 *
	 */
	explicit Affine2Vector(const Vector& x);

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
	 * \brief Set all the elements to
	 *  if !(\a b) Affine2(x)
	 *  else Affine2(n,i+1,x)
	 *
	 * \note Emptiness is "overridden".
	 */
	void init(const Interval& x, bool b =false);

	/**
	 * \brief Set all the elements to x
	 *
	 * \note Emptiness is "overridden".
	 */
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
	 * \brief Return true iff this Affine2Vector is empty
	 */
	bool is_empty() const;

	/**
	 * \brief true iff this interval vector contains an infinite bound.
	 *
	 * \note An empty interval vector is always bounded.
	 */
	bool is_unbounded() const;

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
IntervalVector operator&(const Affine2Vector& x, const Affine2Vector& y);
IntervalVector operator&(const IntervalVector& x, const Affine2Vector& y);
IntervalVector operator&(const Affine2Vector& x, const IntervalVector& y);

/**
 * \brief Return the hull of x & y.
 */
IntervalVector operator|(const Affine2Vector& x, const Affine2Vector& y);
IntervalVector operator|(const IntervalVector& x, const Affine2Vector& y);
IntervalVector operator|(const Affine2Vector& x, const IntervalVector& y);

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

inline IntervalVector operator&(const IntervalVector& x, const Affine2Vector& y) {
	return (y &  x);
}

inline IntervalVector operator|(const IntervalVector& x, const Affine2Vector& y) {
	return (y |  x);
}

inline Affine2Vector cart_prod(const Affine2Vector& x, const Affine2Vector& y) {
	Affine2Vector z(x.size()+y.size());
	z.put(0,x);
	z.put(x.size(),y);
	return z;
}


} // end namespace

#endif /* _IBEX_AFFINE2_VECTOR_H_ */
