/* ============================================================================
 * I B E X - Affine2 definition
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
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
#include "ibex_IntervalVector.h"
#include <cmath>

namespace ibex {


/**
 * \ingroup arithmetic
 *
 * \brief Affine Arithmetic AF2
 *
 */

class Affine2 {

private:

	int _n; 		// dimension (size of val)-1  , ie number of variable
	double * _val; 		// vector of elements of the affine form
	Interval _err; 	// error of the affine form, corresponded to the last term
	bool _actif; 	// set to 1 iff the affine form can be used
	Interval _itv; 		// Minimal interval represented by the affine form

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

	/**
	 * \brief  convert the affine part to an interval
	 * Use to actualize itv
	 */
	void update_itv();

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

	/** \brief Create an empty affine form. */
	Affine2();

	/** \brief Create an affine form with n variables, initialized with  [-oo,+oo]. */
	explicit Affine2(int n);

	/** \brief Create an affine form with n variables and  initialized val[0] with d. */
	Affine2(int n, const double d);

	/** \brief Create an affine form with n variables and  initialized val[0] with  itv. */
	Affine2(int n, const Interval& itv);

	/** \brief Create an affine form with n variables and  initialized the m^th variable with  itv. */
	Affine2(int n, int m, const Interval& itv);


	/** \brief Create an affine form with n variables, initialized with x. */
	Affine2(const Affine2& x);

	/** \brief Create an affine form with n variables, initialized with -x if (b) elseif (!b) with x. */
	Affine2(const Affine2& x, bool b);

	/** \brief  Delete the affine form */
	~Affine2();

	/**
	 *\brief compute the min-range linearization of an unary operator
	 */
	Affine2& linMinRange(affine2_expr num);

	/**
	 *\brief compute the chebyshev linearization of an unary operator
	 */
	Affine2& linChebyshev(affine2_expr num);

	/** \brief Return sqr(*this) */
	Affine2& sqr();

	/** \brief Return pow(*this,n) */
	Affine2& power(int n);

//operator

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
	void resize(int n);

	/**
	 * \brief number of variable represented
	 */
	 int size() const;

	/**
	 * \brief Range of the affine form
	 */
	 Interval itv() const ;
	 Interval& ITV();

	/**
	 * \brief return _val
	 */
	 double * val() const;
	/**
	 * \brief return _val[i]
	 */
	 double val(int i) const;

	/**
	 * \brief return _err
	 */
	 Interval err() const;

	/**
	 * \brief return 1 if the affine form is actif and valid
	 */
	 bool isActif() const;

	/**
	 * \brief return 1 if the affine form is actif and valid
	 */
	 void setActif(bool b);

	/** \brief Lower bound.
	 *
	 * Return the lower bound of *this. */
	 double lb() const;

	/** \brief Upper bound.
	 *
	 * Return the upper bound of *this. */
	 double ub() const;

	/** \brief Midpoint.
	 *
	 * Returns the midpoint of *this.
	 * The return point is guaranteed to be included in *this
	 * but not necessarily to be the closest floating point
	 * from the real midpoint.
	 *
	 * Cases are:
	 * - \emptyset  -> Quiet NaN
	 * - [-oo, +oo] -> midP = 0.0
	 * - [-oo, b]   -> midP = -MAXREAL
	 * - [a, +oo]   -> midP = MAXREAL
	 * - [a, b]     -> midP ~ a + .5*(b-a) */
	 double mid() const;

	/**
	 * \brief Radius.
	 *
	 * Return the diameter of *this.
	 * By convention, 0 if *this is empty.*/
	 double rad() const;

	/**
	 * \brief Diameter.
	 *
	 * Return the diameter of *this.
	 * By convention, 0 if *this is empty.*/
	 double diam() const;
	/**
	 * \brief Mignitude.
	 *
	 * Returns the mignitude of *this:
	 * <lu>
	 * <li> +(lower bound)  if *this > 0
	 * <li> -(upper bound) if *this < 0
	 * <li> 0 otherwise.
	 * </lu> */
	 double mig() const;

	/**
	 * \brief Magnitude.
	 *
	 * Returns the magnitude of *this:
	 * mag(*this)=max(|lower bound|, |upper bound|). */
	 double mag() const;

	/**
	 * \brief True iff this affine form is a subset of \a x.
	 *
	 * \note Always return true if *this is empty. */
	 bool is_subset(const Affine2& x) const;
	/**
	 * \brief True iff this affine form is a subset of \a x.
	 *
	 * \note Always return true if *this is empty. */
	 bool is_subset(const Interval& x) const;

	/**
	 * \brief True iff this interval is in the interior of \a x.
	 *
	 * \note In particular, (-oo,oo) is a strict subset of (-oo,oo)
	 * and the empty set is a strict subset of the empty set.
	 * \note Always return true if *this is empty. */
	 bool is_strict_subset(const Affine2& x) const;
	/**
	 * \brief True iff this interval is in the interior of \a x.
	 *
	 * \note In particular, (-oo,oo) is a strict subset of (-oo,oo)
	 * and the empty set is a strict subset of the empty set.
	 * \note Always return true if *this is empty. */
	 bool is_strict_subset(const Interval& x) const;

	/**
	 * \brief True iff this interval is a superset of \a x.
	 *
	 * \note Always return true if x is empty. */
	 bool is_superset(const Affine2& x) const;

	/**
	 * \brief True iff this interval is a superset of \a x.
	 *
	 * \note Always return true if x is empty. */
	 bool is_superset(const Interval& x) const;

	/**
	 * \brief True iff the interior of *this is a superset of \a x.
	 *
	 * \note In particular, (-oo,oo) is a strict superset of (-oo,oo)
	 */
	 bool is_strict_superset(const Affine2& x) const;

	/**
	 * \brief True iff the interior of *this is a superset of \a x.
	 *
	 * \note In particular, (-oo,oo) is a strict superset of (-oo,oo)
	 */
	 bool is_strict_superset(const Interval& x) const;

	/**
	 * \brief True iff *this contains \a d.
	 *
	 * \note d can also be an "open bound", i.e., infinity.
	 * So this function is not restricted to a set-membership
	 * interpretation. */
	 bool contains(double d) const;

	/**
	 * \brief True iff the interior of *this contains \a d.
	 *
	 * \note d can also be an "open bound", i.e., infinity.
	 * So this function is not restricted to a set-membership
	 * interpretation. */
	 bool strictly_contains(double d) const;

