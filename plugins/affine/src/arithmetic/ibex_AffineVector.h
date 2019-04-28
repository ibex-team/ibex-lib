/* ============================================================================
 * I B E X - Affine Vector definition
 * ============================================================================
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Jordan Ninin
 * Created     : March 13, 2013
 * ---------------------------------------------------------------------------- */

#ifndef __IBEX_AFFINE_VECTOR_H__
#define __IBEX_AFFINE_VECTOR_H__

#include <cassert>
#include <iostream>
#include "ibex_Interval.h"
#include "ibex_TemplateVector.h"
#include "ibex_IntervalVector.h"
#include "ibex_IntervalMatrix.h"
#include "ibex_Vector.h"
#include "ibex_Matrix.h"
#include "ibex_Affine.h"

namespace ibex {

template<class T> class AffineMainMatrix;

/**
 * \ingroup arithmetic
 *
 * \brief Vector of Affine2 form
 *
 * By convention an empty vector has a dimension. A vector becomes empty
 * when one of its component becomes empty and all the components
 * are set to the empty Interval.
 */

typedef AffineMainVector<AF_Default> Affine2Vector;
typedef AffineMainVector<AF_Other> Affine3Vector;

template<class T=AF_Default>
class AffineMainVector {


private:
	friend class AffineMainMatrix<T>;

	AffineMainVector() : _n(0), _vec(NULL) { }

	int _n;             // dimension (size of vec)
	AffineMain<T> *_vec;	   // vector of elements

public:

	/** \brief  Create \a n Affine2 form . All the components are Affine2([-oo,+oo])
	 * \pre n>0
	 */
	explicit AffineMainVector(int n);

	/**
	 * \brief  Create an Affine2MainVector of dimension \a n with
	 * all the components initialized to
	 *  if (!b) Affine2(\a x)
	 *  else Affine2(\a n,i+1,\a x) }.
	 * \pre n>0
	 */
	AffineMainVector(int n, const Interval& x);

	/**
	 * \brief  Create \a n Affine2MainVector of dimension \a n with
	 * all the components initialized to \a x.
	 * \pre n>0
	 */
	AffineMainVector(int n, const AffineMain<T>& x);

	/**
	 * \brief Create a copy of \a x.
	 */
//	Affine2MainVector(const Affine2MainVector& x);


	/**
	 * \brief Create  a copy of  { \a  x if !(\a b)  else -(\a x) }.
	 */
	AffineMainVector(const AffineMainVector& x);

	/**
	 * \brief Create \a n Affine2MainVector  initialized by
	 * if !(\a b)  Affine2(Interval(bounds[i][0],bounds[i][1]) )
	 * else Affine2(\a n, i+1,Interval(bounds[i][0],bounds[i][1]) )
	 *
	 * \param bounds an nx2 array of doubles
	 * \pre n>0
	 */
	AffineMainVector(int n, double  bounds[][2]);

	/**
	 * \brief Create \a x.size Affine2MainVector of dimension \a x.size with
	 * the [i] component initialized to
	 * if !(\a b) Affine2(x[i])
	 * else  Affine2(x.size(), i+1,x[i])
	 */
	explicit AffineMainVector(const IntervalVector& x);

	/**
	 * \brief Create the degenerated Affine2MainVector x
	 *
	 */
	explicit AffineMainVector(const Vector& x);

	/**
	 * \brief Create [empty; ...; empty]
	 *
	 * Create an empty Affine2MainVector of dimension \a n
	 * (all the components being empty Intervals)
	 *
	 * \pre n>0
	 */
	static AffineMainVector empty(int n);

	/**
	 * \brief Delete this vector
	 */
	virtual ~AffineMainVector();

	/**
	 * \brief Return the ith Affine2
	 *
	 * A return a const reference to the
	 * i^th component (i starts from 0)
	 */
	const AffineMain<T>& operator[](int i) const;

	/**
	 * \brief Return the ith Affine2
	 *
	 * A return a non-const reference to the
	 * i^th component (i starts from 0)
	 */
	AffineMain<T>& operator[](int i);

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
	void init(const Interval& x);

	/**
	 * \brief Set all the elements to x
	 *
	 * \note Emptiness is "overridden".
	 */
	void init(const AffineMain<T>& x);

	/**
	 * \brief Add [-rad,+rad] to all the components of *this.
	 *
	 * \return *this.
	 */
	AffineMainVector& inflate(double rad);

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
	AffineMainVector subvector(int start_index, int end_index) const;

