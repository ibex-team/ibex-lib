/* ============================================================================
 * I B E X - Affine definition
 * ============================================================================
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Jordan Ninin
 * Bug fixes   :
 * Created     : Nov 12, 2012
 * ---------------------------------------------------------------------------- */

#ifndef IBEX_AFFINE_H_
#define IBEX_AFFINE_H_

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
#include "ibex_Affine3_fAFFullI.h"


namespace ibex {

template<class T> class AffineMainVector;
template<class T> class AffineMainMatrix;
template<class T> class AffineMainMatrixArray;
class Affine2Eval;
class Affine3Eval;

/**
 * \ingroup arithmetic
 *
 * \brief Affine Arithmetic AF2
 *
 */


//typedef AF_fAF1  AF_Default;
typedef AF_fAF2  AF_Default;
//typedef AF_fAF2_fma  AF_Default;
//typedef AF_iAF  AF_Default;
//typedef AF_sAF  AF_Default;
//typedef AF_No  AF_Default;
//typedef AF_fAFFullI AF_Default;

//typedef AF_fAF1  AF_Other;
//typedef AF_fAF2  AF_Other;
//typedef AF_fAF2_fma  AF_Other;
//typedef AF_iAF  AF_Other;
//typedef AF_sAF  AF_Other;
//typedef AF_No  AF_Other;
typedef AF_fAFFullI AF_Other;

typedef AffineMain<AF_Default> Affine2;
typedef AffineMain<AF_Other>  Affine3;


template<class T=AF_Default>
class AffineMain {

private:
	friend class Affine2Eval;
	friend class Affine3Eval;
	friend class Function;
	/** \brief tolerance for default compact procedure  */


	typedef enum {
		AF_SQRT, AF_EXP, AF_LOG, AF_INV, AF_COS, AF_SIN, AF_TAN, AF_ABS,AF_ACOS, AF_ASIN, AF_ATAN, AF_COSH, AF_SINH, AF_TANH
	} Affine2_expr; // ...etc...

	static const double AF_COMPAC_Tol;
	static const double AF_EM;
	static const double AF_EC;
	static const double AF_EE;

	/**
	 * Code for the particular case:
	 * if the affine form is actif, _n>1  and _n is the size of the affine form
	 * if the set is degenerate, _n = 0 or itv().diam()< AF_EC
	 * if the set is empty, _n = -1
	 * if the set is ]-oo,+oo[, _n = -2 and _ err=]-oo,+oo[
	 * if the set is [a, +oo[ , _n = -3 and _err = [a, +oo[
	 * if the set is ]-oo, a] , _n = -4 and _err = ]-oo, a]
	 *
	 */

	int _n; 		// dimension (size of val)-1  , ie number of variable

	T _elt;			// core of the affine2 form


	/**
	 * \brief  Compute x = alpha*x+y+beta
	 * The boolean indicate which operation need to be done.
	 *  ! Boolean: scalar, add, constant, error
	 * TO WRITE in your Affine implementation
	 */
	AffineMain& saxpy(double alpha, const AffineMain& y, double beta, double delta,
			bool B1, bool B2, bool B3, bool B4);


	/**
	 *\brief compute the min-range linearization of an unary operator
	 */
	AffineMain& linMinRange(Affine2_expr num, const Interval& itv);

	/**
	 *\brief compute the chebyshev linearization of an unary operator
	 */
	AffineMain& linChebyshev(Affine2_expr num, const Interval& itv);

	/**
	 * TO WRITE in your Affine implementation
	 */
	AffineMain&  sqrA(const Interval& itv);


	AffineMain&  powerA(int n, const Interval& itv);
	AffineMain&  invA(const Interval& itv);
	AffineMain&  sqrtA(const Interval& itv);
	AffineMain&  expA(const Interval& itv);
	AffineMain&  logA(const Interval& itv);
	AffineMain&  powerA(double d, const Interval& itv);
	AffineMain&  powerA(const Interval &y, const Interval& itvx);
	AffineMain&  rootA(int n, const Interval& itv);
	AffineMain&  cosA(const Interval& itv);
	AffineMain&  sinA(const Interval& itv);
	AffineMain&  tanA(const Interval& itv);
	AffineMain&  acosA(const Interval& itv);
	AffineMain&  asinA(const Interval& itv);
	AffineMain&  atanA(const Interval& itv);
	AffineMain&  coshA(const Interval& itv);
	AffineMain&  sinhA(const Interval& itv);
	AffineMain&  tanhA(const Interval& itv);
	AffineMain&  absA(const Interval& itv);
	AffineMain&  signA(const Interval& itv);


	/** \brief Create an affine form with n variables and  initialized the m^th variable with  itv.
	 * TO WRITE in your Affine implementation
	 */
	AffineMain(int n, int m, const Interval& itv);