	/**
	 * \brief True iff *this and \a x do not intersect.
	 *
	 */
	 bool is_disjoint(const Affine2 &x) const;

	/**
	 * \brief True iff *this and \a x do not intersect.
	 *
	 */
	 bool is_disjoint(const Interval &x) const;
	/**
	 * \brief True iff *this is empty.
	 */
	 bool is_empty() const;

	/**
	 * \brief True iff *this is degenerated.
	 *
	 * An affine form is degenerated if it is of the form [a, a]
	 *
	 * \note An empty affine form is considered here as degenerated. */
	 bool is_degenerated() const ;

	/**
	 * \brief True if one bound of *this is infinite.
	 *
	 * \note An empty affine form is always bounded.
	 */
	 bool is_unbounded() const;

    /**
     * \brief True iff *this can be bisected along one dimension.
     *
     * \sa #ibex::Interval::is_bisectable().
     */
     bool is_bisectable() const;

	/**
	 * \brief Relative Hausdorff distance between *this and x.
	 *
	 * The relative distance is basically distance(x)/diam(*this).
	 */
	 double rel_distance(const Affine2& x) const;

	/**
	 * \brief Relative Hausdorff distance between *this and x.
	 *
	 * The relative distance is basically distance(x)/diam(*this).
	 */
	 double rel_distance(const Interval& x) const;

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

	/**
	 * \brief Return diam(*this)-diam(x), for x\subseteq *this [deprecated]
	 *
	 * Deprecated. Kept for compatibility with ibex 1.xx.
	 *
	 * \pre \a x must be included in this interval.
	 * \note The result may be +oo (if the set difference is infinite).
	 * \note An empty interval is considered here to have a null diamater (as a degenerated interval). <br>
	 * If either \a x or this interval is empty, then the method returns the diameter of this interval
	 * (which is 0 if the latter is empty).
	 */
	 double delta(const Affine2& x) const;

	/**
	 * \brief Return diam(*this)-diam(x), for x\subseteq *this [deprecated]
	 *
	 * Deprecated. Kept for compatibility with ibex 1.xx.
	 *
	 * \pre \a x must be included in this interval.
	 * \note The result may be +oo (if the set difference is infinite).
	 * \note An empty interval is considered here to have a null diamater (as a degenerated interval). <br>
	 * If either \a x or this interval is empty, then the method returns the diameter of this interval
	 * (which is 0 if the latter is empty).
	 */
	 double delta(const Interval& x) const;

	/**
	 * \brief Compute the ratio of the diameter to #delta(x) [deprecated].
	 *
	 * Deprecated. Kept for compatibility with ibex 1.xx.
	 *
	 * \pre \a x must be included in this interval.
	 * \note An empty interval is considered to have a null diamater (as a degenerated interval). <br>
	 * <ul><li>If either \a x or this interval is empty, then
	 * <ul><li>the method returns 1 (100% of reduction) if this diameter is not null,
	 *     <li>0 otherwise (as if 0/0=0).</ul>
	 * <li>As a pure convention, the method returns \c 1 if one bound of this interval is infinite and the corresponding bound of \a x
	 * is not (in particular if this interval is unbounded and \a x not). </ul>
	 */
	 double ratiodelta(const Affine2& x) const;

	/**
	 * \brief Compute the ratio of the diameter to #delta(x) [deprecated].
	 *
	 * Deprecated. Kept for compatibility with ibex 1.xx.
	 *
	 * \pre \a x must be included in this interval.
	 * \note An empty interval is considered to have a null diamater (as a degenerated interval). <br>
	 * <ul><li>If either \a x or this interval is empty, then
	 * <ul><li>the method returns 1 (100% of reduction) if this diameter is not null,
	 *     <li>0 otherwise (as if 0/0=0).</ul>
	 * <li>As a pure convention, the method returns \c 1 if one bound of this interval is infinite and the corresponding bound of \a x
	 * is not (in particular if this interval is unbounded and \a x not). </ul>
	 */
	 double ratiodelta(const Interval& x) const;

    /**
     * \brief Bisect *this into two subintervals.
     *
     * \param ratio - says where to split (0.5=middle)
     * \pre is_bisectable() must be true.
     * \pre 0<ratio<1.
     */
	 std::pair<Interval,Interval> bisect(double ratio) const;

	 std::pair<Interval,Interval> bisect() const;



};

/** \ingroup arithmetic */
/*@{*/

/** \brief Stream out \a x. */
std::ostream& operator<<(std::ostream& os, const Affine2& x);

/** \brief Return -x. */
 Affine2 operator-(const Affine2& x);

/** \brief $AF[x]_1+AF[x]_2$. */
 Affine2 operator+(const Affine2& x1, const Affine2& x2);

/** \brief $AF[x]+d$. */
 Affine2 operator+(const Affine2& x, double d);

/** \brief $d+AF[x]$. */
 Affine2 operator+(double d, const Affine2& x);

/** \brief $AF[x]_1+[x]_2$. */
 Affine2 operator+(const Affine2& x1, const Interval& x2);

/** \brief $[x]_1+AF[x]_2$. */
 Affine2 operator+(const Interval& x1, const Affine2& x2);

/** \brief $AF[x]_1-AF[x]_2$. */
 Affine2 operator-(const Affine2& x1, const Affine2& x2);

/** \brief $AF[x]-d$. */
 Affine2 operator-(const Affine2& x, double d);

/** \brief $d-AF[x]$. */
 Affine2 operator-(double d, const Affine2& x);

/** \brief $AF[x]_1-[x]_2$. */
 Affine2 operator-(const Affine2& x1, const Interval& x2);

/** \brief $[x]_1-AF[x]_2$. */
 Affine2 operator-(const Interval& x1, const Affine2& x2);

/** \brief $AF[x]_1*AF[x]_2$. */
 Affine2 operator*(const Affine2& x1, const Affine2& x2);

/** \brief $AF[x]*d$. */
 Affine2 operator*(const Affine2& x, double d);

/** \brief $d*AF[x]$. */
 Affine2 operator*(double d, const Affine2& x);

/** \brief $AF[x]_1*[x]_2$. */
 Affine2 operator*(const Affine2& x1, const Interval& x2);

/** \brief $[x]_1*AF[x]_2$. */
 Affine2 operator*(const Interval& x1, const Affine2& x2);

