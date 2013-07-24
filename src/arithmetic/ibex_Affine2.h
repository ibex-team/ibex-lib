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
#include <cmath>
#include <cassert>



//#define _IBEX_WITH_FAF1_
#define _IBEX_WITH_FAF2_


namespace ibex {

class Affine2Vector;
class Affine2Matrix;
class Affine2MatrixArray;

/**
 * \ingroup arithmetic
 *
 * \brief Affine Arithmetic AF2
 *
 */

class Affine2 {

private:



#ifdef _IBEX_WITH_FAF1_


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
	double * _val; 		// vector of elements of the affine form
	Interval _err; 	// error of the affine form, corresponded to the last term
	//	bool _actif; // boolean to know if the affine form is actif or not. This is to manage the particular case of EMPTY and an unbounded Interval


#else
#ifdef _IBEX_WITH_FAF2_

	/**
	 * Code for the particular case:
	 * if the affine form is actif, _n>1  and _n is the size of the affine form
	 * if the set is degenerate, _n = 0 or itv().diam()< AF_EC()
	 * if the set is empty, _n = -1
	 * if the set is ]-oo,+oo[, _n = -2
	 * if the set is [a, +oo[ , _n = -3 and _err= a
	 * if the set is ]-oo, a] , _n = -4 and _err= a
	 *
	 */

	int _n; 		// dimension (size of val)-1  , ie number of variable
	double * _val; 		// vector of elements of the affine form
	double _err; 	// error of the affine form, corresponded to the last term
	//	bool _actif; // boolean to know if the affine form is actif or not. This is to manage the particular case of EMPTY and an unbounded Interval


#endif
#endif

	static double AF_EM() {
		return __builtin_powi(2.0, -51);
	}
	static double AF_EC() {
		return __builtin_powi(2.0, -55);
	}
	static double AF_EE() {
		return 2.0;
	}

	/**
	 * \brief  Compute x = alpha*x+y+beta
	 * The boolean indicate which operation need to be done.
	 *  ! Boolean: scalar, add, constant, error
	 */
	Affine2& saxpy(double alpha, const Affine2& y, double beta, double delta,
			bool B1, bool B2, bool B3, bool B4);


#ifdef _IBEX_WITH_FAF2_

	/**
	 * \brief return the exact rounding error of the addition of 2 floating-point numbers
	 */
	double twoSum(double a, double b, double *res);

	/**
	 * \brief return the exact rounding error of the multiplication of 2 floating-point numbers
	 */
	double twoProd(double a, double b, double *res);
	void Split(double x, int sp, double *x_high, double *x_low);

#endif


public:

	typedef enum {
		AF_SQRT,
		AF_EXP,
		AF_LOG,
		AF_INV,
		AF_COS,
		AF_SIN,
		AF_TAN,
		AF_ABS,
		AF_ACOS,
		AF_ASIN,
		AF_ATAN,
		AF_COSH,
		AF_SINH,
		AF_TANH
	} affine2_expr; // ...etc...


#ifdef _IBEX_WITH_FAF1_


	/**
	 * \brief return _err
	 */
	const Interval err() const;


#else
#ifdef _IBEX_WITH_FAF2_


	/**
	 * \brief return _err
	 */
	double err() const;


#endif
#endif



	/** \brief Create an empty affine form. */
	Affine2();

	/** \brief Create an affine form with n variables and  initialized val[0] with d. */
	explicit Affine2(const double d);

	/** \brief Create an affine form with n variables and  initialized val[0] with  itv. */
	explicit Affine2(const Interval& itv);

	/** \brief Create an affine form with n variables and  initialized the m^th variable with  itv. */
	Affine2(int n, int m, const Interval& itv);

	/** \brief Create an affine form with n variables, initialized with x  */
	Affine2(const Affine2& x);

	/** \brief  Delete the affine form */
	virtual ~Affine2();

