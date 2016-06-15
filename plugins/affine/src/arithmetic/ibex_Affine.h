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


#include "ibex_Affine2_fAF2.h"
#include "ibex_Affine3_fAFFullI.h"


#ifdef _IBEX_WITH_AFFINE_EXTENDED_

#include "ibex_Affine2_fAF1.h"
#include "ibex_Affine2_iAF.h"
#include "ibex_Affine2_fAF2_fma.h"
#include "ibex_Affine2_sAF.h"
#include "ibex_Affine2_No.h"

#endif

namespace ibex {

template<class T> class AffineMainVector;
template<class T> class AffineMainMatrix;

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
typedef AF_fAFFullI AF_Other;

typedef AffineMain<AF_Default> Affine2;
typedef AffineMain<AF_Other>  Affine3;


template<class T=AF_Default>
class AffineMain {
public:

	typedef enum {
		AF_Default, AF_Chebyshev, AF_MinRange
	} Affine_Mode; // ...etc...


	static void change_mode(Affine_Mode tt=AF_Default);

private:



	/** \brief tolerance for default compact procedure  */
	static const double AF_COMPAC_Tol;
	static const double AF_EM;
	static const double AF_EC;
	static const double AF_EE;

	static bool mode;

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


public:


	/** \brief Create an empty affine form. */
	AffineMain();

	/** \brief Create an affine form with n variables and  initialized val[0] with d. */
	explicit AffineMain(double d);

	/** \brief Create an affine form with n variables and  initialized val[0] with  itv. */
	explicit AffineMain(const Interval& itv);

	/** \brief Create an affine form with n variables and  initialized the m^th variable with  itv. */
	AffineMain(int n, int m, const Interval& itv);

	/** \brief Create an affine form with n variables, initialized with x  */
	AffineMain(const AffineMain& x);

	/** \brief  Delete the affine form */
	virtual ~AffineMain() { };

	/** \brief Return -*this. */
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
	 */
	AffineMain& operator=(const AffineMain& x);

	/** \brief Set *this to d.
	 */
	AffineMain& operator=(double x);

	/** \brief Set *this to itv.
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
	 */
	const Interval itv() const ;

	/**
	 * \brief return _val[i]
	 */
	double val(int i) const;

	/**
	 * \brief return _err
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

	/** \brief Multiply *this by \a x and return the result. */
	AffineMain& operator*=(const Interval& x);

	/** \brief Divide *this by \a x and return the result.*/
	AffineMain& operator/=(const Interval& x);

	/** \brief Add \a x to *this and return the result. */
	AffineMain& operator+=(const AffineMain& x);

	/** \brief Subtract \a x to *this and return the result. */
	AffineMain& operator-=(const AffineMain& x);

	/** \brief Multiply *this by \a x and return the result. */
	AffineMain& operator*=(const AffineMain& x);

	/** \brief Divide *this by \a x and return the result. */
	AffineMain& operator/=(const AffineMain& x);


	/** \brief Return sqr(*this) */
	AffineMain& Asqr(const Interval& itv);

	AffineMain&  Aneg();
	AffineMain&  Ainv(const Interval& itv);
	AffineMain&  Asqrt(const Interval& itv);
	AffineMain&  Aexp(const Interval& itv);
	AffineMain&  Alog(const Interval& itv);
	AffineMain&  Apow(int n, const Interval& itv);
	AffineMain&  Apow(double d, const Interval& itv);
	AffineMain&  Apow(const Interval &y, const Interval& itvx);
	AffineMain&  Aroot(int n, const Interval& itv);
	AffineMain&  Acos(const Interval& itv);
	AffineMain&  Asin(const Interval& itv);
	AffineMain&  Atan(const Interval& itv);
	AffineMain&  Aacos(const Interval& itv);
	AffineMain&  Aasin(const Interval& itv);
	AffineMain&  Aatan(const Interval& itv);
	AffineMain&  Acosh(const Interval& itv);
	AffineMain&  Asinh(const Interval& itv);
	AffineMain&  Atanh(const Interval& itv);
	AffineMain&  Aabs(const Interval& itv);



	AffineMain&  Ainv_CH(const Interval& itv);
	AffineMain&  Asqrt_CH(const Interval& itv);
	AffineMain&  Aexp_CH(const Interval& itv);
	AffineMain&  Alog_CH(const Interval& itv);

	AffineMain&  Ainv_MR(const Interval& itv);
	AffineMain&  Asqrt_MR(const Interval& itv);
	AffineMain&  Aexp_MR(const Interval& itv);
	AffineMain&  Alog_MR(const Interval& itv);