/** \brief $AF[x]_1/AF[x]_2$. */
 Affine2 operator/(const Affine2& x1, const Affine2& x2);

/** \brief $AF[x]/d$. */
 Affine2 operator/(const Affine2& x, double d);

/** \brief $d/AF[x]$. */
 Affine2 operator/(double d, const Affine2& x);

/** \brief $AF[x]_1/[x]_2$. */
 Affine2 operator/(const Affine2& x1, const Interval& x2);

/** \brief $[x]_1/AF[x]_2$. */
 Affine2 operator/(const Interval& x1, const Affine2& x2);

/** \brief Hausdorff distance of $AF[x]_1$ and $AF[x]_2$. */
 double distance(const Affine2 &x1, const Affine2 &x2);

/** \brief Hausdorff distance of $[x]_1$ and $AF[x]_2$. */
 double distance(const Interval &x1, const Affine2 &x2);

/** \brief Hausdorff distance of $AF[x]_1$ and $[x]_2$. */
 double distance(const Affine2 &x1, const Interval &x2);

/** \brief $1/AF[x]$ */
 Affine2 inv(const Affine2& x);

/** \brief $AF[x]^2$ */
 Affine2 sqr(const Affine2& x);

/** \brief $\sqrt{AF[x]}$ */
 Affine2 sqrt(const Affine2& x);

/** \brief $\exp(AF[x])$. */
 Affine2 exp(const Affine2& x);

/** \brief $\log(AF[x])$. */
 Affine2 log(const Affine2& x);

/** \brief $AF[x]^n$. */
Affine2 pow(const Affine2& x, int n);

/** \brief $AF[x]^d$. */
Affine2 pow(const Affine2& x, double d);

/** \brief $AF[x]^[y]$. */
 Affine2 pow(const Affine2 &x, const Interval &y);

/** \brief $AF[x]^AF[y]$. */
 Affine2 pow(const Affine2 &x, const Affine2 &y);

/** \brief $n^{th}$ root of $AF[x]$. */
Affine2 root(const Affine2& x, int n);

/** \brief $\cos(AF[x])$. */
 Affine2 cos(const Affine2& x);

/** \brief $\sin(AF[x])$. */
 Affine2 sin(const Affine2& x);

/** \brief $\tan(AF[x])$. */
 Affine2 tan(const Affine2& x);

/** \brief $\acos(AF[x])$. */
 Affine2 acos(const Affine2& x);

/** \brief $\asin(AF[x])$. */
 Affine2 asin(const Affine2& x);

/** \brief $\atan(AF[x])$. */
 Affine2 atan(const Affine2& x);

 Affine2 cosh(const Affine2& x);

/** \brief $\sinh(AF[x])$. */
 Affine2 sinh(const Affine2& x);

/** \brief $\tanh(AF[x])$. */
 Affine2 tanh(const Affine2& x);

/** \brief $\abs(AF[x]) = sqrt(sqr(AF[x]))$. */
 Affine2 abs(const Affine2 &x);


 Interval max(const Affine2& x, const Affine2& y);

 Interval max(const Interval& x, const Affine2& y);

 Interval max(const Affine2& x, const Interval& y);

 Interval min(const Affine2& x, const Affine2& y);

 Interval min(const Interval& x, const Affine2& y) ;

 Interval min(const Affine2& x, const Interval& y);


 /** \brief $[x]_1\cap [x]_2$.
  * \return Interval::EMPTY if the intersection is empty. */
 Interval operator&(const Affine2& x1, const Affine2& x2);

 /** \brief $[x]_1\cap [x]_2$.
  * \return Interval::EMPTY if the intersection is empty. */
 Interval operator&(const Interval& x1, const Affine2& x2);


 /** \brief $[x]_1\cap [x]_2$.
  * \return Interval::EMPTY if the intersection is empty. */
 Interval operator&(const Affine2& x1, const Interval& x2);


 /** \brief $\square([x]_1\cup [x]_2)$. */
 Interval operator|(const Affine2& x1, const Affine2& x2);

 /** \brief $\square([x]_1\cup [x]_2)$. */
 Interval operator|(const Interval& x1, const Affine2& x2);

 /** \brief $\square([x]_1\cup [x]_2)$. */
 Interval operator|(const Affine2& x1, const Interval& x2);

/**
 * \brief Return the largest integer interval included in x.
 *  TODO  not yet implemented integer
 */
 Affine2 integer(const Affine2& x);

/** \brief Sign of $AF[x]$.
 *
 *  Return \f$sign(AF[x]) = hull \{ sign{x}, x\inAF[x] \}\f$.
 * \remark By convention, \f$ 0\inAF[x] \Longrightarrow sign(AF[x])=[-1,1]\f$. */
Affine2 sign(const Affine2& x);

/** \brief Projection of $y=x_1+x_2$.
 *
 * Set $([x]_1,[x]_2)$ to $([x]_1,[x]_2])\cap\{ (x_1,x_2)\in [x]_1\times[x]_2 \ | \ \exists y\in[y],\ y=x_1+x_2\}$. */
bool proj_add(const Affine2& y, Affine2& x1, Affine2& x2);
bool proj_add(const Affine2& y, Interval& x1, Affine2& x2);
bool proj_add(const Affine2& y, Affine2& x1, Interval& x2);
bool proj_add(const Affine2& y, Interval& x1, Interval& x2);
bool proj_add(const Interval& y, Affine2& x1, Affine2& x2);
bool proj_add(const Interval& y, Interval& x1, Affine2& x2);
bool proj_add(const Interval& y, Affine2& x1, Interval& x2);

/** \brief Projection of $y=x_1-x_2$.
 *
 * Set $([x]_1,[x]_2)$ to $([x]_1,[x]_2])\cap\{ (x_1,x_2)\in [x]_1\times[x]_2 \ | \ \exists y\in[y],\ y=x_1-x_2\}$. */
bool proj_sub(const Affine2& y, Affine2& x1, Affine2& x2);
bool proj_sub(const Affine2& y, Interval& x1, Affine2& x2);
bool proj_sub(const Affine2& y, Affine2& x1, Interval& x2);
bool proj_sub(const Affine2& y, Interval& x1, Interval& x2);
bool proj_sub(const Interval& y, Affine2& x1, Affine2& x2);
bool proj_sub(const Interval& y, Interval& x1, Affine2& x2);
bool proj_sub(const Interval& y, Affine2& x1, Interval& x2);