	/** \brief resize the length of the affine form
	 *  TO WRITE in your Affine implementation
	 */
	AffineMain&  resize(int n);

public:

	/** \brief Create an empty affine form.
	 * TO WRITE in your Affine implementation
	 */
	AffineMain();

	/** \brief Create an affine form with n variables and  initialized val[0] with d.
	 * TO WRITE in your Affine implementation
	 */
	explicit AffineMain(const double d);

	/** \brief Create an affine form with n variables and  initialized val[0] with  itv.
	 * TO WRITE in your Affine implementation
	 */
	explicit AffineMain(const Interval& itv);

	/** \brief Create an affine form with n variables, initialized with x
	 * TO WRITE in your Affine implementation
	 */
	AffineMain(const AffineMain& x);

	/** \brief  Delete the affine form
	 * TO WRITE in your Affine implementation
	 */
	virtual ~AffineMain() { };

	/** \brief Return -*this.
	 * TO WRITE in your Affine implementation
	 */
	AffineMain operator-() const;

	/** \brief True iff *this and x are exactly the same intervals. */
	bool operator==(const AffineMain& x) const;

	/** \brief True iff *this and x are exactly the same intervals. */
	bool operator==(const Interval& x) const;

	/** \brief True iff *this and x are not exactly the same intervals. */
	bool operator!=(const AffineMain& x) const;

	/** \brief True iff *this and x are not exactly the same intervals. */
	bool operator!=(const Interval& x) const ;

	/** \brief Set this interval to the empty set. */
	void set_empty();

	/** \brief Set *this to x.
	 * TO WRITE in your Affine implementation
	 */
	AffineMain& operator=(const AffineMain& x);

	/** \brief Set *this to d.
	 * TO WRITE in your Affine implementation
	 */
	AffineMain& operator=(double x);

	/** \brief Set *this to itv.
	 * TO WRITE in your Affine implementation
	 */
	AffineMain& operator=(const Interval& itv);

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
	AffineMain& inflate(double radd);

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
	 * \brief return the center of the affine form
	 * TO WRITE in your Affine implementation
	 */
	double center() const;

	/**
	 * \brief return the coefficient of the ith variable
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
	AffineMain& operator+=(double d);

	/** \brief Subtract \a d to *this and return the result. */
	AffineMain& operator-=(double d);

	/** \brief Multiply *this by \a d and return the result. */
	AffineMain& operator*=(double d);

	/** \brief Divide *this by \a d and return the result. */
	AffineMain& operator/=(double d) ;

	/** \brief Add \a x to *this and return the result. */
	AffineMain& operator+=(const Interval& x);

	/** \brief Subtract \a x to *this and return the result. */
	AffineMain& operator-=(const Interval& x);

	/** \brief Multiply *this by \a x and return the result.
	 * TO WRITE in your Affine implementation
	 */
	AffineMain& operator*=(const Interval& x);

	/** \brief Divide *this by \a x and return the result.*/
	AffineMain& operator/=(const Interval& x);

	/** \brief Add \a x to *this and return the result. */
	AffineMain& operator+=(const AffineMain& x);

	/** \brief Subtract \a x to *this and return the result. */
	AffineMain& operator-=(const AffineMain& x);

	/** \brief Multiply *this by \a x and return the result.
	 * TO WRITE in your Affine implementation
	 */
	AffineMain& operator*=(const AffineMain& x);

	/** \brief Divide *this by \a x and return the result. */
	AffineMain& operator/=(const AffineMain& x);

	typedef AffineMain<T> SCALAR;
	typedef AffineMainVector<T> VECTOR;
	typedef AffineMainMatrix<T> MATRIX;
	typedef AffineMainMatrixArray<T> MATRIX_ARRAY;



	/** Friend function */