	typedef AffineMain<T> SCALAR;
	typedef AffineMainVector<T> VECTOR;
	typedef AffineMainMatrix<T> MATRIX;

};

// the following functions are
// introduced to allow genericity
template<class T> inline bool ___is_empty(const AffineMain<T>& x)       { return x.is_empty(); }
template<class T> inline void ___set_empty(AffineMain<T>& x)               { x.set_empty(); }


/** \ingroup arithmetic */
/*@{*/

/** \brief Stream out \a x. */
template<class T>
std::ostream& operator<<(std::ostream& os, const AffineMain<T>&  x);



/** \brief Return (-x) */
template<class T>
inline AffineMain<T> AffineMain<T>::operator-() const {
	AffineMain<T> res(*this);
	res.Aneg();
	return res;
}

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


/** \brief 1/AF[x] */
template<class T>
AffineMain<T> inv(const AffineMain<T>&  x);

/** \brief AF[x]^2 */
template<class T>
AffineMain<T> sqr(const AffineMain<T>&  x);

/** \brief sqrt{AF[x]} */
template<class T>
AffineMain<T> sqrt(const AffineMain<T>&  x);

/** \brief exp(AF[x]). */
template<class T>
AffineMain<T> exp(const AffineMain<T>&  x);

/** \brief log(AF[x]). */
template<class T>
AffineMain<T> log(const AffineMain<T>&  x);

/** \brief AF[x]^n. */
template<class T>
AffineMain<T> pow(const AffineMain<T>&  x, int n);

/** \brief AF[x]^d. */
template<class T>
AffineMain<T> pow(const AffineMain<T>&  x, double d);

/** \brief AF[x]^[y]. */
template<class T>
AffineMain<T> pow(const AffineMain<T>& x, const Interval &y);

/** \brief AF[x]^AF[y]. */
template<class T>
AffineMain<T> pow(const AffineMain<T>& x, const AffineMain<T>& y);

/** \brief nth root of AF[x]. */
template<class T>
AffineMain<T> root(const AffineMain<T>&  x, int n);

/** \brief cos(AF[x]). */
template<class T>
AffineMain<T> cos(const AffineMain<T>&  x);

/** \brief sin(AF[x]). */
template<class T>
AffineMain<T> sin(const AffineMain<T>&  x);

/** \brief tan(AF[x]). */
template<class T>
AffineMain<T> tan(const AffineMain<T>&  x);

/** \brief acos(AF[x]). */
template<class T>
AffineMain<T> acos(const AffineMain<T>&  x);

/** \brief asin(AF[x]). */
template<class T>
AffineMain<T> asin(const AffineMain<T>&  x);

/** \brief atan(AF[x]). */
template<class T>
AffineMain<T> atan(const AffineMain<T>&  x);

/** \brief cosh(AF[x]). */
template<class T>
AffineMain<T> cosh(const AffineMain<T>&  x);

/** \brief sinh(AF[x]). */
template<class T>
AffineMain<T> sinh(const AffineMain<T>&  x);

/** \brief tanh(AF[x]). */
template<class T>
AffineMain<T> tanh(const AffineMain<T>&  x);

/** \brief abs(AF[x]) = sqrt(sqr(AF[x])). */
template<class T>
AffineMain<T> abs(const AffineMain<T>& x);

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



namespace ibex {


template<class T> const double AffineMain<T>::AF_COMPAC_Tol = 1.e-6;
template<class T> const double AffineMain<T>::AF_EM = __builtin_powi(2.0, -51);
template<class T> const double AffineMain<T>::AF_EC = __builtin_powi(2.0, -55);
template<class T> const double AffineMain<T>::AF_EE = 2.0;
template<class T> bool AffineMain<T>::mode=true;



template<class T>
inline void AffineMain<T>::change_mode(Affine_Mode tt) {
	switch(tt) {
	case AF_Default:
	case AF_Chebyshev: {
		mode =true;
		break;
	}
	case AF_MinRange:
		mode =false;
		break;
	}
}

template<class T>
inline void AffineMain<T>::compact(){	compact(AF_COMPAC_Tol); }


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

//template<class T>
//inline AffineMain<T>& AffineMain<T>::inflate(double radd){
//	if (fabs(radd)>= POS_INFINITY) {
//		*this = Interval::ALL_REALS;
//	} else {
//		saxpy(1.0, AffineMain<T>(),0.0, radd, false, false, false, true);
//	}
//	return *this;
//}

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

//template<class T>
//inline AffineMain<T>& AffineMain<T>::operator+=(double d){
//	return saxpy(1.0, AffineMain<T>(), d, 0.0, false, false, true, false);
//}

template<class T>
inline AffineMain<T>& AffineMain<T>::operator-=(double d){
	*this += (-d);
	return *this;
}

//template<class T>
//inline AffineMain<T>& AffineMain<T>::operator*=(double d){
//	return saxpy(d, AffineMain<T>(), 0.0, 0.0, true, false, false, false);
//}

template<class T>
inline 	AffineMain<T>& AffineMain<T>::operator/=(double d) {
	return *this *= (1.0 / Interval(d)) ;
}

template<class T>
inline AffineMain<T>& AffineMain<T>::operator+=(const Interval& x){
	*this += x.mid();
	this->inflate(x.rad());
	return *this;
}

template<class T>
inline AffineMain<T>& AffineMain<T>::operator-=(const Interval& x){
	return *this += (-x);
}

template<class T>
inline AffineMain<T>& AffineMain<T>::operator/=(const Interval& x){
	return *this *= (1.0/x) ;
}

//template<class T>
//inline AffineMain<T>& AffineMain<T>::operator+=(const AffineMain<T>& x){
//	return saxpy(1.0, x, 0.0, 0.0, false, true, false, false);
//}

template<class T>
inline AffineMain<T>& AffineMain<T>::operator-=(const AffineMain<T>& x){
	return *this += (-AffineMain<T>(x));
}

template<class T>
inline AffineMain<T>& AffineMain<T>::operator/=(const AffineMain<T>& x){
	return *this *= (AffineMain<T>(x).Ainv(x.itv()));
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
	return AffineMain<T>(d) *= (AffineMain<T>(x).Ainv(x.itv()));
}

template<class T>
inline AffineMain<T> operator/(const AffineMain<T>& x1, const Interval& x2){
	return AffineMain<T>(x1) /= x2;
}

template<class T>
inline AffineMain<T> operator/(const Interval& x1, const AffineMain<T>& x2){
	return AffineMain<T>(x1) *= (AffineMain<T>(x2).Ainv(x2.itv()));
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
	return AffineMain<T>(x).Ainv(x.itv());
}

template<class T>
inline AffineMain<T> sqr(const AffineMain<T>& x){
	return AffineMain<T>(x).Asqr(x.itv());
}

template<class T>
inline AffineMain<T> sqrt(const AffineMain<T>& x){
	return AffineMain<T>(x).Asqrt(x.itv());
}

template<class T>
inline AffineMain<T> exp(const AffineMain<T>& x){
	return AffineMain<T>(x).Aexp(x.itv());
}

template<class T>
inline AffineMain<T> log(const AffineMain<T>& x){
	return AffineMain<T>(x).Alog(x.itv());
}

template<class T>
inline AffineMain<T> pow(const AffineMain<T>& x, int n) {
	return AffineMain<T>(x).Apow(n,x.itv());
}
template<class T>
inline AffineMain<T> pow(const AffineMain<T>& x, double d){
	return AffineMain<T>(x).Apow(d,x.itv());
}
template<class T>
inline AffineMain<T> pow(const AffineMain<T> &x, const Interval &y){
	// return exp(y * log(x));
	return AffineMain<T>(x).Apow(y,x.itv());
}
template<class T>
inline AffineMain<T> pow(const AffineMain<T> &x, const AffineMain<T> &y){
	return AffineMain<T>(x).Apow(y.itv(),x.itv());
}

template<class T>
inline AffineMain<T> root(const AffineMain<T>& x, int n) {
	return AffineMain<T>(x).Aroot(n,x.itv());
}

template<class T>
inline AffineMain<T> cos(const AffineMain<T>& x){
	return AffineMain<T>(x).Acos(x.itv());
}

template<class T>
inline AffineMain<T> sin(const AffineMain<T>& x){
	return AffineMain<T>(x).Asin(x.itv());
}

template<class T>
inline AffineMain<T> tan(const AffineMain<T>& x){
	return AffineMain<T>(x).Atan(x.itv());
}

template<class T>
inline AffineMain<T> acos(const AffineMain<T>& x){
	return AffineMain<T>(x).Aacos(x.itv());
}

template<class T>
inline AffineMain<T> asin(const AffineMain<T>& x){
	return AffineMain<T>(x).Aasin(x.itv());
}

template<class T>
inline AffineMain<T> atan(const AffineMain<T>& x){
	return AffineMain<T>(x).Aatan(x.itv());
}

template<class T>
inline AffineMain<T> cosh(const AffineMain<T>& x){
	return AffineMain<T>(x).Acosh(x.itv());
}

template<class T>
inline AffineMain<T> sinh(const AffineMain<T>& x){
	return AffineMain<T>(x).Asinh(x.itv());
}

template<class T>
inline AffineMain<T> tanh(const AffineMain<T>& x){
	return AffineMain<T>(x).Atanh(x.itv());
}

template<class T>
inline AffineMain<T> abs(const AffineMain<T> &x){
	return AffineMain<T>(x).Aabs(x.itv());
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
	return AffineMain<T>(sign(x.itv()));
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




#include "ibex_Affine.h_"

#endif /* IBEX_Affine_H_ */


/** \brief atan2(AF[x],AF[y]). */
//Affine2 atan2(const Affine2& x, const Affine2& y);
/** \brief atan2([x],AF[y]). */
//Affine2 atan2(const Interval& x, const Affine2& y);
/** \brief atan2(AF[x],[y]). */
//Affine2 atan2(const Affine2& x, const Interval& y);
/** \brief cosh(AF[x]). */
