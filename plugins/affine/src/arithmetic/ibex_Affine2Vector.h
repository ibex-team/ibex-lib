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
#include "ibex_IntervalMatrix.h"
#include "ibex_Vector.h"
#include "ibex_Matrix.h"
#include "ibex_Affine2.h"

namespace ibex {

template<class T> class Affine2MainMatrix;

/**
 * \ingroup arithmetic
 *
 * \brief Vector of Affine2 form
 *
 * By convention an empty vector has a dimension. A vector becomes empty
 * when one of its component becomes empty and all the components
 * are set to the empty Interval.
 */

typedef Affine2MainVector<AF_Default> Affine2Vector;
typedef Affine2MainVector<AF_Linear> AffineLinVector;

template<class T=AF_Default>
class Affine2MainVector {


private:
	friend class Affine2MainMatrix<T>;

	Affine2MainVector() : _n(0), _vec(NULL) { }

	int _n;             // dimension (size of vec)
	Affine2Main<T> *_vec;	   // vector of elements

public:

	/** \brief  Create \a n Affine2 form . All the components are Affine2([-oo,+oo])
	 * \pre n>0
	 */
	explicit Affine2MainVector(int n);

	/**
	 * \brief  Create an Affine2MainVector of dimension \a n with
	 * all the components initialized to
	 *  if (!b) Affine2(\a x)
	 *  else Affine2(\a n,i+1,\a x) }.
	 * \pre n>0
	 */
	Affine2MainVector(int n, const Interval& x, bool b =false);

	/**
	 * \brief  Create \a n Affine2MainVector of dimension \a n with
	 * all the components initialized to \a x.
	 * \pre n>0
	 */
	Affine2MainVector(int n, const Affine2Main<T>& x);

	/**
	 * \brief Create a copy of \a x.
	 */
//	Affine2MainVector(const Affine2MainVector& x);


	/**
	 * \brief Create  a copy of  { \a  x if !(\a b)  else -(\a x) }.
	 */
	Affine2MainVector(const Affine2MainVector& x);

	/**
	 * \brief Create \a n Affine2MainVector  initialized by
	 * if !(\a b)  Affine2(Interval(bounds[i][0],bounds[i][1]) )
	 * else Affine2(\a n, i+1,Interval(bounds[i][0],bounds[i][1]) )
	 *
	 * \param bounds an nx2 array of doubles
	 * \pre n>0
	 */
	Affine2MainVector(int n, double  bounds[][2], bool b =false);

	/**
	 * \brief Create \a x.size Affine2MainVector of dimension \a x.size with
	 * the [i] component initialized to
	 * if !(\a b) Affine2(x[i])
	 * else  Affine2(x.size(), i+1,x[i])
	 */
	explicit Affine2MainVector(const IntervalVector& x, bool b =false);

	/**
	 * \brief Create the degenerated Affine2MainVector x
	 *
	 */
	explicit Affine2MainVector(const Vector& x);

	/**
	 * \brief Create [empty; ...; empty]
	 *
	 * Create an empty Affine2MainVector of dimension \a n
	 * (all the components being empty Intervals)
	 *
	 * \pre n>0
	 */
	static Affine2MainVector empty(int n);

	/**
	 * \brief Delete this vector
	 */
	virtual ~Affine2MainVector();

	/**
	 * \brief Return the ith Affine2
	 *
	 * A return a const reference to the
	 * i^th component (i starts from 0)
	 */
	const Affine2Main<T>& operator[](int i) const;

	/**
	 * \brief Return the ith Affine2
	 *
	 * A return a non-const reference to the
	 * i^th component (i starts from 0)
	 */
	Affine2Main<T>& operator[](int i);

	/**
	 * \brief Set this Affine2MainVector to the empty Affine2MainVector
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
	 * \brief reduce the number of noise variable if the value is inferior to \param tol
	 */
	void compact(double tol);
	void compact();

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
	void init(const Affine2Main<T>& x);

	/**
	 * \brief Add [-rad,+rad] to all the components of *this.
	 *
	 * \return *this.
	 */
	Affine2MainVector& inflate(double rad);

	/**
	 * \brief Resize this Affine2MainVector.
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
	Affine2MainVector subvector(int start_index, int end_index) const;

	/**
	 * \brief Put a subvector into *this at a given position.
	 *
	 * \param start_index - the position where the subvector
	 * \param subvec - the subvector
	 *
	 * \pre (*this) must not be empty
	 */
	void put(int start_index, const Affine2MainVector& subvec);

	/**
	 * \brief Assign this Affine2MainVector to x.
	 *
	 * \pre Dimensions of this and x must match.
	 * \note Emptiness is overridden.
	 */
	Affine2MainVector& operator=(const Affine2MainVector& x);
	Affine2MainVector& operator=(const IntervalVector& x);