/** \brief Projection of $y=x_1*x_2$.
 *
 * Set $([x]_1,[x]_2)$ to $([x]_1,[x]_2])\cap\{ (x_1,x_2)\in [x]_1\times[x]_2 \ | \ \exists y\in[y],\ y=x_1\times x_2\}$. */
bool proj_mul(const Affine2& y, Affine2& x1, Affine2& x2);
bool proj_mul(const Affine2& y, Interval& x1, Affine2& x2);
bool proj_mul(const Affine2& y, Affine2& x1, Interval& x2);
bool proj_mul(const Affine2& y, Interval& x1, Interval& x2);
bool proj_mul(const Interval& y, Affine2& x1, Affine2& x2);
bool proj_mul(const Interval& y, Interval& x1, Affine2& x2);
bool proj_mul(const Interval& y, Affine2& x1, Interval& x2);


/** \brief Projection of $y=x_1/x_2$.
 *
 * Set $([x]_1,[x]_2)$ to $([x]_1,[x]_2])\cap\{ (x_1,x_2)\in [x]_1\times[x]_2 \ | \ \exists y\in[y],\ y=x_1/x_2\}$. */
bool proj_div(const Affine2& y, Affine2& x1, Affine2& x2);
bool proj_div(const Affine2& y, Interval& x1, Affine2& x2);
bool proj_div(const Affine2& y, Affine2& x1, Interval& x2);
bool proj_div(const Affine2& y, Interval& x1, Interval& x2);
bool proj_div(const Interval& y, Affine2& x1, Affine2& x2);
bool proj_div(const Interval& y, Interval& x1, Affine2& x2);
bool proj_div(const Interval& y, Affine2& x1, Interval& x2);

/** \brief Projection of $y=x^2$.
 *
 * Set $[x]$ to $[x]\cap { x\in [x] \exists y\in [y], \quad y=x^2 \}$. */
bool proj_sqr(const Affine2& y, Affine2& x);
bool proj_sqr(const Interval& y, Affine2& x);
bool proj_sqr(const Affine2& y, Interval& x);

/** \brief Projection of $y=\sqrt{x}$.
 *
 * Set $[x]$ to $[x]\cap { x\in [x] \exists y\in [y], \quad y=\sqrt{x} \}$. */
bool proj_sqrt(const Affine2& y, Affine2& x);
bool proj_sqrt(const Interval& y, Affine2& x);
bool proj_sqrt(const Affine2& y, Interval& x);

/** \brief Projection of $y=x^n$.
 *
 * Set $[x]$ to $[x]\cap { x\in [x] \exists y\in [y], \quad y=x^n \}$. */
bool proj_pow(const Affine2& y, int n, Affine2& x);
bool proj_pow(const Interval& y,int n, Affine2& x);
bool proj_pow(const Affine2& y, int n, Interval& x);

/** \brief Projection of $y=x_1^{x_2}$.
 *
 * Set $([x]_1,[x]_2)$ to $([x]_1,[x]_2])\cap\{ (x_1,x_2)\in [x]_1\times[x]_2 \ | \ \exists y\in[y],\ y=x_1^{x_2}\}$. */
bool proj_pow(const Affine2& y, Affine2& x1, Affine2& x2);
bool proj_pow(const Affine2& y, Interval& x1, Affine2& x2);
bool proj_pow(const Affine2& y, Affine2& x1, Interval& x2);
bool proj_pow(const Affine2& y, Interval& x1, Interval& x2);
bool proj_pow(const Interval& y, Affine2& x1, Affine2& x2);
bool proj_pow(const Interval& y, Interval& x1, Affine2& x2);
bool proj_pow(const Interval& y, Affine2& x1, Interval& x2);

/** \brief Projection of the $y=x^{\frac{1}{n}}$.
 *
 * Set $[x]$ to $[x]\cap { x\in [x] \exists y\in [y], \quad y=x^{\frac{1}{n}} \}$. */
bool proj_root(const Affine2& y, int n, Affine2& x);
bool proj_root(const Interval& y,int n, Affine2& x);
bool proj_root(const Affine2& y, int n, Interval& x);

/** \brief Projection of $y=\exp(x)$.
 *
 * Set $[x]$ to $[x]\cap { x\in [x] \exists y\in [y], \quad y=\exp(x) \}$. */
bool proj_exp(const Affine2& y, Affine2& x);
bool proj_exp(const Interval& y, Affine2& x);
bool proj_exp(const Affine2& y, Interval& x);

/** \brief Projection of $y=\log(x)$.
 *
 * Set $[x]$ to $[x]\cap { x\in [x] \exists y\in [y], \quad y=\log(x) \}$. */
bool proj_log(const Affine2& y, Affine2& x);
bool proj_log(const Interval& y, Affine2& x);
bool proj_log(const Affine2& y, Interval& x);

/** \brief Projection of $y=\cos(x)$.
 *
 * Set $[x]$ to $[x]\cap { x\in [x] \exists y\in [y], \quad y=\cos(x) \}$. */
bool proj_cos(const Affine2& y, Affine2& x);
bool proj_cos(const Interval& y, Affine2& x);
bool proj_cos(const Affine2& y, Interval& x);

/** \brief Projection of $y=\sin(x)$.
 *
 * Set $[x]$ to $[x]\cap { x\in [x] \exists y\in [y], \quad y=\sin(x) \}$. */
bool proj_sin(const Affine2& y, Affine2& x);
bool proj_sin(const Interval& y, Affine2& x);
bool proj_sin(const Affine2& y, Interval& x);

/** \brief Projection of $y=\tan(x)$.
 *
 * Set $[x]$ to $[x]\cap { x\in [x] \exists y\in [y], \quad y=\tan(x) \}$. */
bool proj_tan(const Affine2& y, Affine2& x);
bool proj_tan(const Interval& y, Affine2& x);
bool proj_tan(const Affine2& y, Interval& x);

/** \brief Projection of $y=\acos(x)$.
 *
 * Set $[x]$ to $[x]\cap { x\in [x] \exists y\in [y], \quad y=\acos(x) \}$. */
bool proj_acos(const Affine2& y, Affine2& x);
bool proj_acos(const Interval& y, Affine2& x);
bool proj_acos(const Affine2& y, Interval& x);

