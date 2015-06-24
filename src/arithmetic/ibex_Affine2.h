/* ============================================================================
 * I B E X - Affine2 definition
 * ============================================================================
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Jordan Ninin
 * Bug fixes   :
 * Created     : Nov 12, 2012
 * ---------------------------------------------------------------------------- */

#ifndef IBEX_AFFINE2_H_
#define IBEX_AFFINE2_H_

#include "ibex_Interval.h"
#include <math.h>
#include <cassert>
#include "ibex_Exception.h"

#include "ibex_Affine2_fAF1.h"
#include "ibex_Affine2_fAF2.h"
#include "ibex_Affine2_iAF.h"
#include "ibex_Affine2_fAF2_fma.h"
#include "ibex_Affine2_sAF.h"
#include "ibex_Affine2_No.h"
#include "ibex_Affine2_fAFFullI.h"


namespace ibex {

template<class T> class Affine2MainVector;
template<class T> class Affine2MainMatrix;
template<class T> class Affine2MainMatrixArray;
class Affine2Eval;
class AffineLinEval;

/**
 * \ingroup arithmetic
 *
 * \brief Affine Arithmetic AF2
 *
 */

//typedef AF_iAF  AF_Linear;
typedef AF_fAF2  AF_Linear;

//typedef AF_fAF1  AF_Default;
//typedef AF_fAF2  AF_Default;
//typedef AF_fAF2_fma  AF_Default;
//typedef AF_iAF  AF_Default;
//typedef AF_sAF  AF_Default;
//typedef AF_No  AF_Default;
typedef AF_fAFFullI AF_Default;

typedef Affine2Main<AF_Default> Affine2;
typedef Affine2Main<AF_Linear>  AffineLin;


template<class T=AF_Default>
class Affine2Main {

private:
	friend class Affine2Eval;
	friend class AffineLinEval;

	/** \brief tolerance for default compact procedure  */
	static const double AF_COMPAC_Tol = 1.e-6;


	typedef enum {
		AF_SQRT, AF_EXP, AF_LOG, AF_INV, AF_COS, AF_SIN, AF_TAN, AF_ABS,AF_ACOS, AF_ASIN, AF_ATAN, AF_COSH, AF_SINH, AF_TANH
	} Affine2_expr; // ...etc...


	/**
	 * Code for the particular case:
	 * if the affine form is actif, _n>1  and _n is the size of the affine form
	 * if the set is degenerate, _n = 0 or itv().diam()< AF_EC()
	 * if the set is empty, _n = -1
	 * if the set is ]-oo,+oo[, _n = -2 and _ err=]-oo,+oo[
	 * if the set is [a, +oo[ , _n = -3 and _err = [a, +oo[
	 * if the set is ]-oo, a] , _n = -4 and _err = ]-oo, a]
	 *
	 */

	int _n; 		// dimension (size of val)-1  , ie number of variable

	T _elt;			// core of the affine2 form


	static double AF_EM()  {
		return __builtin_powi(2.0, -51);
	}
	static double AF_EC()   {
		return __builtin_powi(2.0, -55);
	}
	static double AF_EE()   {
		return 2.0;
	}

	/**
	 * \brief  Compute x = alpha*x+y+beta
	 * The boolean indicate which operation need to be done.
	 *  ! Boolean: scalar, add, constant, error
	 * TO WRITE in your Affine implementation
	 */
	Affine2Main& saxpy(double alpha, const Affine2Main& y, double beta, double delta,
			bool B1, bool B2, bool B3, bool B4);


	/**
	 *\brief compute the min-range linearization of an unary operator
	 */
	Affine2Main& linMinRange(Affine2_expr num, const Interval& itv);

	/**
	 *\brief compute the chebyshev linearization of an unary operator
	 */
	Affine2Main& linChebyshev(Affine2_expr num, const Interval& itv);

	/**
	 * TO WRITE in your Affine implementation
	 */
	Affine2Main&  sqrA(const Interval& itv);


	Affine2Main&  powerA(int n, const Interval& itv);
	Affine2Main&  invA(const Interval& itv);
	Affine2Main&  sqrtA(const Interval& itv);
	Affine2Main&  expA(const Interval& itv);
	Affine2Main&  logA(const Interval& itv);
	Affine2Main&  powerA(double d, const Interval& itv);
	Affine2Main&  powerA(const Interval &y, const Interval& itvx);
	Affine2Main&  rootA(int n, const Interval& itv);
	Affine2Main&  cosA(const Interval& itv);
	Affine2Main&  sinA(const Interval& itv);
	Affine2Main&  tanA(const Interval& itv);
	Affine2Main&  acosA(const Interval& itv);
	Affine2Main&  asinA(const Interval& itv);
	Affine2Main&  atanA(const Interval& itv);
	Affine2Main&  coshA(const Interval& itv);
	Affine2Main&  sinhA(const Interval& itv);
	Affine2Main&  tanhA(const Interval& itv);
	Affine2Main&  absA(const Interval& itv);
	Affine2Main&  signA(const Interval& itv);



public:

	/** \brief Create an empty affine form.
	 * TO WRITE in your Affine implementation
	 */
	Affine2Main();

	/** \brief Create an affine form with n variables and  initialized val[0] with d.
	 * TO WRITE in your Affine implementation
	 */
	explicit Affine2Main(const double d);

	/** \brief Create an affine form with n variables and  initialized val[0] with  itv.
	 * TO WRITE in your Affine implementation
	 */
	explicit Affine2Main(const Interval& itv);

	/** \brief Create an affine form with n variables and  initialized the m^th variable with  itv.
	 * TO WRITE in your Affine implementation
	 */
	Affine2Main(int n, int m, const Interval& itv);

	/** \brief Create an affine form with n variables, initialized with x
	 * TO WRITE in your Affine implementation
	 */
	Affine2Main(const Affine2Main& x);

	/** \brief  Delete the affine form
	 * TO WRITE in your Affine implementation
	 */
	virtual ~Affine2Main() { };

	/** \brief Return -*this.
	 * TO WRITE in your Affine implementation
	 */
	Affine2Main operator-() const;

	/** \brief True iff *this and x are exactly the same intervals. */
	bool operator==(const Affine2Main& x) const;

	/** \brief True iff *this and x are exactly the same intervals. */
	bool operator==(const Interval& x) const;

	/** \brief True iff *this and x are not exactly the same intervals. */
	bool operator!=(const Affine2Main& x) const;

	/** \brief True iff *this and x are not exactly the same intervals. */
	bool operator!=(const Interval& x) const ;

	/** \brief Set this interval to the empty set. */
	void set_empty();

	/** \brief Set *this to x.
	 * TO WRITE in your Affine implementation
	 */
	Affine2Main& operator=(const Affine2Main& x);

	/** \brief Set *this to d.
	 * TO WRITE in your Affine implementation
	 */
	Affine2Main& operator=(double x);

	/** \brief Set *this to itv.
	 * TO WRITE in your Affine implementation
	 */
	Affine2Main& operator=(const Interval& itv);

	/* Union and Intersection of two Affine2 form must not be implemented
	 * That could produce to much confusion.
	 */
	/** \brief Intersection of *this and x.
	 * \param x - the interval to compute the intersection with.*/
	//	Affine2Main& operator&=(const Affine2Main& x);
	/** \brief Union of *this and I.
	 * \param x - the interval to compute the hull with.*/
	//	Affine2Main& operator|=(const Affine2Main& x);

	/**
	 * \brief Add [-rad,+rad] to *this.
	 *
	 * Return a reference to *this.
	 */
	Affine2Main& inflate(double radd);

	/**
	 * \brief Change the number
	 */
	//	void resize(int n);

	/**
	 * \brief number of variable represented
	 */
	int size() const;

	/**
	 * \brief Range of the affine form
	 * TO WRITE in your Affine implementation
	 */
	const Interval itv() const ;

	/**
	 * \brief return _val[i]
	 * TO WRITE in your Affine implementation
	 */
	double val(int i) const;

	/**
	 * \brief return _err
	 * TO WRITE in your Affine implementation
	 */
	double err() const;
	/**
	 * \brief return 1 if the affine form is actif and valid
	 */
	bool is_actif() const;

	/**
	 * \brief True iff *this is empty.
	 */
	bool is_empty() const;

	/**
	 * \brief True iff *this is empty.
	 */
	bool is_degenerated() const;

	/**
	 * \brief True if one bound of *this is infinite.
	 *
	 * \note An empty affine form is always bounded.
	 */
	bool is_unbounded() const;

	/**
	 * \brief the middle of *this
	 */
	double mid() const;

	/**
	 * \brief reduce the number of noise variable if the value is inferior to \param tol
	 * TO WRITE in your Affine implementation
	 */
	void compact(double tol);
	void compact();

	/** \brief Add \a d to *this and return the result.  */
	Affine2Main& operator+=(double d);

	/** \brief Subtract \a d to *this and return the result. */
	Affine2Main& operator-=(double d);

	/** \brief Multiply *this by \a d and return the result. */
	Affine2Main& operator*=(double d);

	/** \brief Divide *this by \a d and return the result. */
	Affine2Main& operator/=(double d) ;

	/** \brief Add \a x to *this and return the result. */
	Affine2Main& operator+=(const Interval& x);

	/** \brief Subtract \a x to *this and return the result. */
	Affine2Main& operator-=(const Interval& x);

	/** \brief Multiply *this by \a x and return the result.
	 * TO WRITE in your Affine implementation
	 */
	Affine2Main& operator*=(const Interval& x);

	/** \brief Divide *this by \a x and return the result.*/
	Affine2Main& operator/=(const Interval& x);