	/**
	 * \brief Return true if the bounds of this Affine2MainVector match that of \a x.
	 */
	bool operator==(const Affine2MainVector& x) const;
	bool operator==(const IntervalVector& x) const;

	/**
	 * \brief Return true if one bounds of one component of *this differs from \a x.
	 */
	bool operator!=(const IntervalVector& x) const;
	bool operator!=(const Affine2MainVector& x) const;


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
	 * \brief Return true iff this Affine2MainVector is empty
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
	Affine2MainVector& operator+=(const Vector& x2);

	/**
	 * \brief (*this)+=x2.
	 */
	Affine2MainVector& operator+=(const IntervalVector& x2);
	Affine2MainVector& operator+=(const Affine2MainVector& x2);

	/**
	 * \brief (*this)-=x2.
	 */
	Affine2MainVector& operator-=(const Vector& x2);

	/**
	 * \brief (*this)-=x2.
	 */
	Affine2MainVector& operator-=(const IntervalVector& x2);
	Affine2MainVector& operator-=(const Affine2MainVector& x2);

	/**
	 * \brief x=d*x
	 */
	Affine2MainVector& operator*=(double d);

	/**
	 * \brief (*this)=x1*(*this).
	 */
	Affine2MainVector& operator*=(const Interval& x1);
	Affine2MainVector& operator*=(const Affine2Main<T>& x1);


};





/**
 * \brief Return the intersection of x and y.
 */
template<class T>
IntervalVector operator&(const Affine2MainVector<T>& x, const Affine2MainVector<T>& y);
template<class T>
IntervalVector operator&(const IntervalVector& x, const Affine2MainVector<T>& y);
template<class T>
IntervalVector operator&(const Affine2MainVector<T>& x, const IntervalVector& y);

/**
 * \brief Return the hull of x & y.
 */
template<class T>
IntervalVector operator|(const Affine2MainVector<T>& x, const Affine2MainVector<T>& y);
template<class T>
IntervalVector operator|(const IntervalVector& x, const Affine2MainVector<T>& y);
template<class T>
IntervalVector operator|(const Affine2MainVector<T>& x, const IntervalVector& y);

/**
 * \brief -x.
 */
template<class T>
Affine2MainVector<T> operator-(const Affine2MainVector<T>& x);

/**
 * \brief x1+x2.
 */

template<class T>
Affine2MainVector<T> operator+(const Vector& x1, const Affine2MainVector<T>& x2);

/**
 * \brief x1+x2.
 */
template<class T>
Affine2MainVector<T> operator+(const Affine2MainVector<T>& x1, const Vector& x2);

/**
 * \brief x1+x2.
 */
template<class T>
Affine2MainVector<T> operator+(const Affine2MainVector<T>& x1, const IntervalVector& x2);
template<class T>
Affine2MainVector<T> operator+(const IntervalVector& x1, const Affine2MainVector<T>& x2);
template<class T>
Affine2MainVector<T> operator+(const Affine2MainVector<T>& x1, const Affine2MainVector<T>& x2);

/**
 * \brief x1-x2.
 */
template<class T>
Affine2MainVector<T> operator-(const Vector& x1, const Affine2MainVector<T>& x2);

/**
 * \brief x1-x2.
 */

template<class T>
Affine2MainVector<T> operator-(const Affine2MainVector<T>& x1, const Vector& x2);

/**
 * \brief x1-x2.
 */
template<class T>
Affine2MainVector<T> operator-(const Affine2MainVector<T>& x1, const IntervalVector& x2);
template<class T>
Affine2MainVector<T> operator-(const IntervalVector& x1, const Affine2MainVector<T>& x2);
template<class T>
Affine2MainVector<T> operator-(const Affine2MainVector<T>& x1, const Affine2MainVector<T>& x2);

/**
 * \brief x1*x2.
 */
template<class T>
Affine2Main<T> operator*(const Vector& x1, const Affine2MainVector<T>& x2);

/**
 * \brief x1*x2.
 */
template<class T>
Affine2Main<T> operator*(const Affine2MainVector<T>& x1, const Vector& x2);

/**
 * \brief x1*x2.
 */
template<class T>
Affine2Main<T> operator*(const Affine2MainVector<T>& x1, const IntervalVector& x2);
template<class T>
Affine2Main<T> operator*(const IntervalVector& x1, const Affine2MainVector<T>& x2);
template<class T>
Affine2Main<T> operator*(const Affine2MainVector<T>& x1, const Affine2MainVector<T>& x2);

/**
 * \brief d*x
 */
template<class T>
Affine2MainVector<T> operator*(double d, const Affine2MainVector<T>& x);

/**
 * \brief x1*x2.
 */
template<class T>
Affine2MainVector<T> operator*(const Affine2Main<T>& x1, const Vector& x2);

/**
 *  \brief x1*x2.
 */
template<class T>
Affine2MainVector<T> operator*(const Affine2Main<T>& x1, const Affine2MainVector<T>& x2);
template<class T>
Affine2MainVector<T> operator*(const Interval& x1, const Affine2MainVector<T>& x2);

/**
 * \brief |x|.
 */
template<class T>
Affine2MainVector<T> abs(const Affine2MainVector<T>& x);

/**
 * \brief Display the Affine2MainVector<T> \a x
 */
template<class T>
std::ostream& operator<<(std::ostream& os, const Affine2MainVector<T>& x);

/**
 * \brief Cartesian product of x and y.
 *
 */
template<class T>
Affine2MainVector<T> cart_prod(const Affine2MainVector<T>& x, const Affine2MainVector<T>& y);


/*@}*/

/*=========================================================== inline implementation ============================================================= */

// the following functions are
// introduced to allow genericity
template<class T> inline bool ___is_empty(const Affine2MainVector<T>& v) { return v.is_empty(); }
template<class T> inline void ___set_empty(Affine2MainVector<T>& v)      { v.set_empty(); }

} // end namespace ibex