/** \brief Projection of $y=\asin(x)$.
 *
 * Set $[x]$ to $[x]\cap { x\in [x] \exists y\in [y], \quad y=\asin(x) \}$. */
bool proj_asin(const Affine2& y, Affine2& x);
bool proj_asin(const Interval& y, Affine2& x);
bool proj_asin(const Affine2& y, Interval& x);

/** \brief Projection of $y=\atan(x)$.
 *
 * Set $[x]$ to $[x]\cap { x\in [x] \exists y\in [y], \quad y=\atan(x) \}$. */
bool proj_atan(const Affine2& y, Affine2& x);
bool proj_atan(const Interval& y, Affine2& x);
bool proj_atan(const Affine2& y, Interval& x);

/** \brief Projection of $y=\cosh(x)$.
 *
 * Set $[x]$ to $[x]\cap { x\in [x] \exists y\in [y], \quad y=\cosh(x) \}$. */
bool proj_cosh(const Affine2& y, Affine2& x);
bool proj_cosh(const Interval& y, Affine2& x);
bool proj_cosh(const Affine2& y, Interval& x);

/** \brief Projection of $y=\sinh(x)$.
 *
 * Set $[x]$ to $[x]\cap { x\in [x] \exists y\in [y], \quad y=\sinh(x) \}$. */
bool proj_sinh(const Affine2& y, Affine2& x);
bool proj_sinh(const Interval& y, Affine2& x);
bool proj_sinh(const Affine2& y, Interval& x);

/** \brief Projection of $y=\tanh(x)$.
 *
 * Set $[x]$ to $[x]\cap { x\in [x] \exists y\in [y], \quad y=\tanh(x) \}$. */
bool proj_tanh(const Affine2& y, Affine2& x);
bool proj_tanh(const Interval& y, Affine2& x);
bool proj_tanh(const Affine2& y, Interval& x);

/** \brief Projection of $y=|x|$.
 *
 * Set $[x]$ to $[x]\cap { x\in [x] \exists y\in [y], \quad y=|x| \}$. */
bool proj_abs(const Affine2& y, Affine2& x);
bool proj_abs(const Interval& y, Affine2& x);
bool proj_abs(const Affine2& y, Interval& x);

/** \brief Projection of $y=sign(x)$.
 *
 * Set $[x]$ to $[x]\cap { x\in [x] \exists y\in [y], \quad y=sign(x) \}$. */
bool proj_sign(const Affine2& y, Affine2& x);
bool proj_sign(const Interval& y, Affine2& x);
bool proj_sign(const Affine2& y, Interval& x);

/** \brief Contract x w.r.t. the fact that it must be integral.
 *
 */
bool proj_integer(Affine2& x);



}

/*@}*/

/*============================================ inline implementation ============================================ */