	/** \brief Add \a x to *this and return the result. */
	Affine2Main& operator+=(const Affine2Main& x);

	/** \brief Subtract \a x to *this and return the result. */
	Affine2Main& operator-=(const Affine2Main& x);

	/** \brief Multiply *this by \a x and return the result.
	 * TO WRITE in your Affine implementation
	 */
	Affine2Main& operator*=(const Affine2Main& x);

	/** \brief Divide *this by \a x and return the result. */
	Affine2Main& operator/=(const Affine2Main& x);

	typedef Affine2Main<T> SCALAR;
	typedef Affine2MainVector<T> VECTOR;
	typedef Affine2MainMatrix<T> MATRIX;
	typedef Affine2MainMatrixArray<T> MATRIX_ARRAY;



	/** Friend function */

	/** \brief 1/AF[x] */
	template<class U>
	friend  Affine2Main<U> inv(const Affine2Main<U>&  x);
	/** \brief AF[x]^2 */
	template<class U>
	friend  Affine2Main<U> sqr(const Affine2Main<U>&  x);
	/** \brief sqrt{AF[x]} */
	template<class U>
	friend  Affine2Main<U> sqrt(const Affine2Main<U>&  x);
	/** \brief exp(AF[x]). */
	template<class U>
	friend  Affine2Main<U> exp(const Affine2Main<U>&  x);
	/** \brief log(AF[x]). */
	template<class U>
	friend  Affine2Main<U> log(const Affine2Main<U>&  x);
	/** \brief AF[x]^n. */
	template<class U>
	friend  Affine2Main<U> pow(const Affine2Main<U>&  x, int n);
	/** \brief AF[x]^d. */
	template<class U>
	friend  Affine2Main<U> pow(const Affine2Main<U>&  x, double d);
	/** \brief AF[x]^[y]. */
	template<class U>
	friend  Affine2Main<U> pow(const Affine2Main<U>& x, const Interval &y);
	/** \brief AF[x]^AF[y]. */
	template<class U>
	friend  Affine2Main<U> pow(const Affine2Main<U>& x, const Affine2Main<U>& y);
	/** \brief nth root of AF[x]. */
	template<class U>
	friend  Affine2Main<U> root(const Affine2Main<U>&  x, int n);
	/** \brief cos(AF[x]). */
	template<class U>
	friend  Affine2Main<U> cos(const Affine2Main<U>&  x);
	/** \brief sin(AF[x]). */
	template<class U>
	friend  Affine2Main<U> sin(const Affine2Main<U>&  x);
	/** \brief tan(AF[x]). */
	template<class U>
	friend  Affine2Main<U> tan(const Affine2Main<U>&  x);
	/** \brief acos(AF[x]). */
	template<class U>
	friend  Affine2Main<U> acos(const Affine2Main<U>&  x);
	/** \brief asin(AF[x]). */
	template<class U>
	friend  Affine2Main<U> asin(const Affine2Main<U>&  x);
	/** \brief atan(AF[x]). */
	template<class U>
	friend  Affine2Main<U> atan(const Affine2Main<U>&  x);
	/** \brief cosh(AF[x]). */
	template<class U>
	friend  Affine2Main<U> cosh(const Affine2Main<U>&  x);
	/** \brief sinh(AF[x]). */
	template<class U>
	friend  Affine2Main<U> sinh(const Affine2Main<U>&  x);
	/** \brief tanh(AF[x]). */
	template<class U>
	friend  Affine2Main<U> tanh(const Affine2Main<U>&  x);
	/** \brief abs(AF[x]) = sqrt(sqr(AF[x])). */
	template<class U>
	friend  Affine2Main<U> abs(const Affine2Main<U>& x);


};


/** \ingroup arithmetic */
/*@{*/

/** \brief Stream out \a x. */
template<class T>
std::ostream& operator<<(std::ostream& os, const Affine2Main<T>&  x);


/** \brief AF[x]_1+AF[x]_2. */
template<class T>
Affine2Main<T> operator+(const Affine2Main<T>&  x1, const Affine2Main<T>&  x2);

/** \brief AF[x]+d. */
template<class T>
Affine2Main<T> operator+(const Affine2Main<T>&  x, double d);

/** \brief d+AF[x]. */
template<class T>
Affine2Main<T> operator+(double d, const Affine2Main<T>&  x);

/** \brief AF[x]_1+[x]_2. */
template<class T>
Affine2Main<T> operator+(const Affine2Main<T>&  x1, const Interval& x2);

/** \brief [x]_1+AF[x]_2. */
template<class T>
Affine2Main<T> operator+(const Interval& x1, const Affine2Main<T>&  x2);

/** \brief AF[x]_1-AF[x]_2. */
template<class T>
Affine2Main<T> operator-(const Affine2Main<T>&  x1, const Affine2Main<T>&  x2);

/** \brief AF[x]-d. */
template<class T>
Affine2Main<T> operator-(const Affine2Main<T>&  x, double d);

/** \brief d-AF[x]. */
template<class T>
Affine2Main<T> operator-(double d, const Affine2Main<T>&  x);

/** \brief AF[x]_1-[x]_2. */
template<class T>
Affine2Main<T> operator-(const Affine2Main<T>&  x1, const Interval& x2);

/** \brief [x]_1-AF[x]_2. */
template<class T>
Affine2Main<T> operator-(const Interval& x1, const Affine2Main<T>&  x2);

/** \brief AF[x]_1*AF[x]_2. */
template<class T>
Affine2Main<T> operator*(const Affine2Main<T>&  x1, const Affine2Main<T>&  x2);

/** \brief AF[x]*d. */
template<class T>
Affine2Main<T> operator*(const Affine2Main<T>&  x, double d);

/** \brief d*AF[x]. */
template<class T>
Affine2Main<T> operator*(double d, const Affine2Main<T>&  x);

/** \brief AF[x]_1*[x]_2. */
template<class T>
Affine2Main<T> operator*(const Affine2Main<T>&  x1, const Interval& x2);

/** \brief [x]_1*AF[x]_2. */
template<class T>
Affine2Main<T> operator*(const Interval& x1, const Affine2Main<T>&  x2);

/** \brief AF[x]_1/AF[x]_2. */
template<class T>
Affine2Main<T> operator/(const Affine2Main<T>&  x1, const Affine2Main<T>&  x2);

/** \brief AF[x]/d. */
template<class T>
Affine2Main<T> operator/(const Affine2Main<T>&  x, double d);

/** \brief d/AF[x]. */
template<class T>
Affine2Main<T> operator/(double d, const Affine2Main<T>&  x);

/** \brief AF[x]_1/[x]_2. */
template<class T>
Affine2Main<T> operator/(const Affine2Main<T>&  x1, const Interval& x2);

/** \brief [x]_1/AF[x]_2. */
template<class T>
Affine2Main<T> operator/(const Interval& x1, const Affine2Main<T>&  x2);


/** \brief Hausdorff distance of AF[x]_1 and AF[x]_2. */
template<class T>
double distance(const Affine2Main<T>& x1, const Affine2Main<T>& x2);

/** \brief Hausdorff distance of [x]_1 and AF[x]_2. */
template<class T>
double distance(const Interval &x1, const Affine2Main<T>& x2);

/** \brief Hausdorff distance of AF[x]_1 and [x]_2. */
template<class T>
double distance(const Affine2Main<T>& x1, const Interval &x2);

template<class T>
Interval max(const Affine2Main<T>&  x, const Affine2Main<T>&  y);
template<class T>
Interval max(const Interval& x, const Affine2Main<T>&  y);
template<class T>
Interval max(const Affine2Main<T>&  x, const Interval& y);

template<class T>
Interval min(const Affine2Main<T>&  x, const Affine2Main<T>&  y);
template<class T>
Interval min(const Interval& x, const Affine2Main<T>&  y) ;
template<class T>
Interval min(const Affine2Main<T>&  x, const Interval& y);


/** \brief \f$[x]_1\cap [x]_2\f$.
 * \return Interval::EMPTY if the intersection is empty. */
template<class T>
Interval operator&(const Affine2Main<T>&  x1, const Affine2Main<T>&  x2);
template<class T>
Interval operator&(const Interval& x1, const Affine2Main<T>&  x2);
template<class T>
Interval operator&(const Affine2Main<T>&  x1, const Interval& x2);


/** \brief \f$\square([x]_1\cup [x]_2)\f$. */
template<class T>
Interval operator|(const Affine2Main<T>&  x1, const Affine2Main<T>&  x2);
template<class T>
Interval operator|(const Interval& x1, const Affine2Main<T>&  x2);
template<class T>
Interval operator|(const Affine2Main<T>&  x1, const Interval& x2);


/**
 * \brief Return the largest integer interval included in x.
 *  TODO Affine2::integer not yet implemented integer
 */
template<class T>
Affine2Main<T> integer(const Affine2Main<T>&  x);

/** \brief Sign of AF[x].
 *
 *  Return \f$sign(AF[x]) = hull \{ sign{x}, x\inAF[x] \}\f$.
 * \remark By convention, \f$ 0\inAF[x] \Longrightarrow sign(AF[x])=[-1,1]\f$. */
template<class T>
Affine2Main<T> sign(const Affine2Main<T>&  x);



/** \brief Chi of AF[a], AF[b] and AF[c].
 *
 *  Return \f$chi(AF[a],AF[b],AF[c]) = 0.5*(1-\sign(AF[a]))*AF[b] + 0.5*(\sign(AF[a])+1)*AF[c]. \f$
 * \remark  chi(AF[a],AF[b],AF[c]) =AF[b] if [a]<=0, AF[c] if AF[a]>0, hull \{AF[b], AF[c]\} else.  */
template<class T>
Affine2Main<T> chi(const Affine2Main<T>&  a,const Affine2Main<T>&  b,const Affine2Main<T>&  c);
template<class T>
Affine2Main<T> chi(const Interval&  a,const Affine2Main<T>&  b,const Affine2Main<T>&  c);
template<class T>
Affine2Main<T> chi(const Interval&  a,const Interval&  b,const Affine2Main<T>&  c);
template<class T>
Affine2Main<T> chi(const Interval&  a,const Affine2Main<T>&  b,const Interval&  c);
template<class T>
Affine2Main<T> chi(const Affine2Main<T>&  a,const Interval&  b,const Affine2Main<T>&  c);
template<class T>
Affine2Main<T> chi(const Affine2Main<T>&  a,const Affine2Main<T>&  b,const Interval&  c);

}