	/**
	 * \brief Put a subvector into *this at a given position.
	 *
	 * \param start_index - the position where the subvector
	 * \param subvec - the subvector
	 *
	 * \pre (*this) must not be empty
	 */
	void put(int start_index, const AffineMainVector& subvec);

	/**
	 * \brief Assign this Affine2MainVector to x.
	 *
	 * \pre Dimensions of this and x must match.
	 * \note Emptiness is overridden.
	 */
	AffineMainVector& operator=(const AffineMainVector& x);
	AffineMainVector& operator=(const IntervalVector& x);

	/**
	 * \brief Return true if the bounds of this Affine2MainVector match that of \a x.
	 */
	bool operator==(const AffineMainVector& x) const;
	bool operator==(const IntervalVector& x) const;

	/**
	 * \brief Return true if one bounds of one component of *this differs from \a x.
	 */
	bool operator!=(const IntervalVector& x) const;
	bool operator!=(const AffineMainVector& x) const;


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
	AffineMainVector& operator+=(const Vector& x2);

	/**
	 * \brief (*this)+=x2.
	 */
	AffineMainVector& operator+=(const IntervalVector& x2);
	AffineMainVector& operator+=(const AffineMainVector& x2);

	/**
	 * \brief (*this)-=x2.
	 */
	AffineMainVector& operator-=(const Vector& x2);

	/**
	 * \brief (*this)-=x2.
	 */
	AffineMainVector& operator-=(const IntervalVector& x2);
	AffineMainVector& operator-=(const AffineMainVector& x2);

	/**
	 * \brief x=d*x
	 */
	AffineMainVector& operator*=(double d);

	/**
	 * \brief (*this)=x1*(*this).
	 */
	AffineMainVector& operator*=(const Interval& x1);
	AffineMainVector& operator*=(const AffineMain<T>& x1);


};





/**
 * \brief Return the intersection of x and y.
 */
template<class T>
IntervalVector operator&(const AffineMainVector<T>& x, const AffineMainVector<T>& y);
template<class T>
IntervalVector operator&(const IntervalVector& x, const AffineMainVector<T>& y);
template<class T>
IntervalVector operator&(const AffineMainVector<T>& x, const IntervalVector& y);

/**
 * \brief Return the hull of x & y.
 */
template<class T>
IntervalVector operator|(const AffineMainVector<T>& x, const AffineMainVector<T>& y);
template<class T>
IntervalVector operator|(const IntervalVector& x, const AffineMainVector<T>& y);
template<class T>
IntervalVector operator|(const AffineMainVector<T>& x, const IntervalVector& y);

/**
 * \brief -x.
 */
template<class T>
AffineMainVector<T> operator-(const AffineMainVector<T>& x);

/**
 * \brief x1+x2.
 */

template<class T>
AffineMainVector<T> operator+(const Vector& x1, const AffineMainVector<T>& x2);

/**
 * \brief x1+x2.
 */
template<class T>
AffineMainVector<T> operator+(const AffineMainVector<T>& x1, const Vector& x2);

/**
 * \brief x1+x2.
 */
template<class T>
AffineMainVector<T> operator+(const AffineMainVector<T>& x1, const IntervalVector& x2);
template<class T>
AffineMainVector<T> operator+(const IntervalVector& x1, const AffineMainVector<T>& x2);
template<class T>
AffineMainVector<T> operator+(const AffineMainVector<T>& x1, const AffineMainVector<T>& x2);

/**
 * \brief x1-x2.
 */
template<class T>
AffineMainVector<T> operator-(const Vector& x1, const AffineMainVector<T>& x2);

/**
 * \brief x1-x2.
 */

template<class T>
AffineMainVector<T> operator-(const AffineMainVector<T>& x1, const Vector& x2);

/**
 * \brief x1-x2.
 */
template<class T>
AffineMainVector<T> operator-(const AffineMainVector<T>& x1, const IntervalVector& x2);
template<class T>
AffineMainVector<T> operator-(const IntervalVector& x1, const AffineMainVector<T>& x2);
template<class T>
AffineMainVector<T> operator-(const AffineMainVector<T>& x1, const AffineMainVector<T>& x2);

/**
 * \brief x1*x2.
 */
template<class T>
AffineMain<T> operator*(const Vector& x1, const AffineMainVector<T>& x2);

/**
 * \brief x1*x2.
 */
template<class T>
AffineMain<T> operator*(const AffineMainVector<T>& x1, const Vector& x2);

/**
 * \brief x1*x2.
 */
template<class T>
AffineMain<T> operator*(const AffineMainVector<T>& x1, const IntervalVector& x2);
template<class T>
AffineMain<T> operator*(const IntervalVector& x1, const AffineMainVector<T>& x2);
template<class T>
AffineMain<T> operator*(const AffineMainVector<T>& x1, const AffineMainVector<T>& x2);

/**
 * \brief d*x
 */
template<class T>
AffineMainVector<T> operator*(double d, const AffineMainVector<T>& x);

/**
 * \brief x1*x2.
 */
template<class T>
AffineMainVector<T> operator*(const AffineMain<T>& x1, const Vector& x2);

/**
 *  \brief x1*x2.
 */
template<class T>
AffineMainVector<T> operator*(const AffineMain<T>& x1, const AffineMainVector<T>& x2);
template<class T>
AffineMainVector<T> operator*(const Interval& x1, const AffineMainVector<T>& x2);

/**
 * \brief |x|.
 */
template<class T>
AffineMainVector<T> abs(const AffineMainVector<T>& x);

/**
 * \brief Display the Affine2MainVector<T> \a x
 */
template<class T>
std::ostream& operator<<(std::ostream& os, const AffineMainVector<T>& x);

/**
 * \brief Cartesian product of x and y.
 *
 */
template<class T>
AffineMainVector<T> cart_prod(const AffineMainVector<T>& x, const AffineMainVector<T>& y);


/*@}*/

/*=========================================================== inline implementation ============================================================= */

// the following functions are
// introduced to allow genericity
template<class T> inline bool ___is_empty(const AffineMainVector<T>& v) { return v.is_empty(); }
template<class T> inline void ___set_empty(AffineMainVector<T>& v)      { v.set_empty(); }

} // end namespace ibex