	/** \brief 1/AF[x] */
	template<class U>
	friend  AffineMain<U> inv(const AffineMain<U>&  x);
	/** \brief AF[x]^2 */
	template<class U>
	friend  AffineMain<U> sqr(const AffineMain<U>&  x);
	/** \brief sqrt{AF[x]} */
	template<class U>
	friend  AffineMain<U> sqrt(const AffineMain<U>&  x);
	/** \brief exp(AF[x]). */
	template<class U>
	friend  AffineMain<U> exp(const AffineMain<U>&  x);
	/** \brief log(AF[x]). */
	template<class U>
	friend  AffineMain<U> log(const AffineMain<U>&  x);
	/** \brief AF[x]^n. */
	template<class U>
	friend  AffineMain<U> pow(const AffineMain<U>&  x, int n);
	/** \brief AF[x]^d. */
	template<class U>
	friend  AffineMain<U> pow(const AffineMain<U>&  x, double d);
	/** \brief AF[x]^[y]. */
	template<class U>
	friend  AffineMain<U> pow(const AffineMain<U>& x, const Interval &y);
	/** \brief AF[x]^AF[y]. */
	template<class U>
	friend  AffineMain<U> pow(const AffineMain<U>& x, const AffineMain<U>& y);
	/** \brief nth root of AF[x]. */
	template<class U>
	friend  AffineMain<U> root(const AffineMain<U>&  x, int n);
	/** \brief cos(AF[x]). */
	template<class U>
	friend  AffineMain<U> cos(const AffineMain<U>&  x);
	/** \brief sin(AF[x]). */
	template<class U>
	friend  AffineMain<U> sin(const AffineMain<U>&  x);
	/** \brief tan(AF[x]). */
	template<class U>
	friend  AffineMain<U> tan(const AffineMain<U>&  x);
	/** \brief acos(AF[x]). */
	template<class U>
	friend  AffineMain<U> acos(const AffineMain<U>&  x);
	/** \brief asin(AF[x]). */
	template<class U>
	friend  AffineMain<U> asin(const AffineMain<U>&  x);
	/** \brief atan(AF[x]). */
	template<class U>
	friend  AffineMain<U> atan(const AffineMain<U>&  x);
	/** \brief cosh(AF[x]). */
	template<class U>
	friend  AffineMain<U> cosh(const AffineMain<U>&  x);
	/** \brief sinh(AF[x]). */
	template<class U>
	friend  AffineMain<U> sinh(const AffineMain<U>&  x);
	/** \brief tanh(AF[x]). */
	template<class U>
	friend  AffineMain<U> tanh(const AffineMain<U>&  x);
	/** \brief abs(AF[x]) = sqrt(sqr(AF[x])). */
	template<class U>
	friend  AffineMain<U> abs(const AffineMain<U>& x);


};


/** \ingroup arithmetic */
/*@{*/

/** \brief Stream out \a x. */
template<class T>
std::ostream& operator<<(std::ostream& os, const AffineMain<T>&  x);


/** \brief AF[x]_1+AF[x]_2. */
template<class T>
AffineMain<T> operator+(const AffineMain<T>&  x1, const AffineMain<T>&  x2);

/** \brief AF[x]+d. */
template<class T>
AffineMain<T> operator+(const AffineMain<T>&  x, double d);

/** \brief d+AF[x]. */
template<class T>
AffineMain<T> operator+(double d, const AffineMain<T>&  x);

/** \brief AF[x]_1+[x]_2. */
template<class T>
AffineMain<T> operator+(const AffineMain<T>&  x1, const Interval& x2);

/** \brief [x]_1+AF[x]_2. */
template<class T>
AffineMain<T> operator+(const Interval& x1, const AffineMain<T>&  x2);

/** \brief AF[x]_1-AF[x]_2. */
template<class T>
AffineMain<T> operator-(const AffineMain<T>&  x1, const AffineMain<T>&  x2);

/** \brief AF[x]-d. */
template<class T>
AffineMain<T> operator-(const AffineMain<T>&  x, double d);

/** \brief d-AF[x]. */
template<class T>
AffineMain<T> operator-(double d, const AffineMain<T>&  x);

/** \brief AF[x]_1-[x]_2. */
template<class T>
AffineMain<T> operator-(const AffineMain<T>&  x1, const Interval& x2);

/** \brief [x]_1-AF[x]_2. */
template<class T>
AffineMain<T> operator-(const Interval& x1, const AffineMain<T>&  x2);

/** \brief AF[x]_1*AF[x]_2. */
template<class T>
AffineMain<T> operator*(const AffineMain<T>&  x1, const AffineMain<T>&  x2);

/** \brief AF[x]*d. */
template<class T>
AffineMain<T> operator*(const AffineMain<T>&  x, double d);

/** \brief d*AF[x]. */
template<class T>
AffineMain<T> operator*(double d, const AffineMain<T>&  x);

/** \brief AF[x]_1*[x]_2. */
template<class T>
AffineMain<T> operator*(const AffineMain<T>&  x1, const Interval& x2);

/** \brief [x]_1*AF[x]_2. */
template<class T>
AffineMain<T> operator*(const Interval& x1, const AffineMain<T>&  x2);

/** \brief AF[x]_1/AF[x]_2. */
template<class T>
AffineMain<T> operator/(const AffineMain<T>&  x1, const AffineMain<T>&  x2);

/** \brief AF[x]/d. */
template<class T>
AffineMain<T> operator/(const AffineMain<T>&  x, double d);

/** \brief d/AF[x]. */
template<class T>
AffineMain<T> operator/(double d, const AffineMain<T>&  x);

/** \brief AF[x]_1/[x]_2. */
template<class T>
AffineMain<T> operator/(const AffineMain<T>&  x1, const Interval& x2);

/** \brief [x]_1/AF[x]_2. */
template<class T>
AffineMain<T> operator/(const Interval& x1, const AffineMain<T>&  x2);


/** \brief Hausdorff distance of AF[x]_1 and AF[x]_2. */
template<class T>
double distance(const AffineMain<T>& x1, const AffineMain<T>& x2);

/** \brief Hausdorff distance of [x]_1 and AF[x]_2. */
template<class T>
double distance(const Interval &x1, const AffineMain<T>& x2);

/** \brief Hausdorff distance of AF[x]_1 and [x]_2. */
template<class T>
double distance(const AffineMain<T>& x1, const Interval &x2);

template<class T>
Interval max(const AffineMain<T>&  x, const AffineMain<T>&  y);
template<class T>
Interval max(const Interval& x, const AffineMain<T>&  y);
template<class T>
Interval max(const AffineMain<T>&  x, const Interval& y);

template<class T>
Interval min(const AffineMain<T>&  x, const AffineMain<T>&  y);
template<class T>
Interval min(const Interval& x, const AffineMain<T>&  y) ;
template<class T>
Interval min(const AffineMain<T>&  x, const Interval& y);


/** \brief \f$[x]_1\cap [x]_2\f$.
 * \return Interval::EMPTY if the intersection is empty. */
template<class T>
Interval operator&(const AffineMain<T>&  x1, const AffineMain<T>&  x2);
template<class T>
Interval operator&(const Interval& x1, const AffineMain<T>&  x2);
template<class T>
Interval operator&(const AffineMain<T>&  x1, const Interval& x2);


/** \brief \f$\square([x]_1\cup [x]_2)\f$. */
template<class T>
Interval operator|(const AffineMain<T>&  x1, const AffineMain<T>&  x2);
template<class T>
Interval operator|(const Interval& x1, const AffineMain<T>&  x2);
template<class T>
Interval operator|(const AffineMain<T>&  x1, const Interval& x2);


/**
 * \brief Return the largest integer interval included in x.
 *  TODO Affine2::integer not yet implemented integer
 */
template<class T>
AffineMain<T> integer(const AffineMain<T>&  x);

/** \brief Sign of AF[x].
 *
 *  Return \f$sign(AF[x]) = hull \{ sign{x}, x\inAF[x] \}\f$.
 * \remark By convention, \f$ 0\inAF[x] \Longrightarrow sign(AF[x])=[-1,1]\f$. */
template<class T>
AffineMain<T> sign(const AffineMain<T>&  x);



/** \brief Chi of AF[a], AF[b] and AF[c].
 *
 *  Return \f$chi(AF[a],AF[b],AF[c]) = 0.5*(1-\sign(AF[a]))*AF[b] + 0.5*(\sign(AF[a])+1)*AF[c]. \f$
 * \remark  chi(AF[a],AF[b],AF[c]) =AF[b] if [a]<=0, AF[c] if AF[a]>0, hull \{AF[b], AF[c]\} else.  */
template<class T>
AffineMain<T> chi(const AffineMain<T>&  a,const AffineMain<T>&  b,const AffineMain<T>&  c);
template<class T>
AffineMain<T> chi(const Interval&  a,const AffineMain<T>&  b,const AffineMain<T>&  c);
template<class T>
AffineMain<T> chi(const Interval&  a,const Interval&  b,const AffineMain<T>&  c);
template<class T>
AffineMain<T> chi(const Interval&  a,const AffineMain<T>&  b,const Interval&  c);
template<class T>
AffineMain<T> chi(const AffineMain<T>&  a,const Interval&  b,const AffineMain<T>&  c);
template<class T>
AffineMain<T> chi(const AffineMain<T>&  a,const AffineMain<T>&  b,const Interval&  c);

}


