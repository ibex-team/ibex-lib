//============================================================================
//                                  I B E X                                   
// File        : ibex_InnerProj.h
// Author      : Gilles Chabert, Ignacio Araya
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jul 2, 2012
// Last Update : Jul 2, 2012
//============================================================================


#ifndef __IBEX_INNER_ARITH_H__
#define __IBEX_INNER_ARITH_H__

#include "ibex_Interval.h"

namespace ibex {

/**\ingroup arithmetic */
/*@{*/

/**
 * \brief Inner image of the addition.
 *
 * Return z such that z ⊆ (x+y).
 */
Interval iadd(const Interval& x, const Interval& y);

/**
 * \brief Inner image of the subtraction.
 *
 * Return z such that z ⊆ (x-y).
 */
Interval isub(const Interval& x, const Interval& y);

/**
 * \brief Inner image of the multiplication.
 *
 * Return z such that z ⊆ (x*y).
 */
Interval imul(const Interval& x, const Interval& y);

/**
 * \brief Inner image of the division.
 *
 * Return z such that z ⊆ (x-y).
 */
Interval idiv(const Interval& x, const Interval& y);

/**
 * \brief Inner image of the maximum.
 *
 * Return z such that z ⊆ max(x,y).
 */
Interval imax(const Interval& x, const Interval& y);

/**
 * \brief Inner image of the minimum.
 *
 * Return z such that z ⊆ min(x,y).
 */
Interval imin(const Interval& x, const Interval& y);

/**
 * \brief Inner image of sqr.
 *
 * Return y such that y ⊆ x^2.
 */
Interval isqr(const Interval& x);

/**
 * \brief Inner image of minus.
 *
 * Return y such that y ⊆ -x.
 */
Interval iminus(const Interval& x);

/**
 * \brief Inner image of log.
 *
 * Return y such that y ⊆ log(x).
 */
Interval ilog(const Interval& x);

/**
 * \brief Inner image of exp.
 *
 * Return y such that y ⊆ exp(x).
 */
Interval iexp(const Interval& x);

/**
 * \brief Inner image of acos.
 *
 * Return y such that y ⊆ acos(x).
 */
Interval iacos(const Interval& x);

/**
 * \brief Inner image of asin.
 *
 * Return y such that y ⊆ asin(x).
 */
Interval iasin(const Interval& x);

/**
 * \brief Inner image of atan.
 *
 * Return y such that  y ⊆ atan(x).
 */
Interval iatan(const Interval& x);

/**
 * TODO: to be continued...
 */

/*=====================================================================================================*/
/*                                       INNER PROJECTIONS                                             */
/*=====================================================================================================*/

/**
 * \brief Inner projection of the addition (inflating version)
 *
 * Inner projections for binary operators act the following way.<br>
 *
 * They contract x (resp. y) to x' (resp. y') such that:
 * <ul>
 * <li> (xin,yin) ⊆ (x',y') (only relevant in inflating mode)
 * <li> x' op y'⊆ z
 * </ul>
 * where op is the operator (either,+,-,* or /)
 *
 * By default xin=&empty; and yin=&empty; (non-inflating mode).
 *
 * \return true iff the result is non empty (always return true in inflating mode)
 *
 * \pre xin=&empty; iff yin=&empty;
 * \pre (xin,yin) ⊆ (x,y)
 * \pre xin op yin ⊆ z
 */
bool ibwd_add(const Interval& z, Interval& x, Interval& y, const Interval &xin=Interval::empty_set(), const Interval& yin=Interval::empty_set());

/**
 * \brief Inner projection of the subtraction (inflating version)
 *
 * \see comments in \link ibex::ibwd_add(const Interval& z, Interval& x, Interval& y, const Interval& xin, const Interval& yin) ibwd_add \endlink
 */
bool ibwd_sub(const Interval& z, Interval& x, Interval& y, const Interval &xin=Interval::empty_set(), const Interval& yin=Interval::empty_set());

/**
 * \brief Inner projection of the multiplication.
 *
 * \see comments in #ibex::ibwd_add(const Interval&, Interval&, Interval&, const Interval&, const Interval&).
 */
bool ibwd_mul(const Interval& z, Interval& x, Interval& y, const Interval &xin=Interval::empty_set(), const Interval& yin=Interval::empty_set());

/**
 * \brief Inner projection of the division (inflating version)
 *
 * \see comments in #ibex::ibwd_add(const Interval&, Interval&, Interval&, const Interval, const Interval&).
 */
bool ibwd_div(const Interval& z, Interval& x, Interval& y, const Interval &xin=Interval::empty_set(), const Interval& yin=Interval::empty_set());

/**
 * \brief Inner projection of the division (inflating version)
 *
 * \see comments in #ibex::ibwd_add(const Interval&, Interval&, Interval&, const Interval, const Interval&).
 */
bool ibwd_max(const Interval& z, Interval& x, Interval& y, const Interval &xin=Interval::empty_set(), const Interval& yin=Interval::empty_set());

/**
 * \brief Inner projection of the division (inflating version)
 *
 * \see comments in #ibex::ibwd_add(const Interval&, Interval&, Interval&, const Interval, const Interval&).
 */
bool ibwd_min(const Interval& z, Interval& x, Interval& y, const Interval &xin=Interval::empty_set(), const Interval& yin=Interval::empty_set());

/**
 * \brief Inner projection of the square function (inflating version)
 *
 * Inner projections for unary operators "f" act the following way.<br>
 *
 * They contract x to x' such that
 * <ul>
 * <li> xin ⊆ x' (only relevant in inflating mode)
 * <li> f(x') ⊆ y
 * </ul>
 * where op is the operator (either,+,-,* or /)
 *
 * By default xin=&empty; (non-inflating mode).
 *
 * \return true iff the result is non empty (always return true in inflating mode)
 *
 * \pre xin ⊆ x
 * \pre f(xin) ⊆ y
 */
bool ibwd_sqr(const Interval& y, Interval& x, const Interval& xin=Interval::empty_set());

/**
 * \brief Inner projection of the minus operator.
 *
 * \see comments in #ibex::ibwd_sqr(const Interval&, Interval&, const Interval&).
 *
 */
bool ibwd_minus(const Interval& y, Interval& x);

/**
 * \brief Inner projection of the absolute value.
 *
 * \see comments in #ibex::ibwd_sqr(const Interval&, Interval&, const Interval&).
 *
 */
bool ibwd_abs(const Interval& y, Interval& x, const Interval& xin=Interval::empty_set());

/**
 * \brief Inner projection of power
 *
 * \see comments in #ibex::ibwd_sqr(const Interval&, Interval&, const Interval&).
 */
bool ibwd_pow(const Interval& y, Interval& x, int p, const Interval &xin=Interval::empty_set());

/**
 * \brief Inner projection of exp
 *
 * \see comments in #ibex::ibwd_sqr(const Interval&, Interval&, const Interval&).
 */
bool ibwd_exp(const Interval& y, Interval& x);

/**
 * \brief Inner projection of log
 *
 * \see comments in #ibex::ibwd_sqr(const Interval&, Interval&, const Interval&).
 */
bool ibwd_log(const Interval& y, Interval& x);


/**
 * \brief Inner projection of cosine
 *
 * \see comments in #ibex::ibwd_sqr(const Interval&, Interval&, const Interval&).
 */
bool ibwd_cos(const Interval& y, Interval& x, const Interval &xin=Interval::empty_set());


/**
 * \brief Inner projection of sine
 *
 * \see comments in #ibex::ibwd_sqr(const Interval&, Interval&, const Interval&).
 */
bool ibwd_sin(const Interval& y, Interval& x, const Interval &xin=Interval::empty_set());


/**
 * \brief Inner projection of tan
 *
 * \see comments in #ibex::ibwd_sqr(const Interval&, Interval&, const Interval&).
 */
bool ibwd_tan(const Interval& y, Interval& x, const Interval &xin=Interval::empty_set());

/**
 * \brief Inner projection of square root
 *
 * \see comments in #ibex::ibwd_sqr(const Interval&, Interval&, const Interval&).
 */
bool ibwd_sqrt(const Interval& y, Interval& x);

/*
 * TODO: to be continued....
 */

/*@}*/

/*================================== inline implementations ========================================*/

inline Interval iminus(const Interval& x) {
	return -x;
}


inline Interval imax(const Interval& x, const Interval& y) {
// there should not be rounding with max
	return max(x,y);
}

inline Interval imin(const Interval& x, const Interval& y) {
	// there should not be rounding with min
	return min(x,y);
}

inline bool ibwd_minus(const Interval& y, Interval& x) {
	return !(x&=iminus(y)).is_empty();
}

inline bool ibwd_exp(const Interval& y, Interval& x) {
	return !(x&=ilog(y)).is_empty();
}

inline bool ibwd_log(const Interval& y, Interval& x) {
	return !(x&=iexp(y)).is_empty();
}

inline bool ibwd_sqrt(const Interval& y, Interval& x) {
	return !(x&=isqr(y & Interval::pos_reals())).is_empty();
}

} // end namespace ibex

#endif // __IBEX_INNER_ARITH_H__
