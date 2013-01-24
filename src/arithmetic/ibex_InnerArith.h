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
 * Return z such that z ⊆ (x-y).
 */
Interval imul(const Interval& x, const Interval& y);

/**
 * \brief Inner image of the division.
 *
 * Return z such that z ⊆ (x-y).
 */
Interval idiv(const Interval& x, const Interval& y);

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
 * By default xin={} and yin={} (non-inflating mode).
 *
 * \return true iff the result is non empty (always return true in inflating mode)
 *
 * \pre xin=\emptyset iff yin=\emptyset
 * \pre (xin,yin) ⊆ (x,y)
 * \pre xin op yin ⊆ z
 */
bool iproj_add(const Interval& z, Interval& x, Interval& y, const Interval &xin=Interval::EMPTY_SET, const Interval& yin=Interval::EMPTY_SET);

/**
 * \brief Inner projection of the subtraction (inflating version)
 *
 * \see comments in \link ibex::iproj_add(const Interval& z, Interval& x, Interval& y, const Interval& xin, const Interval& yin) iproj_add \endlink
 */
bool iproj_sub(const Interval& z, Interval& x, Interval& y, const Interval &xin=Interval::EMPTY_SET, const Interval& yin=Interval::EMPTY_SET);

/**
 * \brief Inner projection of the multiplication.
 *
 * \see comments in #ibex::iproj_add(const Interval&, Interval&, Interval&, const Interval&, const Interval&).
 */
bool iproj_mul(const Interval& z, Interval& x, Interval& y, const Interval &xin=Interval::EMPTY_SET, const Interval& yin=Interval::EMPTY_SET);

/**
 * \brief Inner projection of the division (inflating version)
 *
 * \see comments in #ibex::iproj_add(const Interval&, Interval&, Interval&, const Interval, const Interval&).
 */
bool iproj_div(const Interval& z, Interval& x, Interval& y, const Interval &xin=Interval::EMPTY_SET, const Interval& yin=Interval::EMPTY_SET);

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
 * By default xin={} (non-inflating mode).
 *
 * \return true iff the result is non empty (always return true in inflating mode)
 *
 * \pre xin ⊆ x
 * \pre f(xin) ⊆ y
 */
bool iproj_sqr(const Interval& y, Interval& x, const Interval& xin=Interval::EMPTY_SET);

/**
 * \brief Inner projection of the minus operator.
 *
 * \see comments in #ibex::iproj_sqr(const Interval&, Interval&, const Interval&).
 *
 */
bool iproj_minus(const Interval& y, Interval& x);

/**
 * \brief Inner projection of the absolute value.
 *
 * \see comments in #ibex::iproj_sqr(const Interval&, Interval&, const Interval&).
 *
 */
bool iproj_abs(const Interval& y, Interval& x, const Interval& xin=Interval::EMPTY_SET);

/**
 * \brief Inner projection of power
 *
 * \see comments in #ibex::iproj_sqr(const Interval&, Interval&, const Interval&).
 */
bool iproj_pow(const Interval& y, Interval& x, int p, const Interval &xin=Interval::EMPTY_SET);

/**
 * \brief Inner projection of exp
 *
 * \see comments in #ibex::iproj_sqr(const Interval&, Interval&, const Interval&).
 */
bool iproj_exp(const Interval& y, Interval& x);

/**
 * \brief Inner projection of log
 *
 * \see comments in #ibex::iproj_sqr(const Interval&, Interval&, const Interval&).
 */
bool iproj_log(const Interval& y, Interval& x);


/**
 * \brief Inner projection of cosine
 *
 * \see comments in #ibex::iproj_sqr(const Interval&, Interval&, const Interval&).
 */
bool iproj_cos(const Interval& y, Interval& x, const Interval &xin=Interval::EMPTY_SET);


/**
 * \brief Inner projection of sine
 *
 * \see comments in #ibex::iproj_sqr(const Interval&, Interval&, const Interval&).
 */
bool iproj_sin(const Interval& y, Interval& x, const Interval &xin=Interval::EMPTY_SET);


/**
 * \brief Inner projection of tan
 *
 * \see comments in #ibex::iproj_sqr(const Interval&, Interval&, const Interval&).
 */
bool iproj_tan(const Interval& y, Interval& x, const Interval &xin=Interval::EMPTY_SET);

/**
 * \brief Inner projection of square root
 *
 * \see comments in #ibex::iproj_sqr(const Interval&, Interval&, const Interval&).
 */
bool iproj_sqrt(const Interval& y, Interval& x);

/*
 * TODO: to be continued....
 */

/*@}*/

/*================================== inline implementations ========================================*/

inline Interval iminus(const Interval& x) {
	return -x;
}

inline bool iproj_minus(const Interval& y, Interval& x) {
	return !(x&=iminus(y)).is_empty();
}

inline bool iproj_exp(const Interval& y, Interval& x) {
	return !(x&=ilog(y)).is_empty();
}

inline bool iproj_log(const Interval& y, Interval& x) {
	return !(x&=iexp(y)).is_empty();
}

inline bool iproj_sqrt(const Interval& y, Interval& x) {
	return !(x&=isqr(y & Interval::POS_REALS)).is_empty();
}

} // end namespace ibex

#endif // __IBEX_INNER_ARITH_H__