/*@}*/

/*============================================ inline implementation ============================================ */


#include "ibex_Affine.h_"


namespace ibex {


template<class T> const double AffineMain<T>::AF_COMPAC_Tol = 1.e-6;
template<class T> const double AffineMain<T>::AF_EM = __builtin_powi(2.0, -51);
template<class T> const double AffineMain<T>::AF_EC = __builtin_powi(2.0, -55);
template<class T> const double AffineMain<T>::AF_EE = 2.0;


template<class T>
inline void AffineMain<T>::compact(){ compact(AF_COMPAC_Tol); }


template<class T>
inline Interval& Interval::operator&=(const AffineMain<T>& x) {
	return (*this &= x.itv());
}

template<class T>
inline Interval& Interval::operator|=(const AffineMain<T>& x) {
	return (*this |= x.itv());
}

template<class T>
inline Interval& Interval::operator=(const AffineMain<T>& x) {
	return (*this = x.itv());
}

template<class T>
inline Interval operator&(const AffineMain<T>& x1, const AffineMain<T>& x2) {
	return ((x1.itv())&(x2.itv()));
}

template<class T>
inline Interval operator&(const Interval& x1, const AffineMain<T>& x2) {
	return ((x1)&(x2.itv()));
}

template<class T>
inline Interval operator&(const AffineMain<T>& x1, const Interval& x2) {
	return ((x1.itv())&(x2));
}

template<class T>
inline Interval operator|(const AffineMain<T>& x1, const AffineMain<T>& x2) {
	return ((x1.itv())|(x2.itv()));
}

template<class T>
inline Interval operator|(const Interval& x1, const AffineMain<T>& x2) {
	return ((x1)|(x2.itv()));
}

template<class T>
inline Interval operator|(const AffineMain<T>& x1, const Interval& x2) {
	return ((x1.itv())|(x2));
}

template<class T>
inline bool AffineMain<T>::operator==(const AffineMain<T>& x) const{
	return (this->itv() == x.itv());
}

template<class T>
inline bool AffineMain<T>::operator==(const Interval& x) const{
	return (this->itv() == x);
}

template<class T>
inline bool AffineMain<T>::operator!=(const AffineMain<T>& x) const{
	return (this->itv() != x.itv());
}

template<class T>
inline bool AffineMain<T>::operator!=(const Interval& x) const {
	return (this->itv() != x);
}

template<class T>
inline void AffineMain<T>::set_empty(){
	*this = Interval::EMPTY_SET;
}

template<class T>
inline AffineMain<T>& AffineMain<T>::inflate(double radd){
	if (fabs(radd)>= POS_INFINITY) {
		*this = Interval::ALL_REALS;
	} else {
		saxpy(1.0, AffineMain<T>(),0.0, radd, false, false, false, true);
	}
	return *this;
}

template<class T>
inline int AffineMain<T>::size() const{
	return _n;
}


template<class T>
inline bool AffineMain<T>::is_actif() const{
	//return _actif;
	return (_n>-1);
}

template<class T>
inline bool AffineMain<T>::is_empty() const{
	return (_n==-1);
}

template<class T>
inline bool AffineMain<T>::is_degenerated() const {
	return (itv().diam() <	AF_EC);
}

template<class T>
inline bool AffineMain<T>::is_unbounded() const{
	return ((-1>_n)&&(_n>-5));
}

template<class T>
inline AffineMain<T>& AffineMain<T>::operator+=(double d){
	return saxpy(1.0, AffineMain<T>(), d, 0.0, false, false, true, false);
}

template<class T>
inline AffineMain<T>& AffineMain<T>::operator-=(double d){
	return saxpy(1.0, AffineMain<T>(), (-d), 0.0, false, false, true, false);
}

template<class T>
inline AffineMain<T>& AffineMain<T>::operator*=(double d){
	return saxpy(d, AffineMain<T>(), 0.0, 0.0, true, false, false, false);
}

template<class T>
inline 	AffineMain<T>& AffineMain<T>::operator/=(double d) {
	return *this *= (1.0 / Interval(d)) ;
}

template<class T>
inline AffineMain<T>& AffineMain<T>::operator+=(const Interval& x){
	return saxpy(1.0, AffineMain<T>(), x.mid(), x.rad(), false, false, true, true);
}

template<class T>
inline AffineMain<T>& AffineMain<T>::operator-=(const Interval& x){
	return *this += (-x);
}

template<class T>
inline AffineMain<T>& AffineMain<T>::operator/=(const Interval& x){
	return *this *= (1.0/x) ;
}

template<class T>
inline AffineMain<T>& AffineMain<T>::operator+=(const AffineMain<T>& x){
	return saxpy(1.0, x, 0.0, 0.0, false, true, false, false);
}

template<class T>
inline AffineMain<T>& AffineMain<T>::operator-=(const AffineMain<T>& x){
	return *this += (-AffineMain<T>(x));
}

template<class T>
inline AffineMain<T>& AffineMain<T>::operator/=(const AffineMain<T>& x){
	return *this *= (AffineMain<T>(x).linChebyshev(AF_INV, x.itv()));
}

template<class T>
inline AffineMain<T> operator+(const AffineMain<T>& x1, const AffineMain<T>& x2){
	return AffineMain<T>(x1) += x2;
}

template<class T>
inline AffineMain<T> operator+(const AffineMain<T>& x, double d){
	return AffineMain<T>(x) += d;
}

template<class T>
inline AffineMain<T> operator+(double d, const AffineMain<T>& x){
	return AffineMain<T>(x) += d;
}

template<class T>
inline AffineMain<T> operator+(const AffineMain<T>& x1, const Interval& x2){
	return AffineMain<T>(x1) += x2;
}

template<class T>
inline AffineMain<T> operator+(const Interval& x1, const AffineMain<T>& x2){
	return AffineMain<T>(x2) += x1;
}

template<class T>
inline AffineMain<T> operator-(const AffineMain<T>& x1, const AffineMain<T>& x2){
	return AffineMain<T>(x1) += (-x2);
}

template<class T>
inline AffineMain<T> operator-(const AffineMain<T>& x, double d){
	return AffineMain<T>(x) -= d;
}

template<class T>
inline AffineMain<T> operator-(double d, const AffineMain<T>& x){
	AffineMain<T> res = (-x);
	return res += d;
}

template<class T>
inline AffineMain<T> operator-(const AffineMain<T>& x1, const Interval& x2) {
	return AffineMain<T>(x1) -= x2;
}

template<class T>
inline AffineMain<T> operator-(const Interval& x1, const AffineMain<T>& x2) {
	AffineMain<T> res = (- x2);
	return res += x1;
}

template<class T>
inline AffineMain<T> operator*(const AffineMain<T>& x1, const AffineMain<T>& x2) {
	return AffineMain<T>(x1) *= x2;
}

template<class T>
inline AffineMain<T> operator*(const AffineMain<T>& x, double d){
	return AffineMain<T>(x) *= d;
}

template<class T>
inline AffineMain<T> operator*(double d, const AffineMain<T>& x){
	return AffineMain<T>(x) *= d;
}

template<class T>
inline AffineMain<T> operator*(const AffineMain<T>& x1, const Interval& x2){
	return AffineMain<T>(x1) *= x2;
}

template<class T>
inline AffineMain<T> operator*(const Interval& x1, const AffineMain<T>& x2){
	return AffineMain<T>(x2) *= x1;
}

template<class T>
inline AffineMain<T> operator/(const AffineMain<T>& x1, const AffineMain<T>& x2){
	return AffineMain<T>(x1) /= x2;
}

template<class T>
inline AffineMain<T> operator/(const AffineMain<T>& x, double d){
	return AffineMain<T>(x) /= d;
}

template<class T>
inline AffineMain<T> operator/(double d, const AffineMain<T>& x){
	return AffineMain<T>(d) *= inv(x);
}

template<class T>
inline AffineMain<T> operator/(const AffineMain<T>& x1, const Interval& x2){
	return AffineMain<T>(x1) /= x2;
}

template<class T>
inline AffineMain<T> operator/(const Interval& x1, const AffineMain<T>& x2){
	return AffineMain<T>(x1) *= inv(x2);
}

template<class T>
inline double distance(const AffineMain<T> &x1, const AffineMain<T> &x2){
	return distance(x1.itv(), x2.itv());
}

template<class T>
inline double distance(const Interval &x1, const AffineMain<T> &x2){
	return distance(x1, x2.itv());
}

template<class T>
inline double distance(const AffineMain<T> &x1, const Interval &x2){
	return distance(x1.itv(), x2);
}

template<class T>
inline AffineMain<T> inv(const AffineMain<T>& x){
	return AffineMain<T>(x).invA(x.itv());
}

template<class T>
inline AffineMain<T>& AffineMain<T>::invA(const Interval& itv){
	return (*this).linChebyshev(AffineMain<T>::AF_INV, itv);
}

template<class T>
inline AffineMain<T> sqr(const AffineMain<T>& x){
	return AffineMain<T>(x).sqrA(x.itv());
}

// TO WRITE IN your implementation of affine
//template<class T>
//inline Affine2Main<T>& Affine2Main<T>::sqrA(const Interval& itv);

template<class T>
inline AffineMain<T> sqrt(const AffineMain<T>& x){
	return AffineMain<T>(x).sqrtA(x.itv());
}

template<class T>
inline AffineMain<T>& AffineMain<T>::sqrtA(const Interval& itv){
	return (*this).linChebyshev(AffineMain<T>::AF_SQRT,itv);
}

template<class T>
inline AffineMain<T> exp(const AffineMain<T>& x){
	return AffineMain<T>(x).expA(x.itv());
}

template<class T>
inline AffineMain<T>& AffineMain<T>::expA(const Interval& itv){
	return (*this).linChebyshev(AffineMain<T>::AF_EXP,itv);
}

template<class T>
inline AffineMain<T> log(const AffineMain<T>& x){
	return AffineMain<T>(x).logA(x.itv());
}

template<class T>
inline AffineMain<T>& AffineMain<T>::logA(const Interval& itv){
	return (*this).linChebyshev(AffineMain<T>::AF_LOG,itv);
}

template<class T>
inline AffineMain<T> pow(const AffineMain<T>& x, int n) {
	if (n == 0)
		return AffineMain<T>(1.0);
	else if (n == 1)
		return AffineMain<T>(x);
	else if (n == 2)
		return AffineMain<T>(x).sqrA(x.itv());
	else if (n<0)
		return (AffineMain<T>(x).powerA(-n,x.itv())).inv(pow(x.itv(),-n));
	else
		return AffineMain<T>(x).powerA(n,x.itv());
}

template<class T>
inline AffineMain<T> pow(const AffineMain<T>& x, double d){
	return AffineMain<T>(x).powerA(d,x.itv());
}

template<class T>
inline AffineMain<T> pow(const AffineMain<T> &x, const Interval &y){
	// return exp(y * log(x));
	return AffineMain<T>(x).powerA(y,x.itv());
}

template<class T>
inline AffineMain<T>& AffineMain<T>::powerA(const Interval &y, const Interval& itvx){
	// return exp(y * log(x));
	return (y * ((*this).linChebyshev(AffineMain<T>::AF_LOG, itvx))).linChebyshev(AffineMain<T>::AF_EXP, (y*log(itvx)));
}

template<class T>
inline AffineMain<T> pow(const AffineMain<T> &x, const AffineMain<T> &y){
	return AffineMain<T>(x).powerA(y.itv(),x.itv());
}

template<class T>
inline AffineMain<T>& AffineMain<T>::powerA(double d, const Interval& itv) {
	if ( ((int) (d)) == d) {
		return (*this).powerA((int) (d),itv);
	} else if (d<0) {
		return ((*this).powerA(Interval(-d), itv)).inv(pow(itv,-d));
	} else {
		return (*this).powerA(Interval(d),itv);
	}
}

template<class T>
inline AffineMain<T> root(const AffineMain<T>& x, int n) {
	return AffineMain<T>(x).rootA(n,x.itv());
}

template<class T>
inline AffineMain<T>& AffineMain<T>::rootA( int n, const Interval& itv) {

	if (this->is_empty()) return *this=AffineMain<T>(Interval::EMPTY_SET);
	else if (n==0) return *this=AffineMain<T>(Interval::ONE);
	else if (n==1) return *this;
	else if (n<0) return ((*this).rootA(-n,itv)).inv(root(itv,-n));
	else if (this->is_degenerated()) return AffineMain<T>(pow(Interval(this->val(0)),1.0/n));
	else if (n % 2 == 0) return (*this).powerA(Interval::ONE/n,itv & Interval::POS_REALS); // the negative part of x should be removed
	else if (0 <= itv.lb()) return  (*this).powerA(Interval::ONE/n,itv);
	else if (itv.ub() <= 0) return  *this=(-((-(*this)).powerA(Interval::ONE/n,-itv)));
	else {
		// TODO do the root when x contains ZERO more properly
		//ibex_error("warning: Affine2Main ROOT non completely well implemented yet");
		//		y=pow(x,e) |  // the negative part of x should be removed
		//	    (-pow(-x,e)); // the positive part of x should be removed
		return AffineMain<T>( ((*this).powerA(Interval::ONE/n,itv)) | -((-(*this)).powerA(Interval::ONE/n,-itv)));  // BE CAREFULL the result of this union is an INTERVAL, so y lost all its affine form
	}

}


template<class T>
inline AffineMain<T> cos(const AffineMain<T>& x){
	return AffineMain<T>(x).cosA(x.itv());
}

template<class T>
inline AffineMain<T>& AffineMain<T>::cosA(const Interval& itv){
	return (*this).linChebyshev(AffineMain<T>::AF_COS, itv);
}

template<class T>
inline AffineMain<T> sin(const AffineMain<T>& x){
	return AffineMain<T>(x).sinA(x.itv());
}

template<class T>
inline AffineMain<T>& AffineMain<T>::sinA(const Interval& itv){
	return (*this).linChebyshev(AffineMain<T>::AF_SIN, itv);
}

template<class T>
inline AffineMain<T> tan(const AffineMain<T>& x){
	return AffineMain<T>(x).tanA(x.itv());
}

template<class T>
inline AffineMain<T>& AffineMain<T>::tanA(const Interval& itv){
	return (*this).linChebyshev(AffineMain<T>::AF_TAN, itv);
}

template<class T>
inline AffineMain<T> acos(const AffineMain<T>& x){
	return AffineMain<T>(x).acosA(x.itv());
}

template<class T>
inline AffineMain<T>& AffineMain<T>::acosA(const Interval& itv){
	return (*this).linChebyshev(AffineMain<T>::AF_ACOS, itv);
}

template<class T>
inline AffineMain<T> asin(const AffineMain<T>& x){
	return AffineMain<T>(x).asinA(x.itv());
}

template<class T>
inline AffineMain<T>& AffineMain<T>::asinA(const Interval& itv){
	return (*this).linChebyshev(AffineMain<T>::AF_ASIN, itv);
}

template<class T>
inline AffineMain<T> atan(const AffineMain<T>& x){
	return AffineMain<T>(x).atanA(x.itv());
}

template<class T>
inline AffineMain<T>& AffineMain<T>::atanA(const Interval& itv){
	return (*this).linChebyshev(AffineMain<T>::AF_ATAN, itv);
}

template<class T>
inline AffineMain<T> cosh(const AffineMain<T>& x){
	return AffineMain<T>(x).coshA(x.itv());
}

template<class T>
inline AffineMain<T>& AffineMain<T>::coshA(const Interval& itv){
	return (*this).linChebyshev(AffineMain<T>::AF_COSH, itv);
}

template<class T>
inline AffineMain<T> sinh(const AffineMain<T>& x){
	return AffineMain<T>(x).sinhA(x.itv());
}

template<class T>
inline AffineMain<T>& AffineMain<T>::sinhA(const Interval& itv){
	return (*this).linChebyshev(AffineMain<T>::AF_SINH, itv);
}

template<class T>
inline AffineMain<T> tanh(const AffineMain<T>& x){
	return AffineMain<T>(x).tanhA(x.itv());
}

template<class T>
inline AffineMain<T>& AffineMain<T>::tanhA(const Interval& itv){
	return (*this).linChebyshev(AffineMain<T>::AF_TANH, itv);
}

template<class T>
inline AffineMain<T> abs(const AffineMain<T>& x){
	return AffineMain<T>(x).absA(x.itv());
}

template<class T>
inline AffineMain<T>& AffineMain<T>::absA(const Interval& itv){
	return (*this).linChebyshev(AffineMain<T>::AF_ABS, itv);
}

template<class T>
inline Interval max(const AffineMain<T>& x, const AffineMain<T>& y) {
	return max(x.itv(), y.itv());
}

template<class T>
inline Interval max(const Interval& x, const AffineMain<T>& y) {
	return max(x, y.itv());
}

template<class T>
inline Interval max(const AffineMain<T>& x, const Interval& y) {
	return max(x.itv(), y);
}

template<class T>
inline Interval min(const AffineMain<T>& x, const AffineMain<T>& y) {
	return min(x.itv(), y.itv());
}

template<class T>
inline Interval min(const Interval& x, const AffineMain<T>& y) {
	return min(x, y.itv());
}

template<class T>
inline Interval min(const AffineMain<T>& x, const Interval& y) {
	return min(x.itv(), y);
}

template<class T>
inline AffineMain<T> integer(const AffineMain<T>& x){
	Interval tmp= x.itv();
	if ((tmp.lb()==(int) tmp.lb())&&(tmp.ub()==(int) tmp.ub())) {
		return AffineMain<T>(x);
	} else  {
		return AffineMain<T>(integer(tmp));
	}
}

template<class T>
inline AffineMain<T> sign(const AffineMain<T>& x) {
	Interval itv=x.itv();
	if (itv.lb() > 0) {
		return AffineMain<T>(1.0);
	} else if (itv.ub() < 0) {
		return AffineMain<T>(-1.0);
	} else {
		return AffineMain<T>(Interval(-1.0, 1.0));
	}
}

template<class T>
inline AffineMain<T>& AffineMain<T>::signA(const Interval& itv) {
	if (itv.lb() > 0) {
		return *this=AffineMain<T>(1.0);
	} else if (itv.ub() < 0) {
		return *this=AffineMain<T>(-1.0);
	} else {
		return *this=AffineMain<T>(Interval(-1.0, 1.0));
	}
}


template<class T>
inline AffineMain<T> chi(const AffineMain<T>&  a,const AffineMain<T>&  b,const AffineMain<T>&  c) {
	return chi(a.itv(),b,c);
}
template<class T>
inline AffineMain<T> chi(const Interval&  a,const Interval&  b,const AffineMain<T>&  c) {
	return chi(a,AffineMain<T>(b),c);
}
template<class T>
inline AffineMain<T> chi(const Interval&  a,const AffineMain<T>&  b,const Interval&  c) {
	return chi(a,b,AffineMain<T>(c));
}
template<class T>
inline AffineMain<T> chi(const AffineMain<T>&  a,const Interval&  b,const AffineMain<T>&  c) {
	return chi(a.itv(),AffineMain<T>(b),c);
}
template<class T>
inline AffineMain<T> chi(const AffineMain<T>&  a,const AffineMain<T>&  b,const Interval&  c){
	return chi(a.itv(),b,AffineMain<T>(c));
}
template<class T>
inline AffineMain<T> chi(const Interval&  a,const AffineMain<T>&  b,const AffineMain<T>&  c){
	if (a.ub()<=0) {
		return AffineMain<T>(b);
	} else if (a.lb()>0) {
		return AffineMain<T>(c);
	} else {
		return  AffineMain<T>(b|c);
	}
}


} // end namespace ibex




#endif /* IBEX_Affine_H_ */