	/**
	 *\brief compute the min-range linearization of an unary operator
	 */
	//Affine2& linMinRange(affine2_expr num, const Interval itv);

	/**
	 *\brief compute the chebyshev linearization of an unary operator
	 */
	Affine2& linChebyshev(affine2_expr num, const Interval itv);

	/** \brief Return -*this. */
	Affine2 operator-() const;

	/** \brief Return sqr(*this) */
	Affine2& sqr(const Interval itv);

	/** \brief Return pow(*this,n) */
	Affine2& power(int n, const Interval itv);

	/** \brief True iff *this and x are exactly the same intervals. */
	bool operator==(const Affine2& x) const;

	/** \brief True iff *this and x are exactly the same intervals. */
	bool operator==(const Interval& x) const;

	/** \brief True iff *this and x are not exactly the same intervals. */
	bool operator!=(const Affine2& x) const;

	/** \brief True iff *this and x are not exactly the same intervals. */
	bool operator!=(const Interval& x) const ;

	/** \brief Set this interval to the empty set. */
	void set_empty();

	/** \brief Set *this to x.
	 */
	Affine2& operator=(const Affine2& x);

	/** \brief Set *this to d.
	 */
	Affine2& operator=(double x);

	/** \brief Set *this to itv.
	 */
	Affine2& operator=(const Interval& itv);

	/* Union and Intersection of two affine2 form must not be implemented
	 * That could produce to much confusion.
	 */
	/** \brief Intersection of *this and x.
	 * \param x - the interval to compute the intersection with.*/
	//	Affine2& operator&=(const Affine2& x);
	/** \brief Union of *this and I.
	 * \param x - the interval to compute the hull with.*/
	//	Affine2& operator|=(const Affine2& x);

	/**
	 * \brief Add [-rad,+rad] to *this.
	 *
	 * Return a reference to *this.
	 */
	Affine2& inflate(double radd);

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
	 * \brief return _val
	 */
	double * val() const;
	/**
	 * \brief return _val[i]
	 */
	double val(int i) const;

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

	/** \brief Add \a d to *this and return the result.  */
	Affine2& operator+=(double d);

	/** \brief Subtract \a d to *this and return the result. */
	Affine2& operator-=(double d);

	/** \brief Multiply *this by \a d and return the result. */
	Affine2& operator*=(double d);

	/** \brief Divide *this by \a d and return the result. */
	Affine2& operator/=(double d) ;

	/** \brief Add \a x to *this and return the result. */
	Affine2& operator+=(const Interval& x);

	/** \brief Subtract \a x to *this and return the result. */
	Affine2& operator-=(const Interval& x);

	/** \brief Multiply *this by \a x and return the result. */
	Affine2& operator*=(const Interval& x);

	/** \brief Divide *this by \a x and return the result.*/
	Affine2& operator/=(const Interval& x);

	/** \brief Add \a x to *this and return the result. */
	Affine2& operator+=(const Affine2& x);

	/** \brief Subtract \a x to *this and return the result. */
	Affine2& operator-=(const Affine2& x);

	/** \brief Multiply *this by \a x and return the result. */
	Affine2& operator*=(const Affine2& x);

	/** \brief Divide *this by \a x and return the result. */
	Affine2& operator/=(const Affine2& x);