/*@}*/

/*============================================ inline implementation ============================================ */


#include "ibex_Affine2.h_"


namespace ibex {


template<class T>
inline void Affine2Main<T>::compact(){ compact(AF_COMPAC_Tol); }


template<class T>
inline Interval& Interval::operator&=(const Affine2Main<T>& x) {
	return (*this &= x.itv());
}

template<class T>
inline Interval& Interval::operator|=(const Affine2Main<T>& x) {
	return (*this |= x.itv());
}

template<class T>
inline Interval& Interval::operator=(const Affine2Main<T>& x) {
	return (*this = x.itv());
}

template<class T>
inline Interval operator&(const Affine2Main<T>& x1, const Affine2Main<T>& x2) {
	return ((x1.itv())&(x2.itv()));
}

template<class T>
inline Interval operator&(const Interval& x1, const Affine2Main<T>& x2) {
	return ((x1)&(x2.itv()));
}

template<class T>
inline Interval operator&(const Affine2Main<T>& x1, const Interval& x2) {
	return ((x1.itv())&(x2));
}

template<class T>
inline Interval operator|(const Affine2Main<T>& x1, const Affine2Main<T>& x2) {
	return ((x1.itv())|(x2.itv()));
}

template<class T>
inline Interval operator|(const Interval& x1, const Affine2Main<T>& x2) {
	return ((x1)|(x2.itv()));
}

template<class T>
inline Interval operator|(const Affine2Main<T>& x1, const Interval& x2) {
	return ((x1.itv())|(x2));
}

template<class T>
inline bool Affine2Main<T>::operator==(const Affine2Main<T>& x) const{
	return (this->itv() == x.itv());
}

template<class T>
inline bool Affine2Main<T>::operator==(const Interval& x) const{
	return (this->itv() == x);
}

template<class T>
inline bool Affine2Main<T>::operator!=(const Affine2Main<T>& x) const{
	return (this->itv() != x.itv());
}

template<class T>
inline bool Affine2Main<T>::operator!=(const Interval& x) const {
	return (this->itv() != x);
}

template<class T>
inline void Affine2Main<T>::set_empty(){
	*this = Interval::EMPTY_SET;
}

template<class T>
inline Affine2Main<T>& Affine2Main<T>::inflate(double radd){
	if (fabs(radd)>= POS_INFINITY) {
		*this = Interval::ALL_REALS;
	} else {
		saxpy(1.0, Affine2Main<T>(),0.0, radd, false, false, false, true);
	}
	return *this;
}

template<class T>
inline int Affine2Main<T>::size() const{
	return _n;
}


template<class T>
inline bool Affine2Main<T>::is_actif() const{
	//return _actif;
	return (_n>-1);
}

template<class T>
inline bool Affine2Main<T>::is_empty() const{
	return (_n==-1);
}

template<class T>
inline bool Affine2Main<T>::is_degenerated() const {
	return (itv().diam() <	AF_EC());
}

template<class T>
inline bool Affine2Main<T>::is_unbounded() const{
	return ((-1>_n)&&(_n>-5));
}

template<class T>
inline Affine2Main<T>& Affine2Main<T>::operator+=(double d){
	return saxpy(1.0, Affine2Main<T>(), d, 0.0, false, false, true, false);
}

template<class T>
inline Affine2Main<T>& Affine2Main<T>::operator-=(double d){
	return saxpy(1.0, Affine2Main<T>(), (-d), 0.0, false, false, true, false);
}

template<class T>
inline Affine2Main<T>& Affine2Main<T>::operator*=(double d){
	return saxpy(d, Affine2Main<T>(), 0.0, 0.0, true, false, false, false);
}

template<class T>
inline 	Affine2Main<T>& Affine2Main<T>::operator/=(double d) {
	return *this *= (1.0 / Interval(d)) ;
}

template<class T>
inline Affine2Main<T>& Affine2Main<T>::operator+=(const Interval& x){
	return saxpy(1.0, Affine2Main<T>(), x.mid(), x.rad(), false, false, true, true);
}

template<class T>
inline Affine2Main<T>& Affine2Main<T>::operator-=(const Interval& x){
	return *this += (-x);
}

template<class T>
inline Affine2Main<T>& Affine2Main<T>::operator/=(const Interval& x){
	return *this *= (1.0/x) ;
}

template<class T>
inline Affine2Main<T>& Affine2Main<T>::operator+=(const Affine2Main<T>& x){
	return saxpy(1.0, x, 0.0, 0.0, false, true, false, false);
}

template<class T>
inline Affine2Main<T>& Affine2Main<T>::operator-=(const Affine2Main<T>& x){
	return *this += (-Affine2Main<T>(x));
}

template<class T>
inline Affine2Main<T>& Affine2Main<T>::operator/=(const Affine2Main<T>& x){
	return *this *= (Affine2Main<T>(x).linChebyshev(AF_INV, x.itv()));
}

template<class T>
inline Affine2Main<T> operator+(const Affine2Main<T>& x1, const Affine2Main<T>& x2){
	return Affine2Main<T>(x1) += x2;
}

template<class T>
inline Affine2Main<T> operator+(const Affine2Main<T>& x, double d){
	return Affine2Main<T>(x) += d;
}

template<class T>
inline Affine2Main<T> operator+(double d, const Affine2Main<T>& x){
	return Affine2Main<T>(x) += d;
}

template<class T>
inline Affine2Main<T> operator+(const Affine2Main<T>& x1, const Interval& x2){
	return Affine2Main<T>(x1) += x2;
}

template<class T>
inline Affine2Main<T> operator+(const Interval& x1, const Affine2Main<T>& x2){
	return Affine2Main<T>(x2) += x1;
}

template<class T>
inline Affine2Main<T> operator-(const Affine2Main<T>& x1, const Affine2Main<T>& x2){
	return Affine2Main<T>(x1) += (-x2);
}

template<class T>
inline Affine2Main<T> operator-(const Affine2Main<T>& x, double d){
	return Affine2Main<T>(x) -= d;
}

template<class T>
inline Affine2Main<T> operator-(double d, const Affine2Main<T>& x){
	Affine2Main<T> res = (-x);
	return res += d;
}

template<class T>
inline Affine2Main<T> operator-(const Affine2Main<T>& x1, const Interval& x2) {
	return Affine2Main<T>(x1) -= x2;
}

template<class T>
inline Affine2Main<T> operator-(const Interval& x1, const Affine2Main<T>& x2) {
	Affine2Main<T> res = (- x2);
	return res += x1;
}

template<class T>
inline Affine2Main<T> operator*(const Affine2Main<T>& x1, const Affine2Main<T>& x2) {
	return Affine2Main<T>(x1) *= x2;
}

template<class T>
inline Affine2Main<T> operator*(const Affine2Main<T>& x, double d){
	return Affine2Main<T>(x) *= d;
}

template<class T>
inline Affine2Main<T> operator*(double d, const Affine2Main<T>& x){
	return Affine2Main<T>(x) *= d;
}

template<class T>
inline Affine2Main<T> operator*(const Affine2Main<T>& x1, const Interval& x2){
	return Affine2Main<T>(x1) *= x2;
}

template<class T>
inline Affine2Main<T> operator*(const Interval& x1, const Affine2Main<T>& x2){
	return Affine2Main<T>(x2) *= x1;
}

template<class T>
inline Affine2Main<T> operator/(const Affine2Main<T>& x1, const Affine2Main<T>& x2){
	return Affine2Main<T>(x1) /= x2;
}

template<class T>
inline Affine2Main<T> operator/(const Affine2Main<T>& x, double d){
	return Affine2Main<T>(x) /= d;
}

template<class T>
inline Affine2Main<T> operator/(double d, const Affine2Main<T>& x){
	return Affine2Main<T>(d) *= inv(x);
}

template<class T>
inline Affine2Main<T> operator/(const Affine2Main<T>& x1, const Interval& x2){
	return Affine2Main<T>(x1) /= x2;
}

template<class T>
inline Affine2Main<T> operator/(const Interval& x1, const Affine2Main<T>& x2){
	return Affine2Main<T>(x1) *= inv(x2);
}

template<class T>
inline double distance(const Affine2Main<T> &x1, const Affine2Main<T> &x2){
	return distance(x1.itv(), x2.itv());
}

template<class T>
inline double distance(const Interval &x1, const Affine2Main<T> &x2){
	return distance(x1, x2.itv());
}

template<class T>
inline double distance(const Affine2Main<T> &x1, const Interval &x2){
	return distance(x1.itv(), x2);
}

template<class T>
inline Affine2Main<T> inv(const Affine2Main<T>& x){
	return Affine2Main<T>(x).invA(x.itv());
}

template<class T>
inline Affine2Main<T>& Affine2Main<T>::invA(const Interval& itv){
	return (*this).linChebyshev(Affine2Main<T>::AF_INV, itv);
}

template<class T>
inline Affine2Main<T> sqr(const Affine2Main<T>& x){
	return Affine2Main<T>(x).sqrA(x.itv());
}

// TO WRITE IN your implementation of affine
//template<class T>
//inline Affine2Main<T>& Affine2Main<T>::sqrA(const Interval& itv);

template<class T>
inline Affine2Main<T> sqrt(const Affine2Main<T>& x){
	return Affine2Main<T>(x).sqrtA(x.itv());
}

template<class T>
inline Affine2Main<T>& Affine2Main<T>::sqrtA(const Interval& itv){
	return (*this).linChebyshev(Affine2Main<T>::AF_SQRT,itv);
}

template<class T>
inline Affine2Main<T> exp(const Affine2Main<T>& x){
	return Affine2Main<T>(x).expA(x.itv());
}

template<class T>
inline Affine2Main<T>& Affine2Main<T>::expA(const Interval& itv){
	return (*this).linChebyshev(Affine2Main<T>::AF_EXP,itv);
}

template<class T>
inline Affine2Main<T> log(const Affine2Main<T>& x){
	return Affine2Main<T>(x).logA(x.itv());
}

template<class T>
inline Affine2Main<T>& Affine2Main<T>::logA(const Interval& itv){
	return (*this).linChebyshev(Affine2Main<T>::AF_LOG,itv);
}

template<class T>
inline Affine2Main<T> pow(const Affine2Main<T>& x, int n) {
	if (n == 0)
		return Affine2Main<T>(1.0);
	else if (n == 1)
		return Affine2Main<T>(x);
	else if (n == 2)
		return Affine2Main<T>(x).sqr(x.itv());
	else if (n<0)
		return (Affine2Main<T>(x).power(-n,x.itv())).inv(pow(x.itv(),-n));
	else
		return Affine2Main<T>(x).power(n,x.itv());
}

template<class T>
inline Affine2Main<T> pow(const Affine2Main<T>& x, double d){
	return Affine2Main<T>(x).powerA(d,x.itv());
}

template<class T>
inline Affine2Main<T> pow(const Affine2Main<T> &x, const Interval &y){
	// return exp(y * log(x));
	return Affine2Main<T>(x).powerA(y,x.itv());
}

template<class T>
inline Affine2Main<T>& Affine2Main<T>::powerA(const Interval &y, const Interval& itvx){
	// return exp(y * log(x));
	return (y * ((*this).linChebyshev(Affine2Main<T>::AF_LOG, itvx))).linChebyshev(Affine2Main<T>::AF_EXP, (y*log(itvx)));
}

template<class T>
inline Affine2Main<T> pow(const Affine2Main<T> &x, const Affine2Main<T> &y){
	return Affine2Main<T>(x).powerA(y.itv(),x.itv());
}

template<class T>
inline Affine2Main<T> root(const Affine2Main<T>& x, int n) {
	return Affine2Main<T>(x).rootA(n,x.itv());
}

template<class T>
inline Affine2Main<T> cos(const Affine2Main<T>& x){
	return Affine2Main<T>(x).cosA(x.itv());
}

template<class T>
inline Affine2Main<T>& Affine2Main<T>::cosA(const Interval& itv){
	return (*this).linChebyshev(Affine2Main<T>::AF_COS, itv);
}

template<class T>
inline Affine2Main<T> sin(const Affine2Main<T>& x){
	return Affine2Main<T>(x).sinA(x.itv());
}

template<class T>
inline Affine2Main<T>& Affine2Main<T>::sinA(const Interval& itv){
	return (*this).linChebyshev(Affine2Main<T>::AF_SIN, itv);
}

template<class T>
inline Affine2Main<T> tan(const Affine2Main<T>& x){
	return Affine2Main<T>(x).tanA(x.itv());
}

template<class T>
inline Affine2Main<T>& Affine2Main<T>::tanA(const Interval& itv){
	return (*this).linChebyshev(Affine2Main<T>::AF_TAN, itv);
}

template<class T>
inline Affine2Main<T> acos(const Affine2Main<T>& x){
	return Affine2Main<T>(x).acosA(x.itv());
}

template<class T>
inline Affine2Main<T>& Affine2Main<T>::acosA(const Interval& itv){
	return (*this).linChebyshev(Affine2Main<T>::AF_ACOS, itv);
}

template<class T>
inline Affine2Main<T> asin(const Affine2Main<T>& x){
	return Affine2Main<T>(x).asinA(x.itv());
}

template<class T>
inline Affine2Main<T>& Affine2Main<T>::asinA(const Interval& itv){
	return (*this).linChebyshev(Affine2Main<T>::AF_ASIN, itv);
}

template<class T>
inline Affine2Main<T> atan(const Affine2Main<T>& x){
	return Affine2Main<T>(x).atanA(x.itv());
}

template<class T>
inline Affine2Main<T>& Affine2Main<T>::atanA(const Interval& itv){
	return (*this).linChebyshev(Affine2Main<T>::AF_ATAN, itv);
}

template<class T>
inline Affine2Main<T> cosh(const Affine2Main<T>& x){
	return Affine2Main<T>(x).coshA(x.itv());
}

template<class T>
inline Affine2Main<T>& Affine2Main<T>::coshA(const Interval& itv){
	return (*this).linChebyshev(Affine2Main<T>::AF_COSH, itv);
}

template<class T>
inline Affine2Main<T> sinh(const Affine2Main<T>& x){
	return Affine2Main<T>(x).sinhA(x.itv());
}

template<class T>
inline Affine2Main<T>& Affine2Main<T>::sinhA(const Interval& itv){
	return (*this).linChebyshev(Affine2Main<T>::AF_SINH, itv);
}

template<class T>
inline Affine2Main<T> tanh(const Affine2Main<T>& x){
	return Affine2Main<T>(x).tanhA(x.itv());
}

template<class T>
inline Affine2Main<T>& Affine2Main<T>::tanhA(const Interval& itv){
	return (*this).linChebyshev(Affine2Main<T>::AF_TANH, itv);
}

template<class T>
inline Affine2Main<T> abs(const Affine2Main<T>& x){
	return Affine2Main<T>(x).absA(x.itv());
}

template<class T>
inline Affine2Main<T>& Affine2Main<T>::absA(const Interval& itv){
	return (*this).linChebyshev(Affine2Main<T>::AF_ABS, itv);
}


template<class T>
inline Interval max(const Affine2Main<T>& x, const Affine2Main<T>& y) {
	return max(x.itv(), y.itv());
}

template<class T>
inline Interval max(const Interval& x, const Affine2Main<T>& y) {
	return max(x, y.itv());
}

template<class T>
inline Interval max(const Affine2Main<T>& x, const Interval& y) {
	return max(x.itv(), y);
}

template<class T>
inline Interval min(const Affine2Main<T>& x, const Affine2Main<T>& y) {
	return min(x.itv(), y.itv());
}

template<class T>
inline Interval min(const Interval& x, const Affine2Main<T>& y) {
	return min(x, y.itv());
}

template<class T>
inline Interval min(const Affine2Main<T>& x, const Interval& y) {
	return min(x.itv(), y);
}

template<class T>
inline Affine2Main<T> integer(const Affine2Main<T>& x){
	Interval tmp= x.itv();
	if ((tmp.lb()==(int) tmp.lb())&&(tmp.ub()==(int) tmp.ub())) {
		return Affine2Main<T>(x);
	} else  {
		return Affine2Main<T>(integer(tmp));
	}
}

template<class T>
inline Affine2Main<T> sign(const Affine2Main<T>& x) {
	Interval itv=x.itv();
	if (itv.lb() > 0) {
		return Affine2Main<T>(1.0);
	} else if (itv.ub() < 0) {
		return Affine2Main<T>(-1.0);
	} else {
		return Affine2Main<T>(Interval(-1.0, 1.0));
	}
}

template<class T>
inline Affine2Main<T>& Affine2Main<T>::signA(const Interval& itv) {
	if (itv.lb() > 0) {
		return *this=Affine2Main<T>(1.0);
	} else if (itv.ub() < 0) {
		return *this=Affine2Main<T>(-1.0);
	} else {
		return *this=Affine2Main<T>(Interval(-1.0, 1.0));
	}
}


template<class T>
inline Affine2Main<T> chi(const Affine2Main<T>&  a,const Affine2Main<T>&  b,const Affine2Main<T>&  c) {
	return chi(a.itv(),b,c);
}
template<class T>
inline Affine2Main<T> chi(const Interval&  a,const Interval&  b,const Affine2Main<T>&  c) {
	return chi(a,Affine2Main<T>(b),c);
}
template<class T>
inline Affine2Main<T> chi(const Interval&  a,const Affine2Main<T>&  b,const Interval&  c) {
	return chi(a,b,Affine2Main<T>(c));
}
template<class T>
inline Affine2Main<T> chi(const Affine2Main<T>&  a,const Interval&  b,const Affine2Main<T>&  c) {
	return chi(a.itv(),Affine2Main<T>(b),c);
}
template<class T>
inline Affine2Main<T> chi(const Affine2Main<T>&  a,const Affine2Main<T>&  b,const Interval&  c){
	return chi(a.itv(),b,Affine2Main<T>(c));
}
template<class T>
inline Affine2Main<T> chi(const Interval&  a,const Affine2Main<T>&  b,const Affine2Main<T>&  c){
	if (a.ub()<=0) {
		return Affine2Main<T>(b);
	} else if (a.lb()>0) {
		return Affine2Main<T>(c);
	} else {
		return  Affine2Main<T>(b|c);
	}
}


} // end namespace ibex




#endif /* IBEX_Affine2_H_ */