#include "ibex_LinearArith.h_"
#include "ibex_Affine2Vector.h_"

namespace ibex {


template<class T>
inline Affine2MainVector<T> Affine2MainVector<T>::empty(int n) {
	return Affine2MainVector<T>(n, Interval::EMPTY_SET);
}

template<class T>
inline Affine2MainVector<T>::~Affine2MainVector<T>() {
	delete[] _vec;
}

template<class T>
inline void Affine2MainVector<T>::set_empty() {
	(*this)[0] = Interval::EMPTY_SET;
}

template<class T>
inline const Affine2Main<T>& Affine2MainVector<T>::operator[](int i) const {
	assert(i>=0 && i<_n);
	return _vec[i];
}

template<class T>
inline Affine2Main<T>& Affine2MainVector<T>::operator[](int i) {
	assert(i>=0 && i<_n);
	return _vec[i];
}

template<class T>
inline void Affine2MainVector<T>::clear() {
	init(0);
}

template<class T>
inline bool Affine2MainVector<T>::operator!=(const IntervalVector& x) const {
	return !(*this==x);
}
template<class T>
inline bool Affine2MainVector<T>::operator!=(const Affine2MainVector<T>& x) const {
	return !(*this==x);
}

template<class T>
inline int Affine2MainVector<T>::size() const {
	return _n;
}

template<class T>
inline bool Affine2MainVector<T>::is_empty() const {
	return (*this)[0].is_empty();
}

template<class T>
inline IntervalVector operator&(const IntervalVector& x, const Affine2MainVector<T>& y) {
	return (y &  x);
}

template<class T>
inline IntervalVector operator|(const IntervalVector& x, const Affine2MainVector<T>& y) {
	return (y |  x);
}

template<class T>
inline Affine2MainVector<T> cart_prod(const Affine2MainVector<T>& x, const Affine2MainVector<T>& y) {
	Affine2MainVector<T> z(x.size()+y.size());
	z.put(0,x);
	z.put(x.size(),y);
	return z;
}


template<class T>
inline void Affine2MainVector<T>::compact(double tol) {
	assert(!is_empty());
	for (int i = 0; i < _n; i++) { 	_vec[i].compact(tol);	}
}

template<class T>
inline void Affine2MainVector<T>::compact() {
	assert(!is_empty());
	for (int i = 0; i < _n; i++) { 	_vec[i].compact();	}
}



template<class T>
inline Affine2MainVector<T> operator-(const Affine2MainVector<T>& x) {
	return minusV(x);
}


template<class T>
Affine2MainVector<T>& Affine2MainVector<T>::operator+=(const Vector& x2) {
	return set_addV<Affine2MainVector<T>,Vector>(*this,x2);
}

template<class T>
inline Affine2MainVector<T>& Affine2MainVector<T>::operator+=(const IntervalVector& x2) {
	return set_addV<Affine2MainVector<T>,IntervalVector>(*this,x2);
}

template<class T>
inline Affine2MainVector<T>& Affine2MainVector<T>::operator+=(const Affine2MainVector<T>& x2) {
	return set_addV<Affine2MainVector<T>,Affine2MainVector<T> >(*this,x2);
}

template<class T>
inline Affine2MainVector<T>& Affine2MainVector<T>::operator-=(const Vector& x2) {
	return set_subV<Affine2MainVector<T>,Vector>(*this,x2);
}

template<class T>
inline Affine2MainVector<T>& Affine2MainVector<T>::operator-=(const IntervalVector& x2) {
	return set_subV<Affine2MainVector<T>,IntervalVector>(*this,x2);
}

template<class T>
inline Affine2MainVector<T>& Affine2MainVector<T>::operator-=(const Affine2MainVector<T>& x2) {
	return set_subV<Affine2MainVector<T>,Affine2MainVector<T> >(*this,x2);
}

template<class T>
inline Affine2MainVector<T>& Affine2MainVector<T>::operator*=(double d) {
	return set_mulSV<double,Affine2MainVector<T> >(d,*this);
}

template<class T>
inline Affine2MainVector<T>& Affine2MainVector<T>::operator*=(const Interval& x1) {
	return set_mulSV<Interval,Affine2MainVector<T> >(x1,*this);
}

template<class T>
inline Affine2MainVector<T>& Affine2MainVector<T>::operator*=(const Affine2Main<T>& x1) {
	return set_mulSV<Affine2Main<T>,Affine2MainVector<T> >(x1,*this);
}

template<class T>
inline Affine2MainVector<T> abs( const Affine2MainVector<T>& x) {
	return absV(x);
}


template<class T>
inline Affine2MainVector<T> operator+(const Vector& x1, const Affine2MainVector<T>& x2) {
	return Affine2MainVector<T>(x2)+=x1;
}

template<class T>
inline Affine2MainVector<T> operator+(const Affine2MainVector<T>& x1, const Vector& x2) {
	return Affine2MainVector<T>(x1)+=x2;
}

template<class T>
inline Affine2MainVector<T> operator+(const IntervalVector& x1, const Affine2MainVector<T>& x2) {
	return x2 + x1;
}

template<class T>
inline Affine2MainVector<T> operator+(const Affine2MainVector<T>& x1, const IntervalVector& x2) {
	return Affine2MainVector<T>(x1)+=x2;
}

template<class T>
Affine2MainVector<T> operator+(const Affine2MainVector<T>& x1, const Affine2MainVector<T>& x2) {
	return Affine2MainVector<T>(x1)+=x2;
}


template<class T>
inline Affine2MainVector<T> operator-(const Vector& x1, const Affine2MainVector<T>& x2) {
	Affine2MainVector<T> res(x2.size());
	res = (-x2);
	return res += x1;
}

template<class T>
inline Affine2MainVector<T> operator-(const Affine2MainVector<T>& x1, const Vector& x2) {
	return Affine2MainVector<T>(x1)-=x2;
}

template<class T>
inline Affine2MainVector<T> operator-(const Affine2MainVector<T>& x1, const IntervalVector& x2) {
	return Affine2MainVector<T>(x1)-=x2;
}

template<class T>
inline Affine2MainVector<T> operator-(const IntervalVector& x1, const Affine2MainVector<T>& x2) {
	Affine2MainVector<T> res(x2.size());
	res = (-x2);
	return res += x1;
}
template<class T>
inline Affine2MainVector<T> operator-(const Affine2MainVector<T>& x1, const Affine2MainVector<T>& x2) {
	return Affine2MainVector<T>(x1) += (-x2);
}

template<class T>
inline Affine2MainVector<T> operator*(double d, const Affine2MainVector<T>& x) {
	return Affine2MainVector<T>(x)*=d;
}

template<class T>
inline Affine2MainVector<T> operator*(const Affine2Main<T>& x1, const Affine2MainVector<T>& x2) {
	return Affine2MainVector<T>(x2)*=x1;
}

template<class T>
inline Affine2MainVector<T> operator*(const Interval& x1, const Affine2MainVector<T>& x2) {
	return Affine2MainVector<T>(x2)*=x1;
}

template<class T>
inline Affine2MainVector<T> operator*(const IntervalMatrix& m, const Affine2MainVector<T>& x) {
	return mulMV<IntervalMatrix,Affine2MainVector<T>,Affine2MainVector<T> >(m,x);
}

template<class T>
inline Affine2MainVector<T> operator*(const Affine2MainVector<T>& x, const Matrix& m) {
	return mulVM<Affine2MainVector<T>,Matrix,Affine2MainVector<T> >(x,m);
}


template<class T>
inline Affine2MainVector<T> operator*(const Affine2MainVector<T>& x, const IntervalMatrix& m) {
	return mulVM<Affine2MainVector<T>,IntervalMatrix,Affine2MainVector<T> >(x,m);
}



} // end namespace

#endif /* _IBEX_AFFINE2_VECTOR_H_ */