	typedef Affine2 SCALAR;
	typedef Affine2Vector VECTOR;
	typedef Affine2Matrix MATRIX;
	typedef Affine2MatrixArray MATRIX_ARRAY;

};

/** \ingroup arithmetic */
/*@{*/

/** \brief Stream out \a x. */
std::ostream& operator<<(std::ostream& os, const Affine2& x);


/** \brief AF[x]_1+AF[x]_2. */
Affine2 operator+(const Affine2& x1, const Affine2& x2);

/** \brief AF[x]+d. */
Affine2 operator+(const Affine2& x, double d);

/** \brief d+AF[x]. */
Affine2 operator+(double d, const Affine2& x);

/** \brief AF[x]_1+[x]_2. */
Affine2 operator+(const Affine2& x1, const Interval& x2);

/** \brief [x]_1+AF[x]_2. */
Affine2 operator+(const Interval& x1, const Affine2& x2);

/** \brief AF[x]_1-AF[x]_2. */
Affine2 operator-(const Affine2& x1, const Affine2& x2);

/** \brief AF[x]-d. */
Affine2 operator-(const Affine2& x, double d);

/** \brief d-AF[x]. */
Affine2 operator-(double d, const Affine2& x);

/** \brief AF[x]_1-[x]_2. */
Affine2 operator-(const Affine2& x1, const Interval& x2);

/** \brief [x]_1-AF[x]_2. */
Affine2 operator-(const Interval& x1, const Affine2& x2);

/** \brief AF[x]_1*AF[x]_2. */
Affine2 operator*(const Affine2& x1, const Affine2& x2);

/** \brief AF[x]*d. */
Affine2 operator*(const Affine2& x, double d);

/** \brief d*AF[x]. */
Affine2 operator*(double d, const Affine2& x);

/** \brief AF[x]_1*[x]_2. */
Affine2 operator*(const Affine2& x1, const Interval& x2);

/** \brief [x]_1*AF[x]_2. */
Affine2 operator*(const Interval& x1, const Affine2& x2);

/** \brief AF[x]_1/AF[x]_2. */
Affine2 operator/(const Affine2& x1, const Affine2& x2);

/** \brief AF[x]/d. */
Affine2 operator/(const Affine2& x, double d);

/** \brief d/AF[x]. */
Affine2 operator/(double d, const Affine2& x);

/** \brief AF[x]_1/[x]_2. */
Affine2 operator/(const Affine2& x1, const Interval& x2);

/** \brief [x]_1/AF[x]_2. */
Affine2 operator/(const Interval& x1, const Affine2& x2);


/** \brief Hausdorff distance of AF[x]_1 and AF[x]_2. */
double distance(const Affine2 &x1, const Affine2 &x2);

/** \brief Hausdorff distance of [x]_1 and AF[x]_2. */
double distance(const Interval &x1, const Affine2 &x2);

/** \brief Hausdorff distance of AF[x]_1 and [x]_2. */
double distance(const Affine2 &x1, const Interval &x2);


/** \brief 1/AF[x] */
Affine2 inv(const Affine2& x);
Affine2 inv(const Affine2& x, const Interval itv);

/** \brief AF[x]^2 */
Affine2 sqr(const Affine2& x);
Affine2 sqr(const Affine2& x, const Interval itv);

/** \brief sqrt{AF[x]} */
Affine2 sqrt(const Affine2& x);
Affine2 sqrt(const Affine2& x, const Interval itv);

/** \brief exp(AF[x]). */
Affine2 exp(const Affine2& x);
Affine2 exp(const Affine2& x, const Interval itv);

/** \brief log(AF[x]). */
Affine2 log(const Affine2& x);
Affine2 log(const Affine2& x, const Interval itv);

/** \brief AF[x]^n. */
Affine2 pow(const Affine2& x, int n);
Affine2 pow(const Affine2& x, int n, const Interval itv);

/** \brief AF[x]^d. */
Affine2 pow(const Affine2& x, double d);
Affine2 pow(const Affine2& x, double d, const Interval itv);

/** \brief AF[x]^[y]. */
Affine2 pow(const Affine2 &x, const Interval &y);
Affine2 pow(const Affine2 &x, const Interval &y, const Interval itvx);

/** \brief AF[x]^AF[y]. */
Affine2 pow(const Affine2 &x, const Affine2 &y);

/** \brief nth root of AF[x]. */
Affine2 root(const Affine2& x, int n);
Affine2 root(const Affine2& x, int n, const Interval itv);

/** \brief cos(AF[x]). */
Affine2 cos(const Affine2& x);
Affine2 cos(const Affine2& x, const Interval itv);

/** \brief sin(AF[x]). */
Affine2 sin(const Affine2& x);
Affine2 sin(const Affine2& x, const Interval itv);

/** \brief tan(AF[x]). */
Affine2 tan(const Affine2& x);
Affine2 tan(const Affine2& x, const Interval itv);

/** \brief acos(AF[x]). */
Affine2 acos(const Affine2& x);
Affine2 acos(const Affine2& x, const Interval itv);

/** \brief asin(AF[x]). */
Affine2 asin(const Affine2& x);
Affine2 asin(const Affine2& x, const Interval itv);

/** \brief atan(AF[x]). */
Affine2 atan(const Affine2& x);
Affine2 atan(const Affine2& x, const Interval itv);

/** \brief cosh(AF[x]). */
Affine2 cosh(const Affine2& x);
Affine2 cosh(const Affine2& x, const Interval itv);

/** \brief sinh(AF[x]). */
Affine2 sinh(const Affine2& x);
Affine2 sinh(const Affine2& x, const Interval itv);

/** \brief tanh(AF[x]). */
Affine2 tanh(const Affine2& x);

Affine2 tanh(const Affine2& x, const Interval itv);

/** \brief abs(AF[x]) = sqrt(sqr(AF[x])). */
Affine2 abs(const Affine2 &x);

Affine2 abs(const Affine2 &x, const Interval itv);


Interval max(const Affine2& x, const Affine2& y);
Interval max(const Interval& x, const Affine2& y);
Interval max(const Affine2& x, const Interval& y);

Interval min(const Affine2& x, const Affine2& y);
Interval min(const Interval& x, const Affine2& y) ;
Interval min(const Affine2& x, const Interval& y);


/** \brief \f$[x]_1\cap [x]_2\f$.
 * \return Interval::EMPTY if the intersection is empty. */
Interval operator&(const Affine2& x1, const Affine2& x2);
Interval operator&(const Interval& x1, const Affine2& x2);
Interval operator&(const Affine2& x1, const Interval& x2);


/** \brief \f$\square([x]_1\cup [x]_2)\f$. */
Interval operator|(const Affine2& x1, const Affine2& x2);
Interval operator|(const Interval& x1, const Affine2& x2);
Interval operator|(const Affine2& x1, const Interval& x2);


/**
 * \brief Return the largest integer interval included in x.
 *  TODO Affine2::integer not yet implemented integer
 */
Affine2 integer(const Affine2& x);

/** \brief Sign of AF[x].
 *
 *  Return \f$sign(AF[x]) = hull \{ sign{x}, x\inAF[x] \}\f$.
 * \remark By convention, \f$ 0\inAF[x] \Longrightarrow sign(AF[x])=[-1,1]\f$. */
Affine2 sign(const Affine2& x);
Affine2 sign(const Affine2& x, const Interval itv);


}

