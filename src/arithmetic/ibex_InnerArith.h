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


/**
 * \brief Inner image of the addition
 * .... Useful?
 * Return z such that $z\subseteq x+y$.
 */
//inline Interval iadd(const Interval& x, const Interval& y) {
//}

// other operators are missing.
// TODO
// ............................


/**
 * \brief Inner image of sqr.
 * Return y such that $y\subseteq x^2$.
 */
Interval isqr(const Interval& x);

/**
 * \brief Inner image of minus.
 * Return y such that $y\subseteq -x$.
 */
Interval iminus(const Interval& x);

/**
 * \brief Inner image of log.
 * Return y such that $y\subseteq log(x)$.
 */
Interval ilog(const Interval& x);

/**
 * \brief Inner image of exp.
 * Return y such that $y\subseteq exp(x)$.
 */
Interval iexp(const Interval& x);

/**
 * \brief Inner image of acos.
 * Return y such that $y\subseteq acos(x)$.
 */
Interval iacos(const Interval& x);

/**
 * \brief Inner image of asin.
 * Return y such that $y\subseteq asin(x)$.
 */
Interval iasin(const Interval& x);

/**
 * \brief Inner image of atan.
 *
 * Return y such that $y\subseteq atan(x)$.
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
 * <li> $(xin,yin)\subseteq (x',y')$ (only relevant in inflating mode)
 * <li> $x' op y'\subseteq z$
 * </ul>
 * where op is the operator (either,+,-,* or /)
 *
 * By default xin={} and yin={} (non-inflating mode).
 *
 * \return true iff the result is non empty (always return true in inflating mode)
 *
 * \pre $xin=\emptyset$ iff $yin=\emptyset$
 * \pre $(xin,yin)\subseteq (x,y)$
 * \pre $xin op yin\subseteq z$
 */
bool iproj_add(const Interval& z, Interval& x, Interval& y, const Interval &xin=Interval::EMPTY_SET, const Interval& yin=Interval::EMPTY_SET);

/**
 * \brief Inner projection of the subtraction (inflating version)
 *
 * \see comments in #ibex::iproj_add(const Interval&, Interval&, Interval&, const Interval, const Interval&).
 */
bool iproj_sub(const Interval& z, Interval& x, Interval& y, const Interval &xin=Interval::EMPTY_SET, const Interval& yin=Interval::EMPTY_SET);

/**
 * \brief Inner projection of the multiplication.
 *
 * \see comments in #ibex::iproj_add(const Interval&, Interval&, Interval&, const Interval, const Interval&).
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
 * <li> $xin\subseteq x'$ (only relevant in inflating mode)
 * <li> $f(x')\subseteq y$
 * </ul>
 * where op is the operator (either,+,-,* or /)
 *
 * By default xin={} (non-inflating mode).
 *
 * \return true iff the result is non empty (always return true in inflating mode)
 *
 * \pre $xin\subseteq x$
 * \pre $f(xin)\subseteq y$
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
 * \brief Inner projection of square root
 *
 * \see comments in #ibex::iproj_sqr(const Interval&, Interval&, const Interval&).
 */
bool iproj_sqrt(const Interval& y, Interval& x);

/*
 * TODO: to be continued....
 */

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