namespace ibex {

inline Affine2::~Affine2() {
	delete[] _val;
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


/** \brief True iff *this and x are exactly the same intervals. */
inline bool Affine2::operator==(const Affine2& x) const{
	return (_itv == x.itv());
}

/** \brief True iff *this and x are exactly the same intervals. */
inline bool Affine2::operator==(const Interval& x) const{
	return (_itv == x);
}

/** \brief True iff *this and x are not exactly the same intervals. */
inline bool Affine2::operator!=(const Affine2& x) const{
	return (_itv == x.itv());
}

/** \brief True iff *this and x are not exactly the same intervals. */
inline bool Affine2::operator!=(const Interval& x) const {
	return (_itv != x);
}

/** \brief Set this interval to the empty set. */
inline void Affine2::set_empty(){
	_itv.set_empty();
	_actif = false;
}

inline Affine2& Affine2::inflate(double radd){
	if (_actif) _err += radd;
	_itv += Interval(-radd, radd);
	return *this;
}

/** \brief number of variable represented */
inline int Affine2::size() const{
	return _n;
}

/** \brief Range of the affine form */
inline Interval Affine2::itv() const {
	return _itv;
}
inline Interval& Affine2::ITV() {
	return *_itv;
}



/** \brief return _val*/
inline double * Affine2::val() const{
	return _val;
}

/** \brief return _val[i] */
inline double Affine2::val(int i) const{
	assert(i>=0 && i<=_n);
	return _val[i];
}

/** \brief return _err*/
inline Interval Affine2::err() const{
	return _err;
}

/** \brief return 1 if the affine form is actif and valid*/
inline bool Affine2::isActif() const{
	return _actif;
}

/** \brief return 1 if the affine form is actif and valid*/
inline void Affine2::setActif(bool b){
	_actif = b;
}

/** \brief Lower bound.
 *
 * Return the lower bound of *this. */
inline double Affine2::lb() const{
	return _itv.lb();
}

/** \brief Upper bound.
 *
 * Return the upper bound of *this. */
inline double Affine2::ub() const{
	return _itv.ub();
}

/** \brief Midpoint. */
inline double Affine2::mid() const{
	return _itv.mid();
}

/** \brief Radius.*/
inline double Affine2::rad() const{
	return _itv.rad();
}

/** \brief Diameter.*/
inline double Affine2::diam() const{
	return _itv.diam();
}

/**
 * \brief Mignitude. */
inline double Affine2::mig() const{
	return _itv.mig();
}

/** \brief Magnitude. */
inline double Affine2::mag() const{
	return _itv.mag();
}


inline bool Affine2::is_subset(const Affine2& x) const{
	return (_itv.is_subset(x.itv()));
}

inline bool Affine2::is_subset(const Interval& x) const{
	return (_itv.is_subset(x));
}


inline bool Affine2::is_strict_subset(const Affine2& x) const{
	return (_itv.is_strict_subset(x.itv()));
}

inline bool Affine2::is_strict_subset(const Interval& x) const{
	return (_itv.is_strict_subset(x));
}

inline bool Affine2::is_superset(const Affine2& x) const{
	return (_itv.is_superset(x.itv()));
}


inline bool Affine2::is_superset(const Interval& x) const{
	return (_itv.is_superset(x));
}


inline bool Affine2::is_strict_superset(const Affine2& x) const{
	return (_itv.is_strict_superset(x.itv()));
}

inline bool Affine2::is_strict_superset(const Interval& x) const{
	return (_itv.is_strict_superset(x));
}


inline bool Affine2::contains(double d) const{
	return (_itv.contains(d));
}


inline bool Affine2::strictly_contains(double d) const{
	return (_itv.strictly_contains(d));
}


inline bool Affine2::is_disjoint(const Affine2 &x) const{
	return (_itv.is_disjoint(x.itv()));
}


inline bool Affine2::is_disjoint(const Interval &x) const{
	return (_itv.is_disjoint(x));
}

inline bool Affine2::is_empty() const{
	return (_itv.is_empty());
}


inline bool Affine2::is_degenerated() const {
	return (_itv.is_degenerated());
}

inline bool Affine2::is_unbounded() const{
	return (_itv.is_unbounded());
}

inline bool Affine2::is_bisectable() const {
	return (_itv.is_bisectable());
}

inline double Affine2::rel_distance(const Affine2& x) const{
	return (_itv.rel_distance(x.itv()));
}

inline double Affine2::rel_distance(const Interval& x) const{
	return (_itv.rel_distance(x));
}

/** \brief Add \a d to *this and return the result.  */
inline Affine2& Affine2::operator+=(double d){
	return saxpy(1.0, Affine2(0), d, 0.0, false, false, true, false);
}

/** \brief Subtract \a d to *this and return the result. */
inline Affine2& Affine2::operator-=(double d){
	return saxpy(1.0, Affine2(0), (-d), 0.0, false, false, true, false);
}

/** \brief Multiply *this by \a d and return the result. */
inline Affine2& Affine2::operator*=(double d){
	return saxpy(d, Affine2(0), 0.0, 0.0, true, false, false, false);
}

/** \brief Divide *this by \a d and return the result. */
inline 	Affine2& Affine2::operator/=(double d) {
	return *this *= Affine2(_n, 1.0 / Interval(d));
}

/** \brief Add \a x to *this and return the result. */
inline Affine2& Affine2::operator+=(const Interval& x){
	return saxpy(1.0, Affine2(0), x.mid(), x.rad(), false, false, true, true);
}

/** \brief Subtract \a x to *this and return the result. */
inline Affine2& Affine2::operator-=(const Interval& x){
	return *this += (-x);
}

/** \brief Multiply *this by \a x and return the result. */
inline Affine2& Affine2::operator*=(const Interval& x){
	return *this *= Affine2(_n, x);
}

/** \brief Divide *this by \a x and return the result.*/
inline Affine2& Affine2::operator/=(const Interval& x){
	return *this *= Affine2(_n, 1.0 / x);
}

/** \brief Add \a x to *this and return the result. */
inline Affine2& Affine2::operator+=(const Affine2& x){
	return saxpy(1.0, x, 0.0, 0.0, false, true, false, false);
}

/** \brief Subtract \a x to *this and return the result. */
inline Affine2& Affine2::operator-=(const Affine2& x){
	return *this += Affine2(x,true);
}

/** \brief Divide *this by \a x and return the result. */
inline Affine2& Affine2::operator/=(const Affine2& x){
	return *this *= (Affine2(x).linChebyshev(Affine2::AF_INV));
}


inline double Affine2::delta(const Affine2& x) const{
	return _itv.delta(x.itv());
}


inline double Affine2::delta(const Interval& x) const{
	return _itv.delta(x);
}


inline double Affine2::ratiodelta(const Affine2& x) const {
	return _itv.ratiodelta(x.itv());
}


inline double Affine2::ratiodelta(const Interval& x) const {
	return _itv.ratiodelta(x);
}

inline std::pair<Interval,Interval> Affine2::bisect(double ratio) const{
	return _itv.bisect(ratio);
}
inline std::pair<Interval,Interval> Affine2::bisect() const{
	return _itv.bisect(0.5);
}

/** \brief Return -x. */
inline Affine2 operator-(const Affine2& x){
	return Affine2(x, true); // y = -x
}

/** \brief $AF[x]_1+AF[x]_2$. */
inline Affine2 operator+(const Affine2& x1, const Affine2& x2){
	return Affine2(x1) += x2;
}

/** \brief $AF[x]+d$. */
inline Affine2 operator+(const Affine2& x, double d){
	return Affine2(x) += d;
}

/** \brief $d+AF[x]$. */
inline Affine2 operator+(double d, const Affine2& x){
	return Affine2(x) += d;
}

/** \brief $AF[x]_1+[x]_2$. */
inline Affine2 operator+(const Affine2& x1, const Interval& x2){
	return Affine2(x1) += x2;
}

/** \brief $[x]_1+AF[x]_2$. */
inline Affine2 operator+(const Interval& x1, const Affine2& x2){
	return Affine2(x2) += x1;
}

/** \brief $AF[x]_1-AF[x]_2$. */
inline Affine2 operator-(const Affine2& x1, const Affine2& x2){
	return Affine2(x2, true) += x1;
}

/** \brief $AF[x]-d$. */
inline Affine2 operator-(const Affine2& x, double d){
	return Affine2(x) -= d;
}

/** \brief $d-AF[x]$. */
inline Affine2 operator-(double d, const Affine2& x){
	return Affine2(x, true) += d;
}

/** \brief $AF[x]_1-[x]_2$. */
inline Affine2 operator-(const Affine2& x1, const Interval& x2) {
	return Affine2(x1) -= x2;
}

/** \brief $[x]_1-AF[x]_2$. */
inline Affine2 operator-(const Interval& x1, const Affine2& x2) {
	return Affine2(x2, true) += x1;
}

/** \brief $AF[x]_1*AF[x]_2$. */
inline Affine2 operator*(const Affine2& x1, const Affine2& x2) {
	return Affine2(x1) *= x2;
}

/** \brief $AF[x]*d$. */
inline Affine2 operator*(const Affine2& x, double d){
	return Affine2(x) *= d;
}

/** \brief $d*AF[x]$. */
inline Affine2 operator*(double d, const Affine2& x){
	return Affine2(x) *= d;
}

/** \brief $AF[x]_1*[x]_2$. */
inline Affine2 operator*(const Affine2& x1, const Interval& x2){
	return Affine2(x1) *= x2;
}

/** \brief $[x]_1*AF[x]_2$. */
inline Affine2 operator*(const Interval& x1, const Affine2& x2){
	return Affine2(x2) *= x1;
}

/** \brief $AF[x]_1/AF[x]_2$. */
inline Affine2 operator/(const Affine2& x1, const Affine2& x2){
	return Affine2(x1) /= x2;
}

/** \brief $AF[x]/d$. */
inline Affine2 operator/(const Affine2& x, double d){
	return Affine2(x) /= d;
}

/** \brief $d/AF[x]$. */
inline Affine2 operator/(double d, const Affine2& x){
	return Affine2(x.size(), d) *= (Affine2(x).linChebyshev(Affine2::AF_INV));
}

/** \brief $AF[x]_1/[x]_2$. */
inline Affine2 operator/(const Affine2& x1, const Interval& x2){
	return Affine2(x1) /= x2;
}

/** \brief $[x]_1/AF[x]_2$. */
inline Affine2 operator/(const Interval& x1, const Affine2& x2){
	return Affine2(x2.size(), x1) *= (Affine2(x2).linChebyshev(Affine2::AF_INV));
}

/** \brief Hausdorff distance of $AF[x]_1$ and $AF[x]_2$. */
inline double distance(const Affine2 &x1, const Affine2 &x2){
	return distance(x1.itv(), x2.itv());
}

/** \brief Hausdorff distance of $[x]_1$ and $AF[x]_2$. */
inline double distance(const Interval &x1, const Affine2 &x2){
	return distance(x1, x2.itv());
}

/** \brief Hausdorff distance of $AF[x]_1$ and $[x]_2$. */
inline double distance(const Affine2 &x1, const Interval &x2){
	return distance(x1.itv(), x2);
}

/** \brief $1/AF[x]$ */
inline Affine2 inv(const Affine2& x){
	return Affine2(x).linChebyshev(Affine2::AF_INV);
}

/** \brief $AF[x]^2$ */
inline Affine2 sqr(const Affine2& x){
	return Affine2(x).sqr();
}

/** \brief $\sqrt{AF[x]}$ */
inline Affine2 sqrt(const Affine2& x){
	return Affine2(x).linChebyshev(Affine2::AF_SQRT);
}

/** \brief $\exp(AF[x])$. */
inline Affine2 exp(const Affine2& x){
	return Affine2(x).linChebyshev(Affine2::AF_EXP);
}

/** \brief $\log(AF[x])$. */
inline Affine2 log(const Affine2& x){
	return Affine2(x).linChebyshev(Affine2::AF_LOG);
}
/** \brief $AF[x]^n$. */
Affine2 pow(const Affine2& x, int n);

/** \brief $AF[x]^d$. */
Affine2 pow(const Affine2& x, double d);

/** \brief $AF[x]^[y]$. */
inline Affine2 pow(const Affine2 &x, const Interval &y){
	// return exp(y * log(x));
	return (y * Affine2(x).linChebyshev(Affine2::AF_LOG)).linChebyshev(Affine2::AF_EXP);
}

/** \brief $AF[x]^AF[y]$. */
inline Affine2 pow(const Affine2 &x, const Affine2 &y){
	return pow(x, y.itv());
}

/** \brief $n^{th}$ root of $AF[x]$. */
Affine2 root(const Affine2& x, int n);

/** \brief $\cos(AF[x])$. */
inline Affine2 cos(const Affine2& x){
	return Affine2(x).linChebyshev(Affine2::AF_COS);
}

/** \brief $\sin(AF[x])$. */
inline Affine2 sin(const Affine2& x){
	return Affine2(x).linChebyshev(Affine2::AF_SIN);
}

/** \brief $\tan(AF[x])$. */
inline Affine2 tan(const Affine2& x){
	return Affine2(x).linChebyshev(Affine2::AF_TAN);
}

/** \brief $\acos(AF[x])$. */
inline Affine2 acos(const Affine2& x){
	return Affine2(x).linChebyshev(Affine2::AF_ACOS);
}

/** \brief $\asin(AF[x])$. */
inline Affine2 asin(const Affine2& x){
	return Affine2(x).linChebyshev(Affine2::AF_ASIN);
}

/** \brief $\atan(AF[x])$. */
inline Affine2 atan(const Affine2& x){
	return Affine2(x).linChebyshev(Affine2::AF_ATAN);
}

inline Affine2 cosh(const Affine2& x){
	return Affine2(x).linChebyshev(Affine2::AF_COSH);
}

/** \brief $\sinh(AF[x])$. */
inline Affine2 sinh(const Affine2& x){
	return Affine2(x).linChebyshev(Affine2::AF_SINH);
}

/** \brief $\tanh(AF[x])$. */
inline Affine2 tanh(const Affine2& x){
	return Affine2(x).linChebyshev(Affine2::AF_TANH);
}

/** \brief $\abs(AF[x]) = sqrt(sqr(AF[x]))$. */
inline Affine2 abs(const Affine2 &x){
	return Affine2(x).linChebyshev(Affine2::AF_ABS);
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
	// TODO not yet implemented
	return Affine2(x);
}

inline bool proj_add(const Affine2& y, Affine2& x1, Affine2& x2) {
	return proj_add(y.itv(), x1, x2);
}
inline bool proj_add(const Affine2& y, Interval& x1, Affine2& x2) {
	return proj_add(y.itv(), x2, x1);
}
inline bool proj_add(const Affine2& y, Affine2& x1, Interval& x2) {
	return proj_add(y.itv(), x1, x2);
}
inline bool proj_add(const Interval& y, Interval& x1, Affine2& x2) {
	return proj_add(y, x2, x1);
}
inline bool proj_add(const Affine2& y, Interval& x1, Interval& x2) {
	return proj_add(y.itv(), x1, x2);
}

inline bool proj_sub(const Affine2& y, Affine2& x1, Affine2& x2) {
	return proj_sub(y.itv(), x1, x2);
}
inline bool proj_sub(const Affine2& y, Interval& x1, Affine2& x2) {
	return proj_sub(y.itv(), x1, x2);
}
inline bool proj_sub(const Affine2& y, Affine2& x1, Interval& x2) {
	return proj_sub(y.itv(), x1, x2);
}
inline bool proj_sub(const Affine2& y, Interval& x1, Interval& x2) {
	return proj_sub(y.itv(), x1, x2);
}

inline bool proj_mul(const Affine2& y, Affine2& x1, Affine2& x2) {
	return proj_mul(y.itv(), x1, x2);
}
inline bool proj_mul(const Affine2& y, Interval& x1, Affine2& x2) {
	return proj_mul(y.itv(), x2, x1);
}
inline bool proj_mul(const Affine2& y, Affine2& x1, Interval& x2) {
	return proj_mul(y.itv(), x1, x2);
}
inline bool proj_mul(const Interval& y, Interval& x1, Affine2& x2) {
	return proj_mul(y, x2, x1);
}
inline bool proj_mul(const Affine2& y, Interval& x1, Interval& x2) {
	return proj_mul(y.itv(), x1, x2);
}


inline bool proj_div(const Affine2& y, Affine2& x1, Affine2& x2) {
	return proj_div(y.itv(), x1, x2);
}
inline bool proj_div(const Affine2& y, Interval& x1, Affine2& x2) {
	return proj_div(y.itv(), x1, x2);
}
inline bool proj_div(const Affine2& y, Affine2& x1, Interval& x2) {
	return proj_div(y.itv(), x1, x2);
}
inline bool proj_div(const Affine2& y, Interval& x1, Interval& x2) {
	return proj_div(y.itv(), x1, x2);
}


inline bool proj_sqr(const Affine2& y, Affine2& x){
	return proj_sqr(y.itv(),x);
}
inline bool proj_sqr(const Affine2& y, Interval& x){
	return proj_sqr(y.itv(),x);
}

inline bool proj_pow(const Affine2& y,int n, Affine2& x){
	return proj_pow(y.itv(),n,x);
}
inline bool proj_pow(const Affine2& y,int n, Interval& x){
	return proj_pow(y.itv(),n, x);
}

/** \brief Projection of $y=x_1^{x_2}$.
 *
 * Set $([x]_1,[x]_2)$ to $([x]_1,[x]_2])\cap\{ (x_1,x_2)\in [x]_1\times[x]_2 \ | \ \exists y\in[y],\ y=x_1^{x_2}\}$. */
bool proj_pow(const Affine2& y, Affine2& x1, Affine2& x2){
	return proj_pow(y.itv(), x1, x2);
}
bool proj_pow(const Affine2& y, Interval& x1, Affine2& x2){
	return proj_pow(y.itv(), x1, x2);
}
bool proj_pow(const Affine2& y, Affine2& x1, Interval& x2){
	return proj_pow(y.itv(), x1, x2);
}
bool proj_pow(const Affine2& y, Interval& x1, Interval& x2){
	return proj_pow(y.itv(), x1, x2);
}

/** \brief Projection of the $y=x^{\frac{1}{n}}$.
 *
 * Set $[x]$ to $[x]\cap { x\in [x] \exists y\in [y], \quad y=x^{\frac{1}{n}} \}$. */
bool proj_root(const Affine2& y, int n, Affine2& x){
	return proj_pow(y.itv(),n, x);
}
bool proj_root(const Affine2& y, int n, Interval& x){
	return proj_pow(y.itv(),n, x);
}


inline bool proj_sqrt(const Affine2& y, Affine2& x){
	return proj_sqrt(y.itv(),x);
}
inline bool proj_sqrt(const Affine2& y, Interval& x){
	return proj_sqrt(y.itv(),x);
}
inline bool proj_exp(const Affine2& y, Affine2& x){
	return proj_exp(y.itv(),x);
}
inline bool proj_exp(const Affine2& y, Interval& x){
	return proj_exp(y.itv(),x);
}

inline bool proj_log(const Affine2& y, Affine2& x){
	return proj_log(y.itv(),x);
}
inline bool proj_log(const Affine2& y, Interval& x){
	return proj_log(y.itv(),x);
}

inline bool proj_cos(const Affine2& y, Affine2& x){
	return proj_cos(y.itv(),x);
}
inline bool proj_cos(const Affine2& y, Interval& x){
	return proj_cos(y.itv(),x);
}

inline bool proj_sin(const Affine2& y, Affine2& x){
	return proj_sin(y.itv(),x);
}
inline bool proj_sin(const Affine2& y, Interval& x){
	return proj_sin(y.itv(),x);
}

inline bool proj_tan(const Affine2& y, Affine2& x){
	return proj_tan(y.itv(),x);
}
inline bool proj_tan(const Affine2& y, Interval& x){
	return proj_tan(y.itv(),x);
}

inline bool proj_acos(const Affine2& y, Affine2& x){
	return proj_acos(y.itv(),x);
}
inline bool proj_acos(const Affine2& y, Interval& x){
	return proj_acos(y.itv(),x);
}

inline bool proj_asin(const Affine2& y, Affine2& x){
	return proj_asin(y.itv(),x);
}
inline bool proj_asin(const Affine2& y, Interval& x){
	return proj_asin(y.itv(),x);
}

inline bool proj_atan(const Affine2& y, Affine2& x){
	return proj_atan(y.itv(),x);
}
inline bool proj_atan(const Affine2& y, Interval& x){
	return proj_atan(y.itv(),x);
}

inline bool proj_cosh(const Affine2& y, Affine2& x){
	return proj_cosh(y.itv(),x);
}
inline bool proj_cosh(const Affine2& y, Interval& x){
	return proj_cosh(y.itv(),x);
}

inline bool proj_sinh(const Affine2& y, Affine2& x){
	return proj_sinh(y.itv(),x);
}
inline bool proj_sinh(const Affine2& y, Interval& x){
	return proj_sinh(y.itv(),x);
}

inline bool proj_tanh(const Affine2& y, Affine2& x){
	return proj_tanh(y.itv(),x);
}
inline bool proj_tanh(const Affine2& y, Interval& x){
	return proj_tanh(y.itv(),x);
}

inline bool proj_abs(const Affine2& y, Affine2& x){
	return proj_abs(y.itv(),x);
}
inline bool proj_abs(const Affine2& y, Interval& x){
	return proj_abs(y.itv(),x);
}

inline bool proj_sign(const Affine2& y, Affine2& x){
	return proj_sign(y.itv(),x);
}
inline bool proj_sign(const Affine2& y, Interval& x){
	return proj_sign(y.itv(),x);
}

} // end namespace ibex

#endif /* IBEX_AFFINE2_H_ */

// TODO to finish the projections and other functions

/** \brief $\atan2(AF[x],AF[y])$. */
//Affine2 atan2(const Affine2& x, const Affine2& y);
/** \brief $\atan2([x],AF[y])$. */
//Affine2 atan2(const Interval& x, const Affine2& y);
/** \brief $\atan2(AF[x],[y])$. */
//Affine2 atan2(const Affine2& x, const Interval& y);
/** \brief $\cosh(AF[x])$. */