/*@}*/

/*============================================ inline implementation ============================================ */



namespace ibex {

#ifdef _IBEX_WITH_FAF1_


inline const Interval Affine2::err() const{
	return _err;
}


#else
#ifdef _IBEX_WITH_FAF2_


inline double Affine2::err() const{
	return _err;
}



#endif
#endif







inline Affine2::~Affine2() {
	if (_val!=NULL) delete[] _val;
}


inline Interval& Interval::operator&=(const Affine2& x) {
	return (*this &= x.itv());
}

inline Interval& Interval::operator|=(const Affine2& x) {
	return (*this |= x.itv());
}

inline Interval& Interval::operator=(const Affine2& x) {
	return (*this = x.itv());
}

inline Interval operator&(const Affine2& x1, const Affine2& x2) {
	return ((x1.itv())&(x2.itv()));
}

inline Interval operator&(const Interval& x1, const Affine2& x2) {
	return ((x1)&(x2.itv()));
}

inline Interval operator&(const Affine2& x1, const Interval& x2) {
	return ((x1.itv())&(x2));
}

inline Interval operator|(const Affine2& x1, const Affine2& x2) {
	return ((x1.itv())|(x2.itv()));
}

inline Interval operator|(const Interval& x1, const Affine2& x2) {
	return ((x1)|(x2.itv()));
}

inline Interval operator|(const Affine2& x1, const Interval& x2) {
	return ((x1.itv())|(x2));
}


inline bool Affine2::operator==(const Affine2& x) const{
	return (this->itv() == x.itv());
}

inline bool Affine2::operator==(const Interval& x) const{
	return (this->itv() == x);
}

inline bool Affine2::operator!=(const Affine2& x) const{
	return (this->itv() == x.itv());
}

inline bool Affine2::operator!=(const Interval& x) const {
	return (this->itv() != x);
}


inline void Affine2::set_empty(){
	*this = Interval::EMPTY_SET;
}


inline Affine2& Affine2::inflate(double radd){
	if (fabs(radd)>= POS_INFINITY) {
		*this = Interval::ALL_REALS;
	} else {
		saxpy(1.0, Affine2(),0.0, radd, false, false, false, true);
	}
	return *this;
}

inline int Affine2::size() const{
	return _n;
}

inline double * Affine2::val() const{
	return _val;
}

inline double Affine2::val(int i) const{
	assert((0<=i) && (i<=_n));
	return _val[i];
}

inline bool Affine2::is_actif() const{
	//return _actif;
	return (_n>-1);
}

inline bool Affine2::is_empty() const{
	return (_n==-1);
}

inline bool Affine2::is_degenerated() const {
	return (itv().diam() <	AF_EC());
}

inline bool Affine2::is_unbounded() const{
	return ((-1>_n)&&(_n>-5));
}

inline double Affine2::mid() const{
	return (is_actif())? _val[0] : itv().mid();
}

inline Affine2& Affine2::operator+=(double d){
	return saxpy(1.0, Affine2(), d, 0.0, false, false, true, false);
}

inline Affine2& Affine2::operator-=(double d){
	return saxpy(1.0, Affine2(), (-d), 0.0, false, false, true, false);
}

inline Affine2& Affine2::operator*=(double d){
	return saxpy(d, Affine2(), 0.0, 0.0, true, false, false, false);
}

inline 	Affine2& Affine2::operator/=(double d) {
	return *this *= (1.0 / Interval(d)) ;
}

inline Affine2& Affine2::operator+=(const Interval& x){
	return saxpy(1.0, Affine2(), x.mid(), x.rad(), false, false, true, true);
}

inline Affine2& Affine2::operator-=(const Interval& x){
	return *this += (-x);
}

inline Affine2& Affine2::operator/=(const Interval& x){
	return *this *= (1.0/x) ;
}

inline Affine2& Affine2::operator+=(const Affine2& x){
	return saxpy(1.0, x, 0.0, 0.0, false, true, false, false);
}

inline Affine2& Affine2::operator-=(const Affine2& x){
	return *this += (-Affine2(x));
}

inline Affine2& Affine2::operator/=(const Affine2& x){
	return *this *= (Affine2(x).linChebyshev(Affine2::AF_INV, x.itv()));
}

inline Affine2 operator+(const Affine2& x1, const Affine2& x2){
	return Affine2(x1) += x2;
}

inline Affine2 operator+(const Affine2& x, double d){
	return Affine2(x) += d;
}

inline Affine2 operator+(double d, const Affine2& x){
	return Affine2(x) += d;
}

inline Affine2 operator+(const Affine2& x1, const Interval& x2){
	return Affine2(x1) += x2;
}

inline Affine2 operator+(const Interval& x1, const Affine2& x2){
	return Affine2(x2) += x1;
}

inline Affine2 operator-(const Affine2& x1, const Affine2& x2){
	return Affine2(x1) += (-x2);
}

inline Affine2 operator-(const Affine2& x, double d){
	return Affine2(x) -= d;
}

inline Affine2 operator-(double d, const Affine2& x){
	Affine2 res = (-x);
	return res += d;
}

inline Affine2 operator-(const Affine2& x1, const Interval& x2) {
	return Affine2(x1) -= x2;
}

inline Affine2 operator-(const Interval& x1, const Affine2& x2) {
	Affine2 res = (- x2);
	return res += x1;
}

inline Affine2 operator*(const Affine2& x1, const Affine2& x2) {
	return Affine2(x1) *= x2;
}

inline Affine2 operator*(const Affine2& x, double d){
	return Affine2(x) *= d;
}

inline Affine2 operator*(double d, const Affine2& x){
	return Affine2(x) *= d;
}

inline Affine2 operator*(const Affine2& x1, const Interval& x2){
	return Affine2(x1) *= x2;
}

inline Affine2 operator*(const Interval& x1, const Affine2& x2){
	return Affine2(x2) *= x1;
}

inline Affine2 operator/(const Affine2& x1, const Affine2& x2){
	return Affine2(x1) /= x2;
}

inline Affine2 operator/(const Affine2& x, double d){
	return Affine2(x) /= d;
}

inline Affine2 operator/(double d, const Affine2& x){
	return Affine2(d) *= (Affine2(x).linChebyshev(Affine2::AF_INV,x.itv()));
}

inline Affine2 operator/(const Affine2& x1, const Interval& x2){
	return Affine2(x1) /= x2;
}

inline Affine2 operator/(const Interval& x1, const Affine2& x2){
	return Affine2(x1) *= (Affine2(x2).linChebyshev(Affine2::AF_INV,x2.itv()));
}

inline double distance(const Affine2 &x1, const Affine2 &x2){
	return distance(x1.itv(), x2.itv());
}

inline double distance(const Interval &x1, const Affine2 &x2){
	return distance(x1, x2.itv());
}

inline double distance(const Affine2 &x1, const Interval &x2){
	return distance(x1.itv(), x2);
}

inline Affine2 inv(const Affine2& x){
	return inv(x,x.itv());
}

inline Affine2 inv(const Affine2& x, const Interval itv){
	return Affine2(x).linChebyshev(Affine2::AF_INV, itv);
}

inline Affine2 sqr(const Affine2& x){
	return Affine2(x).sqr(x.itv());
}
inline Affine2 sqr(const Affine2& x, const Interval itv){
	return Affine2(x).sqr(itv);
}

inline Affine2 sqrt(const Affine2& x){
	return sqrt(x,x.itv());
}

inline Affine2 sqrt(const Affine2& x, const Interval itv){
	return Affine2(x).linChebyshev(Affine2::AF_SQRT,itv);
}

inline Affine2 exp(const Affine2& x){
	return exp(x,x.itv());
}

inline Affine2 exp(const Affine2& x, const Interval itv){
	return Affine2(x).linChebyshev(Affine2::AF_EXP,itv);
}

inline Affine2 log(const Affine2& x){
	return log(x,x.itv());
}

inline Affine2 log(const Affine2& x, const Interval itv){
	return Affine2(x).linChebyshev(Affine2::AF_LOG,itv);
}

inline Affine2 pow(const Affine2& x, int n) {
	return pow(x,n,x.itv());
}

inline Affine2 pow(const Affine2& x, double d){
	return pow(x,d,x.itv());
}

inline Affine2 pow(const Affine2 &x, const Interval &y){
	// return exp(y * log(x));
	return pow(x,y,x.itv());
}

inline Affine2 pow(const Affine2 &x, const Interval &y, const Interval itvx){
	// return exp(y * log(x));
	return (y * Affine2(x).linChebyshev(Affine2::AF_LOG, itvx)).linChebyshev(Affine2::AF_EXP, (y*log(itvx)));
}

inline Affine2 pow(const Affine2 &x, const Affine2 &y){
	return pow(x, y.itv());
}

inline Affine2 root(const Affine2& x, int n) {
	return root(x,n,x.itv());
}

inline Affine2 cos(const Affine2& x){
	return cos(x,x.itv());
}

inline Affine2 cos(const Affine2& x, const Interval itv){
	return Affine2(x).linChebyshev(Affine2::AF_COS, itv);
}

inline Affine2 sin(const Affine2& x){
	return sin(x,x.itv());
}

inline Affine2 sin(const Affine2& x, const Interval itv){
	return Affine2(x).linChebyshev(Affine2::AF_SIN,itv);
}

inline Affine2 tan(const Affine2& x){
	return tan(x,x.itv());
}

inline Affine2 tan(const Affine2& x, const Interval itv){
	return Affine2(x).linChebyshev(Affine2::AF_TAN,itv);
}

inline Affine2 acos(const Affine2& x){
	return acos(x,x.itv());
}

inline Affine2 acos(const Affine2& x, const Interval itv){
	return Affine2(x).linChebyshev(Affine2::AF_ACOS, itv);
}

inline Affine2 asin(const Affine2& x){
	return asin(x,x.itv());
}

inline Affine2 asin(const Affine2& x, const Interval itv){
	return Affine2(x).linChebyshev(Affine2::AF_ASIN, itv);
}

inline Affine2 atan(const Affine2& x){
	return atan(x,x.itv());
}

inline Affine2 atan(const Affine2& x, const Interval itv){
	return Affine2(x).linChebyshev(Affine2::AF_ATAN, itv);
}

inline Affine2 cosh(const Affine2& x){
	return cosh(x,x.itv());
}

inline Affine2 cosh(const Affine2& x, const Interval itv){
	return Affine2(x).linChebyshev(Affine2::AF_COSH, itv);
}

inline Affine2 sinh(const Affine2& x){
	return sinh(x,x.itv());
}

inline Affine2 sinh(const Affine2& x, const Interval itv){
	return Affine2(x).linChebyshev(Affine2::AF_SINH, itv);
}

inline Affine2 tanh(const Affine2& x){
	return tanh(x,x.itv());
}

inline Affine2 tanh(const Affine2& x, const Interval itv){
	return Affine2(x).linChebyshev(Affine2::AF_TANH, itv);
}

inline Affine2 abs(const Affine2 &x){
	return abs(x,x.itv());
}

inline Affine2 abs(const Affine2 &x, const Interval itv){
	return Affine2(x).linChebyshev(Affine2::AF_ABS, itv);
}

inline Interval max(const Affine2& x, const Affine2& y) {
	return max(x.itv(), y.itv());
}

inline Interval max(const Interval& x, const Affine2& y) {
	return max(x, y.itv());
}

inline Interval max(const Affine2& x, const Interval& y) {
	return max(x.itv(), y);
}

inline Interval min(const Affine2& x, const Affine2& y) {
	return min(x.itv(), y.itv());
}

inline Interval min(const Interval& x, const Affine2& y) {
	return min(x, y.itv());
}

inline Interval min(const Affine2& x, const Interval& y) {
	return min(x.itv(), y);
}

inline Affine2 integer(const Affine2& x){
	return Affine2(x);
}

inline Affine2 sign(const Affine2& x) {
	return sign(x, x.itv());
}












} // end namespace ibex

#endif /* IBEX_AFFINE2_H_ */


/** \brief atan2(AF[x],AF[y]). */
//Affine2 atan2(const Affine2& x, const Affine2& y);
/** \brief atan2([x],AF[y]). */
//Affine2 atan2(const Interval& x, const Affine2& y);
/** \brief atan2(AF[x],[y]). */
//Affine2 atan2(const Affine2& x, const Interval& y);
/** \brief cosh(AF[x]). */