#include "ibex_LinearArith.h"

namespace ibex {

template<class T>
AffineMainVector<T>::AffineMainVector(int n) :
		_n(n),
		_vec(new AffineMain<T>[n]) {
	assert(n>=1);
	for (int i = 0; i < n; i++){
		_vec[i] = AffineMain<T>();
	}
}

template<class T>
AffineMainVector<T>::AffineMainVector(int n, const Interval& x) :
		_n(n),
		_vec(new AffineMain<T>[n]) {
	assert(n>=1);
	for (int i = 0; i < n; i++) {
		_vec[i] = AffineMain<T>(n, i + 1, x);
	}

}

template<class T>
AffineMainVector<T>::AffineMainVector(int n, const AffineMain<T>& x) :
		_n(n),
		_vec(new AffineMain<T>[n]) {
	assert(n>=1);
	for (int i = 0; i < n; i++) {
		_vec[i] = x;
	}
}


template<class T>
AffineMainVector<T>::AffineMainVector(const AffineMainVector<T>& x) :
		_n(x.size()),
		_vec(new AffineMain<T>[x.size()]) {

	for (int i = 0; i < _n; i++){
		_vec[i] = AffineMain<T>(x[i]);
	}

}

template<class T>
AffineMainVector<T>::AffineMainVector(int n, double bounds[][2]) :
		_n(n),
		_vec(new AffineMain<T>[n]) {
	if (bounds == 0){ // probably, the user called Affine2MainVector<T>(n,0) and 0 is interpreted as NULL!
		for (int i = 0; i < n; i++){
			_vec[i] = AffineMain<T>( 0.0); // Affine2Main<T>(n, i + 1, 0.0);
		}
	}
	else {
		for (int i = 0; i < n; i++){
			_vec[i] =  AffineMain<T>(n, i + 1, Interval(bounds[i][0], bounds[i][1]));
		}

	}
}

template<class T>
AffineMainVector<T>::AffineMainVector(const IntervalVector& x) :
		_n(x.size()),
		_vec(new AffineMain<T>[x.size()]) {

	for (int i = 0; i < x.size(); i++) {
		_vec[i] = AffineMain<T>(x.size(), i + 1, x[i]);
	}
}

template<class T>
AffineMainVector<T>::AffineMainVector(const Vector& x) :
		_n(x.size()),
		_vec(new AffineMain<T>[x.size()]) {
	for (int i = 0; i < _n; i++){
		_vec[i] = AffineMain<T>(x[i]);
	}
}

template<class T>
void AffineMainVector<T>::init(const Interval& x) {

	for (int i = 0; i < size(); i++) {
		(*this)[i] = AffineMain<T>(size(),i+1,x);
	}

}

template<class T>
void AffineMainVector<T>::init(const AffineMain<T>& x) {
	for (int i = 0; i < size(); i++) {
		(*this)[i] = x;
	}
}

template<class T>
void AffineMainVector<T>::resize(int n) {
	assert(n>=1);
	assert((_vec==NULL && _n==0) || (size()!=0 && _vec!=NULL));

	if (n==size()) return;

	AffineMain<T>* newVec=new AffineMain<T>[n];
	int i=0;
	for (; i<size() && i<n; i++){
		newVec[i]=_vec[i];
	}
	for (; i<n; i++){
		newVec[i]= AffineMain<T>();
	}
	if (_vec!=NULL) { // vec==NULL happens when default constructor is used (n==0)
		delete[] _vec;
	}
	_n   = n;
	_vec = newVec;
}

template<class T>
IntervalVector operator&(const AffineMainVector<T>& y,const IntervalVector& x)  {
	// dimensions are non zero henceforth
	if (y.size()!=x.size()) throw InvalidIntervalVectorOp("Cannot intersect Affine2MainVector<T>es with different dimensions");

	if (y.is_empty()||x.is_empty())
		return IntervalVector::empty(y.size());

	IntervalVector res(y.size());
	for (int i=0; i<y.size(); i++) {
		res [i] = y[i] & x[i];
		if (res[i].is_empty()) {
			res.set_empty();
			return res;
		}
	}
	return res;
}
template<class T>
IntervalVector operator&(const AffineMainVector<T>& y,const AffineMainVector<T>& x)  {
	// dimensions are non zero henceforth
	if (y.size()!=x.size()) throw InvalidIntervalVectorOp("Cannot intersect Affine2MainVector<T>es with different dimensions");

	if (y.is_empty()||x.is_empty())
		return IntervalVector::empty(y.size());

	IntervalVector res(y.size());
	for (int i=0; i<y.size(); i++) {
		res [i] = y[i] & x[i];
		if (res[i].is_empty()) {
			res.set_empty();
			return res;
		}
	}
	return res;
}
template<class T>
IntervalVector operator|(const AffineMainVector<T>& y,const IntervalVector& x)  {
	// dimensions are non zero henceforth
	if (y.size()!=x.size()) throw InvalidIntervalVectorOp("Cannot make the hull of Affine2MainVector<T>es with different dimensions");

	if (y.is_empty()&&x.is_empty())
		return IntervalVector::empty(y.size());

	IntervalVector res(y.size());
	for (int i=0; i<y.size(); i++) {
		res [i] = y[i] | x[i];
	}
	return res;
}
template<class T>
IntervalVector operator|(const AffineMainVector<T>& y,const AffineMainVector<T>& x)  {
	// dimensions are non zero henceforth
	if (y.size()!=x.size()) throw InvalidIntervalVectorOp("Cannot make the hull of Affine2MainVector<T>es with different dimensions");

	if (y.is_empty()&&x.is_empty())
		return IntervalVector::empty(y.size());

	IntervalVector res(y.size());
	for (int i=0; i<y.size(); i++) {
		res [i] = y[i] | x[i];
	}
	return res;
}


template<class T>
IntervalVector AffineMainVector<T>::itv() const {
	assert(!is_empty());
	IntervalVector intv(_n);
	for (int i = 0; i < _n; i++) {
		intv[i] = (*this)[i].itv();
	}
	return intv;
}



template<class T>
AffineMain<T> operator*(const Vector& v1, const AffineMainVector<T>& v2) {
	assert(v1.size()==v2.size());

	int n=v1.size();
	AffineMain<T> y(0);

	if (v2.is_empty()) {
		y.set_empty();
		return y;
	}

	for (int i=0; i<n; i++) {
		y+=v1[i]*v2[i];
	}
	return y;
}

template<class T>
AffineMain<T> operator*(const AffineMainVector<T>& v1, const Vector& v2) {
	assert(v1.size()==v2.size());

	int n=v1.size();
	AffineMain<T> y(0);

	if (v1.is_empty()) {
		y.set_empty();
		return y;
	}

	for (int i=0; i<n; i++) {
		y+=v1[i]*v2[i];
	}
	return y;
}


template<class T>
AffineMain<T> operator*(const IntervalVector& x1, const AffineMainVector<T>& x2){
	return x2*x1;
}

template<class T>
AffineMain<T> operator*(const AffineMainVector<T>& v1, const IntervalVector& v2) {
	assert(v1.size()==v2.size());

	int n=v1.size();
	AffineMain<T> y(0);

	if (v1.is_empty() || v2.is_empty()) {
		y.set_empty();
		return y;
	}

	for (int i=0; i<n; i++) {
		y+=v1[i] * v2[i];
	}
	return y;
}

template<class T>
AffineMain<T> operator*(const AffineMainVector<T>& v1, const AffineMainVector<T>& v2) {
	assert(v1.size()==v2.size());
	assert(v1.size()==v2.size());

	int n=v1.size();
	AffineMain<T> y(0);

	if (v1.is_empty() || v2.is_empty()) {
		y.set_empty();
		return y;
	}

	for (int i=0; i<n; i++) {
		y+=v1[i] * v2[i];
	}
	return y;
}

template<class T>
AffineMainVector<T> operator*(const AffineMain<T>& x1, const Vector& x2) {
	AffineMainVector<T> res(x2.size(),x1);
	for (int i=0; i<x2.size(); i++) {
		res[i] *= x2[i];
	}
	return  res;
}

template<class T>
AffineMainVector<T>& AffineMainVector<T>::inflate(double rad1)                              { return _inflate(*this,rad1); }
template<class T>
AffineMainVector<T>  AffineMainVector<T>::subvector(int start_index, int end_index) const   { return _subvector(*this,start_index,end_index); }
template<class T>
void           AffineMainVector<T>::put(int start_index, const AffineMainVector<T>& subvec) { _put(*this, start_index, subvec); }
template<class T>
AffineMainVector<T>& AffineMainVector<T>::operator=(const AffineMainVector<T>& x)                 { resize(x.size()); // see issue #10
                                                                                  return _assignV(*this,x); }
template<class T>
AffineMainVector<T>& AffineMainVector<T>::operator=(const IntervalVector& x)                { return _assignV(*this,x); }
template<class T>
bool           AffineMainVector<T>::operator==(const AffineMainVector<T>& x) const          { return _equalsV(*this,x); }
template<class T>
bool           AffineMainVector<T>::operator==(const IntervalVector& x) const         { return _equalsV(*this,x); }
//Vector         Affine2MainVector<T>::lb() const                                        { return _lb(*this); }
//Vector         Affine2MainVector<T>::ub() const                                        { return _ub(*this); }
//Vector         Affine2MainVector<T>::mid() const                                       { return _mid(*this); }
//Vector         Affine2MainVector<T>::mig() const                                       { return _mig(*this); }
//Vector         Affine2MainVector<T>::mag() const                                       { return _mag(*this); }
//bool           Affine2MainVector<T>::is_flat() const                                   { return _is_flat(*this); }
//bool           Affine2MainVector<T>::contains(const Vector& x) const                   { return _contains(*this,x); }
template<class T>
bool           AffineMainVector<T>::is_unbounded() const                              { return _is_unbounded(*this); }
//bool           Affine2MainVector<T>::is_subset(const Affine2MainVector<T>& x) const           { return _is_subset(*this,x); }
//bool           Affine2MainVector<T>::is_subset(const IntervalVector& x) const          { return _is_subset(*this,x); }
//bool           Affine2MainVector<T>::is_strict_subset(const Affine2MainVector<T>& x) const    { return _is_strict_subset(*this,x); }
//bool           Affine2MainVector<T>::is_strict_subset(const IntervalVector& x) const   { return _is_strict_subset(*this,x); }
//bool           Affine2MainVector<T>::is_zero() const                                   { return _is_zero(*this); }
//bool           Affine2MainVector<T>::is_bisectable() const                             { return _is_bisectable(*this); }
//Vector         Affine2MainVector<T>::rad() const                                       { return _rad(*this); }
//Vector         Affine2MainVector<T>::diam() const                                      { return _diam(*this); }
//int            Affine2MainVector<T>::extr_diam_index(bool min) const                   { return _extr_diam_index(*this,min); }
template<class T>
std::ostream& operator<<(std::ostream& os, const AffineMainVector<T>& x)              { return _displayV(os,x); }
//double         Affine2MainVector<T>::volume() const                                    { return _volume(*this); }
//double         Affine2MainVector<T>::perimeter() const                                 { return _perimeter(*this); }
//double         Affine2MainVector<T>::rel_distance(const Affine2MainVector<T>& x) const        { return _rel_distance(*this,x); }
//double         Affine2MainVector<T>::rel_distance(const IntervalVector& x) const       { return _rel_distance(*this,x); }
//Vector         Affine2MainVector<T>::random() const                                    { return _random<Affine2MainVector<T>,Affine2>(*this); }

//int          Affine2MainVector<T>::diff(const Affine2MainVector<T>& y, IntervalVector*& result) const   { return _diff(itv(), y.itv(), result); }
//int          Affine2MainVector<T>::diff(const IntervalVector& y, IntervalVector*& result) const  { return _diff(itv(), y, result); }
//int          Affine2MainVector<T>::complementary(IntervalVector*& result) const                  { return _complementary(itv(), result); }
//std::pair<IntervalVector,IntervalVector> Affine2MainVector<T>::bisect(int i, double ratio) const { return _bisect(*this, i, ratio); }



template<class T>
inline AffineMainVector<T> AffineMainVector<T>::empty(int n) {
	return AffineMainVector<T>(n, Interval::empty_set());
}

template<class T>
inline AffineMainVector<T>::~AffineMainVector<T>() {
	delete[] _vec;
}

template<class T>
inline void AffineMainVector<T>::set_empty() {
	(*this)[0] = Interval::empty_set();
}

template<class T>
inline const AffineMain<T>& AffineMainVector<T>::operator[](int i) const {
	assert(i>=0 && i<_n);
	return _vec[i];
}

template<class T>
inline AffineMain<T>& AffineMainVector<T>::operator[](int i) {
	assert(i>=0 && i<_n);
	return _vec[i];
}

template<class T>
inline void AffineMainVector<T>::clear() {
	init(Interval::zero());
}

template<class T>
inline bool AffineMainVector<T>::operator!=(const IntervalVector& x) const {
	return !(*this==x);
}
template<class T>
inline bool AffineMainVector<T>::operator!=(const AffineMainVector<T>& x) const {
	return !(*this==x);
}

template<class T>
inline int AffineMainVector<T>::size() const {
	return _n;
}

template<class T>
inline bool AffineMainVector<T>::is_empty() const {
	return (*this)[0].is_empty();
}

template<class T>
inline IntervalVector operator&(const IntervalVector& x, const AffineMainVector<T>& y) {
	return (y &  x);
}

template<class T>
inline IntervalVector operator|(const IntervalVector& x, const AffineMainVector<T>& y) {
	return (y |  x);
}

template<class T>
inline AffineMainVector<T> cart_prod(const AffineMainVector<T>& x, const AffineMainVector<T>& y) {
	AffineMainVector<T> z(x.size()+y.size());
	z.put(0,x);
	z.put(x.size(),y);
	return z;
}


template<class T>
inline void AffineMainVector<T>::compact(double tol) {
	assert(!is_empty());
	for (int i = 0; i < _n; i++) { 	_vec[i].compact(tol);	}
}

template<class T>
inline void AffineMainVector<T>::compact() {
	assert(!is_empty());
	for (int i = 0; i < _n; i++) { 	_vec[i].compact();	}
}



template<class T>
inline AffineMainVector<T> operator-(const AffineMainVector<T>& x) {
	return minusV(x);
}


template<class T>
AffineMainVector<T>& AffineMainVector<T>::operator+=(const Vector& x2) {
	return set_addV<AffineMainVector<T>,Vector>(*this,x2);
}

template<class T>
inline AffineMainVector<T>& AffineMainVector<T>::operator+=(const IntervalVector& x2) {
	return set_addV<AffineMainVector<T>,IntervalVector>(*this,x2);
}

template<class T>
inline AffineMainVector<T>& AffineMainVector<T>::operator+=(const AffineMainVector<T>& x2) {
	return set_addV<AffineMainVector<T>,AffineMainVector<T> >(*this,x2);
}

template<class T>
inline AffineMainVector<T>& AffineMainVector<T>::operator-=(const Vector& x2) {
	return set_subV<AffineMainVector<T>,Vector>(*this,x2);
}

template<class T>
inline AffineMainVector<T>& AffineMainVector<T>::operator-=(const IntervalVector& x2) {
	return set_subV<AffineMainVector<T>,IntervalVector>(*this,x2);
}

template<class T>
inline AffineMainVector<T>& AffineMainVector<T>::operator-=(const AffineMainVector<T>& x2) {
	return set_subV<AffineMainVector<T>,AffineMainVector<T> >(*this,x2);
}

template<class T>
inline AffineMainVector<T>& AffineMainVector<T>::operator*=(double d) {
	return set_mulSV<double,AffineMainVector<T> >(d,*this);
}

template<class T>
inline AffineMainVector<T>& AffineMainVector<T>::operator*=(const Interval& x1) {
	return set_mulSV<Interval,AffineMainVector<T> >(x1,*this);
}

template<class T>
inline AffineMainVector<T>& AffineMainVector<T>::operator*=(const AffineMain<T>& x1) {
	return set_mulSV<AffineMain<T>,AffineMainVector<T> >(x1,*this);
}

template<class T>
inline AffineMainVector<T> abs( const AffineMainVector<T>& x) {
	return absV(x);
}


template<class T>
inline AffineMainVector<T> operator+(const Vector& x1, const AffineMainVector<T>& x2) {
	return AffineMainVector<T>(x2)+=x1;
}

template<class T>
inline AffineMainVector<T> operator+(const AffineMainVector<T>& x1, const Vector& x2) {
	return AffineMainVector<T>(x1)+=x2;
}

template<class T>
inline AffineMainVector<T> operator+(const IntervalVector& x1, const AffineMainVector<T>& x2) {
	return x2 + x1;
}

template<class T>
inline AffineMainVector<T> operator+(const AffineMainVector<T>& x1, const IntervalVector& x2) {
	return AffineMainVector<T>(x1)+=x2;
}

template<class T>
AffineMainVector<T> operator+(const AffineMainVector<T>& x1, const AffineMainVector<T>& x2) {
	return AffineMainVector<T>(x1)+=x2;
}


template<class T>
inline AffineMainVector<T> operator-(const Vector& x1, const AffineMainVector<T>& x2) {
	AffineMainVector<T> res(x2.size());
	res = (-x2);
	return res += x1;
}

template<class T>
inline AffineMainVector<T> operator-(const AffineMainVector<T>& x1, const Vector& x2) {
	return AffineMainVector<T>(x1)-=x2;
}

template<class T>
inline AffineMainVector<T> operator-(const AffineMainVector<T>& x1, const IntervalVector& x2) {
	return AffineMainVector<T>(x1)-=x2;
}

template<class T>
inline AffineMainVector<T> operator-(const IntervalVector& x1, const AffineMainVector<T>& x2) {
	AffineMainVector<T> res(x2.size());
	res = (-x2);
	return res += x1;
}
template<class T>
inline AffineMainVector<T> operator-(const AffineMainVector<T>& x1, const AffineMainVector<T>& x2) {
	return AffineMainVector<T>(x1) += (-x2);
}

template<class T>
inline AffineMainVector<T> operator*(double d, const AffineMainVector<T>& x) {
	return AffineMainVector<T>(x)*=d;
}

template<class T>
inline AffineMainVector<T> operator*(const AffineMain<T>& x1, const AffineMainVector<T>& x2) {
	return AffineMainVector<T>(x2)*=x1;
}

template<class T>
inline AffineMainVector<T> operator*(const Interval& x1, const AffineMainVector<T>& x2) {
	return AffineMainVector<T>(x2)*=x1;
}

template<class T>
inline AffineMainVector<T> operator*(const IntervalMatrix& m, const AffineMainVector<T>& x) {
	return mulMV<IntervalMatrix,AffineMainVector<T>,AffineMainVector<T> >(m,x);
}

template<class T>
inline AffineMainVector<T> operator*(const AffineMainVector<T>& x, const Matrix& m) {
	return mulVM<AffineMainVector<T>,Matrix,AffineMainVector<T> >(x,m);
}


template<class T>
inline AffineMainVector<T> operator*(const AffineMainVector<T>& x, const IntervalMatrix& m) {
	return mulVM<AffineMainVector<T>,IntervalMatrix,AffineMainVector<T> >(x,m);
}



} // end namespace

#endif /* _IBEX_AFFINE_VECTOR_H_ */
